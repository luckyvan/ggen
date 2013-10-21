/* (c) Copyright 2006 International Game Technology */

/*
   $Id: RMLPFlashPresentation.cpp,v 1.0, 2013-03-01 22:30:48Z, Collier, Daniel$
   $Date: 3/2/2013 6:30:48 AM$
   $Revision: 1$

   Purpose:
      Implements the presentation for the
      DoubleUp game.

   Modifications:
      Author                     Date              Explanation
      ---------------------      ------------      ------------------
      Jason Kremer               May 03, 2006      New
*/

#ifndef __ACTORHELPER_HPP__
#include "ActorHelper.hpp"
#endif
#ifndef __ACTORINTERFACE_HPP__
#include "ActorInterface.hpp"
#endif
#ifndef __ACTORMODELINTERFACE_HPP__
#include "ActorModelInterface.hpp"
#endif
#ifndef __RMLPFLASHPRESENTATION_HPP__
#include "RMLPFlashPresentation.hpp"
#endif
#ifndef __TEXTUREATTRIBUTEINTERFACE_HPP__
#include "TextureAttributeInterface.hpp"
#endif
#ifndef __TEXTURERESOURCEINTERFACE_HPP__
#include "TextureResourceInterface.hpp"
#endif

#ifndef __ACTORFLASHINTERFACE_HPP__
#include "ActorFlashInterface.hpp"
#endif
#include "TextureResourceInterface.hpp"
#include "TextureResourceImporterInterface.hpp"
#include "SoundAttributeInterface.hpp"
#include "SoundSystemInterface.hpp"
#include "SoundResourceInterface.hpp"
#include "SoundInterface.hpp"
#include "FlashResourceImporterInterface.hpp"
#include "FlashLiteResourceImporterInterface.hpp"

#ifndef __EBGCOMMANDSEQUENCEEVENT_HPP__
#include "EBGCommandSequenceEvent.hpp"
#endif

#ifndef __STRINGUTILS_HPP__
#include "StringUtils.hpp"
#endif

using namespace StringUtils;

#ifdef __DEBUG__
//#define __VERBOSE__
#ifdef __VERBOSE__
//#define __VERBOSE_CARDS__
//#define __VERBOSE_PAYTABLE__
//#define __VERBOSE_METERS__
//#define __VERBOSE_PAYTABLE_DISPLAY__
//#define __VERBOSE_WIN__
//#define __VERBOSE_HELP_TEXT__
#endif
#endif
//#define __ALLOW_AWARD_STREAMING__

// This parser is automatically registered with the SequenceEventParser upon instantiation.
// Create an instance here to take advantage of the auto registration.

// name of this object
const char *const RMLPFlashPresentation::object_name = "RMLPFlashPresentation";


/////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////// DoubleUpPresentation  //////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

// has the ER been initialized yet?
bool RMLPFlashPresentation::isERInitialized (void)
{
   // cache a pointer to the ER
   const RMLPFlashEssentialRecord *record = getRMLPFlashEssentialRecord ();
   return (record->initialized);
}

// Purpose: Return the DoubleUp-specific essential record pointer
RMLPFlashEssentialRecord *RMLPFlashPresentation::getRMLPFlashEssentialRecord (void) const
{
   return (rmlp_flash_essential_record_ptr);
}

// Purpose: Initialize the object for use.
void RMLPFlashPresentation::postConstructor (void)
{
   #ifdef __VERBOSE__
   debug << "RMLPFlashPresentation::postConstructor" << endl;
   #endif

   // Call the base classes first.
   GamePresentation::postConstructor ();

   // Set history flag
   history_entered = false;
   session_id_check = 0;

   // Set the script functions.
   static const ScriptFunctionMap script_functions [] =
   {
      // Parameters:  Script Function ID, Arguments, Script function name.
      { RMLP_LOAD_CONTENT,          0, "rmlpLoadContent"               },
      { RMLP_UNLOAD_CONTENT,        0, "rmlpUnloadContent"             },
      { ABORT_EBG_CHECK,            0, "abortEBGCheck"                 },
      // Terminator.
      { -1, 0, "" }
   };
   addScriptFunctionMap (script_functions);

   static const ScriptObjectMap script_objects [] =
   {
      // Parameters: Object ID, New requirement, Object type string
      {-1, ScriptObjectMap::NEW_NOT_ALLOWED, "" }   // Terminator.
   };
   addScriptObjectMap (script_objects);
}

// Prepare the object for destruction.
void RMLPFlashPresentation::preDestructor(void)
{
   // Call the base classes first.
   GamePresentation::preDestructor ();
}

// Execute a function given its id.
bool RMLPFlashPresentation::createScriptableObjectById (Mojo<ScriptInterface> &new_object, int32 object_id, class ScriptParameterList& parameter_list, bool new_declared)
{
   // Initialize the return value.
   bool object_created = true;

   switch (object_id)
   {
      default:
         object_created = GamePresentation::createScriptableObjectById (new_object, object_id, parameter_list, new_declared);
         break;
   }

   return object_created;
}

// Purpose: Execute a function given its id.
bool RMLPFlashPresentation::execScriptableFunctionById (int32 func_id, ScriptParameterList& parameter_list)
{
   // Initialize the return value.
   bool result = true;

   switch (func_id)
   {
      case RMLP_LOAD_CONTENT:
         rmlpLoadContent ();
         break;
      case RMLP_UNLOAD_CONTENT:
         rmlpUnloadContent ();
         break;
      case ABORT_EBG_CHECK:
         abortEBGCheck ();
         break;
      default:
         result = GamePresentation::execScriptableFunctionById(func_id, parameter_list);
      break;
   }

   return result;
}

// Purpose: Return the name of this scriptable object.
const char *RMLPFlashPresentation::whoAreYou (void) const
{
   return object_name;
}


void RMLPFlashPresentation::synchronize()
{
   GamePresentation::synchronize();
   // synch denom
   synchronizeDenomination ();
   synchronizeDenominationMode ();
}

/// <summary> Function that processes a multimedia event. </summary>
/// <param name = "event">[in] Multimedia event to process. </param>
/// <returns> True is the event was handled else false. </returns>
bool RMLPFlashPresentation::processEvent (const MultimediaEvent &event)
{
   #ifdef __VERBOSE_METHOD_NAME__
   debugGame << "RMLPFlashPresentation::processEvent (" << event.getString () << ")" << endl;
   #endif

   if (event == "PlayState:Suspend")
   {
      postEventAll("RmlpFlash:Suspend");
   }
   else if (event == "PlayState:Synchronize")
   {
      // post the resume event. this notifies EBGPresentation that the bonus
      // is being resumed, which in turn notifies EBGFlow, which in turn
      // notifies the loaded Flash content.
      postEventAll("RmlpFlash:Resume");
   }
   else if (event == "Button:RmlpButton1:Enabled:Released")
   {
      postEventAll("RmlpFlash:Button1:Released");
   }
   else if (event == "Button:RmlpButton2:Enabled:Released")
   {
      postEventAll("RmlpFlash:Button2:Released");
   }
   else if (event == "Button:RmlpButton3:Enabled:Released")
   {
      postEventAll("RmlpFlash:Button3:Released");
   }
   else if (event == "Button:RmlpButton4:Enabled:Released")
   {
      postEventAll("RmlpFlash:Button4:Released");
   }
   else if (event == "Button:RmlpButton5:Enabled:Released")
   {
      postEventAll("RmlpFlash:Button5:Released");
   }
   else if (event == "Button:RmlpButton11:Enabled:Released")
   {
      postEventAll("RmlpFlash:Button11:Released");
   }
   else if (event == "Button:RmlpButton12:Enabled:Released")
   {
      postEventAll("RmlpFlash:Button12:Released");
   }
   else if (event == "Button:RmlpButton13:Enabled:Released")
   {
      postEventAll("RmlpFlash:Button13:Released");
   }
   else if (event == "Button:RmlpButton14:Enabled:Released")
   {
      postEventAll("RmlpFlash:Button14:Released");
   }
   else if (event == "Button:RmlpButton15:Enabled:Released")
   {
      postEventAll("RmlpFlash:Button15:Released");
   }
   else if (event == "ConfigureFlashActorSound")
   {
      changeFlashActorSoundGroup ();
   }
   return GamePresentation::processEvent (event);

}

bool RMLPFlashPresentation::processEvent (const SequenceEvent &event)
{
   bool handled = false;

   switch (event.getType())
   {
      case SequenceEvent::EBG_COMMAND_SEQUENCE_EVENT:
      {
         const EBGCommandSequenceEvent &ebg_event = reinterpret_cast<const EBGCommandSequenceEvent &> (event);
         switch (ebg_event.commandType ())
         {
            case EBGCommandSequenceEvent::EBG_RMLP_ABORT_BONUS:
            {
               // End RMLP bonus.
               postEventAll ("Flash.Bonus:Completed");

               handled = true;
            }
            break;
         }
      }
      break;
   }

   if (!handled)
      handled = GamePresentation::processEvent (event);

   return (handled);
}

////////////////////////////////////////////////////////////////////////////
/////////////////////  Serializer Support  /////////////////////////////////
////////////////////////////////////////////////////////////////////////////
// Construct the object that will contain and unserialize the essential record data
void RMLPFlashPresentation::constructEssentialRecord (void)
{
   #ifdef __VERBOSE_SERIALIZE__
      debug << "DoubleUpPresentation:" << __FUNCTION__ << endl;
   #endif

   // Destroy the current essential record (if there is one) before constructing a new one
   destructEssentialRecord ();

   // Create the object that contains essential record/critical data
   rmlp_flash_essential_record_ptr = new RMLPFlashEssentialRecord;

   // Set all ER pointers that the base classes need
   essential_record_ptr = rmlp_flash_essential_record_ptr;

   // Identify the class/type of the essential record for the serialize/unserialize functions
   // It is recommened that derived classes of essential record use an id that is different from any
   // of its base classes (for error checking purposes)
   assertion (essential_record_ptr);
   essential_record_ptr->setSerializerTypeId (RMLPFlashEssentialRecord::RMLP_FLASH_ER_SERIALIZER_TYPE_ID);
}
// Purpose: This method is called when the enterMode game_mode == HISTORY method is called.
bool RMLPFlashPresentation::enterModeHistory (void)
{

   // getting the session ID
   int64 session_id = essential_record_ptr->session_id;
   //getting the theme ID
   String theme_id = essential_record_ptr->game_stage.gameIdentifier().asString();
   // getting the history url
   String url_path = rmlp_flash_essential_record_ptr->history_game_uri;
   // getting the bonus_id

   // getting egm_id
   Config config;
   uint32 id;
   // Pull the EGM ID from the config
   id = config.getValue (config.getId ("EGM ID", this->theme_configuration_identifier, false));


   char buf[11];
   sprintf ((char*)&buf, "%u", id);

   // Building the full path
   String history_full_path;
   #ifdef __VERBOSE__
      debug << ">>>> [BEFORE] History String: " << history_full_path << endl;
      debug << ">>>> [URL PATH]             : " << url_path.c_str() << endl;
      debug << ">>>> [SIZE]                 : " << url_path.length() << endl;

      debug << ">>>> [buf]                  : " << buf << endl;
      debug << ">>>> [session_id]           : " << session_id << endl;
      debug << ">>>> History Full Path      : " << history_full_path << endl;
   #endif

   history_full_path.sprintf(url_path.c_str(), buf,session_id);

   rmlpUpdateContent ("Stage.RMLPFlash", "MLD_RMLP_Front", "MLDForegroundRMLP", "Game.RMLPFlash/Models/MLD_RMLP_Front.model", history_full_path, "MLDFrontContainer", true);
   rmlpUpdateContent ("Stage.RMLPFlash", "MLD_RMLP_Back", "MLDBackgroundRMLP", "Game.RMLPFlash/Models/MLD_RMLP_Back.model", history_full_path, "MLDBackContainer", true);

   // Not desirable, however the FI has no interface to the AI to inform them that the flash content is loaded in history.
   if (session_id_check != session_id)
   {
      session_id_check = session_id;
      #ifdef __VERBOSE__
         debug << "first time load" << endl;
      #endif
      msSleep(5000);
   }
   else
   {
      #ifdef __VERBOSE__
         debug << "loaded" << endl;
      #endif
      msSleep(1000);
   }

   // RMLP history was entered, set flag so base game will reload normal game uri
   history_entered = true;

   return GamePresentation::enterModeHistory();
}
/// <summary> This function dynamically swaps the flash resource on an actor flash during game play. </summary>
/// <remarks> This creates the relative path based on the name passed in. </remarks>
/// <param name = "stage_name">[in] Name of the stage where flash actor lives. </param>
/// <param name = "container_name">[in] Name of the container where flash actor lives. </param>
/// <param name = "actor_name">[in] Name of the flash actor. </param>
/// <param name = "path_to_model">[in] Name of the model. </param>
/// <param name = "path_to_new_content">[in] Location of the .swf content. </param>
/// <param name = "show">[in] boolean to set visibility of the content. </param>
void RMLPFlashPresentation::rmlpUpdateContent (String stage_name, String container_name, String actor_name, String path_to_model, String path_to_new_content, String parent_container, bool show )
{
   ConfigID config_id;
   String localized_path_to_new_content;
   String language;
   String currency;
   String separator;
   uint32 grouping;
   String decimal;
   uint32 whole_index;
   uint32 base_index;

   // get language
   config_id = config.getId ("GAME LANGUAGE", false);
   config.getValueString(config_id, language);

   // get currency
   config_id = config.getId ("NATIVE MONETARY TYPE", false);
   config.getValueString(config_id, currency);
   currency.replace(" ", "%20"); // url encode spaces

   // get group separator
   config_id = config.getId ("NUMERIC GROUP SEPARATOR", false);
   config.getValueString(config_id, separator);

   // get group size
   config_id = config.getId ("NUMERIC GROUP SIZE", false);
   grouping = config.getValue(config_id);

   // get decimal separator
   config_id = config.getId ("NUMERIC DECIMAL SEPARATOR", false);
   config.getValueString(config_id, decimal);

   // get native whole currency index
   config_id = config.getId ("NATIVE WHOLE UNIT CURRENCY SYMBOL", false);
   whole_index = config.getIndex (config_id) - 1; // convert to zero-based

   // get native base currency index
   config_id = config.getId ("NATIVE BASE UNIT CURRENCY SYMBOL", false);
   base_index = config.getIndex (config_id) - 1;  // convert to zero-based

   if (path_to_new_content != "" && path_to_new_content[0] == '/')
   {
      // this is an absolute path to one of the transparent SWFs.
      // don't send the localization information to the SWF.
      localized_path_to_new_content = path_to_new_content;
   }
   else
   {
      // add localization arguments to the swf content
      // also add the current timestamp to prevent caching the content and preventing reloads
      localized_path_to_new_content.sprintf("%s&language=%s&currency=%s&separator=%s&grouping=%d&decimal=%s&wholecurrencyindex=%d&basecurrencyindex=%d",
	                              path_to_new_content.c_str(),
								  language.c_str(), currency.c_str(), separator.c_str(), grouping, decimal.c_str(), whole_index, base_index);
   }

   #ifdef __VERBOSE__
   debug << "[RMLPFlashPresentation::rmlpUpdateContent]" << endl;
   debug << " stage_name: " << stage_name << endl;
   debug << " container_name: " << container_name << endl;
   debug << " actor_name: " << actor_name << endl;
   debug << " path_to_model: " << path_to_model << endl;
   debug << " path_to_new_content: " << path_to_new_content << endl;
   debug << " localized_path_to_new_content: " << localized_path_to_new_content << endl;
   debug << " parent_container: " << parent_container << endl;
   #endif

   // get the stage by name
   StageInterfaces main_stage;
   Mojo<ActorStageInterface> main_stage_interface;
   getStage (stage_name, main_stage_interface);
   main_stage.initialize (main_stage_interface);

   // locate the Flash actor within the scene and remove it.
   Mojo<ActorInterface> actor_ctnr;
   Mojo<ActorInterface> actor_flash;
   Mojo<ActorInterface> actor_stage = main_stage.getActorInterface ();
   if (ActorHelper::findActor (actor_stage, actor_name, true, actor_flash))
   {
      // detach the Flash actor and its sub-tree.
      ActorHelper::removeActor (actor_stage, actor_name);
      // detach the container and its sub-tree (which should be empty now).
      if (ActorHelper::findActor (actor_stage, container_name, true, actor_ctnr))
      {
         // detach and release the container actor.
         ActorHelper::removeActor (actor_stage, container_name);
         actor_ctnr.release ();
      }
      // release the Flash actor. this should free associated resources.
      actor_flash.release ();
   }

   // configure the container with a model
   if (ActorHelper::configure (actor_stage, path_to_model, container_name, resource_list->resourceKey(), true, true))
   {
      #ifdef __VERBOSE__
      debug << "^ CONFIGURING ActorFlash (" << actor_name << ") INTO ActorContainer ("<< container_name << ")" << endl;
      #endif

      Mojo<ActorInterface> actor_interface;
      if (ActorHelper::findActor (actor_stage, actor_name, true, actor_interface))
      {
         #ifdef __VERBOSE__
         debug << "^ FOUND ActorFlash (" << actor_name << ") IN ActorContainer ("<< container_name << ")" << endl;
         #endif

         if (actor_interface.isValid ())
         {
            #ifdef __VERBOSE__
            debug << "^ VALID ActorFlash (" << actor_name << ")" << endl;
            #endif

            // create a new texture resource using the specified URL
            Mojo <AttributeInterface> attribute_interface;
            if (ActorHelper::getAttribute (actor_interface, "TextureAttribute" , attribute_interface))
            {
               #ifdef __VERBOSE__
               debug << "^ FOUND TextureAttribute FOR ActorFlash (" << actor_name << ")" << endl;
               #endif

               Mojo <TextureAttributeInterface> texture_attribute (attribute_interface);
               if (texture_attribute.isValid ())
               {
                  #ifdef __VERBOSE__
                  debug << "^ SETTING TextureAttribute FOR ActorFlash (" << actor_name << ")" << endl;
                  #endif
                  texture_attribute->clearTextureResources (0);
                  texture_attribute->addTextureResource (0, localized_path_to_new_content, true);
               }
            }
            else
            {
               #ifdef __VERBOSE__
               debug << "^ FAILED TO FIND TextureAttribute FOR ActorFlash (" << actor_name << ")" << endl;
               #endif
            }

            // inform the actor that it is not a portal
            Mojo <ActorFlashInterface> actor_flash_new (actor_interface);
            if (actor_flash_new.isValid ())
               actor_flash_new->setIsPortal (false);

            // Move the updated Actor to the specified parent
            if (parent_container != "")
            {
               #ifdef __VERBOSE__
               debug << "^ MOVING actor (" << actor_name << ")" << " TO (" << parent_container << ")" << endl;
               #endif
               ActorHelper::moveActor(actor_stage, container_name, parent_container);
            }

            // initialize the flash actor and load the flash movie
            actor_interface->init ();

            // Set the sound group to GAME_SOUNDS
            changeFlashActorSoundGroup (stage_name, actor_name);

            // show and start playback of the SWF
            actor_interface->show(show, true);
            actor_interface->play(show, true);

            // always enable the touch area in this case, since we are in
            // history mode. standard gameplay mode handled in EBGPresentation.
            if (actor_name == "MLDForegroundRMLP")
            {
               Mojo<ActorInterface> touch_interface;
               if (ActorHelper::findActor (actor_interface, "PrimaryTouchArea", true, touch_interface))
               {
                  if (touch_interface.isValid ())
                  {
                     touch_interface->show (true);
                     touch_interface->play (true);
                  }
               }
            }
         }
         else
         {
            #ifdef __VERBOSE__
            debug << "^ INVALID ActorFlash (" << actor_name << ")" << endl;
            #endif
         }
      }
      else
      {
         #ifdef __VERBOSE__
         debug << "^ FAILED TO FIND ActorFlash (" << actor_name << ") IN ActorContainer ("<< container_name << ")" << endl;
         #endif
      }
   }
   else
   {
      #ifdef __VERBOSE__
      debug << "^ FAILED TO CONFIGURE ActorFlash (" << actor_name << ") INTO ActorContainer ("<< container_name << ")" << endl;
      #endif
   }
}

// Purpose:
void RMLPFlashPresentation::rmlpLoadContent (void)
{
   #ifdef __VERBOSE__
   debug << "RMLPFlashPresentation::rmlpLoadContent" << endl;
   #endif

   const RMLPFlashEssentialRecord *record = getRMLPFlashEssentialRecord ();
   String temp = record->normal_game_uri;
   #ifdef __VERBOSE__
   debug << ">>>>> path to load in rmlp:" << temp << endl;
   #endif
   history_entered = false;
   postEventContext ("RMLP:BroadcastStartMsgs", "Context.EBG");
}

// Purpose:
void RMLPFlashPresentation::rmlpUnloadContent (void)
{
   #ifdef __VERBOSE__
   debug << "RMLPFlashPresentation::rmlpUnloadContent" << endl;
   #endif

   StageInterfaces main_stage;
   Mojo<ActorStageInterface> main_stage_interface;
   getStage ("Stage.RMLPFlash", main_stage_interface);
   main_stage.initialize (main_stage_interface);

   char const *back_name  = "MLD_RMLP_Back";
   char const *front_name = "MLD_RMLP_Front";

   Mojo<ActorInterface> actor_back;
   Mojo<ActorInterface> actor_front;
   Mojo<ActorInterface> actor_stage = main_stage.getActorInterface ();
   if (ActorHelper::findActor (actor_stage, back_name, true, actor_back))
   {
      ActorHelper::removeActor (actor_stage, back_name);
      actor_back.release ();
   }
   if (ActorHelper::findActor (actor_stage, front_name, true, actor_front))
   {
      ActorHelper::removeActor (actor_stage, front_name);
      actor_front.release ();
   }
}

// Purpose:
void RMLPFlashPresentation::abortEBGCheck (void)
{
   const RMLPFlashEssentialRecord *record = getRMLPFlashEssentialRecord ();

   // Send sequence event to the flow to check if EBG should be aborted
   EBGCommandSequenceEvent abort_ebg_check_event (EBGCommandSequenceEvent::EBG_RMLP_ABORT_BONUS_CHECK, SequenceEvent::TO_FLOW);
   sequence_event_poster.setGameStage(record->game_stage);
   sequence_event_poster.post (abort_ebg_check_event);
}

void RMLPFlashPresentation::changeFlashActorSoundGroup ()
{
   changeFlashActorSoundGroup ("Stage.RMLPFlash", "MLDBackgroundRMLP");
   changeFlashActorSoundGroup ("Stage.RMLPFlash", "MLDForegroundRMLP");
}

void RMLPFlashPresentation::changeFlashActorSoundGroup (String stage_name, String actor_name)
{
   StageInterfaces main_stage;
   Mojo<ActorStageInterface> main_stage_interface;
   getStage (stage_name, main_stage_interface);
   main_stage.initialize (main_stage_interface);

   Mojo<ActorInterface> actor_flash;
   Mojo<ActorInterface> actor_stage = main_stage.getActorInterface ();

   Mojo<FlashResourceImporterInterface> flash_importer;
   Mojo<FlashLiteResourceImporterInterface> flash_lite_importer;
   Mojo<TextureAttributeInterface> texture_attribute;
   Mojo<SoundAttributeInterface> sound_attribute;
   Mojo<SoundResourceInterface> sound_importer;
   Mojo<SoundInterface> sound;

   if (ActorHelper::findActor (actor_stage, actor_name, true, actor_flash))
   {
      // get the texture attribute
      Mojo<AttributeInterface> attribute;
      if (ActorHelper::getAttribute (actor_flash, TEXTURE_ATTRIBUTE, attribute))
         texture_attribute = attribute;

      // if we have one pass the touch information onto it
      if (texture_attribute.isValid () == true)
      {
         uint32 channel_count = texture_attribute->getChannelCount ();
         for (uint32 channel_id = 0; channel_id < channel_count; ++channel_id)
         {
            if (texture_attribute->size (channel_id))
            {
               Mojo<TextureResourceInterface> texture_resource_interface;
               if (texture_attribute->getTextureResource (channel_id, 0UL, texture_resource_interface))
               {
                  Mojo<TextureResourceImporterInterface> importer;
                  if (texture_resource_interface->getImporter (importer))
                  {
                     flash_importer = importer;
                     flash_lite_importer = importer;
                  }
               }
            }
         }
      }
   }

   uint32 sound_channel = SoundSystemInterface::GAME_SOUNDS;
   if (flash_importer.isValid ())
   {
      flash_importer->releaseSoundAttribute ();

      createResource<SoundAttributeInterface> (SOUND_ATTRIBUTE, "Default", sound_attribute);
      if (sound_attribute.isValid ())
      {
         sound_importer = flash_importer;
         sound_attribute->createSound("flash", sound, sound_channel, 1);
         if (sound.isValid ())
         {
            sound->setSoundResource (sound_importer, false);
            sound->setLoops(uint32 (-1));
            sound->enable (true);
         }
         flash_importer->setSoundAttribute (sound_attribute);

         // KWH: sound playback is controlled by resource importer now... playback occurs only when
         //      there are sound buffers from flash to be played.
         sound_attribute->play (true, true, 0);
         sound->play (false);
      }
   }
   else if (flash_lite_importer.isValid ())
   {
      Mojo<ActorFlashInterface> flash_actor;
      flash_actor = actor_flash;
      flash_actor->setSoundChannel (sound_channel);

      flash_lite_importer->releaseSoundAttribute ();

      createResource<SoundAttributeInterface> (SOUND_ATTRIBUTE, "Default", sound_attribute);
      if (sound_attribute.isValid ())
      {
         sound_importer = flash_lite_importer;

         sound_attribute->createSound("flash", sound, sound_channel, 1);
         if (sound.isValid ())
         {
            sound->setSoundResource (sound_importer, false);
            sound->setLoops(uint32 (-1));
            sound->enable (true);
         }
         flash_lite_importer->setSoundAttribute (sound_attribute);

         sound_attribute->play (true, true, 0);
         sound->play (false);
      }
   }
}
