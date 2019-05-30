#pragma once
#include"IEnState.h"
class EnStateDamage :public IEnState
{
public:
	EnStateDamage(Enemy* en, EnStateMachine*estm);
	~EnStateDamage();
	bool Start();
	void Update();
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

