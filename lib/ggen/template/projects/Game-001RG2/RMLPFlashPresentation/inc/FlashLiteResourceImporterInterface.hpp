/* (c) Copyright 2009 International Game Technology */
/*
   $Id: FlashLiteResourceImporterInterface.hpp,v 1.0, 2013-03-01 22:30:48Z, Collier, Daniel$
   $Date: 3/2/2013 6:30:48 AM$
   $Revision: 1$

   Purpose:
      Define the FlashLiteResourceImporterInterface.

   Modifications:
      Author            Date         Explanation
      ---------------   ----------  ---------------
      Kory Herzinger    01 Oct 09   Initial Version
*/

#ifndef __FLASHLITERESOURCEIMPORTERINTERFACE_HPP__
#define __FLASHLITERESOURCEIMPORTERINTERFACE_HPP__

#ifndef __SOUNDATTRIBUTEINTERFACE_HPP__
#include "SoundAttributeInterface.hpp"
#endif

#include "XOSDynamicObjects.hpp"
using namespace XOS;

class FlashLiteResourceImporterInterface : public UnknownInterface
{
   public:
      struct ActionScriptParameter
      {
         enum ASParameterType
         {
            PARAMETER_TYPE_INT,
            PARAMETER_TYPE_UINT,
            PARAMETER_TYPE_DOUBLE,
            PARAMETER_TYPE_BOOL,
            PARAMETER_TYPE_STRING
         } m_parameterType;
         union ASParameterValue
         {
            int32 m_intParam;
            uint32 m_uintParam;
            double m_doubleParam;
            bool m_boolParam;
            const char* m_stringParam;
         } m_parameter;
      };

   public:
      // Set the stage of the actor in the importer
      virtual void setStage (class ActorStageInterface* stage) = 0;
      // Set the sound attribute
      virtual void setSoundAttribute (Mojo<SoundAttributeInterface> sound_attribute) = 0;
      // Release the sound attribute
      virtual void releaseSoundAttribute (void) = 0;
      // Inform the importer whether or not this is a portal
      virtual void setIsPortal (bool is_portal) = 0;
      // Set the portal id if the importer is a portal
      virtual void setPortalId (uint32 portal_id) = 0;
      // Start execution of the content if not currently executing
      virtual void executeContent (void) = 0;
      // Set the CPU partition to run the content in
      virtual void setCPUPartition (const String& partition_name) = 0;
      // Set the start after load flag
      virtual void setStartAfterLoadFlag (bool start_after_load) = 0;
      // Set the SSL context for the flash player from safe storage paths stored in config items
      virtual void setSSLContextFromConfig (const String& cert_cfg_name, const String& key_cfg_name, const String& ca_cfg_name, const String& crl_cfg_name) = 0;
      // Set the SSL context for the flash player from safe storage paths
      virtual void setSSLContextFromSafeStorage (const String& cert_path, const String& key_path, const String& ca_path, const String& crl_path) = 0;
      // Set the SSL context for the flash player from file system paths
      virtual void setSSLContextFromFileSystem (const String& cert_path, const String& key_path, const String& ca_path) = 0;
      // Set the host and peer verification flags
      virtual void setSSLVerification (bool verify_host, bool verify_peer) = 0;
      // Set the maximum amount of memory in megabytes to use for the flash player
      virtual void setMaxMemory (uint32 max_memory) = 0;
      // Notify the flash subsystem that the actor is ready
      virtual void notifyReady (void) = 0;
      // Update the importer
      virtual void updateImporter (void) = 0;
      // Invokes an ActionScript method
      virtual bool invokeActionScript (const String& method, const vector<ActionScriptParameter>& parameters) = 0;
      // Sets the background alpha value to use
      virtual void setBackgroundAlpha (uint8 alpha) = 0;

   public:
      DECLARE_INTERFACE_NAME ("FlashLiteResourceImporterInterface");
      DECLARE_INTERFACE_VERSION (1,0);
};

#endif
