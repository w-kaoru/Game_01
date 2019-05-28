#pragma once
#include "IPlState.h"
class PlStateDeath : public IPlState
{
public:
	PlStateDeath(Player *pl, PlStateMachine *psm);
	~PlStateDeath();
	bool Start();
	void Update();
};

