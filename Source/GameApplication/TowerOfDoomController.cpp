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
#include <stdlib.h>
//
//void DeInitGUI()	{
//	spMainDlg = NULL;
//	spGUIContext->SetActivate(false);
//	//spGUIContext = NULL;
//	
//}


TowerOfDoomController::TowerOfDoomController(void)
{
	VisBaseEntity_cl *pCamera  =  Vision::Game.CreateEntity("VisBaseEntity_cl", hkvVec3(0, 0, 0));
	Vision::Camera.AttachToEntity(pCamera, hkvVec3(-1500.0f, 0.0f, 500.0f));
	blockCount = 0;
	z = 100;
	menuMode = true;
	menuDisplayed = false;
	blocksStacked = false;
}


TowerOfDoomController::~TowerOfDoomController(void)
{
	this->spContext->CloseDialog(this->dialog);
	dialog = NULL;
}


void TowerOfDoomController::StackBlocks(int numOfBlocks){

	for(int i = 0; i < numOfBlocks; i++){
		VisBaseEntity_cl *ent = Vision::Game.CreateEntity("VisBaseEntity_cl", hkvVec3(0, 0, z), "Models\\Misc\\Cube.Model");
		vHavokRigidBody *cube = new vHavokRigidBody();
		cube->Havok_TightFit = true;
		cube->Havok_Mass = 5.0f;
		cube->Shape_Type = ShapeType_BOX;
		cube->Shape_BoxSize = hkvVec3(100,100,100);
		ent->AddComponent(cube);
		ent->SetObjectKey("Blocks");
		++blockCount;
		z = z +100;
	}
	blocksStacked = true;
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
	if(menuMode){
		if(menuDisplayed){
			if(this->dialog->GetDialogResult() == 42){
				this->blockCount = atoi((((VTextControl *)this->dialog->Items().FindItem(VGUIManager::GetID("Input")))->GetText()));
				this->spContext->CloseDialog(this->dialog);
				menuMode = false;
				menuDisplayed = false;
			}
		}
		else{
			EnableMenu();
		}
	}
	else{
		if(!blocksStacked){
			
				Vision::Game.RemoveEntity("Blocks");
			this->StackBlocks(this->blockCount);
		}
		else{
			if(inputMap->GetTrigger(CUSTOM_CONTROL_ONE)){
				menuMode = true;
				blocksStacked = false;
				z = 50;
			}
		}

	}
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
	VTouchArea* addRagdollArea = new VTouchArea(VInputManager::GetTouchScreen(),VRectanglef((float)width *.8f, (float)height * .8f , (float)width *.8f +  143, (float)height * .8f+59), -900.0f);
	inputMap->MapTrigger(CUSTOM_CONTROL_ONE, addRagdollArea, CT_TOUCH_ANY);
	AddButton("\\Dialogs\\reset.png", width*.8, (float)height * .8f,0,0);
	
#endif
}

int TowerOfDoomController::GetEntityCount() {
	return blockCount;
}

void TowerOfDoomController::EnableMenu(){

	this->dialog = spContext->ShowDialog("Assets\\Dialogs\\InputDialog.xml");
	int x = Vision::Video.GetXRes();
	int y = Vision::Video.GetYRes();
	//To properly scale the dialog box grab input object
	//this->dialog->Items().FindItem(//Find input by id);

	VImageControl* logo = new VImageControl();
	logo->SetPosition(50,50);
	logo->SetSize(300,75);
	logo->Image().SetTexture(Vision::TextureManager.Load2DTexture("Assets\\Dialogs\\TOD.png"));
	this->dialog->AddControl(logo);

	VPushButton* accept = new VPushButton();
	accept->SetPosition(90, 350);
	accept->SetSize(300,75);
	accept->SetEnabled(true);
	accept->SetText("Accept");
	accept->SetDialogResult(42);
	this->dialog->AddControl(accept);

	menuDisplayed = true;
}


void TowerOfDoomController::InitMenu(VAppMenuContext* context){
	spContext = context;
}