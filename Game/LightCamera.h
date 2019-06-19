#pragma once
class Player;

class LightCamera:public IGameObject
{
public:
	LightCamera();
	~LightCamera();
	bool Start()override;
	void Update()override;

	void SetPlayer(Player* player)
	{
		m_player = player;
	}
private:
	Player * m_player = nullptr;	//�v���C���[�B
	CVector3 m_LightCameraPos = CVector3::Zero();
};

