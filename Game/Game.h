#pragma once
#include "level/Level.h"
#include "graphics\RenderTarget.h"
#include "graphics\SkinModel.h"


//これらは前方宣言でよい！
class Stage;
class UI;
//ゲームクラス。
class Game : public IGameObject
{
public:
	Game();
	~Game(); 
	bool Start() override;
	void Update() override;
	void Draw() override;
	void PreDraw() override;
	void Destroy() override;
private:
	UI* m_ui = nullptr;
	Stage* m_stage = nullptr;
};

