#pragma once
#include "../Library/GameObject.h"

class Object : public GameObject {
public:
	Object();
	~Object();

	void Update() override;
	void Draw() override;

private:
	int	Block_m_handle;	// ブロックのハンドル

	float pos_x, pos_y,	pos_z;	
};