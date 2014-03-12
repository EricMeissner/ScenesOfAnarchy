#include "GameApplicationPCH.h"
#include "ParticleRainController.h"
//#include <Vision/Runtime/EnginePlugins/VisionEnginePlugin/Scripting/VScriptComponent.hpp>

int ballCount = 0;
int camNumber = 0;

ParticleRainController::ParticleRainController(void)
{
	VisBaseEntity_cl *pCamera  = Vision::Game.SearchEntity("cam5");
	Vision::Camera.AttachToEntity(pCamera, hkvVec3(0.0f, 0.0f, 0.0f));
	entityCount = 0;
}


ParticleRainController::~ParticleRainController(void)
{
}


void ParticleRainController::RainBalls(int numOfBalls){
	while (ballCount < numOfBalls){
		int randomx = rand() % 6000 - 3255;
		int randomy = rand() % 6000 - 1416;
		VisBaseEntity_cl *ent = Vision::Game.CreateEntity("VisBaseEntity_cl", hkvVec3(randomx, randomy, 3000), "Models\\Misc\\Sphere.Model");
		vHavokRigidBody *ball = new vHavokRigidBody();
		ball->Havok_TightFit = true;
		ball->Havok_Mass = 5.0f;
		ball->Havok_Restitution = 1.0f;
		ball->Shape_Type = ShapeType_SPHERE;
		ent->SetScaling(1.0f);

		ent->AddComponent(ball);
		++ballCount;
	}
	ballCount = 0;
	entityCount += numOfBalls;
}

int ParticleRainController::GetEntityCount() {
	return entityCount;
}
void ParticleRainController::ChangeCam(){
	VisBaseEntity_cl *pCamera;
	switch(camNumber){
	case 0:
		pCamera  = Vision::Game.SearchEntity("cam");
		Vision::Camera.AttachToEntity(pCamera, hkvVec3(0.0f, 0.0f, 0.0f));
		camNumber++; break;
	case 1: 
		pCamera  = Vision::Game.SearchEntity("cam1");
		Vision::Camera.AttachToEntity(pCamera, hkvVec3(0.0f, 0.0f, 0.0f));
		camNumber++; break;
	case 2: 
		pCamera  = Vision::Game.SearchEntity("cam2");
		Vision::Camera.AttachToEntity(pCamera, hkvVec3(0.0f, 0.0f, 0.0f));
		camNumber++; break;
	case 3: 
		pCamera  = Vision::Game.SearchEntity("cam3");
		Vision::Camera.AttachToEntity(pCamera, hkvVec3(0.0f, 0.0f, 0.0f));
		camNumber++; break;
	case 4: 
		pCamera  = Vision::Game.SearchEntity("cam4");
		Vision::Camera.AttachToEntity(pCamera, hkvVec3(0.0f, 0.0f, 0.0f));
		camNumber++; break;
	case 5: 
		pCamera  = Vision::Game.SearchEntity("cam5");
		Vision::Camera.AttachToEntity(pCamera, hkvVec3(0.0f, 0.0f, 0.0f));
		camNumber = 0; break;
	default: break;
	}
}

bool ParticleRainController::Run(VInputMap* inputMap) {
	if(inputMap->GetTrigger(CUSTOM_CONTROL_ONE)){
		this->RainBalls(100);
	}
	if(inputMap->GetTrigger(CUSTOM_CONTROL_TWO)){
		this->ChangeCam();
	}
	return true;
}

void ParticleRainController::MapTriggers(VInputMap* inputMap){

#if defined(WIN32)
	inputMap->MapTrigger(CUSTOM_CONTROL_ONE, V_KEYBOARD, VInputControl::CT_KB_UP, VInputOptions::Once());
	inputMap->MapTrigger(CUSTOM_CONTROL_TWO, V_KEYBOARD, VInputControl::CT_KB_DOWN, VInputOptions::Once());

#endif

#if defined(_VISION_ANDROID)
	int width = Vision::Video.GetXRes();
	int height = Vision::Video.GetYRes();
	
	VTouchArea* addBallsArea = new VTouchArea(VInputManager::GetTouchScreen(),VRectanglef(0.0f, (float)height * 0.5f, (float)width, (float)height), -900.0f);
	inputMap->MapTrigger(CUSTOM_CONTROL_ONE, addBallsArea, CT_TOUCH_ANY);
	VTouchArea* toggleCamera = new VTouchArea(VInputManager::GetTouchScreen(),VRectanglef(0.0f, 0.0f, (float)width, (float)height * 0.5f), -900.0f);
	inputMap->MapTrigger(CUSTOM_CONTROL_TWO, toggleCamera, CT_TOUCH_ANY);

#endif
}