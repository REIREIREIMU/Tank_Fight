#include "ReadyScene.h"
#include "Player.h"
#include <DxLib.h>

static const int READY_WAIT = 120; // ŽŸ‚ÌƒV[ƒ“‚ÉØ‚è‘Ö‚í‚é‚Ü‚Å‚ÌŽžŠÔ

ReadyScene::ReadyScene() : timer(0), lives(2)
{
	image_ = LoadGraph("Assets/READY.png");
}

ReadyScene::ReadyScene(int lives)
	: timer(0),
	lives(lives)
{
}

ReadyScene::~ReadyScene()
{
}

void ReadyScene::SetLives(int l)
{
	lives = l;
}

void ReadyScene::Update()
{
	timer++;
	if (timer >= READY_WAIT)
	{
		SceneManager::ChangeScene("PLAY");
	}
}

void ReadyScene::Draw()
{
	int screenW, screenH;
	GetDrawScreenSize(&screenW, &screenH);

	DrawExtendGraph(0, 0, screenW, screenH, image_, TRUE);

	SetFontSize(40);
	DrawFormatString(
		screenW / 2 - 135,
		screenH / 2 - 120,
		GetColor(255, 255, 255),
		"STAGE : %d",
		Player::GetStage()
	);

	SetFontSize(45);
	DrawFormatString(
		screenW / 2 - 40,
		screenH / 2,
		GetColor(255, 255, 255),
		"~ %d",
		player->Player::GetLives()
	);

}
