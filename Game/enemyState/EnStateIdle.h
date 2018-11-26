#pragma once
#include"IEnState.h"
class EnStateIdle :public IEnState
{
public:
	EnStateIdle(Enemy* en, EnStateMachine*estm);
	~EnStateIdle();
	bool Start();
	void Update();
};

