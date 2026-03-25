#include "Player.h"
#include "Object.h"
#include <cmath>

static const float Speed = 0.05f;
static const float Angle = 0.02f;

Player::Player(Object* obj) :
	Body_m_handle(-1), Head_m_handle(-1),
	pos_x(0.0f), pos_y1(DX_PI_F), pos_y2(0.0f), pos_z(0.0f), object(obj)
{
	// 3Dモデルの読み込み
	Body_m_handle = MV1LoadModel("Assets/Player_Tank_Body.mv1");
	Head_m_handle = MV1LoadModel("Assets/Player_Tank_Head.mv1");

	// 位置管理
	Position      = VGet( pos_x,  0.0f, pos_z);
	Body_Rotation = VGet( 0.0f, pos_y1,  0.0f); // 車体用回転
	Head_Rotation = VGet( 0.0f, pos_y2,  0.0f); // 砲塔用回転

	forward.x	  = sinf(pos_y1);
	forward.z	  = cosf(pos_y1);

	// モデルの位置を設定する(車体)
	MV1SetPosition	 ( Body_m_handle, Position);	  // 位置座標
	MV1SetRotationXYZ( Body_m_handle, Body_Rotation); // 回転座標

	// モデルの位置を設定する(砲塔)
	MV1SetPosition   ( Head_m_handle, Position);	  // 位置座標
	MV1SetRotationXYZ( Head_m_handle, Head_Rotation); // 回転座標
}

Player::~Player()
{
	// モデルの削除
	MV1DeleteModel(Body_m_handle);
	MV1DeleteModel(Head_m_handle);
}

void Player::Update()
{
	// 移動量計算
	move_x = 0.0f, move_z = 0.0f;

	// 砲塔回転
	VECTOR mousePos = GetMouseWorldPos();
	VECTOR dir;
	dir.x = mousePos.x - pos_x;
	dir.z = mousePos.z - pos_z;

	// マウスポインターを追従して回転
	pos_y2 = atan2f(dir.x, dir.z) + DX_PI_F;
	// ↓これがないとマウスポインターと真逆の方を向く
	//pos_y2 += DX_PI_F;

	// 回転操作
	if (CheckHitKey(KEY_INPUT_A)){ // 左回転
		pos_y1 -= Angle;
	}
	if (CheckHitKey(KEY_INPUT_D)){ // 右回転
		pos_y1 += Angle;
	}

	forward.x = sinf(pos_y1);
	forward.z = cosf(pos_y1);

	// 移動操作
	if (CheckHitKey(KEY_INPUT_W)) { // 前進
		move_x -= forward.x * Speed;
		move_z -= forward.z * Speed;
	}
	if (CheckHitKey(KEY_INPUT_S)) { // 後進
		move_x += forward.x * Speed;
		move_z += forward.z * Speed;
	}

	// スライド移動
	float next_x = pos_x + move_x;
	if (!object->CheckHit(next_x, pos_z)) {
		pos_x = next_x;
	}
	float next_z = pos_z + move_z;
	if (!object->CheckHit(pos_x, next_z)) {
		pos_z = next_z;
	}

	// 位置管理
	Position	  = VGet(pos_x, 0.0f, pos_z);
	Body_Rotation = VGet(0.0f, pos_y1, 0.0f); // 車体用回転
	Head_Rotation = VGet(0.0f, pos_y2, 0.0f); // 砲塔用回転

	// モデルの位置を設定する(車体)
	MV1SetPosition(Body_m_handle, Position);		 // 位置座標
	MV1SetRotationXYZ(Body_m_handle, Body_Rotation); // 回転座標

	// モデルの位置を設定する(砲塔)
	MV1SetPosition(Head_m_handle, Position);		 // 位置座標
	MV1SetRotationXYZ(Head_m_handle, Head_Rotation); // 回転座標
}

void Player::Draw()
{
	// モデルの表示
	MV1DrawModel(Body_m_handle);
	MV1DrawModel(Head_m_handle);
}

VECTOR Player::GetMouseWorldPos()
{
	// マウスポインターの座標
	int prevMouseX, prevMouseY;
	GetMousePoint(&prevMouseX, &prevMouseY);

	// 画面上の2点（ニア・ファー）
	VECTOR nearPos = ConvScreenPosToWorldPos(VGet((float)prevMouseX, (float)prevMouseY, 0.0f));
	VECTOR farPos  = ConvScreenPosToWorldPos(VGet((float)prevMouseX, (float)prevMouseY, 1.0f));

	// 地面（Y=0）との交点を求める
	float t = -nearPos.y / (farPos.y - nearPos.y);

	VECTOR hitPos;
	hitPos.x = nearPos.x + (farPos.x - nearPos.x) * t;
	hitPos.z = nearPos.z + (farPos.z - nearPos.z) * t;

	return hitPos;
}
