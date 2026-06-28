#include "Enemy.h"
#include "Player.h"
#include "Object.h"
#include "Config.h"
#include <cmath>

// ====== 静的変数 ======
bool Enemy::Invincible = false; // 無敵の有無

Enemy::Enemy(const VECTOR& spawnPos, EnemyType type, Object* object, Player* player) :
    Speed(0.010f),
    Angle(0.020f),
    type(type),
    player(player),
    Alive(true),
    Position(spawnPos),
    Exploding(false),
    Explosion_Handle(-1),
    Explosion_Scale(0.0f),
    Body_handle(-1),
    Head_handle(-1),
    pos_x(0.0f),
    pos_y1(DX_PI_F),
    pos_y2(0.0f),
    pos_z(0.0f),
    ShootTimer(0),
    object(object)
{
    // 3Dモデルの読み込み
    Body_handle = MV1LoadModel("Assets/Enemy_Tank_A(Body).mv1");
    Head_handle = MV1LoadModel("Assets/Enemy_Tank_A(Head).mv1");

    // サウンドの読み込み
    Explosion_sound = LoadSoundMem("sound/Explosion.mp3");
    Shoot_sound     = LoadSoundMem("sound/Shooting.mp3");
    
    // 音量
    Explosion_volume = 128;
    Shoot_volume     = 128;

    // 敵を設置
    Rotation();
}

Enemy::~Enemy()
{
    //// モデルの削除
    //MV1DeleteModel(Body_m_handle);
    //MV1DeleteModel(Head_m_handle);
}

void Enemy::Update()
{
    Bullets(); // 弾更新

    // 生存中に更新
    if (Alive) {
        EnemyAI();
        Shooting();
        Rotation();
    }
    // 死亡で更新
    else {
        Explosion();
    }
}

// 弾更新
void Enemy::Bullets()
{
    for (auto& b : bullets) b->Update();

    bullets.erase(
        std::remove_if(
            bullets.begin(),
            bullets.end(),
            [](const std::unique_ptr<Bullet>& b)
            {
                return !b->IsAlive();
            }),
        bullets.end()
    );
}

// 各種の敵のAI判別処理
void Enemy::EnemyAI()
{
    switch (type)
    {
    case EnemyType::Turret:
        UpdateTurret();
        break;
    case EnemyType::Chaser:
        UpdateChaser();
        break;
    }
}

// 固定砲台型
void Enemy::UpdateTurret()
{
    VECTOR p = player->GetPosition();
    VECTOR dir = VSub(p, Position);

    // 砲塔だけを回す
    pos_y2 = atan2f(dir.x, dir.z) + DX_PI_F;

    // 車体は動かない
    Body_Rotation = VGet(0.0f, pos_y1, 0.0f); // 車体用回転
    Head_Rotation = VGet(0.0f, pos_y2, 0.0f); // 砲塔用回転
}

// 尾行型
void Enemy::UpdateChaser()
{
    VECTOR p = player->GetPosition();
    VECTOR toPlayer = VSub(p, Position);

    float dist = VSize(toPlayer);
    VECTOR dir = VNorm(toPlayer);

    // プレイヤーを見る
    pos_y2 = atan2f(toPlayer.x, toPlayer.z) + DX_PI_F;

    VECTOR moveDir = VGet(0, 0, 0);

    // 距離が離れすぎていたら近づく
    if (dist > ApproachDistance){
        moveDir = dir;
    }
    else if (dist > OrbitDistance){
        VECTOR left = VGet(-dir.z, 0.0f, dir.x);
        moveDir = VAdd(dir, left);
    }
    else{
        VECTOR left = VGet(-dir.z, 0.0f, dir.x);
        moveDir = VAdd(dir, left);
    }

    // 壁判定つき移動
    float nextX = Position.x + moveDir.x * Speed;
    float nextZ = Position.z + moveDir.z * Speed;

    // X方向の壁判定
    if (!object->CheckHit(nextX, Position.z, Config::Enemy_Half)){
        Position.x = nextX;
    }
    // Z方向の壁判定
    if (!object->CheckHit(Position.x, nextZ, Config::Enemy_Half)){
        Position.z = nextZ;
    }

    // 車体の向きを移動方向に合わせる
    if (VSize(moveDir) > 0.001f)
    {
        float target = atan2f(moveDir.x, moveDir.z) + DX_PI_F;
        pos_y1 += (target - pos_y1) * 0.01f;
    }

    Body_Rotation = VGet(pos_x, pos_y1, pos_z); // 車体用回転
    Head_Rotation = VGet(pos_x, pos_y2, pos_z); // 砲塔用回転
}

// 回転処理
void Enemy::Rotation()
{
    // モデルの位置を設定する(車体)
    MV1SetPosition(Body_handle, Position);			// 位置座標
    MV1SetRotationXYZ(Body_handle, Body_Rotation);	// 回転座標

    // モデルの位置を設定する(砲塔)
    MV1SetPosition(Head_handle, Position);			// 位置座標
    MV1SetRotationXYZ(Head_handle, Head_Rotation);	// 回転座標
}

// 射撃処理
void Enemy::Shooting()
{
    ShootTimer++;	// タイマー更新

    // 弾発射
    if (ShootTimer >= ShootInterval){
        // 固定砲台型の発射条件
        if (type == EnemyType::Turret) {
            Shoot();
            ShootTimer = 0;	// 発射後タイマーリセット
        }
        // 尾行型の発射条件
        if (type == EnemyType::Chaser) {
            VECTOR enemyPos = Position;
            VECTOR playerPos = player->GetPosition();

            // プレイヤーと敵の間に壁があるかどうかの判定
            bool hasWall = object->HasWallBetween(
                enemyPos,
                playerPos,
                Config::Bullet_Half
            );

            // 壁がない時撃つ
            if (!hasWall){
                Shoot();
                ShootTimer = 0;	// 発射後タイマーリセット
            }
        }
    }
}

// 爆発処理
void Enemy::Explosion()
{
    if (!Exploding) return;

    Explosion_Scale += Config::ExplosionGrowSpeed;

    // モデルを拡大
    MV1SetScale(Explosion_Handle,
        VGet(Explosion_Scale, Explosion_Scale, Explosion_Scale));

    // 最大サイズに達したら消す
    if (Explosion_Scale >= Config::ExplosionEndScale) {
        MV1DeleteModel(Explosion_Handle);
        Explosion_Handle = -1;
        Exploding = false;
    }
}

void Enemy::Draw()
{
    // 生存時
    if (Alive) {
        MV1DrawModel(Body_handle);
        MV1DrawModel(Head_handle);
        return;
    }

    // 死亡時
    if (Exploding && Explosion_Handle != -1) {
        MV1DrawModel(Explosion_Handle);
    }

    // 弾描画
    for (auto& b : bullets)
        b->Draw();

    // 当たり判定の可視化（デバッグ用）
    {
        //VECTOR minPos = VGet(
        //	pos_x - Player_Half,
        //	0.0f,
        //	pos_z - Player_Half
        //);
        //VECTOR maxPos = VGet(
        //	pos_x + Player_Half,
        //	1.0f,
        //	pos_z + Player_Half
        //);
        //// 赤いワイヤーフレームの箱
        //DrawCube3D(
        //	minPos,
        //	maxPos,
        //	GetColor(255, 0, 0),
        //	GetColor(255, 0, 0),
        //	FALSE
        //);
    }
}

void Enemy::Shoot()
{
    // 死亡したら何もできない
    if (!Alive) return;

    // 敵（個人）の弾が 3発ステージ上に存在している限り撃てない
    if (CountAliveBullets() >= Max_Enemy_Bullets) return;

    // 銃声音
    ChangeVolumeSoundMem(Shoot_volume, Shoot_sound);
    PlaySoundMem(Shoot_sound, DX_PLAYTYPE_BACK);

    VECTOR shotDir;
    shotDir.x = -sinf(pos_y2);
    shotDir.y = 0.0f;
    shotDir.z = -cosf(pos_y2);
    shotDir = VNorm(shotDir);

    const float MuzzleOffset = Config::Enemy_Half * 2; // 砲身の長さ（弾の発射位置）
    VECTOR muzzlePos = VGet(
        Position.x + shotDir.x * MuzzleOffset,
        Config::Enemy_Half,
        Position.z + shotDir.z * MuzzleOffset
    );

    bullets.push_back(
        std::make_unique<Bullet>(muzzlePos, shotDir, object, player, nullptr)
    );

}

void Enemy::IsDead()
{
    // プレイヤーが死亡したら無敵になる
    if (Invincible) return;

    if (!Alive) return;
    Alive = false;

    // 戦車モデルを消す
    MV1DeleteModel(Body_handle);
    MV1DeleteModel(Head_handle);

    // 爆発音
    ChangeVolumeSoundMem(Explosion_volume, Explosion_sound);
    PlaySoundMem(Explosion_sound, DX_PLAYTYPE_BACK);

    Body_handle = -1;
    Head_handle = -1;

    // 爆発モデル生成
    Explosion_Handle = MV1LoadModel("Assets/Explosion.mv1");
    Explosion_Scale = Config::ExplosionStartScale;
    Exploding = true;

    // 位置は戦車の位置
    MV1SetPosition(Explosion_Handle, Position);
}

void Enemy::DrawBullets()
{
    for (auto& b : bullets) {
        b->Draw();
    }
}

void Enemy::ClearBullets()
{
    bullets.clear();
}

int Enemy::CountAliveBullets() const
{
    int count = 0;
    for (const auto& b : bullets)
    {
        if (b->IsAlive())
            count++;
    }
    return count;
}

// ======== 無敵制御 ========
void Enemy::SetInvincible(bool v) { Invincible = v; }
bool Enemy::IsInvincible() { return Invincible; }
// =========================