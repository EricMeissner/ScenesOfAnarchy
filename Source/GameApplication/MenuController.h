#pragma once
#include <Vision\Runtime\Framework\VisionApp\VAppModule.hpp>

class MenuController
{
public:
	MenuController(void);
	MenuController(VAppMenuContext* c);
	~MenuController(void);
	void Enable();
	void Disable();
	int DialogResultToSceneID(int dialogResult);
	int Run();
	VAppMenuContext* spContext;
	VDialog* m_pMainMenuDialog;
	bool enabled;
	void BackButton();
};

