#pragma once

#include "gameObject\IGameObject.h"
//#include "Player\Player.h"
#include "level/Level.h"
//#include "GameCamera.h"
#include "Enemy\Enemy.h"
#include "Render\RenderTarget.h"
#include "graphics\SkinModel.h"
#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"

//�����͑O���錾�ł悢�I
struct PathPoint;
class Enemy;
class Player;
class GameCamera;
class LightCamera;

//�Q�[���N���X�B
class Game : public IGameObject
{
public:
	Game();
	~Game(); 
	bool Start();
	void Update() override;
	void Draw() override;
	void PreDraw() override;
	void PostDraw() override;
	void Destroy();
private:
	Player* m_player = nullptr;				//�v���C���[
	Enemy* m_enemy = nullptr;				//�G�l�~�[
	GameCamera* m_gameCamera= nullptr;		//�Q�[���J�����B
	LightCamera* m_light = nullptr;			//���C�g
	Level m_level;							//���x�����������B
	//Level* m_level = nullptr;				//���x�����������B
	std::vector< Enemy* > m_enemyList;		//�G�l�~�[�̃��X�g�B

	CSoundEngine m_soundEngine;				//�T�E���h�G���W���B
	CSoundSource m_bgm;						//BGM�B
};

//�O���[�o���ȃA�N�Z�X�|�C���g���O���[�o���ϐ��Ƃ��Ē񋟂���B
//extern Game* g_game;
