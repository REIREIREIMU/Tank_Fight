//
//#include "Enemy.h"
//#include "Player.h"
//#include "Object.h"
//#include <cmath>
//
//
//Enemy::Enemy(const VECTOR& spawnPos, EnemyType type, 
//    Object* object, Player* player) :
//    type(type),
//    Body_m_handle(-1),
//    Head_m_handle(-1),
//    pos_x(spawnPos.x),
//    pos_y1(DX_PI_F),
//    pos_y2(0.0f),
//    pos_z(spawnPos.z)
//{
//    // モデル読み込み（Playerと同じ）
//    Body_m_handle = MV1LoadModel("Assets/Player_Tank_Body.mv1");
//    Head_m_handle = MV1LoadModel("Assets/Player_Tank_Head.mv1");
//
//    // 位置・回転初期化
//    Position = VGet(pos_x, 0.0f, pos_z);
//    Body_Rotation = VGet(0.0f, pos_y1, 0.0f);
//    Head_Rotation = VGet(0.0f, pos_y2, 0.0f);
//
//    MV1SetPosition(Body_m_handle, Position);
//    MV1SetRotationXYZ(Body_m_handle, Body_Rotation);
//
//    MV1SetPosition(Head_m_handle, Position);
//    MV1SetRotationXYZ(Head_m_handle, Head_Rotation);
//}
//
//Enemy::~Enemy()
//{
//}
//
//void Enemy::Update()
//{
//    // 今回は何もしない（表示確認用）
//}
//
//void Enemy::Draw()
//{
//    MV1DrawModel(Body_m_handle);
//    MV1DrawModel(Head_m_handle);
//}

#include "Enemy.h"
#include "Player.h"
#include "Object.h"
#include <cmath>

Enemy::Enemy(const VECTOR& spawnPos, EnemyType type, Object* object, Player* player)
    : type(type),
    player(player),
    object(object),
    position(spawnPos),
    bodyYaw(0.0f),
    headYaw(0.0f),
    moveSpeed(0.015f)
{
    Body_m_handle = MV1LoadModel("Assets/Enemy_Tank_A(Body).mv1");
    Head_m_handle = MV1LoadModel("Assets/Enemy_Tank_A(Head).mv1");

    MV1SetPosition(Body_m_handle, position);
    MV1SetPosition(Head_m_handle, position);
}

Enemy::~Enemy()
{
    // GameObjectなので delete 不要（ObjectManager 管理）
}

void Enemy::Update()
{
    if (!player) return;

    switch (type)
    {
    case EnemyType::Turret:
        UpdateTurret();
        break;
    case EnemyType::Chaser:
        UpdateChaser();
        break;
    }

    MV1SetPosition(Body_m_handle, position);
    MV1SetPosition(Head_m_handle, position);
    MV1SetRotationXYZ(Body_m_handle, VGet(0, bodyYaw, 0));
    MV1SetRotationXYZ(Head_m_handle, VGet(0, headYaw, 0));
}

void Enemy::Draw()
{
    MV1DrawModel(Body_m_handle);
    MV1DrawModel(Head_m_handle);
}

void Enemy::UpdateTurret()
{
    VECTOR p = player->GetPosition();
    VECTOR dir = VSub(p, position);

    headYaw = atan2f(dir.x, dir.z) + DX_PI_F;
    // 車体は回らない
}

void Enemy::UpdateChaser()
{
    VECTOR p = player->GetPosition();
    VECTOR toPlayer = VSub(p, position);
    float dist = VSize(toPlayer);

    constexpr float KeepDistance = 3.0f;

    // プレイヤーを見る
    bodyYaw = atan2f(toPlayer.x, toPlayer.z) + DX_PI_F;
    headYaw = bodyYaw;

    // 距離が離れすぎていたら近づく
    if (dist > KeepDistance)
    {
        VECTOR dir = VNorm(toPlayer);
        position = VAdd(position, VScale(dir, moveSpeed));
    }
}
