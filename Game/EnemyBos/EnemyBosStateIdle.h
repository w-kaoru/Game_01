
#pragma once
#include"IEnemyBosState.h"
class EnemyBosStateIdle :public IEnemyBosState
{
public:
	EnemyBosStateIdle(EnemyBos* enbos, EnemyBosStateMachine*estm);
	~EnemyBosStateIdle();
	bool Start();
	void Update();
};

