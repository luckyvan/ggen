/* (c) Copyright 2006 International Game Technology */
/*
   $Id: FlashResourceImporterInterface.hpp,v 1.0, 2013-03-01 22:30:48Z, Collier, Daniel$
   $Date: 3/2/2013 6:30:48 AM$
   $Revision: 1$

   Purpose:
      Defines the FlashResourceImporterInterface.

   Modifications:
      Author              Date         Explanation
      ------------------- ------------ ------------
      Beaulieu & LeMay    Apr 26, 06   Original implementation
*/

#ifndef __FLASHRESOURCEIMPORTERINTERFACE_HPP__
#define __FLASHRESOURCEIMPORTERINTERFACE_HPP__

#include "XOSDynamicObjects.hpp"
using namespace XOS;

#ifndef __SOUNDATTRIBUTEINTERFACE_HPP__
#include "SoundAttributeInterface.hpp"
#endif


class FlashResourceImporterInterface : public UnknownInterface
{
   public:
      /////////////////////////////////////////////////////////////////////////////////////////////////
      ///////////////////// FlashResourceImporterInterface ////////////////////////////////////////////
      /////////////////////////////////////////////////////////////////////////////////////////////////
      // Set the stage
      virtual void setStage (class ActorStageInterface* new_stage) = 0;
      // Toggle whether the render thread will wait for getFrameData to render next frame
      virtual void alwaysRender (bool render_flag) = 0;
      // Set the sound attribute
      virtual void setSoundAttribute (Mojo<SoundAttributeInterface> sound_attr) = 0;
      // release the sound attribute
      virtual void releaseSoundAttribute (void) = 0;
      // Start execution of the content if not currently executing
      virtual void executeContent (void) = 0;
      // Set whether or not this is a system portal
      virtual void setIsPortal (bool is_portal) = 0;
      // Enable/disable background transparency
      virtual void enableTransparency (bool enabled) = 0;
      // Retrieve the last Curl code from an HTTP request
      virtual uint32 getCurlCode (void) = 0;
      // Retrieve the last HTTP code from an HTTP request
      virtual long getHttpCode (void) = 0;
      // Set the SSL context for the flash player from safe storage paths stored in config items
      virtual void setSSLContextFromConfig (const String& cert_cfg_name, const String& key_cfg_name, const String& ca_cfg_name, const String& crl_cfg_name) = 0;
      // Set the SSL context for the flash player from safe storage paths
      virtual void setSSLContextFromSafeStorage (const String& cert_path, const String& key_path, const String& ca_path, const String& crl_path) = 0;
      // Set the SSL context for the flash player from file system paths
      virtual void setSSLContextFromFileSystem (const String& cert_path, const String& key_path, const String& ca_path) = 0;
      // Set the host and peer verification flags
      virtual void setSSLVerification (bool verify_host, bool verify_peer) = 0;
      // Updates/reloads the current SSL context
      virtual void updateSSLContext (void) = 0;
      // Update the importer
      virtual void updateImporter (void) = 0;

   public:
      DECLARE_INTERFACE_NAME ("FlashResourceImporterInterface");
      DECLARE_INTERFACE_VERSION (1,0);
};

#endif /*FLASHRESOURCEIMPORTERINTERFACE_*/
