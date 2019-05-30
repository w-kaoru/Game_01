#pragma once
#include "IEnemyBosState.h"
class EnemyBosStateDeath :public IEnemyBosState
{
public:
	EnemyBosStateDeath(EnemyBos* enbos, EnemyBosStateMachine* enstm);
	~EnemyBosStateDeath();
	bool Start();
	void Update();
};