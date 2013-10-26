/* (c) Copyright 2006 International Game Technology */

/*
   $Id: RMLPFlashPresentation.hpp,v 1.0, 2013-03-01 22:30:48Z, Collier, Daniel$
   $Date: 3/2/2013 6:30:48 AM$
   $Revision: 1$

   Purpose:
      Defines the presentation for the
      DoubleUp game.

   Modifications:
      Author                     Date              Explanation
      ---------------------      ------------      ------------------
      Jason Kremer               May 03, 2006      New
*/

#ifndef __RMLPFLASHPRESENTATION_HPP__
#define __RMLPFLASHPRESENTATION_HPP__

#ifndef __GAMEPRESENTATION_HPP__
#include "GamePresentation.hpp"
#endif
#ifndef __RMLPFLASHESSENTIALRECORD_HPP__
#include "RMLPFlashEssentialRecord.hpp"
#endif

class RMLPFlashPresentation : public GamePresentation
{

   protected:
      /////////////////////////////////////////////////////////////////////////////////////////////////
      ///////////////////// UnknownInterface //////////////////////////////////////////////////////////
      /////////////////////////////////////////////////////////////////////////////////////////////////
      // Initialize the object for use.
      virtual void postConstructor (void);
      // Prepare the object for destruction.
      virtual void preDestructor (void);



   public:
      // has the ER been initialized yet?
      bool isERInitialized (void);
      virtual bool enterModeHistory (void);

      // Load Flash content from ER
      virtual void rmlpLoadContent (void);
      // Unload Flash content
      virtual void rmlpUnloadContent (void);
      // Check to see if EBG is ended by operator or server intervention.
      virtual void abortEBGCheck (void);
      //
      virtual void synchronize(void);

      virtual void changeFlashActorSoundGroup ();
      virtual void changeFlashActorSoundGroup (String stage_name, String actor_name);

      // Flag used to check if base game uri needs to be loaded after RMLP history was viewed.
      bool history_entered;
      int64 session_id_check;

      /////////////////////////////////////////////////////////////////////////////////////////////////
      ///////////////////// MultimediaEventListenerInterface //////////////////////////////////////////
      /////////////////////////////////////////////////////////////////////////////////////////////////
      virtual bool processEvent (const MultimediaEvent &event);
      virtual bool processEvent (const SequenceEvent &event);

   protected:
      /////////////////////////////////////////////////////////////////////////////////////////////////
      ///////////////////// Script Interface //////////////////////////////////////////////////////////
      /////////////////////////////////////////////////////////////////////////////////////////////////
      // Name of object used in the scripts
      static const char *const object_name;
      // Return the type of this scriptable object.
      virtual const char *whoAreYou (void) const;
      // Return an interface to an object given its id.
      virtual bool createScriptableObjectById (Mojo<ScriptInterface> &new_object, int32 object_id, class ScriptParameterList& parameter_list, bool new_declared);
      // Execute a function given its id.
      virtual bool execScriptableFunctionById (int32 func_id, ScriptParameterList& parameter_list);

      // Script function IDs.  Base class script IDs start at 0.  Derived classes must start at MAX_FUNCTION_ID.
      enum
      {
         // MAX_FUNCTION_ID must always be last in the list.
         RMLP_LOAD_CONTENT = GamePresentation::MAX_FUNCTION_ID,
         RMLP_UNLOAD_CONTENT,
         ABORT_EBG_CHECK,
         CHANGE_FLASH_ACTOR_SOUND_GROUP,
         MAX_FUNCTION_ID,
      };
      static const ScriptFunctionMap script_functions [];

      // Return the DoubleUp-specific essential record pointer
      RMLPFlashEssentialRecord *getRMLPFlashEssentialRecord (void) const;

      ////////////////////////////////////////////////////////////////////////////
      /////////////////////  Serializer Support  /////////////////////////////////
      ////////////////////////////////////////////////////////////////////////////
      // ptr to object that contains essential record data
      RMLPFlashEssentialRecord *rmlp_flash_essential_record_ptr;
      // Construct the object that will contain and unserialize the essential record data.
      virtual void constructEssentialRecord (void);

      // Update flash content dynamically during game play
      virtual void rmlpUpdateContent (String stage_name, String container_name, String actor_name, String path_to_model, String path_to_new_content, String parent_container, bool show);
   public:
      IMPLEMENT_INTERFACE_BEGIN (RMLPFlashPresentation)
         IMPLEMENT_INTERFACE (PresentationExtensionInterface)
         IMPLEMENT_INTERFACE (ScriptInterface)
      IMPLEMENT_INTERFACE_END ()
};

#endif
