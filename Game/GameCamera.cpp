#include "stdafx.h"
#include "GameCamera.h"
#include "Player\Player.h"

GameCamera::GameCamera()
{
	//�J�������������B
	g_camera3D.SetPosition({ 0.0f, 200.0f, 370.0f });
	g_camera3D.SetTarget({ 0.0f, 100.0f, 0.0f });
	g_camera3D.SetFar(10000.0f);
}


GameCamera::~GameCamera()
{
	int u = 0;
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
		float RStick_x = g_pad[0].GetRStickXF();
		float RStick_y = g_pad[0].GetRStickYF();
		if (fabsf(RStick_x) > 0.0f || fabsf(RStick_y) > 0.0) {
			//Y������̉�]
			CQuaternion qRot;
			qRot.SetRotation(CVector3::AxisY(), 0.05f * RStick_x);
			qRot.Multiply(toCameraPos);
			//X������̉�]�B
			CVector3 axisX;
			axisX.Cross(CVector3::AxisY(), toCameraPos);
			axisX.Normalize();
			qRot.SetRotation(axisX, 0.05f * RStick_y);
			qRot.Multiply(toCameraPos);
			//�J�����̉�]�̏�����`�F�b�N����B
			//�����_���王�_�܂ł̃x�N�g���𐳋K������B
			//���K������ƁA�x�N�g���̑傫�����P�ɂȂ�B
			//�傫�����P�ɂȂ�Ƃ������Ƃ́A�x�N�g�����狭�����Ȃ��Ȃ�A�����݂̂̏��ƂȂ�Ƃ������ƁB
			CVector3 toPosDir = toCameraPos;
			toPosDir.Normalize();
			if (toPosDir.y < -0.05f) {
				//�J����������������B
				toCameraPos = toCameraPosOld;
			}
			else if (toPosDir.y > 0.8f) {
				//�J�����������������B
				toCameraPos = toCameraPosOld;
			}
		}
		if (fabsf(RStick_x) < 0.01f) {
			//R�X�e�B�b�N�̓��͂��Ȃ���΃I�[�g�J�����̏������s���B
			//���݂̃J�����ł�XZ���ʏ�ł̒����_���王�_�܂ł̃x�N�g���ƁA���̒��������߂�B
			CVector3 toCameraPosXZ = toCameraPos;
			float height = toCameraPosXZ.y;						//���_�ւ�Y�����̍����͋L�����Ă����B
			toCameraPosXZ.y = 0.0f;								//XZ���ʂɂ���̂ŁAY��0�ɂ���B
			float toCameraPosXZLen = toCameraPosXZ.Length();	//XZ���ʏ�ł̎��_�ƒ����_�̋��������߂�B
			toCameraPosXZ.Normalize();

			//XZ���ʂł̐V�����J�����̎��_�����߂�B
			//�I�[�g�J����
			toCameraPos = g_camera3D.GetPosition() - newTarget;
			toCameraPos.y = 0.0f;
			toCameraPos.Normalize();
			//������ƂÂǔ��B
			float weight = 0.7f;	//���̃E�F�C�g�̒l��0.0�`1.0�̒l���Ƃ�B1.0�ɋ߂Â��قǒǔ��������Ȃ�B
			toCameraPos = toCameraPos * weight + toCameraPosXZ * (1.0f - weight);
			toCameraPos.Normalize();
			toCameraPos *= toCameraPosXZLen;
			toCameraPos.y = height;					//������߂��B
		}

		auto newPositin = newTarget + toCameraPos;
		g_camera3D.SetUpdateProjMatrixFunc(Camera::enUpdateProjMatrixFunc_Perspective);
		g_camera3D.SetTarget(newTarget);
		g_camera3D.SetPosition(newPositin);
		g_camera3D.Update();
	}
}