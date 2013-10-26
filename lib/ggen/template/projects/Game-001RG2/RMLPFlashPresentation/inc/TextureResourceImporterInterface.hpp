/* (c) Copyright 2001 International Game Technology */
/*
   $Workfile: TextureResourceImporterInterface.hpp$
   $Archive$
   $Revision: 1$
   $Date: 3/2/2013 6:30:48 AM$
   $Author: Collier, Daniel$

   Purpose:
      Defines the TextureResourceImporterInterface.  Common interface for importers that handle video data.

   Modifications:
      Author              Date         Explanation
      ------------------- ------------ ------------
      Serge Antonov       11 Feb 2003  Created
*/

#ifndef __TEXTURERESOURCEIMPORTERINTERFACE_HPP__
#define __TEXTURERESOURCEIMPORTERINTERFACE_HPP__

#include "XOSDynamicObjects.hpp"
using namespace XOS;

#ifndef __IGTDEFS_HPP__
#include "IGTdefs.hpp"
#endif
#ifndef __PIXELFORMATS_HPP__
#include "PixelFormats.hpp"
#endif


class TextureResourceImporterInterface : public UnknownInterface
{
   public:
      struct TextureInfo
      {
         bool color_channel_changed;
         bool alpha_channel_changed;
         uint32* color_rect_list;
         uint32 color_rect_list_size;
         uint32* alpha_rect_list;
         uint32 alpha_rect_list_size;   
         uint8* importer_color_pixels;
         uint8* importer_alpha_pixels;
         uint8* texture_color_pixels;
         uint8* texture_alpha_pixels;
         uint8* palette;
         // if the pixel format is YCrCb, this is a reference the the cB chrominance channel
         uint8* cb_pixels;
         // if the pixel format is YCrCb, this is a reference the the cR chrominance channel
         uint8* cr_pixels;
      };
      // Get the current frame that the importer is on
      virtual uint32 getCurrentFrame (void) const = 0;
      // Get the current frame's duration (value returned is in milliseconds)
      virtual uint32 getFrameDuration (void) const = 0;
      // Get the characteristics of the current frame.  If the values passed in were set true is returned
      virtual bool getFrameParameters (uint32 &width, uint32 &height, PixelFormat &format, uint32 &mip_map_count, uint32 &texture_filter) const = 0;
      // Informs the caller is the frame can be allocated by the importer
      virtual bool canAllocateFrameData (void) const = 0;
      // Informs the resource importer whether to allocate the frame data or not
      virtual bool setAllocateFrame (bool flag) = 0;
      // Get the number of frames this media has
      virtual uint32 getNumberOfFrames (void) const = 0;
      virtual uint32 getNumberOfFrames (const int32 section_number) const = 0;
      virtual uint32 getNumberOfFrames (const char *section_name) const = 0;
      // Selects the frame in the media to be used as the current frame.  Returns 0 if successful,
      //    otherwise, a nonzero value
      // NOTE: origin can be set to any of the following values:
      //       SEEK_SET = The new frame is computed relative to the start of the file.
      //       SEEK_CUR = The new frame is computed relative to the current file position. The value of
      //                  frame_number may be positive, negative or zero.
      //       SEEK_END = The new frame is computed relative to the end of the file.         
      virtual uint32 seekFrame (int32 origin, int32 frame_number) = 0;
      // select a section for play back
      virtual bool selectSection (const int32 section_number) = 0;
      virtual bool selectSection (const char *section_name) = 0;
      // Get the specified section's duration (value returned is in milliseconds)
      virtual double getSectionDuration (const int32 section_number) const = 0;
      virtual double getSectionDuration (const char *section_name) const = 0;
            // Get the number of sections in the file
      virtual uint32 getNumberOfSections() const = 0;
      // Get the name of a section in the file
      virtual const char *getSectionName(const int32 section_number) const = 0;
      // Get the number of channels in the file
      virtual uint32 getNumberOfChannels() const = 0;
      // Get the sound that goes with this section
      virtual bool getSectionSound (const int32 section_number, class String &file_name, class String &sound_group, 
         bool &pre_load) const = 0;
      // Get the specified section's loop counter 
      virtual int32 getSectionLoopCount (const int32 section_number) const = 0;
      virtual int32 getSectionLoopCount (const char *section_name) const = 0;
      // lock the importer so we can get data. This returns the neccesary data for blitting
      // warning the pointers may no longer be valid after a call to unlock
      virtual bool lock (TextureResourceImporterInterface::TextureInfo &tex_info) = 0;
      // unlock the importer
      virtual bool unlock (void) = 0;
      // return true if importer is pending a change (texture information change)
      virtual bool isChangePending (void) = 0;
      // return true if importer has changed (texture information change)
      virtual bool importerChanged (void) = 0;
      
      DECLARE_INTERFACE_NAME ("TextureResourceImporter");
      DECLARE_INTERFACE_VERSION (1,0);

   protected:
      virtual ~TextureResourceImporterInterface (void) {}
};

#endif
