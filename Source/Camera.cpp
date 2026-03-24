#include "Camera.h"

Camera::Camera()
{
	//奥行10～8000までをカメラの描画範囲とする
	SetCameraNearFar(10.0f, 8000.0f);

	//カメラ位置の初期位置
	m_cameraPos		= VGet( 0.0f, 10.0f, -15.0f);

	//注視点の初期位置
	m_cameraTarget	= VGet( 0.0f, 1.0f, 0.0f);
}

void Camera::Update()
{
    // カメラの設定
    SetCameraPositionAndTarget_UpVecY( m_cameraPos, m_cameraTarget);
}
