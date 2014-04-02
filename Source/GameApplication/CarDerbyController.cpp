#include "GameApplicationPCH.h"
#include "CarDerbyController.h"

#include <Vision/Runtime/EnginePlugins/Havok/HavokPhysicsEnginePlugin/vHavokPhysicsModule.hpp>

CarDerbyController::CarDerbyController(void)
{
	this->playerCar = new Vehicle(false);
	this->playerCar->Init();
	this->aiCar = new Vehicle(true);
	this->aiCar->Init();
	Vision::Camera.AttachToEntity(this->playerCar->GetCamera(), hkvVec3(-500,0,300));
}


CarDerbyController::~CarDerbyController(void)
{
}

void CarDerbyController::MapTriggers(VInputMap* inputMap) {
	
	//inputMap->MapTrigger(CUSTOM_CONTROL_ONE, V_KEYBOARD, VInputControl::CT_KB_UP);

}
bool CarDerbyController::Run(VInputMap* inputMap){
	   bool brake = false;
    bool reverse = false;
    bool fixedControl = false;
    float steering = 0.f;
    float acceleration = 0.f;
	
	if(inputMap->GetTrigger(CUSTOM_CONTROL_ONE)){
		acceleration = -.3;
	}
	vHavokPhysicsModule::GetInstance()->WaitForSimulationToComplete();
    Vision::Message.Print (1, 10, Vision::Video.GetYRes() - 100, "%f   %f", acceleration,this->playerCar->GetMPH());
	this->playerCar->SetInput(steering,acceleration,brake,reverse,fixedControl);
	this->playerCar->Update();
	this->aiCar->Update();
	return true;
}