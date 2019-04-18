#pragma once

#include "gameObject\IGameObject.h"
//#include "Player\Player.h"
#include "level/Level.h"
//#include "GameCamera.h"
#include "Enemy\Enemy.h"
#include "EnemyBos/EnemyBos.h"
#include "Render\RenderTarget.h"
#include "graphics\SkinModel.h"


//�����͑O���錾�ł悢�I
struct PathPoint;
class Enemy;
class EnemyBos;
class Player;
class GameCamera;
class LightCamera;

//�Q�[���N���X�B
class Game : public IGameObject
{
public:
	Game();
	~Game(); 
	bool Start() override;
	void Update() override;
	void Draw() override;
	void PreDraw() override;
	void PostDraw() override;
	void Destroy() override;

	void EnemyDeath()
	{
		m_enemyDeath++;
	}
	void SetGameEndFlag(bool flag)
	{
		m_endFlag = flag;
	}
private:
	Player* m_player = nullptr;				//�v���C���[
	Enemy* m_enemy = nullptr;				//�G�l�~�[
	EnemyBos* m_enemyBos = nullptr;			//�G�l�~�[�{�X
	GameCamera* m_gameCamera= nullptr;		//�Q�[���J�����B
	LightCamera* m_light = nullptr;			//���C�g
	Level m_level;							//���x�����������B
	//Level* m_level = nullptr;				//���x�����������B
	std::vector< Enemy* > m_enemyList;		//�G�l�~�[�̃��X�g�B
	int m_enemyDeath = 0;					//�G�l�~�[�̎��񂾐��B
	CSoundSource m_bgm;						//BGM�B
	CSoundSource m_bgm_bos;					//BOS��BGM
	bool m_endFlag = false;					//�Q�[�W�̏I���B
};

//�O���[�o���ȃA�N�Z�X�|�C���g���O���[�o���ϐ��Ƃ��Ē񋟂���B
//extern Game* g_game;
