#include "Enemy.h"
#include "Player.h"
#include "Object.h"
#include "Config.h"
#include <cmath>

static const float Speed = 0.015f;		// 移動速度
static const float Angle = 0.020f;		// 車体の移転速度

Enemy::Enemy(const VECTOR& spawnPos, EnemyType type, Object* object, Player* player) :
	type(type),
	player(player),
	m_alive(true),
	Position(spawnPos),
	m_Exploding(false),
	m_Explosion_Handle(-1),
	m_Explosion_Scale(0.0f),
	Body_m_handle(-1),
	Head_m_handle(-1),
	pos_x(0.0f),
	pos_y1(DX_PI_F),
	pos_y2(0.0f),
	pos_z(0.0f),
	shootTimer(0),
	object(object)
{
	// 3Dモデルの読み込み
	Body_m_handle = MV1LoadModel("Assets/Enemy_Tank_A(Body).mv1");
	Head_m_handle = MV1LoadModel("Assets/Enemy_Tank_A(Head).mv1");

	// 位置管理
	Position = VGet(pos_x, 0.0f, pos_z);
	Body_Rotation = VGet(0.0f, pos_y1, 0.0f); // 車体用回転
	Head_Rotation = VGet(0.0f, pos_y2, 0.0f); // 砲塔用回転

	forward.x = sinf(pos_y1);
	forward.z = cosf(pos_y1);

	// モデルの位置を設定する(車体)
	MV1SetPosition(Body_m_handle, Position);	  // 位置座標
	MV1SetRotationXYZ(Body_m_handle, Body_Rotation); // 回転座標

	// モデルの位置を設定する(砲塔)
	MV1SetPosition(Head_m_handle, Position);	  // 位置座標
	MV1SetRotationXYZ(Head_m_handle, Head_Rotation); // 回転座標
}

Enemy::~Enemy()
{
	//// モデルの削除
	//MV1DeleteModel(Body_m_handle);
	//MV1DeleteModel(Head_m_handle);
}

void Enemy::Update()
{
	// 生存中
	if (m_alive) {
		switch (type)
		{
		case EnemyType::Turret:
			UpdateTurret();
			break;
		case EnemyType::Chaser:
			UpdateChaser();
			break;
		}

		shootTimer++;

		// 弾発射
		if (shootTimer >= ShootInterval)
		{
			if (type == EnemyType::Turret){
				Shoot();
				shootTimer = 0;
			}
			if (type == EnemyType::Chaser) {

				VECTOR enemyPos = Position;
				VECTOR playerPos = player->GetPosition();

				bool hasWall = object->HasWallBetween(
					enemyPos,
					playerPos,
					Config::Bullet_Half
				);

				if (!hasWall)
				{
					Shoot();
					shootTimer = 0;
				}
			}
		}

		// 弾更新
		for (auto& b : bullets)
			b->Update();

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

		// モデルの位置を設定する(車体)
		MV1SetPosition(Body_m_handle, Position);		 // 位置座標
		MV1SetRotationXYZ(Body_m_handle, Body_Rotation); // 回転座標

		// モデルの位置を設定する(砲塔)
		MV1SetPosition(Head_m_handle, Position);		 // 位置座標
		MV1SetRotationXYZ(Head_m_handle, Head_Rotation); // 回転座標
	}

	// 爆発中
	if (m_Exploding)
	{
		m_Explosion_Scale += Config::ExplosionGrowSpeed;

		// 拡大
		MV1SetScale(
			m_Explosion_Handle,
			VGet(
				m_Explosion_Scale,
				m_Explosion_Scale,
				m_Explosion_Scale
			)
		);

		// 最大サイズに達したら消す
		if (m_Explosion_Scale >= Config::ExplosionEndScale)
		{
			MV1DeleteModel(m_Explosion_Handle);
			m_Explosion_Handle = -1;
			m_Exploding = false;
		}
	}
}

void Enemy::UpdateTurret()
{
	VECTOR p = player->GetPosition();
	VECTOR dir = VSub(p, Position);

	// 砲塔だけを回す
	pos_y2 = atan2f(dir.x, dir.z) + DX_PI_F;
	
	// 車体は動かない
	Position = VGet(pos_x, 0.0f, pos_z);

	Body_Rotation = VGet(0.0f, pos_y1, 0.0f); // 車体用回転
	Head_Rotation = VGet(0.0f, pos_y2, 0.0f); // 砲塔用回転
}

void Enemy::UpdateChaser()
{
	VECTOR p = player->GetPosition();
	VECTOR toPlayer = VSub(p, Position);
	float dist = VSize(toPlayer);

	// プレイヤーを見る
	pos_y2 = atan2f(toPlayer.x, toPlayer.z) + DX_PI_F;
	//pos_y1 = atan2f(toPlayer.x, toPlayer.z) + DX_PI_F;

	VECTOR dir = VNorm(toPlayer);

	const float ApproachDistance = 8.0f; // 接近距離
	const float OrbitDistance	 = 5.0f; // 回り込み距離

	VECTOR moveDir = VGet(0, 0, 0);

	// 距離が離れすぎていたら近づく
	if (dist > ApproachDistance)
	{
		isOrbiting = false;
		moveDir = dir;
	}
	else if (dist > OrbitDistance)
	{
		isOrbiting = false;
		VECTOR left = VGet(-dir.z, 0.0f, dir.x);
		moveDir = VAdd(dir, left);
		//moveDir = VNorm(moveDir);
	}
	else
	{
		VECTOR left = VGet(-dir.z, 0.0f, dir.x);
		moveDir = left;
	}

	// 壁判定つき移動
	float nextX = Position.x + moveDir.x * Speed;
	float nextZ = Position.z + moveDir.z * Speed;

	// X方向の壁判定
	if (!object->CheckHit(nextX, Position.z, Config::Enemy_Half))
	{
		Position.x = nextX;
	}
	// Z方向の壁判定
	if (!object->CheckHit(Position.x, nextZ, Config::Enemy_Half))
	{
		Position.z = nextZ;
	}

	Body_Rotation = VGet(pos_x, pos_y1, pos_z); // 車体用回転
	Head_Rotation = VGet(pos_x, pos_y2, pos_z); // 砲塔用回転
}

void Enemy::Draw()
{
	// 生存時
	if (m_alive)
	{
		MV1DrawModel(Body_m_handle);
		MV1DrawModel(Head_m_handle);
		return;
	}

	// 死亡時
	if (m_Exploding && m_Explosion_Handle != -1)
	{
		MV1DrawModel(m_Explosion_Handle);
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
	if (!m_alive) return;

	VECTOR shotDir;
	shotDir.x = -sinf(pos_y2);
	shotDir.y = 0.0f;
	shotDir.z = -cosf(pos_y2);
	shotDir = VNorm(shotDir);

	const float MuzzleOffset = 0.8f; // 砲身の長さ（弾の発射位置）
	VECTOR muzzlePos = VGet(
		Position.x + shotDir.x * MuzzleOffset,
		0.4f,
		Position.z + shotDir.z * MuzzleOffset
	);

	bullets.push_back(
		std::make_unique<Bullet>(muzzlePos, shotDir, object, player)
	);
}

void Enemy::IsDead()
{
	if (!m_alive) return;
	m_alive = false;

	// 戦車モデルを消す
	MV1DeleteModel(Body_m_handle);
	MV1DeleteModel(Head_m_handle);
	Body_m_handle = -1;
	Head_m_handle = -1;

	// 爆発モデル生成
	m_Explosion_Handle = MV1LoadModel("Assets/Explosion.mv1");
	m_Explosion_Scale = Config::ExplosionStartScale;
	m_Exploding = true;

	// 位置は戦車の位置
	MV1SetPosition(m_Explosion_Handle, Position);
	MV1SetScale(m_Explosion_Handle,
		VGet(m_Explosion_Scale, m_Explosion_Scale, m_Explosion_Scale)
	);
}
