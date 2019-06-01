#pragma once
#include "IPlState.h"
class PlStateDamage : public IPlState
{
public:
	PlStateDamage(Player *pl, PlStateMachine *psm);
	~PlStateDamage();
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

	float GetDamage()
	{
		return m_damage;
	}
	void SetDamage(float damage)
	{
		m_damage = damage;
	}

private:
	bool m_isDamage = false;
	float m_damage = 0.0f;
};