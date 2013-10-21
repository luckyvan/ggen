/* (c) Copyright 2007 International Game Technology */

/*
   $Id: RMLPFlashEssentialRecord.hpp,v 1.0, 2013-03-01 22:30:48Z, Collier, Daniel$
   $Date: 3/2/2013 6:30:48 AM$
   $Revision: 1$

   Purpose:
      Defines the essential record storage for the
      DoubleUp game.

   Modifications:
      Author                     Date              Explanation
      ---------------------      ------------      ------------------
      David Haag                 Oct 26, 2007      New
*/

#ifndef __RMLPFLASHESSENTIALRECORD_HPP__
#define __RMLPFLASHESSENTIALRECORD_HPP__

#ifndef __ESSENTIALRECORD_HPP__
#include "EssentialRecord.hpp"
#endif
#ifndef __RNGINTERFACE_HPP__
#include "RNGInterface.hpp"
#endif

struct RMLPFlashEssentialRecord : public EssentialRecord
{
   // default ER constructor
   RMLPFlashEssentialRecord (void);

   ////////////////////////////////////////////////////////////////////////
   // data members
   // ---------------------------------------------------------------------
   // has the ER been initialized yet? (used by the presentation for ER validity
   //    since the ER pointer is always valid)
   bool initialized;
   int64 sequence_id;
   char history_game_uri [256];
   char normal_game_uri [256];

   /////////////////////////////////////////////////////////////////////////////////////////////////
   ///////////////////// Serializer ////////////////////////////////////////////////////////////////
   /////////////////////////////////////////////////////////////////////////////////////////////////
   // Type id for serializer error checking.
   // Currently this isn't being checked against anything else; it's here to promote object
   //    factory functionality if the future necessitates it.
   // As long as the value of this class's type ID is 1 or more in addition to the base
   //    class ID value, everything is fine
   static const SerializerTypeId RMLP_FLASH_ER_SERIALIZER_TYPE_ID = ESSENTIAL_REC_SERIALIZER_TYPE_ID + 1;

   // Delete this object.  Note: This should be overriden at the most derived level to properly delete
   //    the derived object if it was new'ed.  The body of the method can be as simple as 'delete this;'.
   // This method need not be used if the object is supported by Mojo reference counting.
   virtual void deleteThisObject (void);
   // Compute the total number of bytes required to store all data members of this object.
   virtual size_t getSerialSize (void) const;
   // Convert data to bytes and append to byte buffer.
   virtual void serialize (vector<uint8> &buffer) const;
   // Load buffer data into the data members of this object.
   virtual void unserialize (const uint8 * &buffer);


   // use this streaming operator to help debug essential record code
   friend ostream& operator << (ostream &o, const RMLPFlashEssentialRecord &record);
};

#endif
