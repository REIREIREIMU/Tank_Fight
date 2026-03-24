#pragma once
#include "../Library/SceneBase.h"
#include "Camera.h"
#include "Player.h"

class PlayScene : public SceneBase
{
public:
	PlayScene();
	~PlayScene();

	void Update() override;
	void Draw() override;
private:
	Camera* camera;		// Cameraのポインタを作成
	Player* player;		// Playerのポインタを作成
};
