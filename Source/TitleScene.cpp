#include "TitleScene.h"
#include <DxLib.h>

TitleScene::TitleScene()
{
	image_ = LoadGraph("Assets/TITLE.png");
}

TitleScene::~TitleScene()
{
	if (image_ >= 0)
	{
		DeleteGraph(image_);
		image_ = -1;
	}
}

void TitleScene::Update()
{
	if (CheckHitKey(KEY_INPUT_SPACE)) {
		SceneManager::ChangeScene("READY");
	}
}

void TitleScene::Draw()
{
	int screenW, screenH;
	GetDrawScreenSize(&screenW, &screenH);

	DrawExtendGraph( 0, 0, screenW, screenH, image_, TRUE);
}
