#include "GameApplicationPCH.h"
#include "MenuController.h"
#include "Constants.h"
#include <string>
#include <iostream>
MenuController::MenuController(void)
{

}

MenuController::MenuController(VAppMenuContext* c){
	this->spContext = c;
	this->enabled = false;
	VGUIManager::GlobalManager().LoadResourceFile("Assets\\MenuSystem.xml");
}


MenuController::~MenuController(void)
{
}

void MenuController::Enable(){
	//VDialogCollection openDialogs = spContext->
	//for(int i = 0; i < openDialogs.Count(); i++){
		//spContext->CloseDialog(openDialogs.GetAt(i));
	//}
	this->m_pMainMenuDialog = spContext->ShowDialog("Assets\\MainMenu.xml");
	this->enabled=true;
	
	/*Screen Dimensions */
	int x = Vision::Video.GetXRes();
	int y = Vision::Video.GetYRes();

	/* Create Buttons */
	VPushButton* buttonGR = new VPushButton();
	VPushButton* buttonToD = new VPushButton();
	VPushButton* buttonScene3 = new VPushButton();
	VPushButton* buttonScene4 = new VPushButton();
	VPushButton* buttonScene5 = new VPushButton();
	VPushButton* buttonScene6 = new VPushButton();

	VPushButton* buttonArray[6] = {buttonGR, buttonToD, buttonScene3, buttonScene4, buttonScene5, buttonScene6};
	char *sceneNames[6] = {"Gravity Room", "Tower of Doom", "Particle Rain", "Tumbler", "Car Derby", "Water Simulation"};

	for(int i = 0; i < 6; i++){
		buttonArray[i]->SetPosition((x/2) - x/4, y * i/6 + y/20);		
		buttonArray[i]->SetText(sceneNames[i]);
		buttonArray[i]->SetSize(x/2, y/20);		
		buttonArray[i]->SetDialogResult(i+1);	//every scene has a dialog result used for loading call
		buttonArray[i]->SetEnabled(true); //not sure if this is necessary

		buttonArray[i]->Text().SetFont(Vision::Fonts.FindFont("MenuFont"));
		buttonArray[i]->Text().SetColor(VColorRef (100,40,245,255));
		
		buttonArray[i]->Text().SetHorizontalAlignment(VisFont_cl::ALIGN_CENTER);
		buttonArray[i]->Text().SetVerticalAlignment(VisFont_cl::ALIGN_CENTER);
		
		m_pMainMenuDialog->AddControl(buttonArray[i]);
	}
}

void MenuController::Disable(){
	this->spContext->CloseDialog(this->m_pMainMenuDialog);
	this->m_pMainMenuDialog = NULL;
	this->enabled = false;
}

int MenuController::DialogResultToSceneID(int dialogResults){
/*	if(dialogResults == 1){ //Gravity Room = 1
		return GRAVITY_ROOM;
	}
	else if(dialogResults == 2){ //Tower of Doom = 2
		return TOWER_OF_DOOM;
	}
	//TODO: add cases 3-6
	else if(dialogResults == 6){
		return WATER_SIMULATION;
	}*/
	return MAIN_MENU;
}
int MenuController::Run(){
	return this->m_pMainMenuDialog->GetDialogResult(); //nothing else is really necessary besides this
}

void MenuController::BackButton(){
	
}
