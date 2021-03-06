#pragma once
#include"IEnState.h"

class EnStateAttack : public IEnState
{
public:
	EnStateAttack(Enemy* en, EnStateMachine* estm);
	~EnStateAttack();
	bool Start() override;
	void Update() override;
	void SetAttack(int hit,int start)
	{
		m_atkHit = hit;
		m_atkAnimStart = start;
	}
	void SetTiming(int timing)
	{
		m_AttackTiming = timing;
	}
	bool GetAtkFlag()
	{
		return m_attackFlag;
	}
	void SetAtkFlag(bool flag)
	{
		m_attackFlag = flag;
	}
private:
	int m_AttackTiming = 0;
	int m_atkAnimStart = 0;
	int m_atkHit = 0;
	bool m_attackFlag = false;
	bool m_isAtk = false;
};

