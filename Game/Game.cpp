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
			m_enemy->SetEnemyType(Enemy::EnemyType::type_troll);
			m_enemy->SetPosition(objData.position);
			m_enemy->SetRotation(objData.rotation);
			m_enemy->SetPlayer(m_player);
			m_enemyList.push_back(m_enemy);
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
	//m_enemyDeath = m_enemyList.size();
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
		m_level.Init(
			L"Assets/level/enemyBos.tkl",
			[&](LevelObjectData& objData) {
			if (objData.EqualName(L"enpath") == true) {
				//エネミー！！！
				m_enemyBos = g_gameObjM->NewGO<EnemyBos>(0);
				m_enemyBos->SetPosition(objData.position);
				m_enemyBos->SetRotation(objData.rotation);
				m_enemyBos->GetPlayer(m_player);
				return true;
			}
			return false;
		});
		m_enemyDeath = 0;
	}
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
}
