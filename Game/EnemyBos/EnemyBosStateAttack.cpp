#include "stdafx.h"
#include "EnemyBosStateAttack.h"
#include "EnemyBos.h"
#include "EnemyBosStateMachine.h"

EnemyBosStateAttack::EnemyBosStateAttack(EnemyBos* enbos, EnemyBosStateMachine*enstm)
	:IEnemyBosState(enbos, enstm)
{
}


EnemyBosStateAttack::~EnemyBosStateAttack()
{
}

bool EnemyBosStateAttack::Start()
{
	m_enemy->SetAnimation(EnemyBosState::attack);
	CVector3 hit = m_enemy->GetPosition();
	hit.y += 50.0f;
	hit += m_enemy->GetForward() * 50.0f;
	g_battleController->Hit(hit, 0.7f, BattleHit::player);
	return false;
}

void EnemyBosStateAttack::Update()
{
}
