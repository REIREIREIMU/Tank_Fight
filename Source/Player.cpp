#include "Player.h"

static const float Speed = 0.05f;
static const float Angle = 0.02f;

Player::Player() :m_handle(-1), 
	pos_x(0.0f), pos_y(0.0f), pos_z(0.0f)
{
	// 3Dモデルの読み込み
	m_handle = MV1LoadModel("Assets/Player_Tank.mv1");

	// 位置管理
	Position  = VGet( pos_x,  0.0f, pos_z);
	Rotation  = VGet(  0.0f, pos_y,  0.0f);
	forward.x = sinf(pos_y);
	forward.z = cosf(pos_y);

	// モデルの位置を設定する
	MV1SetPosition	 ( m_handle, Position); // 位置座標
	MV1SetRotationXYZ( m_handle, Rotation); // 回転座標
}

Player::~Player()
{
	//モデルの削除
	MV1DeleteModel(m_handle);
}

void Player::Update()
{
	// 回転操作
	if (CheckHitKey(KEY_INPUT_A)){ // 左回転
		pos_y -= Angle;
	}
	if (CheckHitKey(KEY_INPUT_D)){ // 右回転
		pos_y += Angle;
	}

	// 移動操作
	if (CheckHitKey(KEY_INPUT_W)) { // 前進
		pos_x -= forward.x * Speed;
		pos_z -= forward.z * Speed;
	}
	if (CheckHitKey(KEY_INPUT_S)) { // 後進
		pos_x += forward.x * Speed;
		pos_z += forward.z * Speed;
	}

	// 位置管理
	Position  = VGet(pos_x, 0.0f, pos_z);
	Rotation  = VGet(0.0f, pos_y, 0.0f);
	forward.x = sinf(pos_y);
	forward.z = cosf(pos_y);

	// モデルの位置を設定する
	MV1SetPosition   (m_handle, Position); // 位置座標
	MV1SetRotationXYZ(m_handle, Rotation); // 回転座標

}

void Player::Draw()
{
	//モデルの表示
	MV1DrawModel(m_handle);
}
