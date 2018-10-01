#pragma once
#include "gameObject\IGameObject.h"

class Title:public IGameObject
{
public:
	Title();
	~Title();
	//オーバーライドした関数にはoverride修飾子をつけていると幸せになれる。
	void Update() override;
	//オーバーライドした関数にはoverride修飾子をつけていると幸せになれる。
	//why?
	//下の関数のようにタイプミスをしたときにコンパイルエラーになって教えてくれる。
	//void Drow() override;
	void Draw() override;
private:
};
