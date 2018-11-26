#include "stdafx.h"
#include "Player.h"
#include "PlStateMachine.h"


Player::Player() :m_stMa(this)
{
	//cmoファイルの読み込み。
	m_model.Init(L"Assets/modelData/ToonRTS_demo_Knight.cmo", enFbxUpAxisZ);

	//tkaファイルの読み込み。
	m_animationClips[PlayerState::idle].Load(L"Assets/animData/plidle.tka");
	m_animationClips[PlayerState::idle].SetLoopFlag(true);

	m_animationClips[PlayerState::run].Load(L"Assets/animData/plrun.tka");
	m_animationClips[PlayerState::run].SetLoopFlag(true);

	//m_animationClips[PlayerState::jump].Load(L"Assets/animData/jump.tka");
	//m_animationClips[PlayerState::jump].SetLoopFlag(false);

	m_animationClips[PlayerState::attack].Load(L"Assets/animData/plattack.tka");
	m_animationClips[PlayerState::attack].SetLoopFlag(false);

	//アニメーションの初期化。
	m_animation.Init(
		m_model,			//アニメーションを流すスキンモデル。
							//これでアニメーションとスキンモデルが関連付けされる。
		m_animationClips,	//アニメーションクリップの配列。
		4					//アニメーションクリップの数。
	);
	//m_scale *= 4;
}
Player::~Player()
{
	m_charaCon.RemoveRigidBoby();
}
bool Player::Start()
{
	m_position.y = 200.0f;
	//キャラクターコントローラーの初期化。
	m_charaCon.Init(10.0f, 50.0f, m_position);
	m_stMa.Start();
	return true;
}
//移動
void Player::Move()
{
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
	m_moveSpeed += cameraForward * lStick_y * 400.0f;	//奥方向への移動速度を代入。
	m_moveSpeed += cameraRight * lStick_x * 400.0f;		//右方向への移動速度を加算。
	m_moveSpeed.y -= 980.0f * (1.0f / 60.0f);
	//キャラコンを使って移動する。
	m_position = m_charaCon.Execute(1.0f / 60.0f, m_moveSpeed);
	//向きも変える。
	if (fabsf(m_moveSpeed.x) > 0.1f
		|| fabsf(m_moveSpeed.z) > 0.1f) {
		auto angle = atan2f(m_moveSpeed.x, m_moveSpeed.z);
		m_rotation.SetRotation(CVector3::AxisY(), angle);
	}
	if (g_pad[0].IsTrigger(enButtonA) == true
		&& m_charaCon.IsOnGround() == true
		) {
		PlayerState::AniMove::jump;
	}
	if (m_charaCon.IsJump() == true && m_charaCon.IsOnGround() == false) {
		m_moveSpeed.y += 450.0f;
	}
}


//ジャンプ
//void Player::Jump()
//{
//	if (m_charaCon.IsOnGround() == false) {
//		m_moveSpeed.y += 450.0f;
//		//キャラコンを使って移動する。
//		m_position = m_charaCon.Execute(1.0f / 60.0f, m_moveSpeed);
//
//	}
//	else
//	{
//		if (m_animation.IsPlaying() == false) {
//			if (fabsf(m_moveSpeed.x) > 0.1f
//				|| fabsf(m_moveSpeed.z) > 0.1f
//				) {
//				animove = run;
//			}
//			else {
//
//				animove = idle;
//			}
//		}
//	}
//}
//攻撃
//void Player::Attack()
//{
//	if (m_animation.IsPlaying() == false) {
//		if (fabsf(m_moveSpeed.x) > 0.1f
//			|| fabsf(m_moveSpeed.z) > 0.1f
//			) {
//			animove = run;
//		}
//		else {
//
//			animove = idle;
//		}
//	}
//}

void Player::Update()
{	
	Move();
	m_stMa.Update();
	//ワールド行列の更新。
	m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);
	//アニメーションを流す。
	m_animation.Update(1.0f / 30.0f);
}

void Player::Draw()
{
	m_model.Draw(
		g_camera3D.GetViewMatrix(), 
		g_camera3D.GetProjectionMatrix()
	);
}
