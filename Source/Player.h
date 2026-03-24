#pragma once
#include "../Library/GameObject.h"

class Player : public GameObject {
public:
	Player();
	~Player();

	void Update() override;
	void Draw() override;

	// Y軸に回転する為の「ラジアン」
	float  GetAngleY()  const { return pos_y; }

private:
	int		m_handle;	//モデルのハンドル
	float	pos_x, 
			pos_y,
			pos_z;		//プレイヤー用座標（x,y,z）

	//プレイヤー用ベクター
	VECTOR Position;	// 移動
	VECTOR Rotation;	// 回転

	//向いている方向に前進後進する為のベクトル
	VECTOR forward;

	/*enum Dir {
		Front = 0,
		Left,
		Back,
		Right,
	};
	Dir direction;
	int counter;*/
};