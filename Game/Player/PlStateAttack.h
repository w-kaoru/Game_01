#pragma once
#include "IPlState.h"
class PlStateAttack:public IPlState
{
public:
	PlStateAttack(Player *pl, PlStateMachine *psm);
	~PlStateAttack();
	bool Start();
	void Update();
};

