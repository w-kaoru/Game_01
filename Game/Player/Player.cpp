#include "stdafx.h"
#include "Player.h"
#include "PlStateMachine.h"
#include "../Game.h"
#include "../GameEnd.h"


Player::Player() :m_stMa(this)
{
	//cmoファイルの読み込み。
	m_model.Init(L"Assets/modelData/paladin.cmo", enFbxUpAxisZ);
	//m_model_01.Init(L"Assets/modelData/plpath.cmo", enFbxUpAxisZ);
	//tkaファイルの読み込み。
	//待機アニメーション
	m_animationClips[PlayerState::AnimState::idle].Load(L"Assets/animData/Player/plidle.tka");
	m_animationClips[PlayerState::AnimState::idle].SetLoopFlag(true);

	//走りアニメーション
	m_animationClips[PlayerState::AnimState::run].Load(L"Assets/animData/Player/plwalk.tka");
	m_animationClips[PlayerState::AnimState::run].SetLoopFlag(true);

	//攻撃アニメーション
	m_animationClips[PlayerState::AnimState::attack].Load(L"Assets/animData/Player/plattack.tka");
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

	//法線マップをロード。
	//ファイル名を使って、テクスチャをロードして、ShaderResrouceViewを作成する。
	DirectX::CreateDDSTextureFromFileEx(
		g_graphicsEngine->GetD3DDevice(), L"Assets/sprite/Paladin_normal.dds", 0,
		D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0, 0,
		false, nullptr, &m_normalMapSRV);
	//スペキュラマップをロード。
	DirectX::CreateDDSTextureFromFileEx(
		g_graphicsEngine->GetD3DDevice(), L"Assets/sprite/Paladin_specular.dds", 0,
		D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0, 0,
		false, nullptr, &m_specularMapSRV);
}
Player::~Player()
{
	//影を除外する。
	g_graphicsEngine->GetShadowMap()->UnregistShadowCaster(&m_model);
	//キャラクターコントローラーを消去する。
	m_charaCon.RemoveRigidBoby();
	//当たり判定を消去。
	g_hitObject->Delete(m_hit);
}
bool Player::Start()
{
	//ステータスの設定
	m_hpFrame = 6;
	m_status.SetLv(1);
	m_status.SetHp(60);
	m_status.SetAgi(1150.0f);
	m_status.SetDef(1.0f);
	m_status.SetAtk(4.5f);
	m_status.SetMaxLv(9);
	m_maxHp = m_status.GetHp();
	m_status.StatusUp();
	m_hpFrame = m_status.GetHp();
	//ノーマルマップをセットする。
	m_model.SetNormalMap(m_normalMapSRV);
	m_model.SetSpecularMap(m_specularMapSRV);
	//ポジションを少し上にしておく。
	m_position.y = 200.0f;
	//初期のポジションを設定してリスポーン地点にする。
	m_respawnPosition = m_position;
	//キャラクターコントローラーの初期化。
	m_charaCon.Init(40.0f, 110.0f, m_position);
	m_stMa.Start();
	m_rotation.SetRotationDeg(CVector3::AxisY(), 180.0f);
	//当たり判定を作る。
	m_hit = g_hitObject->Create(
		&m_position, 150.0f,
		[&](float damage) {Damage(damage); },
		Hit::player
	);
	//SE
	m_se.Init(L"Assets/sound/se_damage.wav");
	m_se.SetVolume(1.0f);
	m_model.SetShadowReciever(true);
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
	m_moveSpeed += cameraForward * lStick_y * m_status.GetAgi();
	//右方向への移動速度を加算。
	m_moveSpeed += cameraRight * lStick_x * m_status.GetAgi();
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
	if (g_pad[0].IsTrigger(enButtonX) && m_atkAnim == false) {
		//Aボタンが押されて攻撃モーションをしてない時。
		m_atkAnim = true;
		//攻撃アニメーションの再生するためにステートの変更
		m_stMa.Change(PlayerState::AnimState::attack);
	}
	if (m_atkAnim == true) {
		m_hitTiming++;
		//攻撃されてからあたったタイミングで攻撃したい（簡易版）
		if (m_hitTiming == 15) {
			//攻撃をヒットさせる。
			g_hitObject->HitTest(m_attckPos, m_status.GetAtk(), Hit::enemy);
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
	float w = -500.0f;
	float h = 350.0f;
	//スプライトの更新
	m_hpFrameSprite.Update(
		{ w, h, 0.0f },
		CQuaternion::Identity(),
		{ m_hpFrame / 10, 1.5f, 1.0f },
		{ 0.0f,1.0f }
	);
	//スプライトの更新
	m_hpSprite.Update(
		{ w, h, 0.0f },
		CQuaternion::Identity(),
		{ m_status.GetHp() / 10, 1.5f, 1.0f },
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
	m_se.Play(false);
	//攻撃をくらったのでHPからくらった分を引く
	float hp = m_status.GetHp();
	hp = (hp + m_status.GetDef()) - Enatk;
	m_status.SetHp(hp);
}

void Player::Update()
{
	int lv = m_status.GetLv();
	//経験値
	//レベルアップの条件
	int exp = lv;
	if (m_exp >= exp) {
		if (lv < m_status.GetMaxLv()) {
			m_status.SetHp(m_maxHp);
			m_status.SetLv(lv);
			m_status.LvUp();
			m_status.StatusUp();
			m_hpFrame = m_status.GetHp();
			m_exp = 0;
		}
	}
	//ステートマシンの更新。
	m_stMa.Update();
	//m_model.SetShadowReciever(true);

	//死亡の判定
	if (m_status.GetHp() <= 0.0f) {
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
	m_font.BeginDraw();	//フォントの描画開始。
	if (m_status.GetLv() < m_status.GetMaxLv()) {
		wchar_t Seconds[256];
		swprintf_s(Seconds, L"Lv.%d", m_status.GetLv());
		m_font.Draw(
			Seconds,
			{ -640.0f, 360.0f },
			{ 0.0f,1.0f,0.0f,1.0f },
			0.0f,
			1.0f,
			{ 1.0f, 1.0f }
		);
	}
	else {
		wchar_t Seconds[256];
		swprintf_s(Seconds, L"Lv.Max");
		m_font.Draw(
			Seconds,
			{ -640.0f, 360.0f },
			{ 0.0f,1.0f,0.0f,1.0f },
			0.0f,
			1.0f,
			{ 1.0f, 1.0f }
		);
	}
	m_font.EndDraw();
	//スプライトの描画。
	HP_Gauge();
}
