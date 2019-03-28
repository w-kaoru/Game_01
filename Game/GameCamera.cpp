#include "stdafx.h"
#include "GameCamera.h"
#include "Player\Player.h"

GameCamera::GameCamera()
{
	//カメラを初期化。
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
		//現在の注視点から視点へのベクトルを求めるよ。
		auto toCameraPos = g_camera3D.GetPosition() - g_camera3D.GetTarget();
		//新しい注視点を求める。
		auto newTarget = m_player->GetPosition();
		//Y方向にちょっと上げる。
		newTarget.y += 25.0f;
		//新しい視点を計算する。

		CVector3 toCameraPosOld = toCameraPos;
		//パッドの入力を使ってカメラを回す。
		float RStick_x = g_pad[0].GetRStickXF();
		float RStick_y = g_pad[0].GetRStickYF();
		if (fabsf(RStick_x) > 0.0f || fabsf(RStick_y) > 0.0) {
			//Y軸周りの回転
			CQuaternion qRot;
			qRot.SetRotation(CVector3::AxisY(), 0.05f * RStick_x);
			qRot.Multiply(toCameraPos);
			//X軸周りの回転。
			CVector3 axisX;
			axisX.Cross(CVector3::AxisY(), toCameraPos);
			axisX.Normalize();
			qRot.SetRotation(axisX, 0.05f * RStick_y);
			qRot.Multiply(toCameraPos);
			//カメラの回転の上限をチェックする。
			//注視点から視点までのベクトルを正規化する。
			//正規化すると、ベクトルの大きさが１になる。
			//大きさが１になるということは、ベクトルから強さがなくなり、方向のみの情報となるということ。
			CVector3 toPosDir = toCameraPos;
			toPosDir.Normalize();
			if (toPosDir.y < -0.05f) {
				//カメラが上向きすぎ。
				toCameraPos = toCameraPosOld;
			}
			else if (toPosDir.y > 0.8f) {
				//カメラが下向きすぎ。
				toCameraPos = toCameraPosOld;
			}
		}
		if (fabsf(RStick_x) < 0.01f) {
			//Rスティックの入力がなければオートカメラの処理を行う。
			//現在のカメラでのXZ平面上での注視点から視点までのベクトルと、その長さを求める。
			CVector3 toCameraPosXZ = toCameraPos;
			float height = toCameraPosXZ.y;						//視点へのY方向の高さは記憶しておく。
			toCameraPosXZ.y = 0.0f;								//XZ平面にするので、Yは0にする。
			float toCameraPosXZLen = toCameraPosXZ.Length();	//XZ平面上での視点と注視点の距離を求める。
			toCameraPosXZ.Normalize();

			//XZ平面での新しいカメラの視点を決める。
			//オートカメラ
			toCameraPos = g_camera3D.GetPosition() - newTarget;
			toCameraPos.y = 0.0f;
			toCameraPos.Normalize();
			//ちょっとづつ追尾。
			float weight = 0.7f;	//このウェイトの値は0.0～1.0の値をとる。1.0に近づくほど追尾が強くなる。
			toCameraPos = toCameraPos * weight + toCameraPosXZ * (1.0f - weight);
			toCameraPos.Normalize();
			toCameraPos *= toCameraPosXZLen;
			toCameraPos.y = height;					//高さを戻す。
		}

		auto newPositin = newTarget + toCameraPos;
		g_camera3D.SetUpdateProjMatrixFunc(Camera::enUpdateProjMatrixFunc_Perspective);
		g_camera3D.SetTarget(newTarget);
		g_camera3D.SetPosition(newPositin);
		g_camera3D.Update();
	}
}