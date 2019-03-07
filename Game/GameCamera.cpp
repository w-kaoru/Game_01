#include "stdafx.h"
#include "GameCamera.h"
#include "Player\Player.h"

GameCamera::GameCamera()
{
	//カメラを初期化。
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
		//現在の注視点から視点へのベクトルを求めるよ。
		auto toCameraPos = g_camera3D.GetPosition() - g_camera3D.GetTarget();
		//新しい注視点を求める。
		auto newTarget = m_player->GetPosition();
		//Y方向にちょっと上げる。
		newTarget.y += 25.0f;
		//新しい視点を計算する。

		CVector3 toCameraPosOld = toCameraPos;
		//パッドの入力を使ってカメラを回す。
		float x = g_pad[0].GetRStickXF();
		float y = g_pad[0].GetRStickYF();
		//Y軸周りの回転
		CQuaternion qRot;
		qRot.SetRotation(CVector3::AxisY(), 0.05f * x);
		qRot.Multiply(toCameraPos);
		//X軸周りの回転。
		CVector3 axisX;
		axisX.Cross(CVector3::AxisY(), toCameraPos);
		axisX.Normalize();
		qRot.SetRotation(axisX, 0.05f * y);
		qRot.Multiply(toCameraPos);
		//カメラの回転の上限をチェックする。
		//注視点から視点までのベクトルを正規化する。
		//正規化すると、ベクトルの大きさが１になる。
		//大きさが１になるということは、ベクトルから強さがなくなり、方向のみの情報となるということ。
		CVector3 toPosDir = toCameraPos;
		toPosDir.Normalize();
		if (toPosDir.y < -0.5f) {
			//カメラが上向きすぎ。
			toCameraPos = toCameraPosOld;
		}
		else if (toPosDir.y > 0.8f) {
			//カメラが下向きすぎ。
			toCameraPos = toCameraPosOld;
		}

		auto newPositin = newTarget + toCameraPos;
		g_camera3D.SetUpdateProjMatrixFunc(Camera::enUpdateProjMatrixFunc_Perspective);
		g_camera3D.SetTarget(newTarget);
		g_camera3D.SetPosition(newPositin);
		g_camera3D.Update();
	}
}