/* (c) Copyright 2001 International Game Technology */
/*
   $Id: Debug.cpp,v 1.0, 2013-03-01 22:30:36Z, Collier, Daniel$
   $Date: 3/2/2013 6:30:36 AM$
   $Revision: 1$

   Purpose:
      Defines the class Debug.
      The idea is to get the whole debug functionality in this single .hpp file to make
      it simple on the user.  When __DEBUG__ is #defined it increases the object code
      size because each #include of this file creates another debug object.  However,
      when __DEBUG__ is NOT #defined the compiler has been seen to completely optimize
      out all references to debug.

   Modifications:
      Author          Date       Explanation
      --------------- ---------- ------------
      Dwayne Nelson   Oct 28, 98 New
*/

#include "XOSIPC.hpp"
using namespace XOS;

#ifndef __DEBUG_HPP__
#include "Debug.hpp"
#endif

#if defined (__DEBUG__)
      #include <vector>
      #ifndef __DEBUGINTERFACE_HPP__
      #include "DebugInterface.hpp"
      #endif

      using namespace std;

      // DebugMonitor
      // This is the class that is the debug client.
      // This class talks to the debug process.  It doesn't use or derive from
      // Sender.  This is intentional so that debug functionality isn't dependent
      // on other functionality.  This ensures that the debug functionality can
      // be used anywhere in the code base.
      class DebugMonitor
      {
         public:
            DebugMonitor (void)
               : client_coid (-1) {}
            ~DebugMonitor (void)
               {
                  if (client_coid != -1)
                     name_close (client_coid);
                  client_coid = -1;
               }

            void print (const char * buf, uint32 len);

         protected:
            int client_coid;      // Connection Id.
      };

      void DebugMonitor::print (const char * buf, uint32 len)
      {
         // Debug can't send debug information to itself
         #if !defined (__DEBUGPROCESS__)
               if (client_coid == -1)
                  client_coid = name_open (DEBUG_REGISTERED_NAME, 0);

            if (client_coid != -1)
            {
               DebugMessagePrint message ((uint16)len);

               iov_t send_mx [2];
               SETIOV (send_mx + 0, &message, sizeof (message));
               SETIOV (send_mx + 1, buf, len);

               // If the send fails, clear the client_coid
               if (MsgSendv(client_coid, send_mx, ARRAY_ELEMENTS (send_mx), 0, 0) == -1)
                  client_coid = -1;
            }
         #endif
      }


      // DebugStreambuf
      // The streambuf used by DebugStream
      // This class basically takes all stream information and sends it to the debug
      // process by using the DebugMonitor class.
      class DebugStreambuf : public streambuf
      {
         public:
            DebugStreambuf (void) :
               buffer (DEFAULT_BUFFER_SIZE)
               {
                  // Reset the put pointers
                  setp (&buffer [0], &buffer [0] + buffer.size ());
               }

                // Purpose: Virtual function called when the put buffer gets full
            virtual int overflow (int ch = EOF)
               {
                  // Did we get here because the buffer was overrun?
                  if (ch != EOF)
                  {
                     // Store the current offset to put the next character
                     uint32 current_offset = buffer.size ();
                     // Resize the buffer to hold additional entries
                     buffer.resize (buffer.size () + GROW_SIZE);
                     // Add the character into the string
                     buffer [current_offset] = (char)ch;
                     // Reset the buffer pointers: first element, next location to put an element
                     //   and a pointer to one past the last element.
                     setp (&buffer [0], &buffer [current_offset + 1], &buffer [0] + buffer.size ());
                  }
                  // Called because endl was encountered
                  else
                  {
                     // Send the information to the Debug process
                     sender.print (pbase (), out_waiting ());
                     // Reset the put pointers
                     setp (&buffer [0], &buffer [0] + buffer.size ());
                  }
                  // Return value should be anything except EOF
                  return !EOF;
               }

            // Purpose: Virtual function that is called to flush/synch up the buffer with the actual device
            // NOTE: The main reason for overriding this is when endl is sent to the stream this
            // method is called.  Therefore we can flush out the debug info to the debug process
            virtual int sync (void)
               {
                  // Flush our buffer
                  overflow ();
                  return streambuf::sync ();
               }

         protected:
            enum {DEFAULT_BUFFER_SIZE = 100, GROW_SIZE = DEFAULT_BUFFER_SIZE};
            vector<char> buffer;         // The storage buffer
            DebugMonitor sender;         // Member responsible for talking to the debug process

            int out_waiting (void) const
               { return (pptr () - pbase ()) / sizeof (char); }
      };

      static DebugStreambuf debug_buf;  // Create a streambuf for the DebugStream to use

      DebugStream::DebugStream (void)
         : ostream (&debug_buf) {}
#else
   DummyDebugStream & time (DummyDebugStream &o) { return o; }
   DummyDebugStream & date (DummyDebugStream &o) { return o; }
   DummyDebugStream & datetime (DummyDebugStream &o) { return o; }
   DummyDebugStream & line (DummyDebugStream &o) { return o; }
   DummyDebugStream & operator << (DummyDebugStream &o, const class address & /*a*/) { return o; }
#endif

