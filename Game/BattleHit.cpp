#include "stdafx.h"
#include "BattleHit.h"


BattleHit::BattleHit()
{
}


BattleHit::~BattleHit()
{
}

void BattleHit::Create(const CVector3 * pos, float radius, std::function<void(float damage)> object, objict_Name name)
{
	m_pos = pos;
	m_radius = radius;
	m_fuk = object;
	m_name = name;
}

bool BattleHit::Hit(CVector3 pos, float damage)
{
	auto rengs = *m_pos - pos;
	//‰~‚Å”»’è
	if (rengs.Length() <= m_radius)
	{
		m_fuk(damage);
	}
	return false;
}
