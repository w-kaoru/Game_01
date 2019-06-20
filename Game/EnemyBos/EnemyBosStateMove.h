#pragma once
#include"IEnemyBosState.h"

class EnemyBosStateMove:public IEnemyBosState
{
public:
	EnemyBosStateMove(EnemyBos* enbos, EnemyBosStateMachine* estm);
	~EnemyBosStateMove();
	bool Start() override;
	void Update() override;
};