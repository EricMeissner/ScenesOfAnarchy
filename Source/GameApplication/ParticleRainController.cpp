#include "GameApplicationPCH.h"
#include "ParticleRainController.h"


int ballCount = 0;

ParticleRainController::ParticleRainController(void)
{
	//VisBaseEntity_cl *pCamera  = Vision::Game.SearchEntity("CameraPosition");
	//Vision::Camera.AttachToEntity(pCamera, hkvVec3(-500.0f, 0.0f, 0.0f));
}


ParticleRainController::~ParticleRainController(void)
{
}


void ParticleRainController::RainBalls(int numOfBalls){
	while (ballCount < numOfBalls){
		int randomx = rand() % 6000 - 3000;
		int randomy = rand() % 6000 - 2000;
		VisBaseEntity_cl *ent = Vision::Game.CreateEntity("VisBaseEntity_cl", hkvVec3(randomx, randomy, 3000), "Models\\Misc\\Sphere.Model");
		vHavokRigidBody *ball = new vHavokRigidBody();

		ball->Havok_TightFit = true;
		ball->Havok_Mass = 5.0f;
		ball->Havok_Restitution = 1.0f;
		ball->Shape_Type = ShapeType_SPHERE;
		ent->SetScaling(5.0f);

		ent->AddComponent(ball);
		++ballCount;
	}
	ballCount = 0;
}




bool ParticleRainController::Run(VInputMap* inputMap){
	if(inputMap->GetTrigger(CUSTOM_CONTROL_ONE)){
		this->RainBalls(10);
	}
	return true;
}

void ParticleRainController::MapTriggers(VInputMap* inputMap){
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