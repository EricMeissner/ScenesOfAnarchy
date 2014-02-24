#pragma once
#include "IController.h"
#if defined(_VISION_ANDROID)
#include <Vision/Runtime/Base/Input/VInputAndroid.hpp>
#endif
#include <Vision/Runtime/EnginePlugins/Havok/HavokPhysicsEnginePlugin/vHavokPhysicsModule.hpp>
class GravityRoomController : public IController{
public:
	GravityRoomController(void);
	virtual ~GravityRoomController(void);
	virtual void MapTriggers(VInputMap* inputMap) HKV_OVERRIDE;
	virtual bool Run(VInputMap* inputMap) HKV_OVERRIDE;
private:
#if defined(_VISION_ANDROID)
	vHavokPhysicsModule* pMod;
	VMotionInputAndroid* pMotionInput;
#endif
};

