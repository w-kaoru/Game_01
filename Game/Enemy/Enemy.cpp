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
	//�L�����N�^�[�R���g���[���[�̏������B
	m_charaCon.Init(40.0f, 50.0f, m_position);
	m_ensm.Start();
	return true;
}

void Enemy::PlLen()
{
	//�G�l�~�[����v���C���[�ɐL�т�x�N�g�������߂�B
	m_moveSpeed = m_player->GetPosition() - m_position;
	//���K�����s���O�ɁA�v���C���[�܂ł̋��������߂Ă����B
	m_toPlayerLen = m_moveSpeed.Length();
	//���K���I
	m_moveSpeed.Normalize();
}

void Enemy::Search()
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
		&& m_toPlayerLen < 600.0f
		) {
		//�������B
		m_ensm.Change(EnemyState::MoveState::move);
	}
	else {
		//�����Ă��Ȃ��B
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
	//�d�͉����x
	m_moveSpeed.y -= 9800.0f * (1.0f / 60.0f);
	//�L�����R�����g���Ĉړ�����B
	m_position = m_charaCon.Execute(1.0f / 60.0f, m_moveSpeed);
	m_hit = g_battleController->Create(
		&m_position, 300.0f,
		[&]() {Damage(); },
		BattleHit::enemy
	);
	//�V���h�E�L���X�^�[��o�^�B
	g_graphicsEngine->GetShadowMap()->RegistShadowCaster(&m_model);
	//m_model.SetShadowReciever(true);
	//���[���h�s������߂�B
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
