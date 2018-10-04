#include "stdafx.h"
#include "Enemy.h"
#include "Player.h"


Enemy::Enemy()
{
	m_model.Init(L"Assets/modelData/enemy.cmo");
	m_position.y = 100.0f;
	//�L�����N�^�[�R���g���[���[�̏������B
	m_charaCon.Init(25.0f, 50.0f, m_position);
	
}


Enemy::~Enemy()
{
	m_charaCon.RemoveRigidBoby();
}

void Enemy::PlLen()
{
	//�G�l�~�[����v���C���[�ɐL�т�x�N�g�������߂�B
	m_moveSpeed = m_player->GetPosition() - m_position;
	//���K�����s���O�ɁA�v���C���[�܂ł̋��������߂Ă����B
	toPlayerLen = m_moveSpeed.Length();
	//���K���I
	m_moveSpeed.Normalize();
}

bool Enemy::Search()
{
	PlLen();
	CVector3 enemyForward = CVector3::AxisZ();
	m_rotation.Multiply(enemyForward);
	//enemyForward��m_moveSpeed�Ƃ̓��ς��v�Z����B
	float d = enemyForward.Dot(m_moveSpeed);
	//���ς̌��ʂ�acos�֐��ɓn���āAenemyForward��m_moveSpeed�̂Ȃ��p�����߂�B
	float angle = acos(d);
	//����p����
	//fabsf�͐�Βl�����߂�֐��I
	//�p�x�̓}�C�i�X�����݂��邩��A��Βl�ɂ���B
	if (fabsf(angle) < CMath::DegToRad(45.0f)
		&& toPlayerLen < 300.0f
		) {
		//�������B
		return true;
	}
	//�����Ă��Ȃ��B
	return false;
}
//�G�l�~�[�ǐՏ�ԁB
void Enemy::Move()
{
	PlLen();
	if (toPlayerLen < 1000.0f) {
		m_moveSpeed.x *= 200.0f;
		m_moveSpeed.z *= 200.0f;
		//�������ς���B
		if (fabsf(m_moveSpeed.x) > 0.1f || fabsf(m_moveSpeed.z) > 0.1f) {
			auto angle = atan2f(m_moveSpeed.x, m_moveSpeed.z);
			m_rotation.SetRotation(CVector3::AxisY(), angle);
		}
	}
	else {
		movestate = idle;
	}
	m_moveSpeed.y -= 9800.0f * (1.0f / 60.0f);
	//�L�����R�����g���Ĉړ�����Baaa
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
	//�L�����R�����g���Ĉړ�����B
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
	//���[���h�s������߂�B
	m_model.UpdateWorldMatrix(m_position, m_rotation, { 3.0f, 3.0f, 3.0f });
}
void Enemy::Draw()
{
	m_model.Draw(g_camera3D.GetViewMatrix(), g_camera3D.GetProjectionMatrix());
}
