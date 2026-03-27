#pragma once
#include "../Library/SceneBase.h"

class Camera;
class Player;
class Object;

class PlayScene : public SceneBase
{
public:
	PlayScene();
	~PlayScene();

	void Update() override;
	void Draw() override;
private:
	Camera* camera = nullptr;		// Cameraのポインタを作成
	Player* player = nullptr;		// Playerのポインタを作成
	Object* object = nullptr;		// Objectのポインタを作成

	// ディバック用グリット線
	const int   Grid_Half =   50;	// グリット線を表示する範囲
	const float Grid_Size = 1.0f;	// グリット線のサイズ
	// 通常グリッド線の色
	int GridColor  = GetColor(80, 80, 80);
	// 軸の色
	int xAxisColor = GetColor(200, 80, 80);  // X軸：赤
	int zAxisColor = GetColor( 80, 80, 200); // Z軸：青
};
