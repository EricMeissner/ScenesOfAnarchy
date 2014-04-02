/*
*
* Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
* prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
* Product and Trade Secret source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2013 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
*
*/

#include "GameApplicationPCH.h"
#include <Vision/Runtime/Framework/VisionApp/VAppImpl.hpp> 
#include <Vision/Runtime/Framework/VisionApp/Modules/VHelp.hpp>
#include <Vision/Runtime/EnginePlugins/VisionEnginePlugin/Input/VFreeCamera.hpp>
#include <Vision/Runtime/EnginePlugins/VisionEnginePlugin/Input/VVirtualThumbStick.hpp>
#include "IController.h"
#include "GravityRoomController.h"
#include "TowerOfDoomController.h"
#include "ParticleRainController.h"
#include "MenuController.h"
#include "Constants.h"
#include "WaterSimulationController.h"
#include "CarDerbyController.h"
#include <Vision/Runtime/EnginePlugins/Havok/HavokPhysicsEnginePlugin/vHavokSync.hpp>
#include <sstream>
#include <iostream>
#include <fstream>
#include <ctime>
using namespace std;
// Use the following line to initialize a plugin that is statically linked.
// Note that only Windows platform links plugins dynamically (on Windows you can comment out this line).
VIMPORT IVisPlugin_cl* GetEnginePlugin_GamePlugin();
VIMPORT IVisPlugin_cl* GetEnginePlugin_vHavok();

const char *sceneNames[7]={"Scenes/Default.vscene", "Scenes/GravityRoom.vscene","Scenes/TowerOfDoom.vscene","Scenes/ParticleRain.vscene","","Scenes/CarDerby.vscene", "Scenes/WaterSimulation.vscene"};

class ProjectTemplateApp : public VAppImpl
{
public:
	ProjectTemplateApp(){}
	virtual ~ProjectTemplateApp() {}

	virtual void SetupAppConfig(VisAppConfig_cl& config) HKV_OVERRIDE;
	virtual void PreloadPlugins() HKV_OVERRIDE;

	virtual void Init() HKV_OVERRIDE;
	virtual void AfterSceneLoaded(bool bLoadingSuccessful) HKV_OVERRIDE;
	virtual bool Run() HKV_OVERRIDE;
	virtual void DeInit() HKV_OVERRIDE;
	void SwitchScene(int sceneID);
	void SwitchController(int sceneID);
	void UpdateFPS();
	void UpdateStats(); // as UpdateFPS, but with number of objects added to the output.
	void RecordFPS();
	IController* controller;
	MenuController* menu;
	int currentSceneID;
	float m_iFrameCounter;
	float m_fTimeAccumulator;
	float m_fCurrentFrameTime;
	float m_fCurrentFps;
	float previousFps;
	ofstream stats;
	clock_t begin, end;

	///changes by carlos
	void addButtons();
	private:
	//State m_state;
	//VDialog* m_pMainMenuDialog;
	///end changes by carlos
};

VAPP_IMPLEMENT_SAMPLE(ProjectTemplateApp);

void ProjectTemplateApp::SetupAppConfig(VisAppConfig_cl& config)
{
	// Set custom file system root name ("havok_sdk" by default)
	config.m_sFileSystemRootName = "template_root";

	// Set the initial starting position of our game window and other properties
	// if not in fullscreen. This is only relevant on windows
	config.m_videoConfig.m_iXRes = 1280; // Set the Window size X if not in fullscreen.
	config.m_videoConfig.m_iYRes = 620;  // Set the Window size Y if not in fullscreen.
	config.m_videoConfig.m_iXPos = 50;   // Set the Window position X if not in fullscreen.
	config.m_videoConfig.m_iYPos = 50;   // Set the Window position Y if not in fullscreen.

	// Name to be displayed in the windows title bar.
	config.m_videoConfig.m_szWindowTitle = "StandAlone Project Template";

	config.m_videoConfig.m_bWaitVRetrace = true;

	// Fullscreen mode with current desktop resolution

#if defined(WIN32)
	/*
	DEVMODEA deviceMode;
	deviceMode = Vision::Video.GetAdapterMode(config.m_videoConfig.m_iAdapter);
	config.m_videoConfig.m_iXRes = deviceMode.dmPelsWidth;
	config.m_videoConfig.m_iYRes = deviceMode.dmPelsHeight;
	config.m_videoConfig.m_bFullScreen = true;
	*/
#endif

}

void ProjectTemplateApp::PreloadPlugins()
{
	// Use the following line to load a plugin. Remember that, except on Windows platform, in addition
	// you still need to statically link your plugin library (e.g. on mobile platforms) through project
	// Properties, Linker, Additional Dependencies.
	VISION_PLUGIN_ENSURE_LOADED(GamePlugin);
	
		  VISION_PLUGIN_ENSURE_LOADED(vHavok);
	  
}



//---------------------------------------------------------------------------------------------------------
// Init function. Here we trigger loading our scene
//---------------------------------------------------------------------------------------------------------
void ProjectTemplateApp::Init()
{
  VISION_HAVOK_SYNC_STATICS();
  VISION_HAVOK_SYNC_PER_THREAD_STATICS(vHavokPhysicsModule::GetInstance());
	//Initiliaze FPS variables to 0.
	m_iFrameCounter=0;
	m_fTimeAccumulator=0;
	m_fCurrentFrameTime=0;
	m_fCurrentFps=0;
	stats.open("stats.csv");
	stats << "Scene Name, FPS, Frame Time, Body Count, Time Elapsed\n";

	//Initliaze the menu
	menu = new MenuController(this->GetContext());
	currentSceneID=MAIN_MENU;
	this->controller = NULL;

	VisAppLoadSettings settings(sceneNames[currentSceneID]);
	settings.m_customSearchPaths.Append(":template_root/Assets");
	LoadScene(settings);
	menu->Enable();



}
//---------------------------------------------------------------------------------------------------------
// Gets called after the scene has been loaded
//---------------------------------------------------------------------------------------------------------
void ProjectTemplateApp::AfterSceneLoaded(bool bLoadingSuccessful)
{
	// Define some help text
	//VArray<const char*> help;
	//help.Append("How to use this demo...");
	//help.Append("");
	//RegisterAppModule(new VHelp(help));




	// Create a mouse controlled camera (set above the ground so that we can see the ground)
	//Vision::Game.CreateEntity("VisMouseCamera_cl", hkvVec3(-600.0f, 0.0f, 170.0f));
	// Add other initial game code here
	// [...]

	//changes by carlos
	/*controller = new GravityRoomController();
	controller->MapTriggers(this->GetInputMap());*/
	

	//end changes by carlos
}



//---------------------------------------------------------------------------------------------------------
// Main Loop of the application until we quit
//---------------------------------------------------------------------------------------------------------
bool ProjectTemplateApp::Run()
{	
	if(currentSceneID==MAIN_MENU){
		//Do menu stuff
		int newSceneID = menu->Run();
		if(newSceneID != MAIN_MENU){
			SwitchScene(newSceneID);
			SwitchController(newSceneID);
			menu->Disable();
		}
	}
	else{
		previousFps = m_fCurrentFps;
		UpdateStats();
		if(m_fCurrentFps != previousFps){ 
			RecordFPS();
		}
		bool doNotExit = controller->Run(this->GetInputMap());
		if(!doNotExit){
			SwitchScene(MAIN_MENU);
			controller = NULL;
			menu->Enable();
		}
		if(ProjectTemplateApp::GetInputMap()->GetTrigger(VAPP_EXIT)){
			SwitchScene(MAIN_MENU);
			currentSceneID = MAIN_MENU;
			controller = NULL;
			menu->Enable();
		}
	}
	return true;
}

void ProjectTemplateApp::UpdateFPS(){
	m_iFrameCounter++;
	m_fTimeAccumulator += Vision::GetUITimer()->GetTimeDifference();

	if (m_fTimeAccumulator >= 1.0f)
	{
		m_fCurrentFrameTime = m_fTimeAccumulator / m_iFrameCounter;
		m_fCurrentFps = m_iFrameCounter / m_fTimeAccumulator;

		m_fTimeAccumulator = 0.0f;
		m_iFrameCounter = 0;
	}
	Vision::Message.Print(1, 10, Vision::Video.GetYRes() - 35, "FPS : %.1f\nFrame Time : %.2f", m_fCurrentFps, m_fCurrentFrameTime * 1000.0f);
}

void ProjectTemplateApp::UpdateStats(){
	m_iFrameCounter++;
	m_fTimeAccumulator += Vision::GetUITimer()->GetTimeDifference();

	if (m_fTimeAccumulator >= 1.0f)
	{
		m_fCurrentFrameTime = m_fTimeAccumulator / m_iFrameCounter;
		m_fCurrentFps = m_iFrameCounter / m_fTimeAccumulator;

		m_fTimeAccumulator = 0.0f;
		m_iFrameCounter = 0;
	}
	end = clock();
	Vision::Message.Print(1, 10, Vision::Video.GetYRes() - 55, "FPS : %.1f\nFrame Time : %.2f\nEntity Count : %d", m_fCurrentFps, m_fCurrentFrameTime * 1000.0f, controller->GetEntityCount());
}

void ProjectTemplateApp::RecordFPS()
{
	std::ostringstream ss;
	double elapsed_secs = double(end-begin)/CLOCKS_PER_SEC;
	ss << sceneNames[currentSceneID];
	ss << ", ";
	ss << m_fCurrentFps;
	ss << ", ";
	ss << m_fCurrentFrameTime * 1000.0f;
	ss << ", ";
	ss << controller->entityStack->getLength();
	ss << ", ";
	ss << elapsed_secs;
	std::string s = ss.str() + "\n";
	stats << s;
}
void ProjectTemplateApp::SwitchScene(int sceneID){
		if(this->controller != NULL){
			this->controller->DeInitGUI();
	}
	this->m_pSceneLoader->UnloadScene();
	VisAppLoadSettings settings(sceneNames[sceneID]);
	settings.m_customSearchPaths.Append(":template_root/Assets");
	LoadScene(settings);
	this->currentSceneID=sceneID;
	begin = clock();
}

void ProjectTemplateApp::SwitchController(int sceneID){

	switch(sceneID){
	case GRAVITY_ROOM:
		this->controller = new GravityRoomController();
		this->controller->MapTriggers(this->GetInputMap());
		break;
	case TOWER_OF_DOOM:
		this->controller = new TowerOfDoomController();
		this->controller->MapTriggers(this->GetInputMap());
		//Extremely hacky, Might be dangerous.
		((TowerOfDoomController*)this->controller)->InitMenu(this->GetContext());
		break;
	case PARTICLE_RAIN:
		this->controller = new ParticleRainController();
		break;
	case TUMBLER:
		break;
	case CAR_DERBY:
		this->controller = new CarDerbyController();
		break;
	case WATER_SIMULATION:
		this->controller = new WaterSimulationController();
		this->controller->MapTriggers(this->GetInputMap());
		break;
	default:
		break;
	}
	this->controller->MapTriggers(this->GetInputMap());
}


void ProjectTemplateApp::DeInit()
{
	// De-Initialization
	
	VISION_HAVOK_UNSYNC_ALL_STATICS()
  VISION_HAVOK_UNSYNC_PER_THREAD_STATICS(vHavokPhysicsModule::GetInstance());
	
}

/*
* Havok SDK - Base file, BUILD(#20131218)
* 
* Confidential Information of Havok.  (C) Copyright 1999-2013
* Telekinesys Research Limited t/a Havok. All Rights Reserved. The Havok
* Logo, and the Havok buzzsaw logo are trademarks of Havok.  Title, ownership
* rights, and intellectual property rights in the Havok software remain in
* Havok and/or its suppliers.
* 
* Use of this software for evaluation purposes is subject to and indicates
* acceptance of the End User licence Agreement for this product. A copy of
* the license is included with this software and is also available from salesteam@havok.com.
* 
*/
