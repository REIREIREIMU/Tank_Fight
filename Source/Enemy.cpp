// Enemy.cpp
#include "Enemy.h"
#include "Object.h"
#include "Player.h"
#include "Config.h"

Enemy::Enemy(const VECTOR& spawnPos, Object* obj, Player* pl)
    : object(obj), player(pl), m_alive(true)
{
    pos_x = spawnPos.x;
    pos_z = spawnPos.z;
    body_y = 0.0f;
    head_y = 0.0f;

    Body_m_handle = MV1LoadModel("Assets/Player_Tank_Body.mv1");
    Head_m_handle = MV1LoadModel("Assets/Player_Tank_Head.mv1");

    Position = VGet(pos_x, 0.0f, pos_z);

    MV1SetPosition(Body_m_handle, Position);
    MV1SetPosition(Head_m_handle, Position);
}

Enemy::~Enemy()
{
    MV1DeleteModel(Body_m_handle);
    MV1DeleteModel(Head_m_handle);
}

void Enemy::Update()
{
    if (!m_alive) return;
    if (!player || !player->IsAlive()) return;

    // ===== プレイヤー方向 =====
    VECTOR p = player->GetPosition();
    VECTOR dir;
    dir.x = p.x - pos_x;
    dir.z = p.z - pos_z;

    float len = sqrtf(dir.x * dir.x + dir.z * dir.z);
    if (len > 0.001f)
    {
        dir.x /= len;
        dir.z /= len;

        // 移動
        float next_x = pos_x + dir.x * Speed;
        float next_z = pos_z + dir.z * Speed;

        if (!object->CheckHit(next_x, pos_z, Enemy_Half))
            pos_x = next_x;

        if (!object->CheckHit(pos_x, next_z, Enemy_Half))
            pos_z = next_z;

        // 車体回転
        body_y = atan2f(dir.x, dir.z) + DX_PI_F;

        // 砲塔は常にプレイヤーを向く
        head_y = body_y;
    }

    // 更新反映
    Position = VGet(pos_x, 0.0f, pos_z);
    Body_Rotation = VGet(0.0f, body_y, 0.0f);
    Head_Rotation = VGet(0.0f, head_y, 0.0f);

    MV1SetPosition(Body_m_handle, Position);
    MV1SetRotationXYZ(Body_m_handle, Body_Rotation);

    MV1SetPosition(Head_m_handle, Position);
    MV1SetRotationXYZ(Head_m_handle, Head_Rotation);
}

void Enemy::Draw()
{
    if (!m_alive) return;
    MV1DrawModel(Body_m_handle);
    MV1DrawModel(Head_m_handle);
}

void Enemy::Kill()
{
    m_alive = false;
}