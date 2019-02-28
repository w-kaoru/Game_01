#pragma once
#include "BattleHit.h"
class BattleController :public Noncopyable
{
public:
	BattleController();
	~BattleController();
	const BattleHit * Create(const CVector3* pos, float radius, std::function<void()> objict, BattleHit::objict_Name name);
	bool Hit(CVector3 pos, BattleHit::objict_Name name);
	void Deleteobjict(const BattleHit* objict);
private:
	std::list<BattleHit*> m_controller;
};
extern BattleController* g_battleController;