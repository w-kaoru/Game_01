#pragma once
#include "IPlState.h"

class PlStateMove :public IPlState
{
public:
	PlStateMove(Player *pl,PlStateMachine *psm);
	~PlStateMove();
	bool Start();
	void Update();
};
