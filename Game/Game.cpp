#include "stdafx.h"
#include "Game.h"
#include "Player.h"
#include "level/Level.h"
#include "GameCamera.h"
#include "Title.h"

//�O���[�o���ȃA�N�Z�X�|�C���g���O���[�o���ϐ��Ƃ��Ē񋟂���B
Game* g_game = nullptr;

Game::Game()
{
	m_player = g_gameObjM->NewGameObject<Player>();
	g_game = this;
	//���x�����������B
	//m_level.Init(L"Assets/level/stage_00.tkl", nullptr);
	//m_player.SetPosition({ 0.0f,0.0f,0.0f });
	m_level.Init(L"Assets/level/stage_00.tkl", [&](LevelObjectData& objData) {
		if (objData.EqualName(L"Thethief_H") == true) {
			//Unity�����B
			m_player->SetPosition(objData.position);
			return true;
		}
		else if (objData.EqualName(L"enemy") == true) {
			//�G�l�~�[�I�I�I
			//auto enemy = new Enemy(objData.position, objData.rotation, &m_player);
			auto m_enemy = g_gameObjM->NewGameObject<Enemy>();
			m_enemyList.push_back(m_enemy);
			return true;
		}
		return false;
	});
	m_gameCamera.SetPlayer(m_player);
	//return false;
}


Game::~Game()
{
	g_game = nullptr;
	//���I�Ɋm�ۂ����C���X�^���X��j���B
	for (auto& enemy : m_enemyList) {
		delete enemy;
	}
}

void Game::Update()
{
	//�v���C���[�̍X�V�B
	
	m_player->Update();
	//Enemy���X�V�B
	for (auto& enemy : m_enemyList) {
		enemy->Update();
		if (enemy->GetEnemydead() == true) {
			m_enemyList.erase(
				std::remove(m_enemyList.begin(), m_enemyList.end(), enemy),
				m_enemyList.end()
			);
		}
	}
	m_gameCamera.Update();
	if (g_pad[0].IsTrigger(enButtonB)) {
		delete g_game;
		//�^�C�g���V�[���̍쐬�B
		g_gameObjM->NewGameObject<Title>();
	}
	
}

void Game::Draw()
{
	//�v���C���[�̕`��B
	m_player->Draw();
	//���x����`��B
	m_level.Draw();
	//�G�l�~�[��`��B
	for (auto& enemy : m_enemyList) {
		enemy->Draw();
	}
}
