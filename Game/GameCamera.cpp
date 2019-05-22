#include "stdafx.h"
#include "GameCamera.h"
#include "Player\Player.h"

GameCamera::GameCamera()
{
}


GameCamera::~GameCamera()
{
	m_springCamera.RemoveRigidBoby();
}
bool GameCamera::Start()
{
	//カメラを初期化。
	g_camera3D.SetPosition(
		{ 0.0f,m_player->GetPosition().y + 130.0f,
		m_player->GetPosition().z + 250.0f
		}
	);
	g_camera3D.SetTarget(m_player->GetPosition());
	g_camera3D.SetFar(20000.0f);
	//バネカメラの大きさ。
	m_springCamera.Init(
		0.0001f,
		0.0001f,
		0.5f,
		g_camera3D.GetPosition()
	);
	return false;
}
void GameCamera::Update()
{
	if (m_player != nullptr) {
		//現在の注視点から視点へのベクトルを求めるよ。
		CVector3 toCameraPos = g_camera3D.GetPosition() - g_camera3D.GetTarget();
		//新しい注視点を求める。
		CVector3 newTarget = m_player->GetPosition();
		//Y方向にちょっと上げる。
		newTarget.y += 120.0f;
		//新しい視点を計算する。

		//パッドの入力を使ってカメラを回す。
		CVector3 RStick = CVector3::Zero();
		RStick.x = 0.05f * g_pad[0].GetRStickXF();
		RStick.y = 0.05f * g_pad[0].GetRStickYF();
		RStick.z = 0.0f;
		//Y軸周りの回転
		CQuaternion qRot;
		qRot.SetRotation(CVector3::AxisY(), RStick.x);
		qRot.Multiply(toCameraPos);
		//X軸周りの回転。
		CVector3 axisX;
		axisX.Cross(CVector3::AxisY(), toCameraPos);
		axisX.Normalize();
		qRot.SetRotation(axisX, RStick.y);
		qRot.Multiply(toCameraPos);
		//カメラの回転の上限をチェックする。
		//注視点から視点までのベクトルを正規化する。
		//正規化すると、ベクトルの大きさが１になる。
		//大きさが１になるということは、ベクトルから強さがなくなり、方向のみの情報となるということ。
		CVector3 toPosDir = toCameraPos;
		toPosDir.Normalize();
		float angle = toPosDir.Dot(g_camera3D.GetUp());
		angle = CMath::RadToDeg(angle);
		float upMax = -40.0f;
		float downMax = 50.0f;
		if(angle < upMax){
		//カメラが上向きすぎ。
			angle -= upMax;
			qRot.SetRotationDeg(axisX, angle);
			qRot.Multiply(toCameraPos);
		}
		 if(angle > downMax){
		//	//カメラが下向きすぎ。
			angle -= downMax;
			qRot.SetRotationDeg(axisX, angle);
			qRot.Multiply(toCameraPos);
		}
		if (fabsf(RStick.x) < 0.01f) {
			//Rスティックの入力がなければオートカメラの処理を行う。
			//現在のカメラでのXZ平面上での注視点から視点までのベクトルと、その長さを求める。
			CVector3 toCameraPosXZ = toCameraPos;
			float height = toCameraPosXZ.y;						//視点へのY方向の高さは記憶しておく。
			toCameraPosXZ.y = 0.0f;								//XZ平面にするので、Yは0にする。
			float toCameraPosXZLen = toCameraPosXZ.Length();	//XZ平面上での視点と注視点の距離を求める。
			toCameraPosXZ.Normalize();

			toCameraPos = g_camera3D.GetPosition() - newTarget;
			toCameraPos.y = 0.0f;
			toCameraPos.Normalize();
			//ちょっとづつ追尾。
			float weight = 0.3f;	//このウェイトの値は0.0～1.0の値をとる。1.0に近づくほど追尾が強くなる。
			toCameraPos = toCameraPos * weight + toCameraPosXZ * (1.0f - weight);
			toCameraPos.Normalize();
			toCameraPos *= toCameraPosXZLen;
			toCameraPos.y = height;					//高さを戻す。
		}//*/

		auto newPositin = newTarget + toCameraPos;
		m_springCamera.SetPosition(newTarget);
		auto move =  newPositin- newTarget;
		move.Normalize();
		move *= 300.0f;
		//バネカメ更新。
		newPositin = m_springCamera.Execute(1.0f, move);
		g_camera3D.SetUpdateProjMatrixFunc(Camera::enUpdateProjMatrixFunc_Perspective);
		g_camera3D.SetTarget(newTarget);
		g_camera3D.SetPosition(newPositin);
		g_camera3D.Update();
	}
}