
#pragma once
#include"IEnemyBosState.h"
class EnemyBosStateDamage :public IEnemyBosState
{
public:
	EnemyBosStateDamage(EnemyBos* enbos, EnemyBosStateMachine*estm);
	~EnemyBosStateDamage();
	bool Start() override;
	void Update() override;
	void SetDamageFlag(bool damage)
	{
		m_isDamage = damage;
	}
	bool GetDamageFlag()
	{
		return m_isDamage;
	}

	void SetDamage(float damage)
	{
		m_damage = damage;
	}
private:
	bool m_isDamage = false;
	float m_damage = 0.0f;
};

