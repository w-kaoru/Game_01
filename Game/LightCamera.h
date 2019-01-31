#pragma once
class Player;

class LightCamera:public IGameObject
{
public:
	LightCamera();
	~LightCamera();

	void SetPlayer(Player* player)
	{
		m_player = player;
	}
	void Update();
private:
	Player * m_player = nullptr;	//プレイヤー。
};

