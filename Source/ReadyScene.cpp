#include "ReadyScene.h"
#include <DxLib.h>

ReadyScene::ReadyScene()
{
}

ReadyScene::~ReadyScene()
{
}

void ReadyScene::Update()
{
	if (CheckHitKey(KEY_INPUT_P)) {
		SceneManager::ChangeScene("PLAY");
	}
}

void ReadyScene::Draw()
{
	DrawString(0, 0, "READY SCENE", GetColor(255, 255, 255));
	DrawString(100, 400, "Push [P]Key To PLAY", GetColor(255, 255, 255));
}
