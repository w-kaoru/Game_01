#include "stdafx.h"
#include "GameCamera.h"
#include "Player\Player.h"

GameCamera::GameCamera()
{
	//�J�������������B
	g_camera3D.SetPosition({ 0.0f, 150.0f, 300.0f });
	g_camera3D.SetTarget({ 0.0f, 100.0f, 0.0f });
	g_camera3D.SetFar(10000.0f);
}


GameCamera::~GameCamera()
{
}
void GameCamera::Update()
{
	if (m_player != nullptr) {
		//���݂̒����_���王�_�ւ̃x�N�g�������߂��B
		auto toCameraPos = g_camera3D.GetPosition() - g_camera3D.GetTarget();
		//�V���������_�����߂�B
		auto newTarget = m_player->GetPosition();
		//Y�����ɂ�����Əグ��B
		newTarget.y += 25.0f;
		//�V�������_���v�Z����B

		CVector3 toCameraPosOld = toCameraPos;
		//�p�b�h�̓��͂��g���ăJ�������񂷁B
		float x = g_pad[0].GetRStickXF();
		float y = g_pad[0].GetRStickYF();
		//Y������̉�]
		CQuaternion qRot;
		qRot.SetRotation(CVector3::AxisY(), 0.05f * x);
		qRot.Multiply(toCameraPos);
		//X������̉�]�B
		CVector3 axisX;
		axisX.Cross(CVector3::AxisY(), toCameraPos);
		axisX.Normalize();
		qRot.SetRotation(axisX, 0.05f * y);
		qRot.Multiply(toCameraPos);
		//�J�����̉�]�̏�����`�F�b�N����B
		//�����_���王�_�܂ł̃x�N�g���𐳋K������B
		//���K������ƁA�x�N�g���̑傫�����P�ɂȂ�B
		//�傫�����P�ɂȂ�Ƃ������Ƃ́A�x�N�g�����狭�����Ȃ��Ȃ�A�����݂̂̏��ƂȂ�Ƃ������ƁB
		CVector3 toPosDir = toCameraPos;
		toPosDir.Normalize();
		if (toPosDir.y < -0.5f) {
			//�J����������������B
			toCameraPos = toCameraPosOld;
		}
		else if (toPosDir.y > 0.8f) {
			//�J�����������������B
			toCameraPos = toCameraPosOld;
		}

		auto newPositin = newTarget + toCameraPos;
		g_camera3D.SetUpdateProjMatrixFunc(Camera::enUpdateProjMatrixFunc_Perspective);
		g_camera3D.SetTarget(newTarget);
		g_camera3D.SetPosition(newPositin);
		g_camera3D.Update();
	}
}