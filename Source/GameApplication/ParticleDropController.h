#pragma once
#include "IController.h"
#if defined(_VISION_ANDROID)
#include <Vision/Runtime/Base/Input/VInputAndroid.hpp>
#endif
#include <Vision/Runtime/EnginePlugins/Havok/HavokPhysicsEnginePlugin/vHavokPhysicsModule.hpp>

class ParticleDropController : public IController
{
public:
	ParticleDropController(void);
	virtual ~ParticleDropController(void);
	virtual void MapTriggers(VInputMap* inputMap) HKV_OVERRIDE;
	virtual void Run(VInputMap* inputMap) HKV_OVERRIDE;
	void RainBalls(int numOfBlocks);

#if defined(_VISION_ANDROID)
	vHavokPhysicsModule* pMod;
	VMotionInputAndroid* pMotionInput;
#endif
};

