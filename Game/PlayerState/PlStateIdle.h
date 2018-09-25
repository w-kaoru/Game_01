#pragma once
#include "IPlState.h"
class PlStateIdle:public IPlState
{
public:
	PlStateIdle(Player *pl,PlStateMachine *psm);
	~PlStateIdle();
	bool Start();
	void Update();
};

