/* (c) Copyright 2006 International Game Technology */

/*
   $Id: ActorFlashInterface.hpp,v 1.0, 2013-03-01 22:30:48Z, Collier, Daniel$
   $Date: 3/2/2013 6:30:48 AM$
   $Revision: 1$

   Purpose:
      Defines the interface through which the flash actor is queried and manipulated.

   Modifications:
      Author              Date         Explanation
      ------------------- ------------ ------------
      Nicole Beaulieu     13 Jun 2006  Created.
*/

#ifndef __ACTORFLASHINTERFACE_HPP__
#define __ACTORFLASHINTERFACE_HPP__

#include "XOSDynamicObjects.hpp"
using namespace XOS;

// Define the interface class for the actor flash.
class ActorFlashInterface : public UnknownInterface
{
   public:

      // Sets whether or not this instance is a system portal.
      virtual void setIsPortal (bool is_portal) = 0;
      // Enable/disable background transparency mode in flash content.
      virtual void enableFlashTransparency (bool enabled) = 0;
      // Set the CPU partition to run the content in
      virtual void setCPUPartition (const String& partition_name) = 0;
      // Set the start after load flag
      virtual void setStartAfterLoadFlag (bool start_after_load) = 0;
      // Set the SSL context from safe storage paths stored in config
      virtual void setSSLContextFromConfig (const String& cert_config, const String& pk_config, const String& ca_config, const String& crl_config) = 0;
      // Set the SSL context from explicit safe storage paths
      virtual void setSSLContextFromSafeStorage (const String& cert_path, const String& pk_path, const String& ca_path, const String& crl_path) = 0;
      // Set the SSL context from explicit file system paths
      virtual void setSSLContextFromFileSystem (const String& cert_path, const String& pk_path, const String& ca_path) = 0;
      // Set the host and peer verification flags
      virtual void setSSLVerification (bool verify_host, bool verify_peer) = 0;
      // Updates/reloads the SSL context
      virtual void updateSSLContext (void) = 0;
      // Set the sound channel to use
      virtual void setSoundChannel (uint32 channel) = 0;
      // Set the portal id for flash instances that are also portals
      virtual void setPortalId (uint32 portal_id) = 0;
      // Set the max memory in megabytes to be used by the flash player
      virtual void setMaxMemory (uint32 max_memory) = 0;
      // Notify the actor that the context is ready
      virtual void notifyReady (void) = 0;
      // Calls an action script method using the ExternalInterface API
      virtual void invokeActionScript (const String& method, const String& format, ...) = 0;

      DECLARE_INTERFACE_NAME ("ActorFlash")
      DECLARE_INTERFACE_VERSION (1,0)

   protected:
      virtual ~ActorFlashInterface (void) {}
};

#endif

