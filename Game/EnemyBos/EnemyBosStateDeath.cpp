#include "stdafx.h"
#include "EnemyBosStateDeath.h"
#include "EnemyBos.h"
#include "EnemyBosStateMachine.h"
#include "../Game.h"
#include "../GameEnd.h"
#include "../Stage/Stage.h"


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
		g_gameObjM->DeleteGO(g_gameObjM->FindGO<Game>("Game"));
		if (g_gameObjM->FindGO<Stage>("Stage")->GetStageType() == Stage::StageType::Stage_Ground) {
			g_gameObjM->NewGO<GameEnd>()->SetGameEnd(GameEnd::GameEndState::gameCleared);
		}
		if (g_gameObjM->FindGO<Stage>("Stage")->GetStageType() == Stage::StageType::Stage_Dungeon) {
			g_gameObjM->NewGO<GameEnd>()->SetGameEnd(GameEnd::GameEndState::Ground);
		}
	}
}
