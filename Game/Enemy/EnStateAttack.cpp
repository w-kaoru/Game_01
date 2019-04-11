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
	return false;
}

void EnStateAttack::Update()
{
}
