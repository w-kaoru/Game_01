#pragma once
#include "IScene.h"

class Title : public IScene
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
