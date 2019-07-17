#include "stdafx.h"
#include "EnemyBosStateDeath.h"
#include "EnemyBos.h"
#include "EnemyBosStateMachine.h"
#include "../Game.h"
#include "../GameEnd.h"
#include "../Stage/Stage.h"
#include "../Player/Player.h"


EnemyBosStateDeath::EnemyBosStateDeath(EnemyBos* enbos, EnemyBosStateMachine* enstm)
	:IEnemyBosState(enbos, enstm)
{
}

EnemyBosStateDeath::~EnemyBosStateDeath()
{
}

bool EnemyBosStateDeath::Start()
{
	static bool f=false;
	if (f == false) {
		f = true;
		float exp = max(10.0f, float(m_enemy->GetStatus()->GetLv()));
		g_gameObjM->FindGO<Player>("Player")->EXP(exp);
	}
	return false;
}

void EnemyBosStateDeath::Update()
{
	m_enemy->SetAnimation(EnemyBosState::AnimationState::AnimDeath);
	if (m_enemy->GetAnimation().IsPlaying() == false) {
		if (g_gameObjM->FindGO<Stage>("Stage")->GetStageType() == StageType::Stage_Ground) {
			g_gameObjM->DeleteGO(g_gameObjM->FindGO<Game>("Game"));
			g_gameObjM->NewGO<GameEnd>()->SetGameEnd(GameEnd::GameEndState::gameCleared);
		}
		if (g_gameObjM->FindGO<Stage>("Stage")->GetStageType() == StageType::Stage_Dungeon) {
			g_gameObjM->FindGO<Stage>("Stage")->Release();
			g_gameObjM->FindGO<Stage>("Stage")->StageSelect(StageType::Stage_Ground);
		}
	}
}
