#include "Object.h"
#include "Enemy.h"
#include "Config.h"
#include <cmath>

// AABB
bool CheckAABB(
	float ax, float az, float aHalf,
	float bx, float bz, float bHalf)
{
	return
		fabs(ax - bx) < (aHalf + bHalf) &&
		fabs(az - bz) < (aHalf + bHalf);
}

Object::Object(int stage) :
	Block_m_handle(-1), pos_x(0.0f), pos_y(0.0f), pos_z(0.0f),
	Ground_m_handle(-1), Half_Size(50), Ground_Size(1.0f)
{
	// 3Dモデルの読み込み
	Block_m_handle  = MV1LoadModel("Assets/Block.mv1");
	Ground_m_handle = MV1LoadModel("Assets/Ground.mv1");

	// 「CSV」読み込み（ステージごとで切り替える）
	char filename[64];
	sprintf_s(filename, "Assets/Stage/Stage_%02d.csv", stage);

	LoadCSV(filename);
}

Object::~Object()
{
	// モデルの削除
	MV1DeleteModel(Block_m_handle);
	MV1DeleteModel(Ground_m_handle);
}

void Object::Update()
{
}

void Object::Draw()
{
	// 地面（ここは「CSV」対応してない）
	{
		// 地面を指定範囲内一面に埋めて表示
		for (int x = -Half_Size; x <= Half_Size; x++){
			for (int z = -Half_Size; z <= Half_Size; z++) {
				pos_x = x * Ground_Size;
				pos_z = z * Ground_Size;

				MV1SetPosition(Ground_m_handle, VGet(pos_x, pos_y, pos_z));
				MV1DrawModel(Ground_m_handle);
			}
		}
	}

	// 壁（CSV配置）
	{
		for (int x = 0; x < Map_pos.x; x++){
			for (int z = 0; z < Map_pos.z; z++) {
				// mapData[1]は「壁」
				if (mapData[z][x] != 1) continue;

				pos_x = (x + 0.5f - Map_pos.x * 0.5f) * Ground_Size;
				pos_z = -(z + 0.5f - Map_pos.z * 0.5f) * Ground_Size;

				// ブロックモデルの表示
				MV1SetPosition(Block_m_handle, VGet(pos_x, pos_y, pos_z));
				MV1DrawModel(Block_m_handle);

				// デバッグ用 当たり判定
				{
					//VECTOR minPos = VGet(
					//	pos_x - Block_Half,
					//	0.0f,
					//	pos_z - Block_Half
					//);
					//VECTOR maxPos = VGet(
					//	pos_x + Block_Half,
					//	1.0f,
					//	pos_z + Block_Half
					//);
					//DrawCube3D(
					//	minPos,
					//	maxPos,
					//	GetColor(0, 255, 0),  // 緑
					//	GetColor(0, 255, 0),
					//	FALSE
					//);
				}
			}
		}
	}
}

bool Object::CheckHit(float px, float pz, float halfSize)
{
	for (int x = 0; x < Map_pos.x; x++) {
		for (int z = 0; z < Map_pos.z; z++) {

			if (mapData[z][x] != 1) continue;

			float bx =  (x + 0.5f - Map_pos.x * 0.5f) * Ground_Size;
			float bz = -(z + 0.5f - Map_pos.z * 0.5f) * Ground_Size;

			if (CheckAABB(
				px, pz, halfSize,
				bx, bz, Config::Block_Half))
			{
				return true;
			}
		}
	}
	return false;
}

bool Object::GetPlayerSpawnPos(VECTOR& outPos)
{
	for (int z = 0; z < Map_pos.z; z++){
		for (int x = 0; x < Map_pos.x; x++)
		{
			if (mapData[z][x] == 9)
			{
				float px =  (x + 0.5f - Map_pos.x * 0.5f) * Ground_Size;
				float pz = -(z + 0.5f - Map_pos.z * 0.5f) * Ground_Size;

				outPos = VGet(px, 0.0f, pz);
				return true;
			}
		}
	}
	return false;
}

void Object::GetEnemySpawnPos(std::vector<Enemy*>& outEnemies, Player* player)
{
	for (int z = 0; z < Map_pos.z; z++)
	{
		for (int x = 0; x < Map_pos.x; x++)
		{
			int v = mapData[z][x];

			VECTOR pos = VGet(
				(x + 0.5f - Map_pos.x * 0.5f) * Ground_Size,
				0.0f,
				-(z + 0.5f - Map_pos.z * 0.5f) * Ground_Size
			);

			if (v == 2)
			{
				outEnemies.push_back(
					new Enemy(pos, EnemyType::Turret, this, player)
				);
			}
			else if (v == 3)
			{
				outEnemies.push_back(
					new Enemy(pos, EnemyType::Chaser, this, player)
				);
			}
		}
	}
}

bool Object::HasWallBetween(const VECTOR& from, const VECTOR& to, float radius)
{
	VECTOR dir = VSub(to, from);
	float length = VSize(dir);
	dir = VNorm(dir);

	const float step = Ground_Size * 0.25f; // 判定間隔
	int steps = static_cast<int>(length / step);

	VECTOR pos = from;

	for (int i = 0; i < steps; i++)
	{
		pos = VAdd(pos, VScale(dir, step));

		if (CheckHit(pos.x, pos.z, radius))
		{
			return true; // 壁がある
		}
	}
	return false;
}

void Object::LoadCSV(const char* filename)
{
	std::ifstream file(filename);
	std::string line;
	mapData.clear();

	while (std::getline(file, line))
	{
		std::stringstream ss(line);
		std::string value;
		std::vector<int> row;

		while (std::getline(ss, value, ',')){
			row.push_back(std::stoi(value));
		}
		mapData.push_back(row);
	}

	Map_pos.x = (int)mapData[0].size();
	Map_pos.z = (int)mapData.size();
}
