
#pragma once
#include"IEnemyBosState.h"
class EnemyBosStateDamage :public IEnemyBosState
{
public:
	EnemyBosStateDamage(EnemyBos* enbos, EnemyBosStateMachine*estm);
	~EnemyBosStateDamage();
	bool Start();
	void Update();
	void SetDamage(bool damage)
	{
		m_isDamage = damage;
	}
	bool GetDamage()
	{
		return m_isDamage;
	}
private:
	bool m_isDamage = false;
};

