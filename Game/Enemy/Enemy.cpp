#include "stdafx.h"
#include "Enemy.h"
#include "Player\Player.h"


Enemy::Enemy():m_ensm(this)
{
}


Enemy::~Enemy()
{
	g_graphicsEngine->GetShadowMap()->UnregistShadowCaster(&m_model);
	m_charaCon.RemoveRigidBoby();
	g_battleController->Deleteobjict(m_hit);
}

bool Enemy::Start()
{
	//m_model.Init(L"Assets/modelData/enemy.cmo");

	m_hp = 1.0f;
	m_agi = 400.0f;
	if (m_selectModel == 1) {
		m_model.Init(L"Assets/modelData/enemy_01.cmo");
	}
	if (m_selectModel == 2) {
		m_model.Init(L"Assets/modelData/enemy_01.cmo");
	}

	//tkaファイルの読み込み。
	m_animationClips[EnemyState::idle].Load(L"Assets/animData/enidle.tka");
	m_animationClips[EnemyState::idle].SetLoopFlag(true);

	m_animationClips[EnemyState::move].Load(L"Assets/animData/enwalk.tka");
	m_animationClips[EnemyState::move].SetLoopFlag(true);

	m_animationClips[EnemyState::attack].Load(L"Assets/animData/enattack.tka");
	m_animationClips[EnemyState::attack].SetLoopFlag(false);
	//アニメーションの初期化。
	m_animation.Init(
		m_model,			//アニメーションを流すスキンモデル。
							//これでアニメーションとスキンモデルが関連付けされる。
		m_animationClips,	//アニメーションクリップの配列。
		4					//アニメーションクリップの数。
	);
	m_hpSprite.Init(L"Assets/sprite/hp_gauge.dds", 100.0f, 10.0f);
	m_position.y = 200.0f;
	//キャラクターコントローラーの初期化。
	m_charaCon.Init(40.0f, 70.0f, m_position);
	m_ensm.Start();
	m_scale *= 2.0f;
	//当たった？
	m_hit = g_battleController->Create(
		&m_position, 150.0f,
		[&](float damage) {Damage(damage); },
		BattleHit::enemy
	);
	return true;
}

void Enemy::PlLen()
{
	//エネミーからプレイヤーに伸びるベクトルを求める。
	m_moveSpeed = m_player->GetPosition() - m_position;
	//正規化を行う前に、プレイヤーまでの距離を求めておく。
	m_toPlayerLen = m_moveSpeed.Length();
	//正規化！
	m_moveSpeed.Normalize();
}

void Enemy::Search()
{
	PlLen();
	m_forward = CVector3::AxisZ();
	m_rotation.Multiply(m_forward);
	//enemyForwardとm_moveSpeedとの内積を計算する。
	float d = m_forward.Dot(m_moveSpeed);
	//内積の結果をacos関数に渡して、enemyForwardとm_moveSpeedのなす角を求める。
	float angle = acos(d);
	//視野角判定
	//fabsfは絶対値を求める関数！
	//角度はマイナスが存在するから、絶対値にする。
	// /*
	if (fabsf(angle) < CMath::DegToRad(60.0f)) {
		if (m_toPlayerLen <= 150.0) {
			Attack();
		}
	}
	// */
	if (attackFlag == true) {
		if (m_animation.IsPlaying() == false) {
			attackFlag = false;
		}
	}
	else
	{
		if (m_toPlayerLen > 150.0f && m_toPlayerLen < 600.0f) {
			m_ensm.Change(EnemyState::MoveState::move);
		}
		else {
			m_ensm.Change(EnemyState::MoveState::idle);
		}
	}
}
 // /*
void Enemy::Attack()
{
	m_AttackTiming++;
	if (m_AttackTiming == 70.0f) {
		m_ensm.Change(EnemyState::MoveState::attack);
		attackFlag = true;
		m_AttackTiming = 0;
	}
}
//*/

void Enemy::Damage(float damage)
{
	m_hp -= damage;
}

void Enemy::HP_Gauge()
{
	auto pos = m_position;
	pos.y += 200.0f;
	CVector3 hp_angle;
	hp_angle = g_camera3D.GetPosition() - m_position;
	hp_angle.y = 0;
	hp_angle.Normalize();
	float angle = acos(hp_angle.Dot(m_Sprite_Front));
	angle = CMath::RadToDeg(angle);
	CVector3 hp_axis;
	hp_axis.Cross(m_Sprite_Front, hp_angle);
	if (hp_axis.y > 0) {
		m_Sprite_angle.SetRotationDeg(CVector3::AxisY(), angle);
	}
	else {
		m_Sprite_angle.SetRotationDeg(CVector3::AxisY()*-1, angle);
	}
	m_hpSprite.Update(pos, m_Sprite_angle, { m_hp, 1.0f, 1.0f });
	m_hpSprite.Draw(
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
}

void Enemy::Update()
{
	m_damageTiming = 0;
	//Move();
	Search();
	m_ensm.Update();
	//重力加速度
	m_moveSpeed.y -= 9800.0f * (1.0f / 60.0f);
	//キャラコンを使って移動する。
	m_position = m_charaCon.Execute(1.0f / 60.0f, m_moveSpeed);

	//シャドウキャスターを登録。
	g_graphicsEngine->GetShadowMap()->RegistShadowCaster(&m_model);
	//m_model.SetShadowReciever(true);
	//ワールド行列を求める。
	m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);
	//アニメーションを流す。
	m_animation.Update(1.0f / 30.0f);
	if (m_hp <= 0.01f) {
		g_gameObjM->DeleteGameObject(this);
	}
}

void Enemy::Draw()
{
	m_model.Draw(
		enRenderMode_Normal,
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
}

void Enemy::PostDraw()
{
	HP_Gauge();
}
