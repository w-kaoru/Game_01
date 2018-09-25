#pragma once
class Player;

class GameCamera
{
public:
	GameCamera();
	~GameCamera();
	void SetPlayer(Player* player)
	{
		m_player = player;
	}
	void Update();
private:
	Player * m_player = nullptr;	//プレイヤー。
};


