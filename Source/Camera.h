#pragma once
#include "../Library/GameObject.h"

class Camera : public GameObject {
public:
	Camera();

	void Update() override;
private:
	VECTOR m_cameraPos;		//カメラの座標
	VECTOR m_cameraTarget;	//カメラの注視点
};
