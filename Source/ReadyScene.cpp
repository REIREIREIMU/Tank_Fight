#include "ReadyScene.h"
#include "Player.h"
#include <DxLib.h>
#include "Config.h"

ReadyScene::ReadyScene() : timer(0), lives(2)
{
	image_ = LoadGraph("Assets/READY.png");
}

ReadyScene::~ReadyScene() {}

void ReadyScene::SetLives(int l)
{
	lives = l;
}

// ここは時間経過で勝手にプレイに飛ぶ
void ReadyScene::Update()
{
	timer++;
	if (timer >= Config::READY_WAIT)
		SceneManager::ChangeScene("PLAY");
}

void ReadyScene::Draw()
{
	int screenW, screenH;
	GetDrawScreenSize(&screenW, &screenH);
	DrawExtendGraph(0, 0, screenW, screenH, image_, TRUE);

	// ==== ステージの表示 ====
	SetFontSize(40);
	DrawFormatString(
		screenW / 2 - 135,
		screenH / 2 - 120,
		GetColor(255, 255, 255),
		"STAGE : %d",
		Player::GetStage()
	);

	// ====== 残機の表示 ======
	SetFontSize(45);
	DrawFormatString(
		screenW / 2 - 40,
		screenH / 2,
		GetColor(255, 255, 255),
		"× %d",
		player->Player::GetLives()
	);
}
