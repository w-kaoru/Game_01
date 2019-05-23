#pragma once
#include"IEnState.h"
class EnStateDamage :public IEnState
{
public:
	EnStateDamage(Enemy* en, EnStateMachine*estm);
	~EnStateDamage();
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

