#pragma once
#include "gameObject\IGameObject.h"

class Title:public IGameObject
{
public:
	Title();
	~Title();
	bool Start() override;
	//オーバーライドした関数にはoverride修飾子をつけていると幸せになれる。
	void Update() override;
	//オーバーライドした関数にはoverride修飾子をつけていると幸せになれる。
	//why?
	//下の関数のようにタイプミスをしたときにコンパイルエラーになって教えてくれる。
	void PostDraw() override;
private:
	Sprite m_titelBackSprite;
	Sprite m_titelSprite;
	CSoundSource m_bgm;						//BGM。
};
