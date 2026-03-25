#include "Object.h"
#include <cmath>

// 当たり判定
constexpr float Player_Half = 0.4f; // Player用（箱型）
constexpr float Block_Half  = 0.5f; // Block用

// AABB
bool CheckAABB(
	float ax, float az, float aHalf,
	float bx, float bz, float bHalf)
{
	return
		fabs(ax - bx) < (aHalf + bHalf) &&
		fabs(az - bz) < (aHalf + bHalf);
}

Object::Object() :
	Block_m_handle(-1), pos_x(0.0f), pos_y(0.0f), pos_z(0.0f),
	Ground_m_handle(-1), Half_Size(50), Ground_Size(1.0f)
{
	// 3Dモデルの読み込み
	Block_m_handle  = MV1LoadModel("Assets/Block.mv1");
	Ground_m_handle = MV1LoadModel("Assets/Ground.mv1");

	// 「CSV」読み込み
	LoadCSV("Assets/Stage/Stage_00.csv");
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

				pos_x = (x - Map_pos.x / 2) * Ground_Size;
				pos_z = -(z - Map_pos.z / 2.5) * Ground_Size;

				// ブロックモデルの表示
				MV1SetPosition(Block_m_handle, VGet(pos_x, pos_y, pos_z));
				MV1DrawModel(Block_m_handle);
			}
		}
	}
}

bool Object::CheckHit(float px, float pz)
{
	for (int x = 0; x < Map_pos.x; x++) {
		for (int z = 0; z < Map_pos.z; z++) {

			if (mapData[z][x] != 1) continue;

			float bx = (x - Map_pos.x / 2) * Ground_Size;
			float bz = -(z - Map_pos.z / 2.5f) * Ground_Size;

			if (CheckAABB(
				px, pz, Player_Half,
				bx, bz, Block_Half))
			{
				return true;
			}
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
