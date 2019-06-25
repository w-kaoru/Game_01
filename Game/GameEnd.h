#pragma once
class GameEnd:public IGameObject
{
public:
	enum GameEndState
	{
		gameCleared,
		gameOver,
		Ground,
		gameDefault
	};
	GameEnd();
	~GameEnd();
	bool Start() override;
	void Update() override;
	void PostDraw() override;
	void Destroy() override;

	void SelectYesNo();
	void SetGameEnd(GameEndState state)
	{
		m_ges = state;
	}
private:
	Sprite m_gameClearedSprite;
	Sprite m_gameOverSprite;
	Sprite m_gameEndSprite;
	Sprite m_titelBackSprite;
	Sprite m_ArrowSprite;
	GameEndState m_ges = gameDefault;
	CVector3 m_ArrowPos = CVector3::Zero();
	const CVector3 m_ArrowYesPos = { -200.0f,-70.0f,0.0f };
	const CVector3 m_ArrowNoPos = { -200.0f,-235.0f,0.0f };
	bool m_yesNoFlag = true;
};

