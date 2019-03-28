#include "stdafx.h"
#include "EnStateAttack.h"
#include "Enemy.h"
#include "EnStateMachine.h"

EnStateAttack::EnStateAttack(Enemy* en, EnStateMachine*enstm)
	:IEnState(en, enstm)
{
}


EnStateAttack::~EnStateAttack()
{
}

bool EnStateAttack::Start()
{
	m_enemy->SetAnimation(EnemyState::attack);
	CVector3 hit = m_enemy->GetPosition();
	hit.y += 50.0f;
	hit += m_enemy->GetForward() * 50.0f;
	g_battleController->Hit(hit, BattleHit::player);
	return false;
}

void EnStateAttack::Update()
{
}
