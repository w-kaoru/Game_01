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
	/*!
	* @brief	�R���X�g���N�^�B
	*/
	Game();
	/*!
	* @brief	�f�X�g���N�^
	*/
	~Game();
	/*!
	* @brief	�X�V�B
	*/
	void Update() override;
	/*!
	* @brief	�`��B
	*/
	void Draw() override;
private:
	Player m_player;						//�v���C���[
	GameCamera m_gameCamera;				//�Q�[���J�����B
	Level m_level;							//���x�����������B
	std::vector< Enemy* > m_enemyList;		//�G�l�~�[�̃��X�g�B
};

//�O���[�o���ȃA�N�Z�X�|�C���g���O���[�o���ϐ��Ƃ��Ē񋟂���B
extern Game* g_game;


