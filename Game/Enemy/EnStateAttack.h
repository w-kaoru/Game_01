#pragma once
#include"IEnState.h"

class EnStateAttack : public IEnState
{
public:
	EnStateAttack(Enemy* en, EnStateMachine* estm);
	~EnStateAttack();
	bool Start();
	void Update();
};

