#include "stdafx.h"
#include "PlStateAttack.h"
#include "Player.h"

PlStateAttack::PlStateAttack(Player *pl, PlStateMachine *psm) :
	IPlState(pl, psm)
{
}


PlStateAttack::~PlStateAttack()
{
}

bool PlStateAttack::Start()
{
	return false;
}

void PlStateAttack::Update()
{

	m_moveSpeed = m_player->GetMoveSpeed();
	m_moveSpeed *= 0.0f;
	//攻撃の当たり判定のポジション
	//プレイヤーのポジションの少し前の位置
	m_player->SetAnimation(PlayerState::AnimationState::AnimAttack);
	m_attckPos.x = m_player->GetPosition().x + m_player->GetForward().x * 65.0f;
	m_attckPos.z = m_player->GetPosition().z + m_player->GetForward().z * 65.0f;
	m_attckPos.y = 100.0f;
	m_hitTiming++;
	//攻撃されてからあたったタイミングで攻撃したい（簡易版）
	if (m_hitTiming == 15) {
		//攻撃をヒットさせる。
		g_hitObject->HitTest(m_attckPos, m_player->GetStatus()->GetAtk(), Hit::enemy);
		m_hitTiming == 0;
	}
	if (m_player->GetAnimation().IsPlaying() == false) {
		m_atk = false;
		//攻撃の間隔を0に戻す。
		m_hitTiming = 0;
	}
	m_player->SetMoveSpeed(m_moveSpeed);
}
