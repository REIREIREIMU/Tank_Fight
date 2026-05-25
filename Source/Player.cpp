#include "Player.h"
#include "Enemy.h"
#include "Object.h"
#include "Config.h"
#include <cmath>

// ====== 静的変数 ======
int  Player::TotalEnemyKill = 0;	// 倒した敵数
int  Player::Lives			= 2;	// 残機の個数
int  Player::Stage			= 1;	// 最初のステージ
bool Player::Invincible		= false;	// 無敵の有無
bool Player::ControlEnabled = true;		// 操作停止の有無

Player::Player(Object* obj, std::vector<Enemy*>* enemyList) :
	Speed(0.025f),
	Angle(0.020f),
	move_x(0.0f),
	move_z(0.0f),
	Alive(true),
	Exploding(false),
	Explosion_Handle(-1),
	Explosion_Scale(0.0f),
	Body_handle(-1),
	Head_handle(-1),
	pos_x(0.0f),
	pos_y1(DX_PI_F),
	pos_y2(0.0f),
	pos_z(0.0f),
	object(obj),
	enemies(enemyList)
{
	VECTOR spawnPos;
	if (object->GetPlayerSpawnPos(spawnPos)) {
		pos_x = spawnPos.x;
		pos_z = spawnPos.z;
	}

	// 3Dモデルの読み込み
	Body_handle = MV1LoadModel("Assets/Player_Tank_Body.mv1");
	Head_handle = MV1LoadModel("Assets/Player_Tank_Head.mv1");

	// プレイヤーを設置
	Rotation();
}

Player::~Player()
{
	// モデルの削除
	MV1DeleteModel(Body_handle);
	MV1DeleteModel(Head_handle);
}

void Player::Update()
{
	// 操作停止中は移動・射撃しない
	if (!ControlEnabled) return;

	Bullets(); // 弾更新

	// プレイヤー生存中に更新
	if (Alive) {
		Move();
		Shooting();
		Rotation();
	}
	// プレイヤー死亡で更新
	else {
		Explosion();
	}
}

// 弾更新
void Player::Bullets()
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

// 移動処理
void Player::Move()
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

	// 回転操作
	if (CheckHitKey(KEY_INPUT_A)) pos_y1 -= Angle;	// 左回転
	if (CheckHitKey(KEY_INPUT_D)) pos_y1 += Angle;	// 右回転

	// 前方向
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
	if (!object->CheckHit(next_x, pos_z, Config::Player_Half)) {
		pos_x = next_x;
	}
	float next_z = pos_z + move_z;
	if (!object->CheckHit(pos_x, next_z, Config::Player_Half)) {
		pos_z = next_z;
	}
}

// 回転処理
void Player::Rotation()
{
	// 位置管理
	Position = VGet(pos_x, 0.0f, pos_z);
	Body_Rotation = VGet(0.0f, pos_y1, 0.0f); // 車体用回転
	Head_Rotation = VGet(0.0f, pos_y2, 0.0f); // 砲塔用回転

	// モデルの位置を設定する(車体)
	MV1SetPosition(Body_handle, Position);		   // 位置座標
	MV1SetRotationXYZ(Body_handle, Body_Rotation); // 回転座標

	// モデルの位置を設定する(砲塔)
	MV1SetPosition(Head_handle, Position);		   // 位置座標
	MV1SetRotationXYZ(Head_handle, Head_Rotation); // 回転座標
}

// 射撃処理
void Player::Shooting()
{
	// 弾発射
	static bool prevShot = false;
	bool nowShot = GetMouseInput();

	// マウス左クリックで弾発射
	if ((nowShot & MOUSE_INPUT_LEFT) && !(prevShot & MOUSE_INPUT_LEFT)) {
		Shoot();
	}

	prevShot = nowShot;
}

// 爆発処理
void Player::Explosion()
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

void Player::Draw()
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

void Player::Shoot()
{
	// 死亡したら何もできない
	if (!Alive) return;

	// プレイヤーの弾が 3発ステージ上に存在している限り撃てない
	if (CountAliveBullets() >= Max_Player_Bullets) {
		return;
	}

	VECTOR shotDir;
	shotDir.x = -sinf(pos_y2);
	shotDir.y = 0.0f;
	shotDir.z = -cosf(pos_y2);
	shotDir = VNorm(shotDir);

	const float MuzzleOffset = Config::Player_Half * 2; // 砲身の長さ（弾の発射位置）
	VECTOR muzzlePos = VGet(
		pos_x + shotDir.x * MuzzleOffset,
		Config::Player_Half,
		pos_z + shotDir.z * MuzzleOffset
	);

	bullets.push_back(
		std::make_unique<Bullet>(muzzlePos, shotDir, object, this, enemies)
	);
}

void Player::IsDead()
{
	if (!Alive) return;
	Alive = false;

	// 戦車モデルを消す
	MV1DeleteModel(Body_handle);
	MV1DeleteModel(Head_handle);

	Body_handle = -1;
	Head_handle = -1;

	// 爆発モデル生成
	Explosion_Handle = MV1LoadModel("Assets/Explosion.mv1");
	Explosion_Scale = Config::ExplosionStartScale;
	Exploding = true;

	// 位置は戦車の位置
	MV1SetPosition(Explosion_Handle, Position);
}

void Player::DrawBullets()
{
	for (auto& b : bullets) {
		b->Draw();
	}
}

void Player::ClearBullets()
{
	bullets.clear();
}

int Player::CountAliveBullets() const
{
	int count = 0;
	for (const auto& b : bullets)
	{
		if (b->IsAlive())
			count++;
	}
	return count;
}

VECTOR Player::GetMouseWorldPos()
{
	// マウスポインターの座標
	int prevMouseX, prevMouseY;
	GetMousePoint(&prevMouseX, &prevMouseY);

	// 画面上の2点（ニア・ファー）
	VECTOR nearPos = ConvScreenPosToWorldPos(VGet((float)prevMouseX, (float)prevMouseY, 0.0f));
	VECTOR farPos = ConvScreenPosToWorldPos(VGet((float)prevMouseX, (float)prevMouseY, 1.0f));

	// 地面（Y=0）との交点を求める
	float t = -nearPos.y / (farPos.y - nearPos.y);

	VECTOR hitPos;
	hitPos.x = nearPos.x + (farPos.x - nearPos.x) * t;
	hitPos.z = nearPos.z + (farPos.z - nearPos.z) * t;

	return hitPos;
}

// ======= 敵撃破関連 =======
int Player::GetTotalEnemyKill() { return TotalEnemyKill; }
void Player::AddEnemyKill(int count) { TotalEnemyKill += count; }
void Player::ResetEnemyKill() { TotalEnemyKill = 0; }
// =========================

// ======= 残機関連 =======
int  Player::GetLives() { return Lives; }
void Player::DecreaseLives() { if (Lives > 0) Lives--; }
void Player::ResetLives(int v) { Lives = v; }
// ========================

// ====== ステージ関連 ======
int  Player::GetStage() { return Stage; }
void Player::NextStage() { Stage++; }
void Player::ResetStage() { Stage = 1; }
// =========================

// ======== 無敵制御 ========
void Player::SetInvincible(bool v) { Invincible = v; }
bool Player::IsInvincible() { return Invincible; }
// =========================

// ====== 操作停止制御 ======
void Player::SetControlEnabled(bool v) { ControlEnabled = v; }
bool Player::IsControlEnabled() { return ControlEnabled; }
// =========================