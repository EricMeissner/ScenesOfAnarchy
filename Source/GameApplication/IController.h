#ifndef ENTITYSTACK_H  
#define ENTITYSTACK_H
#endif
#pragma once
#include "Constants.h"
#include "EntityStack.h"
#include "StackNode.h"
#include <Vision/Runtime/EnginePlugins/VisionEnginePlugin/Entities/TriggerBoxEntity.hpp>
class IController
{
public:
	EntityStack* entityStack;
	IController(void);
	virtual ~IController(void);
	virtual void MapTriggers(VInputMap* inputMap){}
	virtual bool Run(VInputMap* inputMap){
		return true;
	}
protected:
	//VisBaseEntity_cl* AddEntity();
	VisScreenMask_cl* masks[7];
	int maskCount;
	void AddButton(char* buttonImage, int x, int y, int height, int width);
	VisBaseEntity_cl* AddSphere();
	VisBaseEntity_cl* AddCube();
	VisBaseEntity_cl* AddRagdoll();
	VisBaseEntity_cl* AddSphere(float x, float y, float z);
	VisBaseEntity_cl* AddCube(float x, float y, float z);
	VisBaseEntity_cl* AddRagdoll(float x, float y, float z);
	void RemoveLast(void);   //Removes the last entity added.

	VisBaseEntity_cl* AddWaterDrop(float x, float y, float z, float scaling);
	void Drain();
};

