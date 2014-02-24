#include "GameApplicationPCH.h"
#include "ParticleDropController.h"


int ballCount = 0;
int h = 100;

ParticleDropController::ParticleDropController(void)
{
	//VisBaseEntity_cl *pCamera  = Vision::Game.SearchEntity("CameraPosition");
	//Vision::Camera.AttachToEntity(pCamera, hkvVec3(-500.0f, 0.0f, 0.0f));
}


ParticleDropController::~ParticleDropController(void)
{
}


void ParticleDropController::RainBalls(int numOfBalls){
	while (ballCount < numOfBalls){
		VisBaseEntity_cl *ent = Vision::Game.CreateEntity("VisBaseEntity_cl", hkvVec3(0, 0, h+3000), "Models\\Misc\\Cube.Model");
		vHavokRigidBody *ball = new vHavokRigidBody();

		ball->Havok_TightFit = true;
		ball->Havok_Mass = 5.0f;
		//ball->Shape_Type = ShapeType_SPHERE;

		ent->AddComponent(ball);
		++ballCount;
		h = h +100;
	}
	h = 100;
}




void ParticleDropController::Run(VInputMap* inputMap){
		
	this->RainBalls(50);

	if(inputMap->GetTrigger(CUSTOM_CONTROL_ONE)){
	}
}

void ParticleDropController::MapTriggers(VInputMap* inputMap){
#if defined(WIN32)
	inputMap->MapTrigger(CUSTOM_CONTROL_ONE, V_KEYBOARD, VInputControl::CT_KB_UP, VInputOptions::Once());

#endif

#if defined(_VISION_ANDROID)
	int width = Vision::Video.GetXRes();
	int height = Vision::Video.GetYRes();
	
	VTouchArea* addCubeArea = new VTouchArea(VInputManager::GetTouchScreen(),VRectanglef(0.0f, 0.0f, (float)width, (float)height), -900.0f);
	inputMap->MapTrigger(CUSTOM_CONTROL_ONE, addCubeArea, CT_TOUCH_ANY);
	
#endif
}