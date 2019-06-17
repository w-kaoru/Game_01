#include "stdafx.h"
#include "Enemy.h"
#include "Player\Player.h"
#include "../Game.h"


Enemy::Enemy():m_ensm(this)
{
}


Enemy::~Enemy()
{
	//影を除外する。
	g_graphicsEngine->GetShadowMap()->UnregistShadowCaster(&m_model);
	//キャラクターコントローラーを消去する。
	m_charaCon.RemoveRigidBoby();
	//当たり判定を消去。
	g_hitObject->Delete(m_hit);

	m_effect->Release();
}

void Enemy::Destroy()
{
}

bool Enemy::Start()
{
	//m_model.Init(L"Assets/modelData/enemy.cmo");

	//m_spd = 500.0f;
	switch (m_type)
	{
	case type_skeleton:
		m_model.Init(L"Assets/modelData/enemy_01.cmo");
		//tkaファイルの読み込み。
		//待機アニメーション
		m_animationClips[EnemyState::AnimationState::AnimIdle].Load(L"Assets/animData/enemy/enidle.tka");
		m_animationClips[EnemyState::AnimationState::AnimIdle].SetLoopFlag(true);
		//歩きアニメーション
		m_animationClips[EnemyState::AnimationState::AnimMove].Load(L"Assets/animData/enemy/enwalk.tka");
		m_animationClips[EnemyState::AnimationState::AnimMove].SetLoopFlag(true);
		//攻撃アニメーション
		m_animationClips[EnemyState::AnimationState::AnimAttack].Load(L"Assets/animData/enemy/enattack.tka");
		m_animationClips[EnemyState::AnimationState::AnimAttack].SetLoopFlag(false);
		//ダメージアニメーション
		m_animationClips[EnemyState::AnimationState::AnimDamage].Load(L"Assets/animData/enemy/endamage.tka");
		m_animationClips[EnemyState::AnimationState::AnimDamage].SetLoopFlag(false);
		//エネミーの大きさ。
		m_scale *= 2.0f;
		//攻撃を当てるタイミング
		m_atkHit = 25;
		break;
	case type_troll:
		m_model.Init(L"Assets/modelData/TrollGiant.cmo");
		//tkaファイルの読み込み。
		//待機アニメーション
		m_animationClips[EnemyState::AnimationState::AnimIdle].Load(L"Assets/animData/troll_anim/troll_idle.tka");
		m_animationClips[EnemyState::AnimationState::AnimIdle].SetLoopFlag(true);
		//歩きアニメーション
		m_animationClips[EnemyState::AnimationState::AnimMove].Load(L"Assets/animData/troll_anim/troll_walk.tka");
		m_animationClips[EnemyState::AnimationState::AnimMove].SetLoopFlag(true);
		//攻撃アニメーション
		m_animationClips[EnemyState::AnimationState::AnimAttack].Load(L"Assets/animData/troll_anim/troll_attack.tka");
		m_animationClips[EnemyState::AnimationState::AnimAttack].SetLoopFlag(false);
		//ダメージアニメーション
		m_animationClips[EnemyState::AnimationState::AnimDamage].Load(L"Assets/animData/troll_anim/troll_damage.tka");
		m_animationClips[EnemyState::AnimationState::AnimDamage].SetLoopFlag(false);
		m_scale *= 90.0f;
		//攻撃を当てるタイミング
		m_atkHit = 23;
		break;
	}
	//アニメーションの初期化。
	m_animation.Init(
		m_model,								//アニメーションを流すスキンモデル。
												//これでアニメーションとスキンモデルが関連付けされる。
		m_animationClips,						//アニメーションクリップの配列。
		EnemyState::AnimationState::AnimNum		//アニメーションクリップの数。
	);
	//hpバーのスプライト。
	m_hpSprite.Init(L"Assets/sprite/hp_gauge.dds", 40.0f, 10.0f);
	m_shieldSprite.Init(L"Assets/sprite/shield.dds", 20.0f, 20.0f);
	m_position.y = 200.0f;
	//キャラクターコントローラーの初期化。
	m_charaCon.Init(40.0f, 70.0f, m_position);
	//エネミーのステートマシンのスタート関数を呼ぶ。
	//ステートマシンの初期化。
	m_ensm.Start();
	//当たり判定の作成。
	m_hit = g_hitObject->Create(
		&m_position, 150.0f,
		[&](float damage) {Damage(damage); },
		Hit::enemy
	);
	//SE
	m_se_damade.Init(L"Assets/sound/se_damage.wav");
	m_se_damade.SetVolume(1.0f);	
	m_effect = Effekseer::Effect::Create(
		g_graphicsEngine->GetEffekseerManager(),
		(const EFK_CHAR*)L"Assets/effect/death.efk"
	);
	m_player = g_gameObjM->FindGO<Player>("Player");
	return true;
}

//プレイヤーに向かうベクトルと長さの取得。
void Enemy::PlLen()
{
	//エネミーからプレイヤーに伸びるベクトルを求める。
	m_moveSpeed = m_player->GetPosition() - m_position;
	//正規化を行う前に、プレイヤーまでの距離を求めておく。
	m_toPlayerLen = m_moveSpeed.Length();
	//正規化！
	m_moveSpeed.Normalize();
}

void Enemy::Search()
{
	PlLen();
	//前方向の取得。
	m_forward = CVector3::AxisZ();
	m_rotation.Multiply(m_forward);
	//fabsfは絶対値を求める関数！
	//角度はマイナスが存在するから、絶対値にする。
	if (m_toPlayerLen <= 160.0) {
		//プレイヤーとの距離が一定以下で
		//攻撃ステートをよべー。
		m_ensm.Change(EnemyState::MoveState::attack);
		m_ensm.StateAttack()->SetAttack(m_atkHit, m_atkAnimStart);
	}
	if(m_toPlayerLen > 160.0
		&&m_ensm.StateAttack()->GetAtkFlag() == false)
	{
		m_ensm.Change(EnemyState::MoveState::move);
		m_ensm.StateAttack()->SetTiming(0);
	}
	if (m_ensm.StateAttack()->GetAtkFlag() == true) {
		//攻撃アニメーションが終了したら。
		if (m_animation.IsPlaying() == false) {
			m_ensm.StateAttack()->SetAtkFlag(false);
			m_ensm.StateAttack()->SetTiming(0);
		}
	}
}
//ダメージ
void Enemy::Damage(float damage)
{
	m_se_damade.Play(false);
	if (!m_damageCut) {
		m_ensm.StateDamage()->SetDamage(damage);
		m_ensm.Change(EnemyState::MoveState::damage);
	}
	else {
		//攻撃をくらったのでHPからくらった分を引く
		float hp = m_status.GetHp();
		float Damage = damage - m_status.GetDef();
		Damage = max(0.0f, Damage);
		hp -= (Damage / 3.0f);
		hp = min(hp, m_status.GetHp());
		hp = max(0.0f, hp);
		m_status.SetHp(hp);
	}
	m_damageCutCounter++;
}

void Enemy::DamageCut()
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

//HPを表示するスプライトのための関係。
void Enemy::HP_Gauge()
{
	//ポジションを頭の上付近にする。
	auto pos = m_position;
	auto pos_s = m_position;
	pos.y += 200.0f;
	pos_s.y += 220.0f;
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
	if (m_damageCut) {
		//HPスプライトの更新
		m_shieldSprite.Update(pos_s, m_Sprite_angle, { 1.0f, 1.0f, 1.0f });
		//HPスプライトの表示
		m_shieldSprite.Draw(
			g_camera3D.GetViewMatrix(),
			g_camera3D.GetProjectionMatrix()
		);
	}
}

void Enemy::Update()
{
	if (m_isDeath == false) {
		if (m_ensm.StateDamage()->GetDamageFlag() == false) {
			Search();
		}
		else {
			if (m_animation.IsPlaying() == false) {
				m_ensm.Change(EnemyState::MoveState::move);
				m_ensm.StateDamage()->SetDamageFlag(false);
			}
		}
		//ステートマシンの更新
		m_ensm.Update();
		
		if (m_status.GetHp() < 0.01f&&m_isDeath == false) {
			m_isDeath = true;
			m_effectPos = m_position;
			//エフェクトの再生。
			m_playEffectHandle = g_graphicsEngine->GetEffekseerManager()->Play(
				m_effect, m_effectPos.x, m_effectPos.y+100.0f, m_effectPos.z
			);
			//エフェクトの拡大率。
			g_graphicsEngine->GetEffekseerManager()->SetScale(
				m_playEffectHandle,
				20.0f, 20.0f, 20.0f
			);
			m_position.y = -10000.0f;
			m_charaCon.SetPosition(m_position);
			g_gameObjM->FindGO<Game>("Game")->EnemyDeath();
			m_player->EXP(1);
		}
		DamageCut();
		//重力加速度
		if (m_charaCon.IsOnGround()) {
			m_moveSpeed.y = 0.0f;
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
		m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);
		//アニメーションを流す。
		m_animation.Update(1.0f / 30.0f);
	
	}
}

void Enemy::Draw()
{
	//モデルを表示する。
	m_model.Draw(
		enRenderMode_Normal,
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
}

void Enemy::PostDraw()
{
	//HPスプライトの表示をする。
	HP_Gauge();
}
