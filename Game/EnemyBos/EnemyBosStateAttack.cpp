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
	//m_enemy->SetAnimation(EnemyBosState::attack);
	return false;
}

void EnemyBosStateAttack::Update()
{
	m_AttackTiming++;
	//攻撃の間隔
	if (m_AttackTiming == m_atkAnimStart) {
		//攻撃アニメーションの再生するためにステートの変更
		m_enemy->SetAnimation(EnemyBosState::AnimationState::AnimAttack);
		//m_ensm.Change(EnemyState::MoveState::attack);
		m_attackFlag = true;
	}
	//攻撃されてからあたったタイミングで攻撃したい（簡易版）
	if (m_AttackTiming == m_atkHit) {
		//当たったと思われるタイミングで
		CVector3 hit = m_enemy->GetPosition();
		hit.y += 50.0f;
		hit += m_enemy->GetForward() * 50.0f;
		//攻撃をヒットさせる。
		g_hitObject->HitTest(hit, 20.0f, m_enemy->GetStatus()->GetAtk(), Hit::player);
		//攻撃の間隔を0に戻す。
		m_AttackTiming = 0;
		m_isAtk = true;
	}
	if (m_isAtk) {
		m_AttackTiming = 0;
	}
	if (m_attackFlag == false) {
		m_enemy->SetAnimation(EnemyBosState::AnimationState::AnimIdle);
		m_isAtk = false;
	}
}
