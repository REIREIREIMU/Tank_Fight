#pragma once
#include "../Library/GameObject.h"

class Player : public GameObject {
public:
	Player();
	~Player();

	void Update() override;
	void Draw() override;
private:
	int		m_handle;	//モデルのハンドル
	float	pos_x, 
			pos_y,
			pos_z;		//プレイヤー用座標
	VECTOR	Position;	//プレイヤー用ベクター

	/*enum Dir {
		Front = 0,
		Left,
		Back,
		Right,
	};
	Dir direction;
	int counter;*/
};