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
	//m_enemy->SetAnimation(EnemyState::attack);
	return false;
}

void EnStateAttack::Update()
{
	m_AttackTiming++;
	//攻撃の間隔
	if (m_AttackTiming == m_atkAnimStart) {
		//攻撃アニメーションの再生するためにステートの変更
		m_enemy->SetAnimation(EnemyState::AnimationState::AnimAttack);
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
		g_hitObject->HitTest(hit, 40.0f, m_enemy->GetStatus()->GetAtk(), Hit::player);
		//攻撃の間隔を0に戻す。
		m_AttackTiming = 0;
		m_isAtk = true;
	}
	if (m_isAtk) {
		m_AttackTiming = 0;
	}
	if (m_attackFlag == false) {
		m_enemy->SetAnimation(EnemyState::AnimationState::AnimIdle);
		m_isAtk = false;
	}
}
