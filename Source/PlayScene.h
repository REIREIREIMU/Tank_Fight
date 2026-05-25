#pragma once
#include <vector>
#include "../Library/SceneBase.h"

class Camera;
class Player;
class Enemy;
class Object;

class PlayScene : public SceneBase
{
public:
	PlayScene();
	~PlayScene();

	void Update() override;
	void Draw() override;

private:
	// ========= 更新処理 =========
	void PlayerEnemyCollision();   // プレイヤーと敵の当たり判定処理
	void EnemyCollision();         // 敵同士の当たり判定処理
	void PlayerDeath();            // プレイヤー死亡処理
	void StageClear();             // ステージクリア処理

	// =========== 描画 ===========
	void Draw3D(); // 3D描画
	void DrawUI(); // UI描画

	// ======= ポインター群 =======
	Camera* camera = nullptr;		// Cameraのポインタを作成
	Player* player = nullptr;		// Playerのポインタを作成
	Object* object = nullptr;		// Objectのポインタを作成
	std::vector<Enemy*> enemies;	// Enemyのポインタを作成

	// ========= 状態管理 =========
	int timer = 0;				// プレイヤー死亡後に使うタイマー
	bool stageClear = false;	// クリア状態フラグ
	int stageClearTimer = 0;	// クリア後ステージの待機時間
	const int MAX_STAGE = 3;	// 最大ステージ数

	const int DEATH_WAIT = 120;	// プレイヤーが死んでから次の処理に行くまでの時間(120 = 2秒)
	const int STAGE_WAIT = 120;	// 敵全滅後、次の処理に行くまでの時間(120 = 2秒)

	// ==== デバック用グリット線 ====
	/*
	const int   Grid_Half = 50;	// グリット線を表示する範囲
	const float Grid_Size = 1.0f;	// グリット線のサイズ
	// 通常グリッド線の色
	int GridColor = GetColor(80, 80, 80);
	// 軸の色
	int xAxisColor = GetColor(200, 80, 80);  // X軸：赤
	int zAxisColor = GetColor(80, 80, 200);  // Z軸：青
	*/
};
