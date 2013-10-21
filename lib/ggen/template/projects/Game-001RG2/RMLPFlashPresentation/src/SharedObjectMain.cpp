/* (c) Copyright 1999 International Game Technology */
/*
   $Id: SharedObjectMain.cpp,v 1.0, 2013-03-01 22:30:48Z, Collier, Daniel$
   $Date: 3/2/2013 6:30:48 AM$
   $Revision: 1$

   Purpose:
      Defines the entry point for this shared object.

   Modifications:
      Author            Date        Explanation
      --------------    ----------  ------------
      Jamal Benbrahim   May  6, 00  Created
*/

#ifndef __SHAREDOBJECTMAIN_HPP__
#include "SharedObjectMain.hpp"
#endif
#include "assertion.h"

#if defined (WIN32)
   #define IMPORT_EXPORT __declspec(dllexport)
#else
   #define IMPORT_EXPORT
#endif

// Need to define this function in the most inherited shared object CPP file.
extern ObjectQueryInterface *createMainSharedObject (void);

// This is the object that insures the creation and deletion of the shared object's main object.
class SharedObjectMain
{
   public:
      SharedObjectMain (void) : object (0) {}
      ~SharedObjectMain (void)
         {
            // Hasta la vista...
            object->release ();
            object = 0;
         }
      // Create the main object or just return a pointer to it if it has already been created.
      ObjectQueryInterface* create (void)
         {
            if (!object)
               object = createMainSharedObject ();
            return object;
         }

   private:
      ObjectQueryInterface* object;
};

// Global object that is constructed and destructed by the OS.
static SharedObjectMain* main_shared_object = 0;

// Expose this function to the outside world. Create the main object.
extern "C" IMPORT_EXPORT ObjectQueryInterface* __CreateMainObject (void)
{
   if (!main_shared_object)
      main_shared_object = new SharedObjectMain;
   return main_shared_object->create ();
}

// Expose this function to the outside world. Delete the main object.
extern "C" IMPORT_EXPORT void __DeleteMainObject (void)
{
   if (main_shared_object)
   {
      delete main_shared_object;
      main_shared_object = 0;
   }
}


// --------------------------------------------------------------
// --------------------- ObjectQuery ----------------------------
// --------------------------------------------------------------

// Purpose: Get the number of objects that can be created by this shared object.
uint32 ObjectQuery::getObjectCount (void) const
{
   assertion (main_shared_object);
   assertion (main_shared_object->create ());
   return main_shared_object->create ()->getObjectCount ();
}

// Purpose: Get the names of the objects that can be created by this shared object.
void ObjectQuery::getObjectsNameList (const char* __name_list []) const
{
   assertion (main_shared_object);
   assertion (main_shared_object->create ());
   main_shared_object->create ()->getObjectsNameList (__name_list);
}

// Purpose: Check if object "__object_name" can be created by this shared object.
bool ObjectQuery::objectSupported (const char* __object_name) const
{
   assertion (main_shared_object);
   assertion (main_shared_object->create ());
   return main_shared_object->create ()->objectSupported (__object_name);
}

// Purpose: Get the number of interfaces supported by object "__object_name".
uint32 ObjectQuery::getInterfaceCount (const char* __object_name) const
{
   assertion (main_shared_object);
   assertion (main_shared_object->create ());
   return main_shared_object->create ()->getInterfaceCount (__object_name);
}

// Purpose: Get the names of the interfaces supported by object "__object_name".
void ObjectQuery::getInterfacesNameList (const char* __object_name, const char* __name_list []) const
{
   assertion (main_shared_object);
   assertion (main_shared_object->create ());
   main_shared_object->create ()->getInterfacesNameList (__object_name, __name_list);
}

// Purpose: Check if interface "__interface_name" is supported by object "__object_name".
bool ObjectQuery::interfaceSupported (const char* __object_name, const char* __interface_name) const
{
   assertion (main_shared_object);
   assertion (main_shared_object->create ());
   return main_shared_object->create ()->interfaceSupported (__object_name, __interface_name);
}

UnknownInterface* ObjectQuery::createObject (const char* /*__object_name*/)
{
   assertion (0 && "Method not supported");
   return 0;
}

// Purpose: Called when the constructor is done.
void ObjectQuery::postConstructor (void) {}

// Purpose: Called when the destructor starts.
void ObjectQuery::preDestructor (void) {}
