#include "Camera.h"

Camera::Camera()
{
	// 描画範囲（ニア・ファー）
	SetCameraNearFar(2.0f, 8000.0f);

	// 初期位置
	CameraPos = VGet(0.0f, 14.0f, -5.0f);	// カメラ位置
	CameraTarget = VGet(0.0f, 0.0f, 0.0f);		// 注視点
}

void Camera::Update()
{
	// カメラの設定
	SetCameraPositionAndTarget_UpVecY(CameraPos, CameraTarget);
}
