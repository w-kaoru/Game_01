#include "stdafx.h"
#include "Game.h"
#include "Player\Player.h"
#include "level/Level.h"
#include "GameCamera.h"
#include "Title.h"
#include "GameEnd.h"
#include "LightCamera.h"

//グローバルなアクセスポイントをグローバル変数として提供する。
//Game* g_game = nullptr;

Game::Game()
{
	//m_player = g_gameObjM->NewGameObject<Player>();

}

Game::~Game()
{
}
void Game::Destroy()
{
	g_gameObjM->DeleteGO(m_gameCamera);
	g_gameObjM->DeleteGO(m_light);
	g_gameObjM->DeleteGO(m_player);
	//動的に確保したインスタンスを破棄。
	for (auto& enemy : m_enemyList) {
		g_gameObjM->DeleteGO(enemy);
	}
	if (m_enemyBos) {
		g_gameObjM->DeleteGO(m_enemyBos);
	}
}

bool Game::Start()
{
	//レベルを初期化。
	m_level.Init(
		L"Assets/level/Dungeon_01.tkl",
		[&](LevelObjectData& objData) {
		return false;
	});
	//レベルを初期化。
	m_level.Init(
		L"Assets/level/player_01.tkl",
		[&](LevelObjectData& objData) {
		if (objData.EqualName(L"plpath") == true) {
			m_player = g_gameObjM->NewGO<Player>(0);
			m_player->SetPosition(objData.position);
			//ステータスの設定
			m_player->GetStatus()->SetLv(1);
			m_player->GetStatus()->SetHp(60);
			m_player->GetStatus()->SetAgi(1150.0f);
			m_player->GetStatus()->SetDef(1.0f);
			m_player->GetStatus()->SetAtk(5.0f);
			m_player->GetStatus()->SetMaxLv(9);
			m_player->GetStatus()->StatusUp();
			return true;
		}
		return false;
	});
	m_level.Init(
		L"Assets/level/enemy_01.tkl",
		[&](LevelObjectData& objData) {
		if (objData.EqualName(L"enpath") == true) {
			//エネミー！！！
			m_enemy = g_gameObjM->NewGO<Enemy>(0);
			m_enemy->SetEnemyType(Enemy::EnemyType::type_skeleton);
			m_enemy->SetPosition(objData.position);
			m_enemy->SetRotation(objData.rotation);
			m_enemy->SetPlayer(m_player);
			m_enemyList.push_back(m_enemy);
			//エネミーのレベル
			m_enemy->GetStatus()->SetLv(3);
			m_enemy->GetStatus()->SetHp(30.0f);
			m_enemy->GetStatus()->SetAgi(550.0f);
			m_enemy->GetStatus()->SetDef(1.5f);
			m_enemy->GetStatus()->SetAtk(3.0f);
			m_enemy->GetStatus()->StatusUp();
			return true;
		}
		if (objData.EqualName(L"enpath2") == true) {
			//エネミー！！！
			m_enemy = g_gameObjM->NewGO<Enemy>(0);
			m_enemy->SetEnemyType(Enemy::EnemyType::type_troll);
			m_enemy->SetPosition(objData.position);
			m_enemy->SetRotation(objData.rotation);
			m_enemy->SetPlayer(m_player);
			m_enemyList.push_back(m_enemy);
			//エネミーのレベル
			m_enemy->GetStatus()->SetLv(3); 
			m_enemy->GetStatus()->SetHp(35.0f);
			m_enemy->GetStatus()->SetAgi(550.0f);
			m_enemy->GetStatus()->SetDef(1.0f);
			m_enemy->GetStatus()->SetAtk(3.5f);
			m_enemy->GetStatus()->StatusUp();
			return true;
		}
		return false;
	});
	m_level.Init(
		L"Assets/level/enemy_02.tkl",
		[&](LevelObjectData& objData) {
		if (objData.EqualName(L"enpath") == true) {
			//エネミー！！！
			m_enemy = g_gameObjM->NewGO<Enemy>(0);
			m_enemy->SetEnemyType(Enemy::EnemyType::type_skeleton);
			m_enemy->SetPosition(objData.position);
			m_enemy->SetRotation(objData.rotation);
			m_enemy->SetPlayer(m_player);
			m_enemyList.push_back(m_enemy);
			//エネミーのレベル
			m_enemy->GetStatus()->SetLv(7);
			m_enemy->GetStatus()->SetHp(30.0f);
			m_enemy->GetStatus()->SetAgi(550.0f);
			m_enemy->GetStatus()->SetDef(1.5f);
			m_enemy->GetStatus()->SetAtk(3.0f);
			m_enemy->GetStatus()->StatusUp();
			return true;
		}
		if (objData.EqualName(L"enpath2") == true) {
			//エネミー！！！
			m_enemy = g_gameObjM->NewGO<Enemy>(0);
			m_enemy->SetEnemyType(Enemy::EnemyType::type_troll);
			m_enemy->SetPosition(objData.position);
			m_enemy->SetRotation(objData.rotation);
			m_enemy->SetPlayer(m_player);
			m_enemyList.push_back(m_enemy);
			//エネミーのレベル
			m_enemy->GetStatus()->SetLv(6);
			m_enemy->GetStatus()->SetHp(35.0f);
			m_enemy->GetStatus()->SetAgi(550.0f);
			m_enemy->GetStatus()->SetDef(1.0f);
			m_enemy->GetStatus()->SetAtk(3.5f);
			m_enemy->GetStatus()->StatusUp();
			return true;
		}
		return false;
	});
	
	m_level.Init(
			L"Assets/level/enemyBos.tkl",
			[&](LevelObjectData& objData) {
			if (objData.EqualName(L"enpath") == true) {
				//エネミー！！！
				m_enemyBos = g_gameObjM->NewGO<EnemyBos>(0);
				m_enemyBos->SetPosition(objData.position);
				m_enemyBos->SetRotation(objData.rotation);
				m_enemyBos->GetPlayer(m_player);
				m_enemyBos->GetStatus()->SetLv(8);
				m_enemyBos->GetStatus()->SetHp(100.0f);
				m_enemyBos->GetStatus()->SetAgi(500.0f);
				m_enemyBos->GetStatus()->SetDef(6.0f);
				m_enemyBos->GetStatus()->SetAtk(17.0f);
				m_enemyBos->GetStatus()->StatusUp();
				return true;
			}
			return false;
		});
	m_gameCamera = g_gameObjM->NewGO<GameCamera>(1);
	m_gameCamera->SetPlayer(m_player);
	m_light = g_gameObjM->NewGO<LightCamera>(1);
	m_light->SetPlayer(m_player);
	//BGM
	m_bgm.Init(L"Assets/sound/bgm_Dungeon.wav");
	m_bgm_bos.Init(L"Assets/sound/bgm_Bos.wav");
	m_bgm.Play(true);
	m_bgm.SetVolume(0.17f);
	m_bgm_bos.SetVolume(0.17f);

	//HPの画像の読み込み
	m_ui[0].Init(L"Assets/sprite/item_controller .dds", m_spriteScaleX, m_spriteScaleY);
	m_ui[1].Init(L"Assets/sprite/game_ken.dds", m_spriteScaleX, m_spriteScaleY);
	m_ui[2].Init(L"Assets/sprite/shield.dds", m_spriteScaleX, m_spriteScaleY);
	m_ui[3].Init(L"Assets/sprite/frame.dds", 1280.0f, 300);
	m_ui[3].SetAlpha(0.5f);
	return false;
}

void Game::Update()
{
	if (g_pad[0].IsTrigger(enButtonSelect)) {
		g_gameObjM->DeleteGO(this);
		//タイトルシーンの作成。
		//g_gameObjM->NewGameObject<Title>();
		g_gameObjM->NewGO<GameEnd>()->SetGameEnd(GameEnd::GameEndState::gameDefault);
	}
	if (!m_enemyBos->GetBGMFlag()) {

		m_bgm_bos.Stop();
		m_bgm.Play(true);
	}else{
		m_bgm.Stop();
		m_bgm_bos.Play(true);
	}
}

//スプライトの描画
void Game::SpriteDraw()
{
	m_spriteRot.SetRotationDeg(CVector3::AxisZ(), -5.0f);
	//スプライトの更新
	m_ui[0].Update(
		{ -610.0f, 300.0f, 0.0f },
		CQuaternion::Identity(),
		{ 1.0f, 1.0f, 1.0f },
		{ 0.0f,1.0f }
	);
	//スプライトの更新
	m_ui[1].Update(
		{ -630.0f, 270.0f, 0.0f },
		m_spriteRot,
		{ 0.3f, 0.3f, 1.0f },
		{ 0.0f,1.0f }
	);
	//スプライトの更新
	m_ui[2].Update(
		{ -572.0f, 310.0f, 0.0f },
		CQuaternion::Identity(),
		{ 0.2f, 0.2f, 1.0f },
		{ 0.0f,1.0f }
	);
	//スプライトの更新
	m_ui[3].Update(
		{ -640.0f, 370.0f ,0.0f},
		CQuaternion::Identity(),
		{ 0.5f, 0.2f, 1.0f },
		{ 0.0f,1.0f }
	);
	//スプライトを２次元で表示をする。
	m_ui[3].Draw(
		g_camera2D.GetViewMatrix(),
		g_camera2D.GetProjectionMatrix()
	);
	//スプライトを２次元で表示をする。
	m_ui[0].Draw(
		g_camera2D.GetViewMatrix(),
		g_camera2D.GetProjectionMatrix()
	);
	//スプライトを２次元で表示をする。
	m_ui[1].Draw(
		g_camera2D.GetViewMatrix(),
		g_camera2D.GetProjectionMatrix()
	);
	m_ui[2].Draw(
		g_camera2D.GetViewMatrix(),
		g_camera2D.GetProjectionMatrix()
	);
}

void Game::PreDraw()
{
}

void Game::Draw()
{
	m_level.Draw();
}

void Game::PostPostDraw()
{
	SpriteDraw();
}
