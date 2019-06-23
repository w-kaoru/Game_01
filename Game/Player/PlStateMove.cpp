#include "stdafx.h"
#include "PlStateMove.h"
#include "Player.h"

PlStateMove::PlStateMove(Player *pl, PlStateMachine *psm)
	:IPlState(pl,psm)
{
}


PlStateMove::~PlStateMove()
{
}

bool PlStateMove::Start()
{
	return false;
}

void PlStateMove::Update()
{
	m_moveSpeed = m_player->GetMoveSpeed();
	m_rotation = m_player->GetRotation();
	//スティックの入力量を受け取る。
	float lStick_x = g_pad[0].GetLStickXF();
	float lStick_y = g_pad[0].GetLStickYF();
	//カメラの前方方向と右方向を取得。
	CVector3 cameraForward = g_camera3D.GetForward();
	CVector3 cameraRight = g_camera3D.GetRight();
	//XZ平面での前方方向、右方向に変換する。
	cameraForward.y = 0.0f;
	cameraForward.Normalize();
	cameraRight.y = 0.0f;
	cameraRight.Normalize();
	//XZ成分の移動速度をクリア。
	m_moveSpeed.x = 0.0f;
	m_moveSpeed.z = 0.0f;
	//奥方向への移動速度を代入。
	m_moveSpeed += cameraForward * lStick_y * m_player->GetStatus()->GetAgi();
	//右方向への移動速度を加算。
	m_moveSpeed += cameraRight * lStick_x * m_player->GetStatus()->GetAgi();
	//向きを変える。
	if (fabsf(m_moveSpeed.x) > 0.1f
		|| fabsf(m_moveSpeed.z) > 0.1f) {
		auto angle = atan2f(m_moveSpeed.x, m_moveSpeed.z);
		m_rotation.SetRotation(CVector3::AxisY(), angle);
		//走るアニメーションの再生するためにステートの変更
		m_player->SetAnimation(PlayerState::AnimationState::AnimRun);
	}
	else {
		//待機アニメーションの再生するためにステートの変更
		m_player->SetAnimation(PlayerState::AnimationState::AnimIdle);
	}
	m_player->SetMoveSpeed(m_moveSpeed);
	m_player->SetRotation(m_rotation);
}
