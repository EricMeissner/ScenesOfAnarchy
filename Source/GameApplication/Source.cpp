#include "GameApplicationPCH.h"
#include "ParticleDropController.h"


int blockCount = 0;
int z = 100;

TowerOfDoomController::TowerOfDoomController(void)
{
	//VisBaseEntity_cl *pCamera  = Vision::Game.SearchEntity("CameraPosition");
	//Vision::Camera.AttachToEntity(pCamera, hkvVec3(-500.0f, 0.0f, 0.0f));
}


TowerOfDoomController::~TowerOfDoomController(void)
{
}


void TowerOfDoomController::StackBlocks(int numOfBlocks){
	VisBaseEntity_cl * blocks[500];
	while (blockCount < numOfBlocks){
		VisBaseEntity_cl *ent = Vision::Game.CreateEntity("VisBaseEntity_cl", hkvVec3(0, 0, z), "Models\\Misc\\Cube.Model");
		
		blocks[blockCount] = ent;
		++blockCount;
		z = z +100;
	}
	for(int i = 0; i <numOfBlocks;i++) {
		vHavokRigidBody *cube = new vHavokRigidBody();
		
		cube->Havok_TightFit = true;
		cube->Havok_Mass = 5.0f;
		cube->Shape_Type = ShapeType_BOX;
		cube->Shape_BoxSize = hkvVec3(100,100,100);
		blocks[i]->AddComponent(cube);
		
	}
}

void MakeEntityPickable(VisBaseEntity_cl *ent){
	ent->SetTraceAccuracy(VIS_TRACEACC_POLYGONS);
	
}

void TowerOfDoomController::Swipe(VInputMap *inputMap){
	float x = inputMap->GetTrigger(X);
	float y = inputMap->GetTrigger(Y);

	//hkvVec3 point = project3D(x, y, 1000);
}

hkvVec3 project3D(float x, float y, float fDist)
{
    hkvVec3 traceStart = Vision::Camera.GetCurrentCameraPosition();
    hkvVec3 traceDir;
    Vision::Contexts.GetCurrentContext()->GetTraceDirFromScreenPos(x, y, traceDir, fDist);
    hkvVec3 traceEnd = traceStart + traceDir;    
    return hkvVec3(traceEnd);
}

void TowerOfDoomController::Run(VInputMap* inputMap){
	this->StackBlocks(500);

	//if(inputMap->GetTrigger(CUSTOM_CONTROL_ONE)){
	//	//this->AddCube();
	//	this->RemoveLast();
	//}
	//if(inputMap->GetTrigger(CUSTOM_CONTROL_TWO)){
	//	this->AddSphere();
	//	//this->RemoveLast();

	//}
	//if(inputMap->GetTrigger(CUSTOM_CONTROL_THREE)){
	//	this->AddRagdoll();
	//}
	//if(inputMap->GetTrigger(PICK)){
	//	this->Swipe(inputMap);
	//}
}

void TowerOfDoomController::MapTriggers(VInputMap* inputMap){
#if defined(WIN32)
	inputMap->MapTrigger(CUSTOM_CONTROL_ONE, V_KEYBOARD, VInputControl::CT_KB_UP, VInputOptions::Once());
	inputMap->MapTrigger(CUSTOM_CONTROL_TWO, V_KEYBOARD, VInputControl::CT_KB_KP_DOWN, VInputOptions::Once());
	inputMap->MapTrigger(CUSTOM_CONTROL_THREE, V_KEYBOARD, VInputControl::CT_KB_LEFT, VInputOptions::Once());
	// For swiping
	inputMap->MapTrigger(X, V_MOUSE, VInputControl::CT_MOUSE_ABS_X);
	inputMap->MapTrigger(Y, V_MOUSE, VInputControl::CT_MOUSE_ABS_Y);
	inputMap->MapTrigger(PICK, V_MOUSE, VInputControl::CT_MOUSE_LEFT_BUTTON);
#endif

#if defined(_VISION_ANDROID)
	int width = Vision::Video.GetXRes();
	int height = Vision::Video.GetYRes();
	
	VTouchArea* addCubeArea = new VTouchArea(VInputManager::GetTouchScreen(),VRectanglef(0.0f, (float)height*.70f, (float)width*.35f, (float)height), -900.0f);
	inputMap->MapTrigger(CUSTOM_CONTROL_ONE, addCubeArea, CT_TOUCH_ANY);
	
#endif
}