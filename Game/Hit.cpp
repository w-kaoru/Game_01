#include "stdafx.h"
#include "Hit.h"


Hit::Hit()
{
}


Hit::~Hit()
{
}

void Hit::Create(const CVector3 * pos, float radius, std::function<void(float damage)> object, objict_Name name)
{
	m_pos = pos;
	m_radius = radius;
	m_fuk = object;
	m_name = name;
}

bool Hit::HitTest(CVector3 pos, float damage)
{
	auto rengs = *m_pos - pos;
	//‰~‚Å”»’è
	if (rengs.Length() <= m_radius)
	{
		m_fuk(damage);
	}
	return false;
}
