#pragma once
#include "IController.h"
#include "Vehicle.h";

const int CAR_ACCELERATION = 3;

class CarDerbyController : public IController {
public:
	CarDerbyController(void);
	virtual ~CarDerbyController(void);
	virtual void MapTriggers(VInputMap* inputMap) HKV_OVERRIDE;
	virtual bool Run(VInputMap* inputMap) HKV_OVERRIDE;
private:
	Vehicle* playerCar;
	Vehicle* aiCar;
};

