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
			m_player->GetStatus()->SetLv(1);
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
			return true;
		}
		return false;
	});
	
	m_level.Init(
			L"Assets/level/enemyBos.tkl",
			[&](LevelObjectData& objData) {
			if (objData.EqualName(L"enpath") == true) {
				m_bosPos = objData.position;
				m_bosRot = objData.rotation;
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
	m_ui_01.Init(L"Assets/sprite/item_controller .dds", m_spriteScaleX, m_spriteScaleY);
	m_ui_02.Init(L"Assets/sprite/game_ken.dds", m_spriteScaleX, m_spriteScaleY);
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
	if (m_enemyDeath >= m_enemyList.size()) {
		m_bgm.Stop();
		m_bgm_bos.Play(true);

		//エネミー！！！
		m_enemyBos = g_gameObjM->NewGO<EnemyBos>(0);
		m_enemyBos->SetPosition(m_bosPos);
		m_enemyBos->SetRotation(m_bosRot);
		m_enemyBos->GetPlayer(m_player);
		m_enemyBos->GetStatus()->SetLv(8);

		m_enemyDeath = 0;
	}
}

//スプライトの描画
void Game::SpriteDraw()
{
	m_spriteRot.SetRotationDeg(CVector3::AxisZ(), -5.0f);
	//スプライトの更新
	m_ui_01.Update(
		{ -610, 300, 0.0f },
		CQuaternion::Identity(),
		{ 1.0f, 1.0f, 1.0f },
		{ 0.0f,1.0f }
	);
	//スプライトの更新
	m_ui_02.Update(
		{ -630, 270, 0.0f },
		m_spriteRot,
		{ 0.3f, 0.3f, 1.0f },
		{ 0.0f,1.0f }
	);
	//スプライトを２次元で表示をする。
	m_ui_01.Draw(
		g_camera2D.GetViewMatrix(),
		g_camera2D.GetProjectionMatrix()
	);
	//スプライトを２次元で表示をする。
	m_ui_02.Draw(
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

void Game::PostDraw()
{
	SpriteDraw();
}
