/*
 *
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Product and Trade Secret source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2013 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 *
 */


// Basic template to base a project EXE on.
//
//
#include "GameApplicationPCH.h"
#include <cstring>
#include <string>
#include <Vision\Runtime\EnginePlugins\Havok\HavokPhysicsEnginePlugin\vHavokPhysicsModule.hpp>
using namespace std;

#ifdef WIN32
#include <Vision/Runtime/EnginePlugins/RemoteInputEnginePlugin/IVRemoteInput.hpp>
#endif

bool TDScene = false;
bool addOnce = true;
int count = 10;

enum G_S //Game state
{
  GS_INIT,
  GS_MAIN_MENU,
  GS_GAME,
  GS_GRAVITY_ROOM,
  GS_TOD
};

G_S g_state = GS_INIT;


#if defined(_VISION_PSP2)
  VMotionInputPSP2* pMotionInput = NULL;
#elif defined(_VISION_IOS)
  VMotionInputIOS* pMotionInput = NULL;
#elif defined(_VISION_ANDROID)
  VMotionInputAndroid* pMotionInput = NULL;
#elif defined(_VISION_WINRT)
  VMotionInputWinRT* pMotionInput = NULL;
#elif defined(_VISION_TIZEN)
  VMotionInputTizen* pMotionInput = NULL;
#elif defined(WIN32)
  VMotionInputPC* pMotionInput = NULL;
  IVRemoteInput* pRemoteInput = NULL;
#endif

//============================================================================================================
// Properties for start up. Some of the settings are not relevant for mobile devices
//============================================================================================================
int windowSizeX    = 1024;               // Set the Window size X if not in fullscreen.
int windowSizeY    = 768;                // Set the Window size Y if not in fullscreen.
int windowPosX    = 500;                 // Set the Window position X if not in fullscreen.
int windowPosy    = 50;                  // Set the Window position Y if not in fullscreen.

char name[]      = "Project Anarchizzle";  // Name to be displayed in the windows title bar.

float cameraInitX = 0;                    //
float cameraInitY = 0;                    //
float cameraInitZ = 170;                  // Set our camera above the ground so that we can see 
                                          // the ground.

// Use the following line to initialize a plugin that is statically linked. 
// Note that only Windows platform links plugins dynamically (on Windows you can comment out this line).
VIMPORT IVisPlugin_cl* GetEnginePlugin_GamePlugin();
VIMPORT IVisPlugin_cl* GetEnginePlugin_VisionEnginePlugin();
VIMPORT IVisPlugin_cl* GetEnginePlugin_vHavok();
//VIMPORT IVisPlugin_cl* GetEnginePlugin_vHavokAi(); 

VisSampleAppPtr spApp;
VSmartPtr<VGUIMainContext> spGUIContext;
VDialogPtr spMainDlg;

void InitGUI()

{
	

  spGUIContext = new VGUIMainContext(NULL);

  spGUIContext->SetActivate(true); 

  // Load some default resources (like fonts or the image for the cursor)

  VGUIManager::GlobalManager().LoadResourceFile("Dialogs\\MenuSystem.xml");
  //VGUIManager::GlobalManager().LoadResourceFile("Dialogs\\MenuTest.xml");

  // Load and show the Main Menu layout
  spMainDlg = spGUIContext->ShowDialog("Dialogs\\MainMenu.xml");

  VASSERT(spMainDlg);

}

void DeInitGUI()

{
  spMainDlg = NULL;                  // destroy the Main Dialog object
  
  spGUIContext->SetActivate(false);  // deactivate the GUI context ...

  spGUIContext = NULL;               // ... and destroy it

}



//---------------------------------------------------------------------------------------------------------
// Init function. Here we trigger loading our scene
//---------------------------------------------------------------------------------------------------------
VISION_INIT
{
	VISION_PLUGIN_ENSURE_LOADED(GamePlugin);
VISION_PLUGIN_ENSURE_LOADED(VisionEnginePlugin);
VISION_PLUGIN_ENSURE_LOADED(vHavok);

    // Create our new application.
  spApp = new VisSampleApp();
  
  // set the initial starting position of our game window
  // and other properties if not in fullscreen. This is only relevant on windows
#if defined(WIN32)
  spApp->m_appConfig.m_videoConfig.m_iXPos = windowPosX;
  spApp->m_appConfig.m_videoConfig.m_iYPos = windowPosy;
  spApp->m_appConfig.m_videoConfig.m_szWindowTitle = name;
#endif

  // Set the executable directory the current directory
  VisionAppHelpers::MakeEXEDirCurrent();

  // Set the paths to our stand alone version to override the VisSAampleApp paths.
  // The paths are platform dependent
#if defined(WIN32)
  const VString szRoot = "..\\..\\..\\..";
  Vision::File.AddDataDirectory( szRoot + "\\Assets" );
  Vision::File.AddDataDirectory( szRoot + "\\Data\\Vision\\Base" );
  Vision::File.AddDataDirectory( szRoot + "\\AnarchySDK" );




#elif defined(_VISION_ANDROID)
  VString szRoot = VisSampleApp::GetApkDirectory();
  szRoot += "?assets";
  Vision::File.AddDataDirectory( szRoot + "\\Assets" );
  // "/Data/Vision/Base" is always added by the sample app
  
#elif defined(_VISION_TIZEN)
  VString szRoot = VisSampleApp::GetDataRootDirectory();
  Vision::File.AddDataDirectory( szRoot + "\\Assets" );
  // "/Data/Vision/Base" is always added by the sample app

#elif defined(_VISION_IOS)
  // setup directories, does nothing on platforms other than iOS,
  // pass true if you want load from the documents directory
  VISION_SET_DIRECTORIES(false);
  VString szRoot = VisSampleApp::GetRootDirectory();
  // our deploy script always copies the asset data below the "Data" folder
  Vision::File.AddDataDirectory( szRoot + "/Data/Assets" );
  // "/Data/Vision/Base" is always added by the sample app
  
#endif

#if defined(VISION_OUTPUT_DIR)
  // Set the output directory manually since VSAMPLE_CUSTOMDATADIRECTORIES was specified
  // at the initialization.
  Vision::File.SetOutputDirectory(VISION_OUTPUT_DIR);
  Vision::File.AddDataDirectory(VISION_OUTPUT_DIR);
#endif

  spApp->LoadVisionEnginePlugin();

  // Use the following line to load a plugin. Remember that, except on Windows platform, in addition
  // you still need to statically link your plugin library (e.g. on mobile platforms) through project
  // Properties, Linker, Additional Dependencies.
  VISION_PLUGIN_ENSURE_LOADED(GamePlugin);

  // Init the application and point it to the start up scene.
  if (!spApp->InitSample( "", NULL, VSampleFlags::VSAMPLE_INIT_DEFAULTS|VSampleFlags::VSAMPLE_CUSTOMDATADIRECTORIES,windowSizeX,windowSizeY))
    return false;
	
  InitGUI();
  return true;
}

//---------------------------------------------------------------------------------------------------------
// Gets called after the scene has been loaded
//---------------------------------------------------------------------------------------------------------

VISION_SAMPLEAPP_AFTER_LOADING
{
	if(g_state == GS_INIT){

		Vision::InitWorld();
		g_state = GS_MAIN_MENU;
		#if defined(_VISION_PSP2)
		  pMotionInput = (VMotionInputPSP2*)(&VInputManager::GetInputDevice(INPUT_DEVICE_MOTION_SENSOR));
		  pMotionInput->SetEnabled(true);
		#elif defined(_VISION_IOS)
		  pMotionInput = (VMotionInputIOS*)(&VInputManager::GetInputDevice(INPUT_DEVICE_MOTION_SENSOR));
		  pMotionInput->SetEnabled(true);
		#elif defined(_VISION_WINRT)
		  pMotionInput = (VMotionInputWinRT*)(&VInputManager::GetInputDevice(INPUT_DEVICE_MOTION_SENSOR));
		#elif defined(WIN32)
		  pMotionInput = (VMotionInputPC*)(&VInputManager::GetInputDevice(INPUT_DEVICE_MOTION_SENSOR));
		#elif defined(_VISION_ANDROID)
		  pMotionInput = (VMotionInputAndroid*)(&VInputManager::GetInputDevice(INPUT_DEVICE_MOTION_SENSOR));
		  pMotionInput->SetEnabled(true);
		#elif defined(_VISION_TIZEN)
		  pMotionInput = (VMotionInputTizen*)(&VInputManager::GetInputDevice(INPUT_DEVICE_MOTION_SENSOR));
		  pMotionInput->SetEnabled(true);
		#endif 
	}

	else if (g_state == GS_GAME || g_state == GS_TOD || g_state == GS_GRAVITY_ROOM){
		VisBaseEntity_cl *pCamera = spApp->EnableMouseCamera();
		pCamera->SetPosition(-700, 0, 100);
		pCamera->SetOrientation(0, 0, 0);

		if(TDScene == true){
			spGUIContext->SetActivate(true);
			spMainDlg = spGUIContext->ShowDialog("Dialogs//TDXML");
			VASSERT(spMainDlg);
			spApp->DisableMouseCamera();
	    
		}

		if( g_state == GS_GRAVITY_ROOM){
			//spGUIContext->SetActivate(true);
			//spMainDlg = spGUIContext->ShowDialog("Dialogs//TDXML");
			//VASSERT(spMainDlg);



			spApp->DisableMouseCamera();
	    
		}
	}

}


//---------------------------------------------------------------------------------------------------------
// main loop of the application until we quit
//---------------------------------------------------------------------------------------------------------

VISION_SAMPLEAPP_RUN
{
	switch(g_state){
		
	case GS_MAIN_MENU:
		{
			VASSERT(spMainDlg);
			int dlgResult = spMainDlg->GetDialogResult();
		
			if(dlgResult == VGUIManager::GetID("LOADTD")){
				char* str = new char[10]; //Ignore this, it's for debugging purposes 
				//itoa(dlgResult, str, 10); //It displays the number of the button 
				spApp->AddHelpText(str); 
				
				spGUIContext->CloseDialog(spMainDlg);
				spMainDlg = NULL;
				spGUIContext->SetActivate(false);
				g_state = GS_TOD;
				spApp->LoadScene("Scenes//TowerOfDoom");
				TDScene = true;
				return true;
			}
			else if(dlgResult == VGUIManager::GetID("LOADGR")){
				char* str = new char[10];
			//	itoa(dlgResult, str, 10);
				spApp->AddHelpText(str);

				spGUIContext->CloseDialog(spMainDlg);
				spMainDlg = NULL;
				spGUIContext->SetActivate(false);
				g_state = GS_GRAVITY_ROOM;
				spApp->LoadScene("Scenes//GravityRoom");
				return true;
			}
		 }
	case GS_GAME:
		{
			int TDDlgResult = spMainDlg->GetDialogResult();
		 
			if(TDScene == true && TDDlgResult == VGUIManager::GetID("ADD")){	
				char* str = new char[10];
				//itoa(TDDlgResult, str, 10);

				VisBaseEntity_cl *ent2 = Vision::Game.CreateEntity("VisBaseEntity_cl", hkvVec3(-100.0f, (float)count*5, (float)count*10), "Cube.MODEL");
			count++;
			}
			spApp->Run(); //spApp->Run() applies the changes to the current frame
			spMainDlg->SetDialogResult(VGUIManager::ID_OK);		//it is important to apply changes first and THEN "deactivate" dialog result otherwise it won't display new cube. 	
		}
	case GS_TOD:
		{
			int TDDlgResult = spMainDlg->GetDialogResult();
		 
			if(TDScene == true && TDDlgResult == VGUIManager::GetID("ADD")){	
				char* str = new char[10];
				//itoa(TDDlgResult, str, 10);

				VisBaseEntity_cl *ent2 = Vision::Game.CreateEntity("VisBaseEntity_cl", hkvVec3(-100.0f, (float)count*5, (float)count*10), "Cube.MODEL");
			count++;
			}
			spApp->Run(); //spApp->Run() applies the changes to the current frame
			spMainDlg->SetDialogResult(VGUIManager::ID_OK);		//it is important to apply changes first and THEN "deactivate" dialog result otherwise it won't display new cube. 	
			
		}
	case GS_GRAVITY_ROOM:
		{
			#if defined(_VISION_IOS) || defined(_VISION_PSP2) || defined(WIN32) || defined(_VISION_ANDROID) || defined(_VISION_WIIU) || defined(_VISION_TIZEN)
				hkvVec3 vAcceleration;
			#if defined(_VISION_WIIU)
				vAcceleration.x = -VInputManager::GetDRC(V_DRC_FIRST).GetControlValue(CT_PAD_WIIU_ACCELERATION_X, 0.0f);
				vAcceleration.y = VInputManager::GetDRC(V_DRC_FIRST).GetControlValue(CT_PAD_WIIU_ACCELERATION_Z, 0.0f);
				vAcceleration.z = VInputManager::GetDRC(V_DRC_FIRST).GetControlValue(CT_PAD_WIIU_ACCELERATION_Y, 0.0f);
			#else
				vAcceleration = pMotionInput->GetAcceleration();
			#endif

			vHavokPhysicsModule* mod = static_cast<vHavokPhysicsModule*>(spApp->GetPhysicsModule());
			hkvVec3 gravity = vAcceleration;
			//hkvVec3 gravity = hkvVec3(0.0f,0.0f,-980.0f);
			mod->SetGravity(gravity);
			#endif
			spApp->Run(); //spApp->Run() applies the changes to the current frame
			//spMainDlg->SetDialogResult(VGUIManager::ID_OK);		//it is important to apply changes first and THEN "deactivate" dialog result otherwise
		}
	}
  return spApp->Run();
}

VISION_DEINIT
{

	spGUIContext->SetActivate(false);
	spGUIContext = NULL;
	VGUIManager::GlobalManager().CleanupResources();
	spMainDlg = NULL;

  // Deinit the application
	  spApp->DeInitSample();
  spApp = NULL;

  Vision::Plugins.UnloadAllEnginePlugins();

  return 0;
}

VISION_MAIN_DEFAULT

/*
 * Havok SDK - Base file, BUILD(#20131022)
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


















///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// DO NOT TOUCH ANYTHING AFTER THIS! //////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


