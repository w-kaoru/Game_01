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
	m_attckPos.x = m_player->GetPosition().x + (m_player->GetForward().x * 65.0f);
	m_attckPos.z = m_player->GetPosition().z + (m_player->GetForward().z * 65.0f);
	m_attckPos.y = m_player->GetPosition().y + 50.0f;
	switch (m_combo)
	{
	case Combo::attack_00:
		m_atkHit = 14;
		m_comboTiming = 15;
		m_hitTimer++;
		m_comboTimer++;
		m_player->SetAnimation(PlayerState::AnimationState::AnimAttack_00, 0.1f);
		//攻撃されてからあたったタイミングで攻撃したい（簡易版）
		if (m_hitTimer == m_atkHit) {
			//攻撃をヒットさせる。
			g_hitObject->HitTest(m_attckPos, 30.0f, m_player->GetStatus()->GetAtk(), Hit::enemy);
			m_isAtk = true;
		}
		if (m_isAtk) {
			m_hitTimer = 0;
		}
		if (!m_player->GetAnimation().IsPlaying()) {
			//アニメーションが終わるため初期化
			m_atk = false;
			//攻撃の間隔を0に戻す。
			m_hitTimer = 0;
			m_comboTimer = 0;
			m_isAtk = false;
		}
		if (m_comboTimer > m_comboTiming) {
			if (g_pad[0].IsTrigger(enButtonX)) {
				//コンボでアニメーションの変更のため初期化
				m_combo = Combo::attack_01;
				m_hitTimer = 0;
				m_comboTimer = 0;
				m_isAtk = false;
			}
		}
		break;
	case Combo::attack_01:
		m_atkHit = 10;
		m_comboTiming = 14;
		m_hitTimer++;
		m_comboTimer++;
		m_player->SetAnimation(PlayerState::AnimationState::AnimAttack_01, 0.1f);
		//攻撃されてからあたったタイミングで攻撃したい（簡易版）
		if (m_hitTimer == m_atkHit) {
			//攻撃をヒットさせる。
			g_hitObject->HitTest(m_attckPos, 30.0f, m_player->GetStatus()->GetAtk(), Hit::enemy);
			m_isAtk = true;
		}
		if (m_isAtk) {
			m_hitTimer = 0;
		}
		if (!m_player->GetAnimation().IsPlaying()) {
			//アニメーションが終わるため初期化
			m_combo = Combo::attack_00;
			m_atk = false;
			//攻撃の間隔を0に戻す。
			m_hitTimer = 0;
			m_comboTimer = 0;
			m_isAtk = false;
		}
		if (m_comboTimer > m_comboTiming) {
			if (g_pad[0].IsTrigger(enButtonX)) {
				//コンボでアニメーションの変更のため初期化
				m_combo = Combo::attack_02;
				m_hitTimer = 0;
				m_comboTimer = 0;
				m_isAtk = false;
			}
		}
		break;
	case Combo::attack_02:
		m_atkHit = 8;
		m_hitTimer++;
		m_player->SetAnimation(PlayerState::AnimationState::AnimAttack_02, 0.1f);
		//攻撃されてからあたったタイミングで攻撃したい（簡易版）
		if (m_hitTimer == m_atkHit) {
			//攻撃をヒットさせる。
			g_hitObject->HitTest(m_player->GetPosition(), 70.0f, m_player->GetStatus()->GetAtk() / 1.5f, Hit::enemy);
			m_isAtk = true;
			m_hitTimer = 0;
		}
		if (!m_player->GetAnimation().IsPlaying()) {
			//アニメーションが終わるため初期化
			m_combo = Combo::attack_00;
			m_atk = false;
			//攻撃の間隔を0に戻す。
			m_hitTimer = 0;
			m_comboTimer = 0;
			m_isAtk = false;
		}
		break;
	}
	m_player->SetMoveSpeed(m_moveSpeed);
}
