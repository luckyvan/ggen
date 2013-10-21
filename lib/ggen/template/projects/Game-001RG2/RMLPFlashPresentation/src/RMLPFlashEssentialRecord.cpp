/* (c) Copyright 2007 International Game Technology */

/*
   $Id: RMLPFlashEssentialRecord.cpp,v 1.0, 2013-03-01 22:30:48Z, Collier, Daniel$
   $Date: 3/2/2013 6:30:48 AM$
   $Revision: 1$

   Purpose:
      Implements the essential record storage for the
      DoubleUp game.

   Modifications:
      Author                     Date              Explanation
      ---------------------      ------------      ------------------
      David Haag                 Oct 26, 2007      New
*/

#ifndef __DEBUG_HPP__
#include "Debug.hpp"
#endif
#ifndef __UTIL_HPP__
#include "Util.hpp"
#endif
#ifndef __RMLPFLASHESSENTIALRECORD_HPP__
#include "RMLPFlashEssentialRecord.hpp"
#endif

#ifdef __DEBUG__
//   #define __VERBOSE__
//   #define __VERBOSE_SERIALIZE__
#endif

// Default constructor
RMLPFlashEssentialRecord::RMLPFlashEssentialRecord (void)
{
   initialized = false; // initially, the ER hasn't been initialized by the flow
   sequence_id = 0;
}

////////////////////////////////////////////////////////////////////////////
/////////////////////  Serializer Support  /////////////////////////////////
////////////////////////////////////////////////////////////////////////////

// Delete this object.
void RMLPFlashEssentialRecord::deleteThisObject (void)
{
   #ifdef __VERBOSE_SERIALIZE__
      debug << "DoubleUpEssentialRecord:" << __FUNCTION__ << endl;
   #endif

   // Delete this object.  Note: deleteThisObject () should be overriden at the most
   //    derived level to properly delete the derived object if it was new'ed.
   //    body of the method can be as simple as 'delete this;'.
   // This method need not be used if the object is supported by Mojo reference counting.
   delete (this);
}

// Purpose: Store the data into the given buffer.
void RMLPFlashEssentialRecord::serialize (vector<uint8> &buffer) const
{
   #ifdef __VERBOSE_SERIALIZE__
      debug << "DoubleUpEssentialRecord:" << __FUNCTION__ << "  @" << (void*) this << endl;
   #endif

   // Have base classes serialize their data first!
   EssentialRecord::serialize (buffer);

   serializeValue (buffer, initialized);
   serializeValue (buffer, sequence_id);
   serializeValue (buffer, history_game_uri);
   serializeValue (buffer, normal_game_uri);
}

// Load buffer data into the data members of this object.
void RMLPFlashEssentialRecord::unserialize (const uint8 * &buffer)
{
   #ifdef __VERBOSE_SERIALIZE__
      debug << "RMLPFlashEssentialRecord:" << __FUNCTION__ << "  @" << (void*) this << endl;
   #endif

   // Unserialize base classes first - MUST be called in the same order as the serialize call!
   // The 'buffer' pointer is updated as the unserialize is done.
   EssentialRecord::unserialize (buffer);

   // Load data items.
   unserializeValue (buffer, initialized);
   unserializeValue (buffer, sequence_id);
   unserializeValue (buffer, history_game_uri);
   unserializeValue (buffer, normal_game_uri);
}

// Compute the total number of bytes required to store local data members and base classes.
size_t RMLPFlashEssentialRecord::getSerialSize (void) const
{
   // Figure out what the base class needs
   size_t total_size = EssentialRecord::getSerialSize ();

   // Add the size this class needs
   total_size += sizeof (initialized);
   total_size += sizeof (sequence_id);
   total_size += sizeof (history_game_uri);
   total_size += sizeof (normal_game_uri);

   return (total_size);
}
