#pragma once;
#include "GameApplicationPCH.h"
#include "EntityStack.h"
#include "IController.h"
#include <Vision/Runtime/EnginePlugins/Havok/HavokPhysicsEnginePlugin/vHavokPhysicsModule.hpp>
#include <Vision/Runtime/EnginePlugins/Havok/HavokPhysicsEnginePlugin/vHavokRagdoll.hpp>


IController::IController(void)
{
	entityStack = new EntityStack();
}


IController::~IController(void)
{
}


VisBaseEntity_cl* IController::AddRagdoll(){
	VisBaseEntity_cl *ent = Vision::Game.CreateEntity("VisBaseEntity_cl", hkvVec3(-100.0f, 5, 100), "Models\\Warrior\\Warrior.model");
	vHavokRagdoll *ragdoll = new vHavokRagdoll();
	ragdoll->SetRagdollCollisionFile("Models\\Warrior\\WarriorRagdoll.hkt");
	ent->AddComponent(ragdoll);
	//EntityStack stack = *entityStack;
	//stack.push(ent);
	entityStack->push(ent);
	return ent;
}

VisBaseEntity_cl* IController::AddSphere(){
	VisBaseEntity_cl *ent = Vision::Game.CreateEntity("VisBaseEntity_cl", hkvVec3(-100.0f, -30, 100), "Models\\Misc\\Sphere.Model");
	vHavokRigidBody *sphere = new vHavokRigidBody();
	sphere->Havok_TightFit = true;
	sphere->Havok_Restitution = 1.0f;
	ent->AddComponent(sphere);
	//EntityStack stack = *entityStack;
	//stack.push(ent);
	entityStack->push(ent);
	return ent;
}

VisBaseEntity_cl* IController::AddCube(){
	VisBaseEntity_cl *ent = Vision::Game.CreateEntity("VisBaseEntity_cl", hkvVec3(-100.0f, 30, 100), "Models\\Misc\\Cube.Model");
	vHavokRigidBody *cube = new vHavokRigidBody();
	cube->Havok_TightFit = true;
	ent->AddComponent(cube);
	//EntityStack stack = *entityStack;
	//stack.push(ent);
	entityStack->push(ent);
	return ent;
}

VisBaseEntity_cl* IController::AddSphere(float x, float y, float z){
	VisBaseEntity_cl *ent = Vision::Game.CreateEntity("VisBaseEntity_cl", hkvVec3(x, y, z), "Models\\Misc\\Sphere.Model");
	vHavokRigidBody *sphere = new vHavokRigidBody();
	sphere->Havok_TightFit = true;
	sphere->Havok_Restitution = 1.0f;
	ent->AddComponent(sphere);
	//EntityStack stack = *entityStack;
	//stack.push(ent);
	entityStack->push(ent);
	return ent;
}

VisBaseEntity_cl* IController::AddCube(float x, float y, float z){
	VisBaseEntity_cl *ent = Vision::Game.CreateEntity("VisBaseEntity_cl", hkvVec3(x, y, z), "Models\\Misc\\Cube.Model");
	vHavokRigidBody *cube = new vHavokRigidBody();
	cube->Havok_TightFit = true;
	ent->AddComponent(cube);
	//EntityStack stack = *entityStack;
	//stack.push(ent);
	entityStack->push(ent);
	return ent;
}

VisBaseEntity_cl* IController::AddRagdoll(float x, float y, float z){
	VisBaseEntity_cl *ent = Vision::Game.CreateEntity("VisBaseEntity_cl", hkvVec3(x, y, z), "Models\\Warrior\\Warrior.model");
	vHavokRagdoll *ragdoll = new vHavokRagdoll();
	ragdoll->SetRagdollCollisionFile("Models\\Warrior\\WarriorRagdoll.hkt");
	ent->AddComponent(ragdoll);
	//EntityStack stack = *entityStack;
	//stack.push(ent);
	entityStack->push(ent);
	return ent;
}


void IController::RemoveLast(void)
{
	VisBaseEntity_cl *ent = entityStack->pop();
	if (ent != NULL)
	{
		ent->DisposeObject();
	}
}
