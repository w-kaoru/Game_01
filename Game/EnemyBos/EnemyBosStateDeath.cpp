#include "stdafx.h"
#include "EnemyBosStateDeath.h"
#include "EnemyBos.h"
#include "EnemyBosStateMachine.h"
#include "../Game.h"
#include "../GameEnd.h"


EnemyBosStateDeath::EnemyBosStateDeath(EnemyBos* enbos, EnemyBosStateMachine* enstm)
	:IEnemyBosState(enbos, enstm)
{
}

EnemyBosStateDeath::~EnemyBosStateDeath()
{
}

bool EnemyBosStateDeath::Start()
{
	return false;
}

void EnemyBosStateDeath::Update()
{
	m_enemy->SetAnimation(EnemyBosState::AnimationState::AnimDeath);
	if (m_enemy->GetAnimation().IsPlaying() == false) {
		g_gameObjM->DeleteGO(g_gameObjM->FindGO<Game>());
		g_gameObjM->NewGO<GameEnd>()->SetGameEnd(GameEnd::GameEndState::gameCleared);
	}
}
