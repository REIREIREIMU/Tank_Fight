#pragma once
#include "../Library/GameObject.h"

class Object : public GameObject {
public:
	Object();
	~Object();

	void Update() override;
	void Draw() override;

private:
	int	Ground_m_handle; // 地面のハンドル
	int	Block_m_handle;	 // ブロックのハンドル

	float pos_x, pos_y,	pos_z;	
};