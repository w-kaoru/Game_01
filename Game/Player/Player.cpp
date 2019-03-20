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
	//m_scale *= 0.1f;
	m_hpSprite.Init(L"Assets/sprite/hp_gauge.dds", 100.0f, 10.0f);
	m_hpFrameSprite.Init(L"Assets/sprite/hp_frame.dds", 100.0f, 10.0f);
}
Player::~Player()
{
	g_graphicsEngine->GetShadowMap()->UnregistShadowCaster(&m_model);
	m_charaCon.RemoveRigidBoby();
}
bool Player::Start()
{
	//ステータス設定
	//m_status.Lv = 1;
	m_hp =  6.0f;			//体力
	m_hpFrame = m_hp;
	m_atk = 70.0f;			//攻撃力
	m_def = 50.0f;			//防御力
	m_agi = 400.0f;			//素早さ
	m_position.y = 200.0f;
	//キャラクターコントローラーの初期化。
	m_charaCon.Init(10.0f, 50.0f, m_position);
	m_stMa.Start();
	/*m_battle->SetHP(m_hp);
	m_battle->SetATK(m_atk);
	m_battle->SetDEF(m_def);*/

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
	m_moveSpeed += cameraForward * lStick_y * m_agi;	//奥方向への移動速度を代入。
	m_moveSpeed += cameraRight * lStick_x * m_agi;		//右方向への移動速度を加算。
	//m_moveSpeed.y -= 980.0f * (1.0f / 60.0f);
	//キャラコンを使って移動する。
	m_position = m_charaCon.Execute(1.0f / 60.0f, m_moveSpeed);
	//向きも変える。
	if (fabsf(m_moveSpeed.x) > 0.1f
		|| fabsf(m_moveSpeed.z) > 0.1f) {
		auto angle = atan2f(m_moveSpeed.x, m_moveSpeed.z);
		m_rotation.SetRotation(CVector3::AxisY(), angle);
		m_stMa.Change(PlayerState::AniMove::run);
	}
	else {
		m_stMa.Change(PlayerState::AniMove::idle);
	}

	/*if (m_charaCon.IsJump() == true && m_charaCon.IsOnGround() == false) {
		m_moveSpeed.y += 450.0f;
	}*/
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
void Player::Attack()
{
	m_stMa.Change(PlayerState::AniMove::attack);
}

void Player::HP_Gauge()
{
	m_hpFrameSprite.Update(
		{ -640.0f,360.0f, 0.0f },
		CQuaternion::Identity(),
		{ m_hpFrame, 1.5f, 1.0f },
		{0.0f,1.0f}
	);
	m_hpSprite.Update(
		{ -640.0f,360.0f, 0.0f },
		CQuaternion::Identity(),
		{ m_hp, 1.5f, 1.0f },
		{ 0.0f,1.0f }
	);
	m_hpFrameSprite.Draw(
		g_camera2D.GetViewMatrix(),
		g_camera2D.GetProjectionMatrix()
	);
	m_hpSprite.Draw(
		g_camera2D.GetViewMatrix(),
		g_camera2D.GetProjectionMatrix()
	);
}

void Player::Damage()
{
	m_damageTiming++;
	if (m_damageTiming == 50) {
		m_hp -= 0.1f;
	}
}

void Player::Update()
{
	m_damageTiming = 0.0f;
	m_stMa.Update();
	//m_right = CVector3::AxisX();
	//m_right.Normalize();
	////y軸で上を求める。
	//m_up = CVector3::AxisY();
	//m_up.Normalize();
	//z軸で前を求める。
	m_forward = CVector3::AxisY();
	m_forward.Normalize();
	
	//当たった？
	m_hit = g_battleController->Create(
		&m_position, 300.0f,
		[&]() {Damage(); },
		BattleHit::player
	);

	if (g_pad[0].IsTrigger(enButtonA)) {
		atk = true;
		Attack();
	}
	if (atk == true) {
		if (m_animation.IsPlaying() == false) {
			atk = false;
		}
	}
	else {
		Move();
	}
	m_rotMatrix.MakeRotationFromQuaternion(m_rotation);
	m_forward.x = m_rotMatrix.m[2][0];
	m_forward.y = m_rotMatrix.m[2][1];
	m_forward.z = m_rotMatrix.m[2][2];
	m_forward.Normalize();
	//シャドウキャスターを登録。
	g_graphicsEngine->GetShadowMap()->RegistShadowCaster(&m_model);
	//m_model.SetShadowReciever(true);
	//ワールド行列の更新。
	m_moveSpeed.y -= 980.0f * (1.0f / 60.0f);
	m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);
	//アニメーションを流す。
	m_animation.Update(1.0f / 30.0f);
}

void Player::Draw()
{
	m_model.Draw(
		enRenderMode_Normal,
		g_camera3D.GetViewMatrix(), 
		g_camera3D.GetProjectionMatrix()
	);
}

void Player::PostDraw()
{
	HP_Gauge();
}
