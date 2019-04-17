#include "stdafx.h"
#include "Player.h"
#include "PlStateMachine.h"
#include "../Game.h"
#include "../GameEnd.h"


Player::Player() :m_stMa(this)
{
	//cmoファイルの読み込み。
	m_model.Init(L"Assets/modelData/ToonRTS_demo_Knight.cmo", enFbxUpAxisZ);
	//m_model_01.Init(L"Assets/modelData/plpath.cmo", enFbxUpAxisZ);
	//tkaファイルの読み込み。
	//待機アニメーション
	m_animationClips[PlayerState::AnimState::idle].Load(L"Assets/animData/plidle.tka");
	m_animationClips[PlayerState::AnimState::idle].SetLoopFlag(true);

	//走りアニメーション
	m_animationClips[PlayerState::AnimState::run].Load(L"Assets/animData/plrun.tka");
	m_animationClips[PlayerState::AnimState::run].SetLoopFlag(true);

	//攻撃アニメーション
	m_animationClips[PlayerState::AnimState::attack].Load(L"Assets/animData/plattack.tka");
	m_animationClips[PlayerState::AnimState::attack].SetLoopFlag(false);

	//アニメーションの初期化。
	m_animation.Init(
		m_model,			//アニメーションを流すスキンモデル。
							//アニメーションとスキンモデルの関連付けがされる。
		m_animationClips,	//アニメーションクリップの配列。
		4					//アニメーションクリップの数。
	);
	//m_scale *= 0.1f;
	//HPの画像の読み込み
	m_hpSprite.Init(L"Assets/sprite/hp_gauge.dds", m_HpScaleX, m_HpScaleY);
	m_hpFrameSprite.Init(L"Assets/sprite/hp_frame.dds", m_HpScaleX, m_HpScaleY);
}
Player::~Player()
{
	//影を除外する。
	g_graphicsEngine->GetShadowMap()->UnregistShadowCaster(&m_model);
	//キャラクターコントローラーを消去する。
	m_charaCon.RemoveRigidBoby();
	//当たり判定を消去。
	g_battleController->Deleteobjict(m_hit);
}
bool Player::Start()
{
	//ステータスの設定
	//m_status.Lv = 1;
	m_hpFrame = 6;
	m_hp = 60;
	m_agi = 1000.0f; //400.0f;			//素早さ
	//ポジションを少し上にしておく。
	m_position.y = 200.0f;
	//初期のポジションを設定してリスポーン地点にする。
	m_respawnPosition = m_position;
	//キャラクターコントローラーの初期化。
	m_charaCon.Init(40.0f, 70.0f, m_position);
	m_stMa.Start();
	m_rotation.SetRotationDeg(CVector3::AxisY(), 180.0f);
	//当たり判定を作る。
	m_hit = g_battleController->Create(
		&m_position, 150.0f,
		[&](float damage) {Damage(damage); },
		BattleHit::player
	);
	return true;
}
//移動
void Player::Move()
{
	//スティックの入力量を受け取る。
	float lStick_x = g_pad[0].GetLStickXF();
	float lStick_y = g_pad[0].GetLStickYF();
	//カメラの前方方向と右方向を取得。
	CVector3 cameraForward = g_camera3D.GetForward();
	CVector3 cameraRight = g_camera3D.GetRight();
	//XZ平面での前方方向、右方向に変換する。
	cameraForward.y = 0.0f;
	cameraForward.Normalize();
	cameraRight.y = 0.0f;
	cameraRight.Normalize();
	//XZ成分の移動速度をクリア。
	m_moveSpeed.x = 0.0f;
	m_moveSpeed.z = 0.0f;
	//奥方向への移動速度を代入。
	m_moveSpeed += cameraForward * lStick_y * m_agi;
	//右方向への移動速度を加算。
	m_moveSpeed += cameraRight * lStick_x * m_agi;
	//キャラコンを使って移動する。
	m_position = m_charaCon.Execute(1.0f / 60.0f, m_moveSpeed);
	//向きを変える。
	if (fabsf(m_moveSpeed.x) > 0.1f
		|| fabsf(m_moveSpeed.z) > 0.1f) {
		auto angle = atan2f(m_moveSpeed.x, m_moveSpeed.z);
		m_rotation.SetRotation(CVector3::AxisY(), angle);
		//走るアニメーションの再生するためにステートの変更
		m_stMa.Change(PlayerState::AnimState::run);
	}
	else {
		//待機アニメーションの再生するためにステートの変更
		m_stMa.Change(PlayerState::AnimState::idle);
	}
}

//攻撃
void Player::Attack()
{
	if (g_pad[0].IsTrigger(enButtonA) && m_atkAnim == false) {
		//Aボタンが押されて攻撃モーションをしてない時。
		m_atkAnim = true;
		//攻撃アニメーションの再生するためにステートの変更
		m_stMa.Change(PlayerState::AnimState::attack);
	}
	if (m_atkAnim == true) {
		m_hitTiming++;
		//攻撃されてからあたったタイミングで攻撃したい（簡易版）
		if (m_hitTiming == 12) {
			//攻撃をヒットさせる。
			g_battleController->Hit(m_attckPos, 1.0f, BattleHit::enemy);
		}
		if (m_animation.IsPlaying() == false) {
			m_atkAnim = false;
			//攻撃の間隔を0に戻す。
			m_hitTiming = 0;
		}
	}
	else {

		//攻撃していない時に移動などの処理。
		Move();
	}
}

//HPを表示するスプライトのための関係。
void Player::HP_Gauge()
{
	//スプライトの更新
	m_hpFrameSprite.Update(
		{ -640.0f,350.0f, 0.0f },
		CQuaternion::Identity(),
		{ m_hpFrame, 1.5f, 1.0f },
		{ 0.0f,1.0f }
	);
	//スプライトの更新
	m_hpSprite.Update(
		{ -640.0f,350.0f, 0.0f },
		CQuaternion::Identity(),
		{ m_hp / 10, 1.5f, 1.0f },
		{ 0.0f,1.0f }
	);
	//スプライトを２次元で表示をする。
	m_hpFrameSprite.Draw(
		g_camera2D.GetViewMatrix(),
		g_camera2D.GetProjectionMatrix()
	);
	//スプライトを２次元で表示をする。
	m_hpSprite.Draw(
		g_camera2D.GetViewMatrix(),
		g_camera2D.GetProjectionMatrix()
	);
}

//ダメージ
void Player::Damage(float Enatk)
{
	//攻撃をくらったのでHPからくらった分を引く
	m_hp -= Enatk;
}

void Player::Update()
{
	//ステートマシンの更新。
	m_stMa.Update();
	//m_model.SetShadowReciever(true);

	//死亡の判定
	if (m_hp <= 0.0f) {
		//if (ki <= 3) {
		//	//HPを初期の値に戻す。
		//	m_hp = 60;
		//	//リスポーン地点へ転送
		//	m_position = m_respawnPosition;
		//	//キャラコンにポジションを設定。
		//	m_charaCon.SetPosition(m_position);
		//	ki++;
		//}
		g_gameObjM->DeleteGO(g_gameObjM->FindGO<Game>());
		g_gameObjM->NewGO<GameEnd>()->SetGameEnd(GameEnd::GameEndState::gameOver);
	}
	//攻撃
	Attack();
	//前方向の取得。
	m_rotMatrix.MakeRotationFromQuaternion(m_rotation);
	m_forward.x = m_rotMatrix.m[2][0];
	m_forward.y = m_rotMatrix.m[2][1];
	m_forward.z = m_rotMatrix.m[2][2];
	m_forward.Normalize();
	//攻撃の当たり判定のポジション
	//プレイヤーのポジションの少し前の位置
	m_attckPos.x = m_position.x + m_forward.x * 60.0f;
	m_attckPos.z = m_position.z + m_forward.z * 60.0f;
	m_attckPos.y = 100.0f;
	//重力
	m_moveSpeed.y -= 980.0f * (1.0f / 60.0f);
	//ワールド行列の更新。
	m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);
	//m_model_01.UpdateWorldMatrix(m_attckPos, CQuaternion::Identity(), CVector3::One());
	//アニメーションを流す。
	m_animation.Update(1.0f / 30.0f);
	//シャドウキャスターを登録。
	g_graphicsEngine->GetShadowMap()->RegistShadowCaster(&m_model);

}

void Player::Draw()
{
	//モデルを描画。
	m_model.Draw(
		enRenderMode_Normal,
		g_camera3D.GetViewMatrix(), 
		g_camera3D.GetProjectionMatrix()
	);
	/*m_model_01.Draw(
		enRenderMode_Normal,
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);*/
}

void Player::PostDraw()
{
	//スプライトの描画。
	HP_Gauge();
}
