#ifndef __RMLPFLOWINTERFACE_HPP__
#define __RMLPFLOWINTERFACE_HPP__

#ifndef __XOSDYNAMICOBJECTS_HPP__
#include "XOSDynamicObjects.hpp"
#endif

class RMLPFlowToFlowInterface : public UnknownInterface {

   public:
      virtual int64 getBonusId (void) = 0;
      virtual String getHistoryBonusUri(void) = 0;
      virtual String getNormalBonusUri(void) = 0;
      virtual bool getAbortEBGFlag(void) = 0;

      DECLARE_INTERFACE_NAME ("RMLPFlowToFlow");
      DECLARE_INTERFACE_VERSION (1, 0);


   protected:
      virtual ~RMLPFlowToFlowInterface (void) {}


};

#endif
