#pragma once
#include"IEnemyBosState.h"

class EnemyBosStateAttack : public IEnemyBosState
{
public:
	EnemyBosStateAttack(EnemyBos* enbos, EnemyBosStateMachine* estm);
	~EnemyBosStateAttack();
	bool Start();
	void Update();
};

