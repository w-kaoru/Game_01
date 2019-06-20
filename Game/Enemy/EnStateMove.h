#pragma once
#include"IEnState.h"

class EnStateMove:public IEnState
{
public:
	EnStateMove(Enemy* en,EnStateMachine* estm);
	~EnStateMove();
	bool Start() override;
	void Update() override;
};