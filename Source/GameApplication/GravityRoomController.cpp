#include "GameApplicationPCH.h"
#include "GravityRoomController.h"

GravityRoomController::GravityRoomController(void)
{
	VisBaseEntity_cl *pCamera  = Vision::Game.SearchEntity("CameraPosition");
	Vision::Camera.AttachToEntity(pCamera, hkvVec3::ZeroVector());
#if defined(_VISION_ANDROID)
	pMod = static_cast<vHavokPhysicsModule*>(vHavokPhysicsModule::GetInstance());
	pMotionInput = (VMotionInputAndroid*)(&VInputManager::GetInputDevice(INPUT_DEVICE_MOTION_SENSOR));
	pMotionInput->SetEnabled(true);
#endif
}


GravityRoomController::~GravityRoomController(void)
{
}

void GravityRoomController::Run(VInputMap* inputMap){
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
		this->RemoveLast();
	}
	if(inputMap->GetTrigger(CUSTOM_CONTROL_TWO)){
		this->AddSphere(-100.0f, -30, 100);
		//this->RemoveLast();

	}
	if(inputMap->GetTrigger(CUSTOM_CONTROL_THREE)){
		this->AddRagdoll(-100.0f, 5, 100);
	}
}

void GravityRoomController::MapTriggers(VInputMap* inputMap){

#if defined(WIN32)
	inputMap->MapTrigger(CUSTOM_CONTROL_ONE, V_KEYBOARD, VInputControl::CT_KB_UP, VInputOptions::Once());
	inputMap->MapTrigger(CUSTOM_CONTROL_TWO, V_KEYBOARD, VInputControl::CT_KB_DOWN, VInputOptions::Once()); // Typo on the input control fixed.
	inputMap->MapTrigger(CUSTOM_CONTROL_THREE, V_KEYBOARD, VInputControl::CT_KB_LEFT, VInputOptions::Once());
#endif


#if defined(_VISION_ANDROID)
	int width = Vision::Video.GetXRes();
	int height = Vision::Video.GetYRes();
	VTouchArea* addRagdollArea = new VTouchArea(VInputManager::GetTouchScreen(),VRectanglef((float)width * .65f, (float)height * .70f, (float)width, (float)height), -900.0f);
	inputMap->MapTrigger(CUSTOM_CONTROL_ONE, addRagdollArea, CT_TOUCH_ANY);
	VTouchArea* addCubeArea = new VTouchArea(VInputManager::GetTouchScreen(),VRectanglef(0.0f, (float)height*.70f, (float)width*.35f, (float)height), -900.0f);
	inputMap->MapTrigger(CUSTOM_CONTROL_TWO, addCubeArea, CT_TOUCH_ANY);
	VTouchArea* addSphereArea = new VTouchArea(VInputManager::GetTouchScreen(),VRectanglef((float)width * .65f, 0.0f, (float)width, (float)height * .35f), -900.0f);
	inputMap->MapTrigger(CUSTOM_CONTROL_THREE, addSphereArea, CT_TOUCH_ANY);
#endif
}