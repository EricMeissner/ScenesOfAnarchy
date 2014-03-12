#include "GameApplicationPCH.h"
#include "GravityRoomController.h"
#include "EntityStack.h"

GravityRoomController::GravityRoomController(void)
{
	VisBaseEntity_cl *pCamera  = Vision::Game.SearchEntity("CameraPosition");
	Vision::Camera.AttachToEntity(pCamera, hkvVec3::ZeroVector());
#if defined(_VISION_ANDROID)
	pMod = static_cast<vHavokPhysicsModule*>(vHavokPhysicsModule::GetInstance());
	pMotionInput = (VMotionInputAndroid*)(&VInputManager::GetInputDevice(INPUT_DEVICE_MOTION_SENSOR));
	pMotionInput->SetEnabled(true);
#endif
	entityStack = new EntityStack();
}


GravityRoomController::~GravityRoomController(void)
{
}

bool GravityRoomController::Run(VInputMap* inputMap){
#if defined(_VISION_ANDROID)
	hkvVec3 accel = pMotionInput->GetAcceleration();
	//Multiply it by 1K to increase the intensity
	accel = accel *1000;
	//Havok uses weird axises (axi?) so they had to be swapped and negated
	hkvVec3 gravity = hkvVec3(-1*accel.z,-1*accel.x,accel.y);
	//set the new gravity
	pMod->SetGravity(gravity);
#endif
	if(inputMap->GetTrigger(CUSTOM_CONTROL_ONE)){
		//this->AddCube();
		entityStack->push(this->AddCube());
		//this->RemoveLast();
	}
	if(inputMap->GetTrigger(CUSTOM_CONTROL_TWO)){
		entityStack->push(this->AddSphere(-100.0f, -30, 100));
		//this->AddSphere(-100.0f, -30, 100);
		//this->RemoveLast();
	}
	if(inputMap->GetTrigger(CUSTOM_CONTROL_THREE)){
		entityStack->push(this->AddRagdoll(-100.0f, 5, 100));
		//this->AddRagdoll(-100.0f, 5, 100);
	}
	if(inputMap->GetTrigger(CUSTOM_CONTROL_FOUR)){ // Input not implemented
		this->RemoveLast();
	}
	return true;
}

void GravityRoomController::RemoveLast(void)
{
	VisBaseEntity_cl *ent = entityStack->pop();
	if (ent != NULL)
	{
		ent->DisposeObject();
	}
}

void GravityRoomController::MapTriggers(VInputMap* inputMap){

#if defined(WIN32)
	inputMap->MapTrigger(CUSTOM_CONTROL_ONE, V_KEYBOARD, VInputControl::CT_KB_UP, VInputOptions::Once());
	inputMap->MapTrigger(CUSTOM_CONTROL_TWO, V_KEYBOARD, VInputControl::CT_KB_DOWN, VInputOptions::Once());
	inputMap->MapTrigger(CUSTOM_CONTROL_THREE, V_KEYBOARD, VInputControl::CT_KB_LEFT, VInputOptions::Once());
	inputMap->MapTrigger(CUSTOM_CONTROL_FOUR, V_KEYBOARD, VInputControl::CT_KB_RIGHT, VInputOptions::Once());
#endif
	
#if defined(_VISION_ANDROID)
	int width = Vision::Video.GetXRes();
	int height = Vision::Video.GetYRes();
	VTouchArea* addRagdollArea = new VTouchArea(VInputManager::GetTouchScreen(),VRectanglef((float)width *.8f, (float)height * 0 , (float)width , (float)height * .2f), -900.0f);
	inputMap->MapTrigger(CUSTOM_CONTROL_ONE, addRagdollArea, CT_TOUCH_ANY);
	AddButton("\\GravityRoomGUI\\button.tga", width*.8, 0, width*.2, height*.2);
	
	VTouchArea* addCubeArea = new VTouchArea(VInputManager::GetTouchScreen(),VRectanglef(0.0f, (float)height*.8f, (float)width*.2f, (float)height), -900.0f);
	inputMap->MapTrigger(CUSTOM_CONTROL_TWO, addCubeArea, CT_TOUCH_ANY);
	AddButton("\\GravityRoomGUI\\button.tga", 0,height*.8, width*.2, height*.2);
	
	VTouchArea* addSphereArea = new VTouchArea(VInputManager::GetTouchScreen(),VRectanglef((float)width * .8f,(float)height * .8f , (float)width, (float)height), -900.0f);
	inputMap->MapTrigger(CUSTOM_CONTROL_THREE, addSphereArea, CT_TOUCH_ANY);
	AddButton("\\GravityRoomGUI\\button.tga", width*.8, height*.8, width*.2, height*.2);

	VTouchArea* removeLastArea = new VTouchArea(VInputManager::GetTouchScreen(),VRectanglef( 0.0f, 0.0f ,width * .2f , height * .2f ), -900.0f);
	inputMap->MapTrigger(CUSTOM_CONTROL_FOUR, removeLastArea, CT_TOUCH_ANY);
	AddButton("\\GravityRoomGUI\\button.tga", 0, 0, width*.2, height*.2);
#endif

}

int GravityRoomController::GetEntityCount() {
	return entityStack->getLength();
}