#include "stdafx.h"
#include "EnemyBos.h"
#include "Player\Player.h"
#include "../Game.h"
#include "../GameEnd.h"

EnemyBos::EnemyBos():m_enbos_stm(this)
{
}


EnemyBos::~EnemyBos()
{
	//影を除外する。
	g_graphicsEngine->GetShadowMap()->UnregistShadowCaster(&m_model);
	//キャラクターコントローラーを消去する
	m_charaCon.RemoveRigidBoby();
	//当たり判定を消去。
	g_battleController->Deleteobjict(m_hit);
	//法線マップをロード。
	//ファイル名を使って、テクスチャをロードして、ShaderResrouceViewを作成する。
	DirectX::CreateDDSTextureFromFileEx(
		g_graphicsEngine->GetD3DDevice(), L"Assets/modelData/mutant_Bos/Mutant_normal.dds", 0,
		D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0, 0,
		false, nullptr, &m_normalMapSRV);
}

bool EnemyBos::Start()
{
	//m_model.Init(L"Assets/modelData/enemy.cmo");

	//ステータスの設定
	m_status.SetLv(4);
	m_status.SetHp(20.0f);
	m_status.SetAgi(500.0f);
	m_status.SetDef(3.0f);
	m_status.SetAtk(15.0f);
	m_status.StatusUp();
	m_model.Init(L"Assets/modelData/enemy_Bos.cmo");
	m_model.SetNormalMap(m_normalMapSRV);
	//tkaファイルの読み込み。
	//待機アニメーション
	m_animationClips[EnemyBosState::idle].Load(L"Assets/animData/enBosidle.tka");
	m_animationClips[EnemyBosState::idle].SetLoopFlag(true);
	//歩きアニメーション
	m_animationClips[EnemyBosState::move].Load(L"Assets/animData/enBosrun.tka");
	m_animationClips[EnemyBosState::move].SetLoopFlag(true);
	//攻撃アニメーション
	m_animationClips[EnemyBosState::attack].Load(L"Assets/animData/enBosattack.tka");
	m_animationClips[EnemyBosState::attack].SetLoopFlag(false);
	//アニメーションの初期化。
	m_animation.Init(
		m_model,			//アニメーションを流すスキンモデル。
							//これでアニメーションとスキンモデルが関連付けされる。
		m_animationClips,	//アニメーションクリップの配列。
		4					//アニメーションクリップの数。
	);
	//hpバーのスプライト。
	m_hpSprite.Init(L"Assets/sprite/hp_gauge.dds", 100.0f, 10.0f);
	m_position.y = 200.0f;
	//キャラクターコントローラーの初期化。
	m_charaCon.Init(40.0f, 50.0f, m_position);
	//エネミーのステートマシンのスタート関数を呼ぶ。
	//ステートマシンの初期化。
	m_enbos_stm.Start();
	//当たり判定の作成。
	m_hit = g_battleController->Create(
		&m_position, 150.0f,
		[&](float damage) {Damage(damage); },
		BattleHit::enemy
	);
	//SE
	m_se_damade.Init(L"Assets/sound/se_damage.wav");
	m_se_damade.SetVolume(1.0f);
	return true;
}

void EnemyBos::PlLen()
{
	//エネミーからプレイヤーに伸びるベクトルを求める。
	m_moveSpeed = m_player->GetPosition() - m_position;
	//正規化を行う前に、プレイヤーまでの距離を求めておく。
	m_toPlayerLen = m_moveSpeed.Length();
	//正規化！
	m_moveSpeed.Normalize();
}

void EnemyBos::Search()
{
	PlLen();
	//前方向の取得。
	m_forward = CVector3::AxisZ();
	m_rotation.Multiply(m_forward);
	//enemyForwardとm_moveSpeedとの内積を計算する。
	float d = m_forward.Dot(m_moveSpeed);
	//内積の結果をacos関数に渡して、enemyForwardとm_moveSpeedのなす角を求める。
	float angle = acos(d);
	//視野角判定
	//fabsfは絶対値を求める関数！
	//角度はマイナスが存在するから、絶対値にする。
	// /*
	if (fabsf(angle) < CMath::DegToRad(60.0f)) {
		if (m_toPlayerLen <= 150.0) {
			//プレイヤーとの距離が一定以下で
			//攻撃関数をよべー。
			Attack();
		}
	}
	// */
	if (attackFlag == true) {
		//攻撃アニメーションが終了したら。
		if (m_animation.IsPlaying() == false) {
			attackFlag = false;
			m_AttackTiming = 0;
		}
	}
	else
	{
		//攻撃アニメーションが再生されてない時
		if (m_toPlayerLen > 130.0f && m_toPlayerLen < 1300.0f) {
			//歩きアニメーションの再生するためにステートの変更
			m_enbos_stm.Change(EnemyBosState::MoveState::move);
		}
		else {
			//待機アニメーションの再生するためにステートの変更
			m_enbos_stm.Change(EnemyBosState::MoveState::idle);
		}
	}
}

//攻撃
void EnemyBos::Attack()
{
	m_AttackTiming++;
	//攻撃の間隔
	if (m_AttackTiming == 25) {
		//攻撃アニメーションの再生するためにステートの変更
		m_enbos_stm.Change(EnemyBosState::MoveState::attack);
		attackFlag = true;
	}
	//攻撃されてからあたったタイミングで攻撃したい（簡易版）
	if (m_AttackTiming == 60) {
		//当たったと思われるタイミングで
		CVector3 hit = m_position;
		hit.y += 50.0f;
		hit += m_forward * 50.0f;
		//攻撃をヒットさせる。
		g_battleController->Hit(hit, m_status.GetAtk(), BattleHit::player);
		//攻撃の間隔を0に戻す。
		m_AttackTiming = 0;
	}
}

//ダメージ
void EnemyBos::Damage(float damage)
{
	m_se_damade.Play(false);
	//攻撃をくらったのでHPからくらった分を引く
	float hp = m_status.GetHp();
	hp = (hp + m_status.GetDef()) - damage;
	m_status.SetHp(hp);
}

//HPを表示するスプライトのための関係。
void EnemyBos::HP_Gauge()
{
	//ポジションを頭の上付近にする。
	auto pos = m_position;
	pos.y += 170.0f;
	//カメラのポジション向きのベクトルを取得。
	CVector3 hp_angle;
	hp_angle = g_camera3D.GetPosition() - m_position;
	hp_angle.y = 0;
	hp_angle.Normalize();
	//HPの画像がカメラに向かって前を向くようにする。
	float angle = acos(hp_angle.Dot(m_Sprite_Front));
	angle = CMath::RadToDeg(angle);
	CVector3 hp_axis;
	hp_axis.Cross(m_Sprite_Front, hp_angle);
	if (hp_axis.y > 0) {
		m_Sprite_angle.SetRotationDeg(CVector3::AxisY(), angle);
	}
	else {
		m_Sprite_angle.SetRotationDeg(CVector3::AxisY()*-1, angle);
	}
	//HPスプライトの更新
	m_hpSprite.Update(pos, m_Sprite_angle, { m_status.GetHp() / 10, 1.0f, 1.0f });
	//HPスプライトの表示
	m_hpSprite.Draw(
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
}

void EnemyBos::Update()
{
	m_damageTiming = 0;
	Search();
	//ステートマシンの更新
	m_enbos_stm.Update();
	//重力加速度
	m_moveSpeed.y -= 9800.0f * (1.0f / 60.0f);
	//キャラコンを使って移動する。
	m_position = m_charaCon.Execute(1.0f / 60.0f, m_moveSpeed);
	//シャドウキャスターを登録。
	g_graphicsEngine->GetShadowMap()->RegistShadowCaster(&m_model);
	//m_model.SetShadowReciever(true);
	//ワールド行列を求める。
	m_model.UpdateWorldMatrix(m_position, m_rotation, { 1.0f, 1.0f, 1.0f });
	//アニメーションを流す。
	m_animation.Update(1.0f / 30.0f);
	if (m_status.GetHp() <= 0.01f) {
		g_gameObjM->DeleteGO(g_gameObjM->FindGO<Game>());
		g_gameObjM->NewGO<GameEnd>()->SetGameEnd(GameEnd::GameEndState::gameCleared);
	}
}

void EnemyBos::Draw()
{
	//モデルを表示する。
	m_model.Draw(
		enRenderMode_Normal,
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
}

void EnemyBos::PostDraw()
{
	//HPスプライトの表示をする。
	HP_Gauge();
}
