#pragma once
#include "IPlState.h"

class PlStateRun:public IPlState
{
public:
	PlStateRun(Player *pl,PlStateMachine *psm);
	~PlStateRun();
	bool Start();
	void Update();
};

