/* (c) Copyright 2000 International Game Technology */
/*
   $Id: SharedObjectMain.hpp,v 1.0, 2013-03-01 22:30:36Z, Collier, Daniel$
   $Date: 3/2/2013 6:30:36 AM$
   $Revision: 1$

   Purpose:
      Defines the shared objects convenience macros. This file is used on the shared object side.
      See document "Shared Objects" for more details.

   Modifications:
      Author            Date        Explanation
      ---------------   ----------  ------------
      Jamal Benbrahim   May 07, 00  Ripped off Dwayne's version.
*/

#ifndef __SHAREDOBJECTMAIN_HPP__
#define __SHAREDOBJECTMAIN_HPP__

#ifndef __XOSDYNAMICOBJECTS_HPP__
   #include "XOSDynamicObjects.hpp"
#endif
using namespace XOS;

#include <list>
#include <iostream>
#include <typeinfo>

#ifdef __DYNAMIC_DEBUG__
   #ifndef __DYNAMICDEBUG_HPP__
   #include "DynamicDebug.hpp"
   #endif
#else
   #define IN_DDEBUG 1
#endif

#ifndef __IGTDEFS_HPP__
#include "IGTdefs.hpp"
#endif

// --------------------------- Object Creation -----------------------

template <class __BaseClass> 
class DoNotUseDirectlyStaticImplement : public __BaseOfXImplement <__BaseClass>
{
   public:
      DoNotUseDirectlyStaticImplement (void)
         : __BaseOfXImplement <__BaseClass> ()
         {
            __BaseOfXImplement<__BaseClass>::__is_static = true;
         }
          
      virtual ~DoNotUseDirectlyStaticImplement (void)
         {
            #if defined (__DYNAMIC_DEBUG__) | defined (__MOJO_VERBOSE__) | defined (__MOJO_RTTO_VERBOSE__)
               if (IN_DDEBUG)
               {
                  cout << "Deleted static object ";
                  #ifdef __MOJO_RTTI_VERBOSE__
                     cout << typeid (__BaseClass).name ();
                  #endif
                  cout << ":" << this << endl;
               }
            #endif
         }

      virtual uint32 release (void)
         {
            uint32 __result = __BaseOfXImplement<__BaseClass>::release ();

            #if defined (__DYNAMIC_DEBUG__) | defined (__MOJO_VERBOSE__) | defined (__MOJO_RTTO_VERBOSE__)
               if (IN_DDEBUG)
                  cout << "Releasing static object ";
                  #ifdef __MOJO_RTTI_VERBOSE__
                     cout << typeid (__BaseClass).name ();
                  #endif
                  cout << ":" << this << ",  count = " << __result << endl;
            #endif
            
            if (__result == 0)
            {
               // See if an object was created from .so
               void* source_so = getSource ();
               if (source_so != 0)
               {
                  // Save a copy of the unknown interface to be used with the object factory.
                  UnknownInterface* temp = this->getUnknown ();
                  delete this;
                  // Release this object from the object factory. If this object was not created from
                  // the object factory, the releaseInterface call will be ignored.
                  SharedObjectMacrosObjectFactoryInterface::releaseInterface (temp, source_so);
                  /*
                     WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING 
                     
                     Do not optimize the code above to look like this.

                     // If the object was created from an object factory, delete it.
                     if (SharedObjectMacrosObjectFactoryInterface::releaseInterface (getUnknown ()))
                        delete this;

                     This will cause the shared object from where this object got created to be unloaded
                     before the destructor is done.

                     WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING 
                  */
               }
               else
                  delete this;
            }

            return __result;
         }

      virtual uint32 addReference (void)
         {
            uint32 __result = __BaseOfXImplement<__BaseClass>::addReference ();

            #if defined (__DYNAMIC_DEBUG__) | defined (__MOJO_VERBOSE__) | defined (__MOJO_RTTO_VERBOSE__)
               if (IN_DDEBUG)
                  cout << "Adding reference on static object ";
                  #ifdef __MOJO_RTTI_VERBOSE__
                     cout << typeid (__BaseClass).name ();
                  #endif
                  cout << ":" << this << ",  count = " << __result << endl;
            #endif
            
            return __result;
         }

      // If this object was created from an SO, sets the source to pointer to that SO
      // This function MUST be implemented here
      virtual void setSource (void* source)
         { __BaseOfXImplement<__BaseClass>::setSource (source); }

      // Returns pointer to SO if this object was created from an SO, otherwise returns 0
      // This function MUST be implemented here
      virtual void* getSource (void) const
         { return __BaseOfXImplement<__BaseClass>::getSource (); }

      // Returns true if this is a static object
      // This function MUST be implemented here
      virtual bool isStatic (void) const
         { return __BaseOfXImplement<__BaseClass>::isStatic (); }
};


// --------------------------- Object Declaration Macros ------------------------------

class ObjectQuery : public ObjectQueryInterface
{
   public:
      virtual uint32 getObjectCount (void) const;
      virtual void getObjectsNameList (const char* __name_list []) const;
      virtual bool objectSupported (const char* __object_name) const;
      virtual uint32 getInterfaceCount (const char* __object_name) const;
      virtual void getInterfacesNameList (const char* __object_name, const char* __name_list []) const;
      virtual bool interfaceSupported (const char* __object_name, const char* __interface_name) const;
      virtual UnknownInterface* createObject (const char* __object_name);
   
   protected:
      virtual void postConstructor (void);
      virtual void preDestructor (void);

   public:
      IMPLEMENT_INTERFACE_BEGIN (ObjectQuery)
         IMPLEMENT_INTERFACE (ObjectQueryInterface)
      IMPLEMENT_INTERFACE_END ()
}; 

template <class T>
class NoCreateObjectQueryPtr : public T
{
   private:
      virtual UnknownInterface* createObject (const char* __object_name) = 0;
};

struct __ObjectInfo  \
{  \
   enum __TableEntryType   \
   {  \
      __ENTRY_TYPE_INVALID       = 0,  \
      __ENTRY_TYPE_OBJECT        = 1,  \
      __ENTRY_TYPE_TABLE         = 2,  \
      __ENTRY_TYPE_END_OF_TABLE  = 3,  \
   }; \
   __ObjectInfo (const __TableEntryType _type, const char* _name, const __InterfaceInfo* _interface_table, list<__ObjectInfo> *_table) \
      : type (_type),   \
        name (_name),   \
        interface_table (_interface_table),  \
        table (_table)  \
      {} \
   const __TableEntryType type;    \
   String name;   \
   const __InterfaceInfo* interface_table;   \
   list<__ObjectInfo>* table; \
   private: \
   __ObjectInfo& operator= (const __ObjectInfo& rhs); \
}; \

typedef list<__ObjectInfo> __ObjectList; \
typedef __ObjectList::iterator __ObjectIterator; \

#define DECLARE_CHAINABLE_OBJECT_BEGIN(__ID)   \
class __ThisClass##__ID : public ObjectQueryInterface\
{\
   private: \
      virtual bool interfaceSupported (const char* /*interface_name*/) const { return false; }   \
      virtual void * getInterface (const char* /*interface_name*/) { return 0; }  \
      virtual uint32 release (void) { delete this; return 0; }  \
      virtual uint32 addReference (void) { return 0; }  \
      virtual void setSource (void* /*source*/) {}  \
      virtual void* getSource (void) const { return 0; }  \
      virtual bool isStatic (void) const { return false; } \
      virtual void postConstructor (void) {}  \
      virtual void preDestructor (void) {}   \
   public:  \
      static __ObjectList* __object_list;   \
      static void __initObjectList (void)  \
         {  \
            if (!__object_list)   \
            {  \
               __object_list = new __ObjectList;  \
               staticCreateObject ("", true);   \
            }  \
         }  \
   public:  \
      virtual ~__ThisClass##__ID (void)  \
         {  \
            delete __object_list;   \
            __object_list = 0;   \
         }  \
      const __ObjectInfo* getObjectInfo (const char* __object_name, uint32& __count, const char* __name_list []) const   \
         {  \
            __initObjectList (); \
            __count = 0;   \
            __ObjectList* __objs = __object_list; \
            for (__ObjectIterator iterator (__objs->begin ()); __objs && (iterator != __objs->end ()); ++iterator)   \
            {  \
               switch (iterator->type) \
               {  \
                  case __ObjectInfo::__ENTRY_TYPE_OBJECT:  \
                     if (__name_list)  \
                     {  \
                        *__name_list = iterator->name;   \
                        __name_list++; \
                     }  \
                     if (__object_name && (iterator->name == __object_name)) \
                        return &(*iterator);\
                     __count++;  \
                     break;   \
                  case __ObjectInfo::__ENTRY_TYPE_TABLE:   \
                     __objs = iterator->table;   \
                     iterator = __objs->begin ();  \
                     break;   \
                  default: \
                     __objs = 0;  \
                     break;   \
               }  \
            }  \
            return 0;   \
         }  \
      uint32 getObjectCount (void) const   \
         {  uint32 __count;   getObjectInfo (0, __count, 0);   return __count;   }  \
      void getObjectsNameList (const char* __name_list []) const   \
         {  uint32 __dummy;   getObjectInfo (0, __dummy, __name_list);  }  \
      bool objectSupported (const char* __object_name) const \
         {  uint32 __dummy;   return getObjectInfo (__object_name, __dummy, 0) ? true : false;  }  \
      uint32 getInterfaceCount (const char* __object_name) const   \
         {  uint32 __dummy; const __ObjectInfo* __oi = getObjectInfo (__object_name, __dummy, 0);   \
            uint32 __count = 0;  \
            if (__oi && __oi->interface_table)  \
            {  \
               __GetInterfaceInfo __get_interface_info;  \
               __get_interface_info.getInterfaceInfo (__oi->interface_table, 0, __count, __dummy);   \
            }  \
            return __count;   \
         }  \
      void getInterfacesNameList (const char* __object_name, const char* __name_list []) const   \
         {  uint32 __dummy; const __ObjectInfo* __oi = getObjectInfo (__object_name, __dummy, 0);   \
            if (__oi && __oi->interface_table)  \
            {  \
               __GetInterfaceInfo __get_interface_info;  \
               __get_interface_info.getInterfaceInfo (__oi->interface_table, 0, __dummy, __dummy, __name_list);   \
            }  \
         }  \
      bool interfaceSupported (const char* __object_name, const char* __interface_name) const \
         {  uint32 __dummy; const __ObjectInfo* __oi = getObjectInfo (__object_name, __dummy, 0);   \
            if (__oi && __oi->interface_table)  \
            {  \
               if (strcmp (__interface_name, UnknownInterface::getInterfaceName ()) == 0) return true; \
               __GetInterfaceInfo __get_interface_info;  \
               return (__get_interface_info.getInterfaceInfo (__oi->interface_table, __interface_name, __dummy, __dummy)) ? true : false;   \
            }  \
            return false;  \
         }  \
      UnknownInterface* createObject (const char* __object_name)  \
         {  \
            __initObjectList (); \
            return staticCreateObject (__object_name, false);   \
         }  \
      static UnknownInterface* staticCreateObject (const char* __object_name, bool __initialize) {\

#define RETURN_AS_HEAP(__BaseClass) \
{\
   return (new DoNotUseDirectlyHeapImplement <__BaseClass>)->getUnknown (); \
}\

#define RETURN_AS_STATIC(__BaseClass)\
{\
   static UnknownInterface * __instance = 0; \
   if (!__instance)  \
      __instance = ((new DoNotUseDirectlyStaticImplement <__BaseClass>)->getUnknown ());  \
   return __instance;   \
}\

#define DECLARE_OBJECT(__Type, __ObjectClass, __ObjectName) \
   if (__initialize)   \
   {  \
      __object_list->push_back (__ObjectInfo (__ObjectInfo::__ENTRY_TYPE_OBJECT, __ObjectName, __ObjectClass::getInterfaces (), 0));   \
   }  \
   else if (strcmp (__ObjectName, __object_name) == 0) \
   {  \
      RETURN_AS_##__Type (__ObjectClass);   \
   }  \

#define DECLARE_PARENT_OBJECT(__ParentID) \
   if (__initialize)   \
   {  \
      __ThisClass##__ParentID::__initObjectList ();   \
      __object_list->push_back (__ObjectInfo (__ObjectInfo::__ENTRY_TYPE_TABLE, "", 0, __ThisClass##__ParentID::__object_list));   \
   }  \
   else  \
   {  \
      __ThisClass##__ParentID* __parent = new __ThisClass##__ParentID;  \
      UnknownInterface* p = __parent->createObject (__object_name); \
      if (p)   \
         return p;   \
      else  \
      {  \
         delete __parent;  \
         __parent = 0;  \
      }  \
   }  \

#define DECLARE_CHAINABLE_OBJECT_END(__ID)  \
   DECLARE_OBJECT(STATIC, ObjectQuery, "ObjectQuery");   \
   if (__initialize)   \
   {  \
      __object_list->push_back (__ObjectInfo (__ObjectInfo::__ENTRY_TYPE_END_OF_TABLE, "", 0, 0));   \
   }  \
   return 0;   }  \
};  \
__ObjectList* __ThisClass##__ID::__object_list = 0; \

#define EXPORT_CHAINABLE_OBJECT(__ID)   \
   ObjectQueryInterface *createMainSharedObject (void) {  return new __ThisClass##__ID; }  \

#define DECLARE_MAIN_OBJECT_BEGIN() DECLARE_CHAINABLE_OBJECT_BEGIN(__Unique_Name)
#define DECLARE_MAIN_OBJECT_END() DECLARE_CHAINABLE_OBJECT_END(__Unique_Name)
#define EXPORT_MAIN_OBJECT() EXPORT_CHAINABLE_OBJECT(__Unique_Name)

#endif
