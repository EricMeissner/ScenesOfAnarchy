#pragma once
#include "IController.h"
#if defined(_VISION_ANDROID)
#include <Vision/Runtime/Base/Input/VInputAndroid.hpp>
#endif
#include <Vision/Runtime/EnginePlugins/Havok/HavokPhysicsEnginePlugin/vHavokPhysicsModule.hpp>
#include <Vision/Runtime/EnginePlugins/VisionEnginePlugin/Input/VFreeCamera.hpp>
#include <Vision/Runtime/EnginePlugins/VisionEnginePlugin/Entities/TriggerBoxEntity.hpp>
class WaterSimulationController : public IController
{
public:
	WaterSimulationController(void);
	~WaterSimulationController(void);
	virtual void MapTriggers(VInputMap* inputMap) HKV_OVERRIDE;
	virtual bool Run(VInputMap* inputMap) HKV_OVERRIDE;
	virtual int GetEntityCount() HKV_OVERRIDE;
	void decrementEntityCount();
	VisBaseEntity_cl* AddWaterDrop(float x, float y, float z, float scaling);

private:
	int entityCount;
	//TriggerBoxEntity_cl *triggerbox;
};

