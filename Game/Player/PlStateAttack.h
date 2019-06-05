#pragma once
#include "IPlState.h"
class PlStateAttack:public IPlState
{
public:
	PlStateAttack(Player *pl, PlStateMachine *psm);
	~PlStateAttack();
	bool Start();
	void Update();
	void SetAttack(int hit)
	{
		m_atkHit = hit;
	}
	void SetHit(bool atk)
	{
		m_atk = atk;
	}
	bool GetHit()
	{
		return m_atk;
	}
private:
	CVector3 m_attckPos = CVector3::Zero();
	int m_hitTiming = 0;
	bool m_atk = false;
	bool m_isAtk = false;
	int m_atkHit = 0;
};

