#pragma once
#include "../Library/GameObject.h"

class Camera : public GameObject {
public:
	Camera();

	void Update() override;
private:
	VECTOR CameraPos;		// カメラの座標
	VECTOR CameraTarget;	// カメラの注視点
};
