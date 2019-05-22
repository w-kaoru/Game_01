#pragma once
#include "IPlState.h"
class PlStateDamage : public IPlState
{
public:
	PlStateDamage(Player *pl, PlStateMachine *psm);
	~PlStateDamage();
	bool Start();
	void Update();
};