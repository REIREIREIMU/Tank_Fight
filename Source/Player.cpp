#include "Player.h"

static const float Speed = 1.5f;

Player::Player() :m_handle(-1), 
	pos_x(0.0f), pos_y(0.0f), pos_z(0.0f)
{
	// 3Dモデルの読み込み
	m_handle = MV1LoadModel("Assets/Player_Tank.mv1");

	// 位置管理
	Position = VGet( pos_x, pos_y, pos_z);

	// モデルの位置を設定する
	MV1SetPosition ( m_handle, Position);

	/*direction = Dir::Front;
	counter = 0;*/
	SetDrawOrder(-1);
}

Player::~Player()
{
	//モデルの削除
	MV1DeleteModel(m_handle);
}

void Player::Update()
{
	/*if (CheckHitKey(KEY_INPUT_W)) {
		positionY -= Speed;
		direction = Dir::Back;
	}
	if (CheckHitKey(KEY_INPUT_S)) {
		positionY += Speed;
		direction = Dir::Front;
	}
	if (CheckHitKey(KEY_INPUT_D)) {
		positionX += Speed;
		direction = Dir::Right;
	}
	if (CheckHitKey(KEY_INPUT_A)) {
		positionX -= Speed;
		direction = Dir::Left;
	}
	counter++;*/
}

void Player::Draw()
{
	//モデルの表示
	MV1DrawModel(m_handle);
}
