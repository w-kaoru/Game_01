#include "stdafx.h"
#include "InitLevel.h"
#include "Player\Player.h"
#include "Enemy\Enemy.h"
#include "EnemyBos/EnemyBos.h"
#include "Background.h"


InitLevel::InitLevel()
{
}


InitLevel::~InitLevel()
{
}

bool InitLevel::Start()
{
	//レベルを初期化。
	m_level.Init(
		L"Assets/level/Dungeon_01.tkl",
		[&](LevelObjectData& objData) {
		if (objData.EqualName(L"Dungeon") == true) {
			m_background = g_gameObjM->NewGO<Background>(0, "Dungeon");
			m_background->SetPosition(objData.position);
			m_background->SetRotation(objData.rotation);
			return true;
		}
		return true;
	});
	//レベルを初期化。
	m_level.Init(
		L"Assets/level/player_01.tkl",
		[&](LevelObjectData& objData) {
		if (objData.EqualName(L"plpath") == true) {
			m_player = g_gameObjM->NewGO<Player>(0, "Player");
			m_player->SetPosition(objData.position);
			//ステータスの設定
			m_player->GetStatus()->SetLv(1);
			m_player->GetStatus()->SetStandardHp(60);
			m_player->GetStatus()->SetStandardAgi(1150.0f);
			m_player->GetStatus()->SetStandardDef(1.0f);
			m_player->GetStatus()->SetStandardAtk(12.5f);
			m_player->GetStatus()->SetMaxLv(9);
			m_player->GetStatus()->StatusUp();
			return true;
		}
		return true;
	});
	m_level.Init(
		L"Assets/level/enemy_01.tkl",
		[&](LevelObjectData& objData) {
		if (objData.EqualName(L"enpath") == true) {
			//エネミー！！！
			char enemyName[256];
			sprintf(enemyName, "EnemyS%d", m_enemyNo++);
			m_enemy = g_gameObjM->NewGO<Enemy>(0, enemyName);
			m_enemy->SetEnemyType(Enemy::EnemyType::type_skeleton);
			m_enemy->SetPosition(objData.position);
			m_enemy->SetRotation(objData.rotation);
			m_enemy->SetPlayer(m_player);
			m_enemyList.push_back(m_enemy);
			//エネミーのレベル
			m_enemy->GetStatus()->SetLv(3);
			m_enemy->GetStatus()->SetStandardHp(30.0f);
			m_enemy->GetStatus()->SetStandardAgi(550.0f);
			m_enemy->GetStatus()->SetStandardDef(1.5f);
			m_enemy->GetStatus()->SetStandardAtk(3.0f);
			m_enemy->GetStatus()->StatusUp();
			return true;
		}
		if (objData.EqualName(L"enpath2") == true) {
			//エネミー！！！
			char enemyName[256];
			sprintf(enemyName, "EnemyT%d", m_enemyNo++);
			m_enemy = g_gameObjM->NewGO<Enemy>(0, enemyName);
			m_enemy->SetEnemyType(Enemy::EnemyType::type_troll);
			m_enemy->SetPosition(objData.position);
			m_enemy->SetRotation(objData.rotation);
			m_enemy->SetPlayer(m_player);
			m_enemyList.push_back(m_enemy);
			//エネミーのレベル
			m_enemy->GetStatus()->SetLv(3);
			m_enemy->GetStatus()->SetStandardHp(35.0f);
			m_enemy->GetStatus()->SetStandardAgi(550.0f);
			m_enemy->GetStatus()->SetStandardDef(1.0f);
			m_enemy->GetStatus()->SetStandardAtk(3.5f);
			m_enemy->GetStatus()->StatusUp();
			return true;
		}
		return true;
	});
	m_level.Init(
		L"Assets/level/enemy_02.tkl",
		[&](LevelObjectData& objData) {
		if (objData.EqualName(L"enpath") == true) {
			//エネミー！！！
			char enemyName[256];
			sprintf(enemyName, "EnemyS%d", m_enemyNo++);
			m_enemy = g_gameObjM->NewGO<Enemy>(0, enemyName);
			m_enemy->SetEnemyType(Enemy::EnemyType::type_skeleton);
			m_enemy->SetPosition(objData.position);
			m_enemy->SetRotation(objData.rotation);
			m_enemy->SetPlayer(m_player);
			m_enemyList.push_back(m_enemy);
			//エネミーのレベル
			m_enemy->GetStatus()->SetLv(7);
			m_enemy->GetStatus()->SetStandardHp(30.0f);
			m_enemy->GetStatus()->SetStandardAgi(550.0f);
			m_enemy->GetStatus()->SetStandardDef(1.5f);
			m_enemy->GetStatus()->SetStandardAtk(3.0f);
			m_enemy->GetStatus()->StatusUp();
			return true;
		}
		if (objData.EqualName(L"enpath2") == true) {
			//エネミー！！！
			char enemyName[256];
			sprintf(enemyName, "EnemyT%d", m_enemyNo++);
			m_enemy = g_gameObjM->NewGO<Enemy>(0, enemyName);
			m_enemy->SetEnemyType(Enemy::EnemyType::type_troll);
			m_enemy->SetPosition(objData.position);
			m_enemy->SetRotation(objData.rotation);
			m_enemy->SetPlayer(m_player);
			m_enemyList.push_back(m_enemy);
			//エネミーのレベル
			m_enemy->GetStatus()->SetLv(6);
			m_enemy->GetStatus()->SetStandardHp(35.0f);
			m_enemy->GetStatus()->SetStandardAgi(550.0f);
			m_enemy->GetStatus()->SetStandardDef(1.0f);
			m_enemy->GetStatus()->SetStandardAtk(3.5f);
			m_enemy->GetStatus()->StatusUp();
			return true;
		}
		return true;
	});

	m_level.Init(
		L"Assets/level/enemyBos.tkl",
		[&](LevelObjectData& objData) {
		if (objData.EqualName(L"enpath") == true) {
			//エネミー！！！
			m_enemyBos = g_gameObjM->NewGO<EnemyBos>(0, "EnemyBos");
			m_enemyBos->SetPosition(objData.position);
			m_enemyBos->SetRotation(objData.rotation);
			m_enemyBos->GetPlayer(m_player);
			m_enemyBos->GetStatus()->SetLv(8);
			m_enemyBos->GetStatus()->SetStandardHp(100.0f);
			m_enemyBos->GetStatus()->SetStandardAgi(500.0f);
			m_enemyBos->GetStatus()->SetStandardDef(6.0f);
			m_enemyBos->GetStatus()->SetStandardAtk(17.0f);
			m_enemyBos->GetStatus()->StatusUp();
			return true;
		}
		return true;
	});
	return false;
}

void InitLevel::Update()
{
}

void InitLevel::Destroy()
{
}
