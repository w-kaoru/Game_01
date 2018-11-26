#pragma once
#include"IEnState.h"

class EnStateMove:public IEnState
{
public:
	EnStateMove(Enemy* en,EnStateMachine*estm);
	~EnStateMove();
	bool Start();
	void Update();
};