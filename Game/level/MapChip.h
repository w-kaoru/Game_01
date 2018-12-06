#pragma once

#include "physics/PhysicsStaticObject.h"

class Level;
struct LevelObjectData;
class ShadowMap;

/*!
* @brief	マップチップ。
*/
class MapChip {
public:
	/*!
	* @brief	コンストラクタ。
	*@param[in] objData			オブジェクト情報。
	*/
	MapChip(const LevelObjectData& objData);
	/*!
	* @brief	描画。
	*/
	void Draw();
	void SetShadowMap(ShadowMap* shadowMap) {
		m_shadowMap = shadowMap;
	}
private:
	SkinModel m_model;		//!<モデル。
	PhysicsStaticObject m_physicsStaticObject;	//!<静的物理オブジェクト。
	ShadowMap* m_shadowMap = nullptr;
};