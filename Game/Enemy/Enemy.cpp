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
}

bool Enemy::Start()
{
	//m_model.Init(L"Assets/modelData/enemy.cmo");

	m_hp = 200.0f;
	m_atk = 20.0f;
	m_def = 30.0f;
	m_agi = 300.0f;
	if (m_selectModel == 1) {
		m_model.Init(L"Assets/modelData/enemy.cmo");
	}
	if (m_selectModel == 2) {
		m_model.Init(L"Assets/modelData/ToonRTS_demo_Knight.cmo");
	}
	m_position.y = 200.0f;
	//キャラクターコントローラーの初期化。
	m_charaCon.Init(40.0f, 50.0f, m_position);
	m_ensm.Start();
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
	CVector3 enemyForward = CVector3::AxisZ();
	m_rotation.Multiply(enemyForward);
	//enemyForwardとm_moveSpeedとの内積を計算する。
	float d = enemyForward.Dot(m_moveSpeed);
	//内積の結果をacos関数に渡して、enemyForwardとm_moveSpeedのなす角を求める。
	float angle = acos(d);
	//視野角判定
	//fabsfは絶対値を求める関数！
	//角度はマイナスが存在するから、絶対値にする。
	if (fabsf(angle) < CMath::DegToRad(45.0f)
		&& m_toPlayerLen < 600.0f
		) {
		//見つけた。
		m_ensm.Change(EnemyState::MoveState::move);
	}
	else {
		//見つけていない。
		m_ensm.Change(EnemyState::MoveState::idle);
	}
}
void Enemy::Attack()
{

}

void Enemy::Damage()
{
	m_damageTiming++;
	if (m_damageTiming == 15) {
		m_hp -= 30.0f;
	}
}

void Enemy::Update()
{
	m_damageTiming = 0;
	//Move();
	Search();
	m_ensm.Update();
	if (m_toPlayerLen < 40) {
		m_isDead = true;
	}
	//重力加速度
	m_moveSpeed.y -= 9800.0f * (1.0f / 60.0f);
	//キャラコンを使って移動する。
	m_position = m_charaCon.Execute(1.0f / 60.0f, m_moveSpeed);
	m_hit = g_battleController->Create(
		&m_position, 300.0f,
		[&]() {Damage(); },
		BattleHit::enemy
	);
	//シャドウキャスターを登録。
	g_graphicsEngine->GetShadowMap()->RegistShadowCaster(&m_model);
	//m_model.SetShadowReciever(true);
	//ワールド行列を求める。
	m_model.UpdateWorldMatrix(m_position, m_rotation, { 3.0f, 3.0f, 3.0f });
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
