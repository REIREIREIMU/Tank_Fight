#include "ReadyScene.h"
#include "Player.h"
#include <DxLib.h>

static const int READY_WAIT = 120; // ŽŸ‚ÌƒV[ƒ“‚ÉØ‚è‘Ö‚í‚é‚Ü‚Å‚ÌŽžŠÔ

ReadyScene::ReadyScene() : timer(0)
{
	image_ = LoadGraph("Assets/READY.png");
}

ReadyScene::~ReadyScene()
{
	if (image_ >= 0)
	{
		DeleteGraph(image_);
		image_ = -1;
	}
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


	DrawString(
		360, 340,
		"PLAYER LIFE :  2",
		GetColor(255, 255, 255)
	);
}
