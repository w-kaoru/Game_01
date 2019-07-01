#include "stdafx.h"
#include "EnemyBos.h"
#include "Player\Player.h"
#include "../Game.h"
#include "../GameEnd.h"

EnemyBos::EnemyBos():m_stMa(this)
{
}


EnemyBos::~EnemyBos()
{
	//影を除外する。
	g_graphicsEngine->GetShadowMap()->UnregistShadowCaster(&m_model);
	//キャラクターコントローラーを消去する
	m_charaCon.RemoveRigidBoby();
	//当たり判定を消去。
	g_hitObject->Delete(m_hit);
	//法線マップをロード。
	//ファイル名を使って、テクスチャをロードして、ShaderResrouceViewを作成する。
	DirectX::CreateDDSTextureFromFileEx(
		g_graphicsEngine->GetD3DDevice(), L"Assets/Assets/sprite/Mutant_normal.dds", 0,
		D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0, 0,
		false, nullptr, &m_normalMapSRV);
}

bool EnemyBos::Start()
{
	m_model.Init(L"Assets/modelData/enemy_Bos.cmo");
	//ノーマルマップをセットする。
	m_model.SetNormalMap(m_normalMapSRV);
	//tkaファイルの読み込み。
	//待機アニメーション
	m_animationClips[EnemyBosState::AnimationState::AnimIdle].Load(L"Assets/animData/enemy_Bos/enBosidle.tka");
	m_animationClips[EnemyBosState::AnimationState::AnimIdle].SetLoopFlag(true);
	//歩きアニメーション
	m_animationClips[EnemyBosState::AnimationState::AnimMove].Load(L"Assets/animData/enemy_Bos/enBosrun.tka");
	m_animationClips[EnemyBosState::AnimationState::AnimMove].SetLoopFlag(true);
	//攻撃アニメーション
	m_animationClips[EnemyBosState::AnimationState::AnimAttack].Load(L"Assets/animData/enemy_Bos/enBosattack.tka");
	m_animationClips[EnemyBosState::AnimationState::AnimAttack].SetLoopFlag(false);
	//ダメージアニメーション
	m_animationClips[EnemyBosState::AnimationState::AnimDamage].Load(L"Assets/animData/enemy_Bos/enBosdamage.tka");
	m_animationClips[EnemyBosState::AnimationState::AnimDamage].SetLoopFlag(false);
	//ダメージアニメーション
	m_animationClips[EnemyBosState::AnimationState::AnimDeath].Load(L"Assets/animData/enemy_Bos/enBosdeath.tka");
	m_animationClips[EnemyBosState::AnimationState::AnimDeath].SetLoopFlag(false);
	//アニメーションの初期化。
	m_animation.Init(
		m_model,								//アニメーションを流すスキンモデル。
												//これでアニメーションとスキンモデルが関連付けされる。
		m_animationClips,						//アニメーションクリップの配列。
		EnemyBosState::AnimationState::AnimNum//アニメーションクリップの数。
	);
	//hpバーのスプライト。
	//m_hpSprite.Init(L"Assets/sprite/hp_gauge.dds", 40.0f, 10.0f);
	m_shieldSprite.Init(L"Assets/sprite/shield.dds", 20.0f, 20.0f);
	m_position.y += 200.0f;
	//キャラクターコントローラーの初期化。
	m_charaCon.Init(50.0f, 110.0f, m_position);
	//エネミーのステートマシンのスタート関数を呼ぶ。
	//ステートマシンの初期化。
	m_stMa.Start();
	//当たり判定の作成。
	m_hit = g_hitObject->Create(
		&m_position, 90.0f,
		[&](float damage) {Damage(damage); },
		Hit::enemy
	);
	//SE
	m_se_damade.Init(L"Assets/sound/se_damage.wav");
	m_se_damade.SetVolume(1.0f);
	m_player = g_gameObjM->FindGO<Player>("Player");


	m_status.SetStandardHp(100.0f);
	m_status.SetStandardAgi(550.0f);
	m_status.SetStandardDef(6.0f);
	m_status.SetStandardAtk(17.0f);
	m_status.SetMaxLv(100);
	m_status.StatusUp();
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
	//fabsfは絶対値を求める関数！
	//角度はマイナスが存在するから、絶対値にする。
	if (m_toPlayerLen < 2500.0f) {
		m_bosBgm = true;
	}
	else {
		m_bosBgm = false;
	}
	if (m_toPlayerLen <= 160.0) {
		//プレイヤーとの距離が一定以下で
		//攻撃ステートをよべー。
		m_stMa.Change(EnemyBosState::MoveState::attack);
		m_stMa.StateAttack()->SetAttack(25, 10);
	}
	if(m_toPlayerLen > 160.0 &&
		m_stMa.StateAttack()->GetAtkFlag() == false)
	{
		m_stMa.Change(EnemyBosState::MoveState::move);
		m_stMa.StateAttack()->SetTiming(0);
	}
	if (m_stMa.StateAttack()->GetAtkFlag() == true) {
		//攻撃アニメーションが終了したら。
		if (m_animation.IsPlaying() == false) {
			m_stMa.StateAttack()->SetAtkFlag(false);
			m_stMa.StateAttack()->SetTiming(0);
		}
	}
}

//ダメージ
void EnemyBos::Damage(float damage)
{
	if (m_status.GetHp() > 0.0f) {
		m_se_damade.Play(false);
		if (!m_damageCut) {
			m_stMa.StateDamage()->SetDamage(damage);
			m_stMa.Change(EnemyBosState::MoveState::damage);
		}
		else {
			//攻撃をくらったのでHPからくらった分を引く
			float hp = m_status.GetHp();
			float Damage = damage - m_status.GetDef();
			Damage = max(0.0f, Damage);
			hp -= (Damage/3.0f);
			hp = min(hp, m_status.GetHp());
			hp = max(0.0f, hp);
			m_status.SetHp(hp);
		}
		m_damageCutCounter++;
	}
}

void EnemyBos::DamageCut()
{
	if (m_damageCutCounter >= 2 && m_damageCutSpan >= m_damageCutValue) {
		m_damageCut = true;
	}
	if (m_damageCut) {
		m_damageCutSpan -= 1.5f * (1.0f / 60.0f);
		m_damageCutSpan = max(0.0f, m_damageCutSpan);
		if (m_damageCutSpan <= 0.0f) {
			m_damageCut = false;
		}
	}
	else {
		m_damageCutSpan += 1.0f * (1.0f / 60.0f);
		m_damageCutSpan = min(m_damageCutValue, m_damageCutSpan);
	}
}

void EnemyBos::DamegeCutSprite()
{
	if (m_damageCut) {//ポジションを頭の上付近にする。
		auto pos = m_position;
		pos.y += 180.0f;
		//カメラのポジション向きのベクトルを取得。
		CVector3 s_angle;
		s_angle = g_camera3D.GetPosition() - m_position;
		s_angle.y = 0;
		s_angle.Normalize();
		//HPの画像がカメラに向かって前を向くようにする。
		float angle = acos(s_angle.Dot(m_Sprite_Front));
		angle = CMath::RadToDeg(angle);
		CVector3 axis;
		axis.Cross(m_Sprite_Front, s_angle);
		if (axis.y > 0) {
			m_Sprite_angle.SetRotationDeg(CVector3::AxisY(), angle);
		}
		else {
			m_Sprite_angle.SetRotationDeg(CVector3::AxisY()*-1, angle);
		}
		m_hpGauge = (m_status.GetHp() / m_status.GetMaxHp()) * m_spriteScale;

		//スプライトの更新
		m_shieldSprite.Update(pos, m_Sprite_angle, { 1.0f, 1.0f, 1.0f });
		//スプライトの表示
		m_shieldSprite.Draw(
			g_camera3D.GetViewMatrix(),
			g_camera3D.GetProjectionMatrix()
		);
	}
}

//HPを表示するスプライトのための関係。
/*void EnemyBos::HP_Gauge()
{
	//ポジションを頭の上付近にする。
	auto pos = m_position;
	auto pos_s = m_position;
	pos.y += 170.0f;
	pos_s.y += 180.0f;
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
	m_hpGauge = (m_status.GetHp() / m_status.GetMaxHp()) * m_spriteScale;

	//HPスプライトの更新
	m_hpSprite.Update(pos, m_Sprite_angle, { m_hpGauge, 1.0f, 1.0f });
	//HPスプライトの表示
	m_hpSprite.Draw(
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	); 
}*/

void EnemyBos::Update()
{
	if (m_status.GetHp() <= 0.0f) {
		m_moveSpeed *= 0.0f;
		m_stMa.Change(EnemyBosState::MoveState::death);
	}
	else {
		if (m_stMa.StateDamage()->GetDamageFlag() == false) {
			Search();
		}
		else {
			if (m_animation.IsPlaying() == false) {
				m_stMa.Change(EnemyBosState::MoveState::move);
				m_stMa.StateDamage()->SetDamageFlag(false);
			}
		}
	}
	//ステートマシンの更新
	m_stMa.Update();
	DamageCut();
	//重力加速度
	if (m_charaCon.IsOnGround()) {
		m_moveSpeed.y *= 0.0f; 
		m_speedY *= 0.0f;
	}
	else {
		m_speedY -= 980.0f * (1.0f / 60.0f);
		m_moveSpeed.y = m_speedY;
	}
	//キャラコンを使って移動する。
	m_position = m_charaCon.Execute(1.0f / 60.0f, m_moveSpeed);
	//シャドウキャスターを登録。
	g_graphicsEngine->GetShadowMap()->RegistShadowCaster(&m_model);
	//m_model.SetShadowReciever(true);
	//ワールド行列を求める。
	m_model.UpdateWorldMatrix(m_position, m_rotation, { 1.0f, 1.0f, 1.0f });
	//アニメーションを流す。
	m_animation.Update(1.0f / 30.0f);
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
	//HP_Gauge();
	DamegeCutSprite();
}
