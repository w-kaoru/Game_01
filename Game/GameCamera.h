#pragma once
#include "graphics/SpringCamera.h"
class Player;

class GameCamera:public IGameObject
{
public:
	GameCamera();
	~GameCamera();
	bool Start()override;
	void SetPlayer(Player* player)
	{
		m_player = player;
	}
	void Update();
private:
	Player * m_player = nullptr;	//�v���C���[�B
	SpringCamera m_springCamera;
};


