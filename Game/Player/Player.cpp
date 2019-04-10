#include "stdafx.h"
#include "Player.h"
#include "PlStateMachine.h"


Player::Player() :m_stMa(this)
{
	//cmoファイルの読み込み。
	m_model.Init(L"Assets/modelData/ToonRTS_demo_Knight.cmo", enFbxUpAxisZ);
	//m_model_01.Init(L"Assets/modelData/plpath.cmo", enFbxUpAxisZ);
	//tkaファイルの読み込み。
	m_animationClips[PlayerState::AnimState::idle].Load(L"Assets/animData/plidle.tka");
	m_animationClips[PlayerState::AnimState::idle].SetLoopFlag(true);

	m_animationClips[PlayerState::AnimState::run].Load(L"Assets/animData/plrun.tka");
	m_animationClips[PlayerState::AnimState::run].SetLoopFlag(true);

	//m_animationClips[PlayerState::AnimState::jump].Load(L"Assets/animData/jump.tka");
	//m_animationClips[PlayerState::AnimState::jump].SetLoopFlag(false);

	m_animationClips[PlayerState::AnimState::attack].Load(L"Assets/animData/plattack.tka");
	m_animationClips[PlayerState::AnimState::attack].SetLoopFlag(false);

	//アニメーションの初期化。
	m_animation.Init(
		m_model,			//アニメーションを流すスキンモデル。
							//これでアニメーションとスキンモデルが関連付けされる。
		m_animationClips,	//アニメーションクリップの配列。
		4					//アニメーションクリップの数。
	);
	//m_scale *= 0.1f;
	m_hpSprite.Init(L"Assets/sprite/hp_gauge.dds", m_HpScaleX, m_HpScaleY);
	m_hpFrameSprite.Init(L"Assets/sprite/hp_frame.dds", m_HpScaleX, m_HpScaleY);
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
	m_maxHp =  6.0f;		//体力
	m_hpFrame = m_maxHp;
	m_hp = m_maxHp;
	m_agi = 1000.0f; //400.0f;			//素早さ
	m_position.y = 200.0f;
	m_respawnPosition = m_position;
	//キャラクターコントローラーの初期化。
	m_charaCon.Init(40.0f, 70.0f, m_position);
	m_stMa.Start();

	//当たった？
	m_hit = g_battleController->Create(
		&m_position, 150.0f,
		[&](float damage) {Damage(damage); },
		BattleHit::player
	);
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
		m_stMa.Change(PlayerState::AnimState::run);
	}
	else {
		m_stMa.Change(PlayerState::AnimState::idle);
	}

	/*if (m_charaCon.IsJump() == true && m_charaCon.IsOnGround() == false) {
		m_moveSpeed.y += 450.0f;
	}*/
}

//攻撃
void Player::Attack()
{
	m_stMa.Change(PlayerState::AnimState::attack);
		g_battleController->Hit(m_attckPos, 0.2f, BattleHit::enemy);
	// */
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

void Player::Damage(float Enatk)
{
	m_hp -= Enatk;
}

void Player::Update()
{

	m_damageTiming = 0.0f;
	m_stMa.Update();


	if (g_pad[0].IsTrigger(enButtonA) && atk == false) {
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
	//*
	//*/
	//シャドウキャスターを登録。
	g_graphicsEngine->GetShadowMap()->RegistShadowCaster(&m_model);
	//m_model.SetShadowReciever(true);
	if (m_hp <= 0.0f) {
		m_hp = m_maxHp;
		m_position = m_respawnPosition;
		m_charaCon.SetPosition(m_position);
	}
	//ワールド行列の更新。
	m_moveSpeed.y -= 980.0f * (1.0f / 60.0f);
	m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);
	//m_model_01.UpdateWorldMatrix(m_attckPos, CQuaternion::Identity(), CVector3::One());
	//アニメーションを流す。
	m_animation.Update(1.0f / 30.0f);

	m_rotMatrix.MakeRotationFromQuaternion(m_rotation);
	m_forward.x = m_rotMatrix.m[2][0];
	m_forward.y = m_rotMatrix.m[2][1];
	m_forward.z = m_rotMatrix.m[2][2];
	m_forward.Normalize();
	m_attckPos.x = m_position.x + m_forward.x * 60.0f;
	m_attckPos.z = m_position.z + m_forward.z * 60.0f;
	m_attckPos.y = 100.0f;
}

void Player::Draw()
{
	m_model.Draw(
		enRenderMode_Normal,
		g_camera3D.GetViewMatrix(), 
		g_camera3D.GetProjectionMatrix()
	);
	/*m_model_01.Draw(
		enRenderMode_Normal,
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);*/
}

void Player::PostDraw()
{
	HP_Gauge();
}
