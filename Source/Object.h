#pragma once
#include "../Library/GameObject.h"
#include <fstream>
#include <sstream>
#include <vector>

class Enemy;
class Player;

class Object : public GameObject {
public:
	Object(int stage);
	~Object();

	void Update() override;
	void Draw() override;

	// 衝突判定
	bool CheckHit(float px, float pz, float halfSize);

	// プレイヤーと敵の間に壁があるかどうかの関数
	bool HasWallBetween(const VECTOR& from, const VECTOR& to, float radius);

	// ========== スポーン  ==========
	bool GetPlayerSpawnPos(VECTOR& outPos);	// プレイヤー初期位置取得
	void GetEnemySpawnPos(					// 敵の初期位置取得（複数）
		std::vector<Enemy*>& outEnemies, Player* player);

private:
	// =========== 描画 ===========
	void DrawGround();	// 地面描画
	void DrawBlocks();	// 壁描画

	// ========= 読み込み =========
	void LoadCSV(const char* filename); // CSV対応可

	// ========== モデル ==========
	int	Ground_handle = -1;	// 地面のハンドル
	int	Block_handle = -1;	// ブロックのハンドル

	// ========== マップ ==========
	std::vector<std::vector<int>> mapData;
	VECTOR Map_pos;

	// ========== サイズ ==========
	int   Half_Size;     // 地面の表示する範囲
	float Ground_Size;   // 地面の１マスのサイズ 

	// ======== 描画用座標 ========
	float pos_x, pos_y, pos_z;
};