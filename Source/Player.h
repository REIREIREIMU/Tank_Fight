#pragma once
#include "../Library/GameObject.h"

class Object;

class Player : public GameObject {
public:
	Player(Object* object);
	~Player();

	void Update() override;
	void Draw() override;

private:
	int	Body_m_handle,  // 車体のハンドル
		Head_m_handle;  // 砲塔のハンドル

	float pos_x, 
		  pos_y1,	// 車体用 Y座標
		  pos_y2,	// 砲塔用 Y座標
		  pos_z;	// プレイヤー用座標（x,y,z）

	// プレイヤー移動量
	float move_x, move_z;

	// プレイヤー用ベクター
	VECTOR Position;		// 移動
	VECTOR Body_Rotation,	// 回転(車体)
		   Head_Rotation;	// 回転(砲塔)

	// 向いている方向に前進後進する為のベクター
	VECTOR forward;

	// 当たり判定用（壁）
	Object* object;

	// マウスポインター用ベクター
	VECTOR GetMouseWorldPos();

	/*enum Dir {
		Front = 0,
		Left,
		Back,
		Right,
	};
	Dir direction;
	int counter;*/
};