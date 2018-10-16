#pragma once

#include "gameObject\IGameObject.h"
#include "Player.h"
#include "level/Level.h"
#include "GameCamera.h"
#include "Enemy.h"

//�����͑O���錾�ł悢�I
struct PathPoint;
class Enemy;

//�Q�[���N���X�B
class Game : public IGameObject
{
public:
	Game();
	~Game();
	//bool Start();
	void Update() override;
	void Draw() override;
	void Destroy();
private:
	Player* m_player = nullptr;				//�v���C���[
	Enemy* m_enemy = nullptr;				//�G�l�~�[
	GameCamera m_gameCamera;				//�Q�[���J�����B
	Level m_level;							//���x�����������B
	//Level* m_level = nullptr;				//���x�����������B
	std::vector< Enemy* > m_enemyList;		//�G�l�~�[�̃��X�g�B
};

//�O���[�o���ȃA�N�Z�X�|�C���g���O���[�o���ϐ��Ƃ��Ē񋟂���B
extern Game* g_game;


