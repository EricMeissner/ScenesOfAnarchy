#pragma once
#include "IController.h"
#if defined(_VISION_ANDROID)
#include <Vision/Runtime/Base/Input/VInputAndroid.hpp>
#endif
#include <Vision/Runtime/EnginePlugins/Havok/HavokPhysicsEnginePlugin/vHavokPhysicsModule.hpp>

class ParticleRainController : public IController
{
public:
	ParticleRainController(void);
	virtual ~ParticleRainController(void);
	virtual void MapTriggers(VInputMap* inputMap) HKV_OVERRIDE;
	virtual bool Run(VInputMap* inputMap) HKV_OVERRIDE;
	virtual int GetEntityCount() HKV_OVERRIDE;
	void RainBalls(int numOfBlocks);

#if defined(_VISION_ANDROID)
	vHavokPhysicsModule* pMod;
	VMotionInputAndroid* pMotionInput;
#endif
private:
	int entityCount;
};

