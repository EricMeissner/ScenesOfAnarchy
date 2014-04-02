#pragma once
#include "IController.h"
#if defined(_VISION_ANDROID)
#include <Vision/Runtime/Base/Input/VInputAndroid.hpp>
#endif
#include <Vision/Runtime/EnginePlugins/Havok/HavokPhysicsEnginePlugin/vHavokPhysicsModule.hpp>

class TowerOfDoomController : public IController
{
public:
	TowerOfDoomController(void);
	virtual ~TowerOfDoomController(void);
	virtual void MapTriggers(VInputMap* inputMap) HKV_OVERRIDE;
	virtual bool Run(VInputMap* inputMap) HKV_OVERRIDE;
	virtual int GetEntityCount() HKV_OVERRIDE;
	virtual void DeInitGUI() HKV_OVERRIDE;
	void StackBlocks(int numOfBlocks);
	void Swipe(VInputMap *inputMap);
	int blockCount;
	int z;
	VDialog* dialog;
	void InitMenu(VAppMenuContext* context);
	void EnableMenu();
	bool menuMode;
	bool menuDisplayed;
	bool blocksStacked;
	VAppMenuContext* spContext;
#if defined(_VISION_ANDROID)
	vHavokPhysicsModule* pMod;
	VMotionInputAndroid* pMotionInput;
#endif
};

