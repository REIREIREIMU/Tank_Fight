#include "Enemy.h"

Enemy::Enemy(const VECTOR& spawnPos)
    : Body_m_handle(-1),
    Head_m_handle(-1),
    pos_x(spawnPos.x),
    pos_y1(DX_PI_F),
    pos_y2(0.0f),
    pos_z(spawnPos.z)
{
    // モデル読み込み（Playerと同じ）
    Body_m_handle = MV1LoadModel("Assets/Player_Tank_Body.mv1");
    Head_m_handle = MV1LoadModel("Assets/Player_Tank_Head.mv1");

    // 位置・回転初期化
    Position = VGet(pos_x, 0.0f, pos_z);
    Body_Rotation = VGet(0.0f, pos_y1, 0.0f);
    Head_Rotation = VGet(0.0f, pos_y2, 0.0f);

    MV1SetPosition(Body_m_handle, Position);
    MV1SetRotationXYZ(Body_m_handle, Body_Rotation);

    MV1SetPosition(Head_m_handle, Position);
    MV1SetRotationXYZ(Head_m_handle, Head_Rotation);
}

Enemy::~Enemy()
{
    MV1DeleteModel(Body_m_handle);
    MV1DeleteModel(Head_m_handle);
}

void Enemy::Update()
{
    // 今回は何もしない（表示確認用）
}

void Enemy::Draw()
{
    MV1DrawModel(Body_m_handle);
    MV1DrawModel(Head_m_handle);
}