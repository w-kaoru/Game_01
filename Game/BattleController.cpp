#include "stdafx.h"
#include "BattleController.h"


BattleController::BattleController()
{
}


BattleController::~BattleController()
{
}

const BattleHit * BattleController::Create(const CVector3 * pos, float radius, std::function<void()> objict, BattleHit::objict_Name name)
{
	auto hitter = new BattleHit;
	hitter->Create(pos, radius, objict, name);
	m_controller.push_back(hitter);
	return hitter;
}

bool BattleController::Hit(CVector3 pos, BattleHit::objict_Name name)
{
	for (auto hitter : m_controller)
	{
		if (hitter->Getobjict_Name() == name) {
			if (hitter->Hit(pos))
				return true;
		}
	}
	return false;
}

void BattleController::Deleteobjict(const BattleHit * objict)
{
	m_controller.erase(std::find(m_controller.begin(), m_controller.end(), objict));
	delete objict;
}
