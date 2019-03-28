#include "stdafx.h"
#include "Game.h"
#include "Player\Player.h"
#include "level/Level.h"
#include "GameCamera.h"
#include "Title.h"
#include "LightCamera.h"

//グローバルなアクセスポイントをグローバル変数として提供する。
Game* g_game = nullptr;

Game::Game()
{
	//m_player = g_gameObjM->NewGameObject<Player>();

}

Game::~Game()
{
}
void Game::Destroy()
{
	g_gameObjM->DeleteGameObject(m_gameCamera);
	g_gameObjM->DeleteGameObject(m_light);
	g_gameObjM->DeleteGameObject(m_player);
	//動的に確保したインスタンスを破棄。
	for (auto& enemy : m_enemyList) {
		g_gameObjM->DeleteGameObject(enemy);
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
			m_player = g_gameObjM->NewGameObject<Player>(0);
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
			m_enemy = g_gameObjM->NewGameObject<Enemy>(0);
			m_enemy->SetEnemySelect(1);
			m_enemy->SetPosition(objData.position);
			m_enemy->SetRotation(objData.rotation);
			m_enemy->GetPlayer(m_player);
			m_enemyList.push_back(m_enemy);
			return true;
		}
		return false;
	});
	m_gameCamera = g_gameObjM->NewGameObject<GameCamera>(1);
	m_gameCamera->SetPlayer(m_player);
	m_light = g_gameObjM->NewGameObject<LightCamera>(1);
	m_light->SetPlayer(m_player);
	g_game = this;

	//サウンドエンジンを初期化。
	m_soundEngine.Init();
	//BGM
	//m_bgm.Init(L"Assets/sound/bgm_00.wav");
	m_bgm.Play(true);
	return false;
}

void Game::Update()
{
	if (g_pad[0].IsTrigger(enButtonB)) {
		g_gameObjM->DeleteGameObject(this);
		//タイトルシーンの作成。
		g_gameObjM->NewGameObject<Title>();
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

