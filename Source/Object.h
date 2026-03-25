#pragma once
#include "../Library/GameObject.h"
#include <fstream>
#include <sstream>
#include <vector>

class Object : public GameObject {
public:
	Object();
	~Object();

	void Update() override;
	void Draw() override;

	bool CheckHit(float px, float pz);

private:
	int	Ground_m_handle; // 地面のハンドル
	int	Block_m_handle;	 // ブロックのハンドル

	std::vector<std::vector<int>> mapData;

	int   Half_Size;     // 地面の表示する範囲
	float Ground_Size;   // 地面の１マスのサイズ 

	// オブジェクトの「Position」関連
	VECTOR Map_pos;
	float pos_x, pos_y,	pos_z;	

	// CSV対応可
	void LoadCSV(const char* filename);
};