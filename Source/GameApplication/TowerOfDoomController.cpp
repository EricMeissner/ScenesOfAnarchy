#include "GameApplicationPCH.h"
#include "TowerOfDoomController.h"
#include <Vision/Runtime/Framework/VisionApp/VAppImpl.hpp>
#include <Vision/Runtime/EnginePlugins/VisionEnginePlugin/GUI/vGUI.hpp>
#include <Vision/Runtime/EnginePlugins/VisionEnginePlugin/GUI/Controls/VTextControl.hpp>
#include <string>
#include <Vision/Runtime/Framework/VisionApp/Modules/VHelp.hpp>
#include <Vision/Runtime/EnginePlugins/VisionEnginePlugin/GUI/VDlgControlBase.hpp>
#include <Vision/Runtime/EnginePlugins/VisionEnginePlugin/GUI/Controls/VItemContainer.hpp>
#include <Vision/Runtime/Framework/VisionApp/Modules/VDefaultMenu.hpp>
#include <Vision/Runtime/EnginePlugins/VisionEnginePlugin/GUI/VGUIManager.hpp>
#include <iostream>
#include <sstream>

VSmartPtr<VGUIMainContext> spGUIContext;
VDialogPtr spMainDlg;
VTextControl *blocksInput;
const char *blocks;
int value, count;
bool once = true;

void InitGUI()	{

	spGUIContext = new VGUIMainContext(NULL);
	spGUIContext->SetActivate(true);
	VGUIManager::GlobalManager().LoadResourceFile("Dialogs\\MenuSystem.xml");
	spMainDlg = spGUIContext->ShowDialog("Dialogs\\MainMenu.xml");
	VASSERT(spMainDlg);

}
void GrabInput()	{
	
	blocksInput = (VTextControl *)(spMainDlg->Items().FindItem(VGUIManager::GetID("TEXT1")));
	blocks= blocksInput->GetText();
	value = atoi(blocks);	
}
void printInput()	{
	Vision::Message.Print(1, 500, 100, "Input = %d", value);
	
}
void DeInitGUI()	{
	spMainDlg = NULL;
	spGUIContext->SetActivate(false);
	//spGUIContext = NULL;
	
}

TowerOfDoomController::TowerOfDoomController(void)
{
	//VisBaseEntity_cl *pCamera  = Vision::Game.SearchEntity("CameraPosition");
	//Vision::Camera.AttachToEntity(pCamera, hkvVec3(-500.0f, 0.0f, 0.0f));
	blockCount = 0;
	z = 100;
	InitGUI();
}


TowerOfDoomController::~TowerOfDoomController(void)
{
}


void TowerOfDoomController::StackBlocks(int numOfBlocks){
	
	while (blockCount < numOfBlocks){
		VisBaseEntity_cl *ent = Vision::Game.CreateEntity("VisBaseEntity_cl", hkvVec3(0, 0, z), "Models\\Misc\\Cube.Model");
		vHavokRigidBody *cube = new vHavokRigidBody();
		
		cube->Havok_TightFit = true;
		cube->Havok_Mass = 5.0f;
		cube->Shape_Type = ShapeType_BOX;
		cube->Shape_BoxSize = hkvVec3(100,100,100);
		ent->AddComponent(cube);
		++blockCount;
		z = z +100;
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
bool TowerOfDoomController::Run(VInputMap* inputMap){
	if(once)	{
		int dialogResult = spMainDlg->GetDialogResult();
		if(dialogResult==VGUIManager::GetID("ENTER"))	{
			GrabInput();
			printInput();
			this->StackBlocks(value);
			once = false;
		}
	}
	
	else
		DeInitGUI();
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
	return true;
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

int TowerOfDoomController::GetEntityCount() {
	return blockCount;
}