#include "stdafx.h"
#include "Enemy.h"
#include "Player.h"


Enemy::Enemy()
{
	m_model.Init(L"Assets/modelData/enemy.cmo");
	m_position.y = 100.0f;
	//キャラクターコントローラーの初期化。
	m_charaCon.Init(25.0f, 50.0f, m_position);
	
}


Enemy::~Enemy()
{
	m_charaCon.RemoveRigidBoby();
}

void Enemy::PlLen()
{
	//エネミーからプレイヤーに伸びるベクトルを求める。
	m_moveSpeed = m_player->GetPosition() - m_position;
	//正規化を行う前に、プレイヤーまでの距離を求めておく。
	toPlayerLen = m_moveSpeed.Length();
	//正規化！
	m_moveSpeed.Normalize();
}

bool Enemy::Search()
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
		&& toPlayerLen < 300.0f
		) {
		//見つけた。
		return true;
	}
	//見つけていない。
	return false;
}
//エネミー追跡状態。
void Enemy::Move()
{
	PlLen();
	if (toPlayerLen < 1000.0f) {
		m_moveSpeed.x *= 200.0f;
		m_moveSpeed.z *= 200.0f;
		//向きも変える。
		if (fabsf(m_moveSpeed.x) > 0.1f || fabsf(m_moveSpeed.z) > 0.1f) {
			auto angle = atan2f(m_moveSpeed.x, m_moveSpeed.z);
			m_rotation.SetRotation(CVector3::AxisY(), angle);
		}
	}
	else {
		movestate = idle;
	}
	m_moveSpeed.y -= 9800.0f * (1.0f / 60.0f);
	//キャラコンを使って移動する。aaa
	m_position = m_charaCon.Execute(1.0f / 60.0f, m_moveSpeed);
}

void Enemy::Idle()
{
	PlLen();
	if (Search() == true
		&&toPlayerLen < 1000.0f
		) {
		movestate = move;
	}
	else
	{
		m_moveSpeed.x *= 0.0f;
		m_moveSpeed.z *= 0.0f;
	}
	m_moveSpeed.y -= 9800.0f * (1.0f / 60.0f);
	//キャラコンを使って移動する。
	m_position = m_charaCon.Execute(1.0f / 60.0f, m_moveSpeed);
	
}

void Enemy::Update()
{
	switch (movestate)
	{
	case idle:
		Idle();
		break;
	case move:
		Move();
		break;
	}
	if (toPlayerLen < 40) {
		isDead = true;
		m_charaCon.RemoveRigidBoby();
	}
	//ワールド行列を求める。
	m_model.UpdateWorldMatrix(m_position, m_rotation, { 3.0f, 3.0f, 3.0f });
}
void Enemy::Draw()
{
	m_model.Draw(g_camera3D.GetViewMatrix(), g_camera3D.GetProjectionMatrix());
}
