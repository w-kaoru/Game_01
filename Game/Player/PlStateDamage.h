#pragma once
#include "IPlState.h"
class PlStateDamage : public IPlState
{
public:
	PlStateDamage(Player *pl, PlStateMachine *psm);
	~PlStateDamage();
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