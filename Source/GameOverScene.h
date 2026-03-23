#pragma once
#include "../Library/SceneBase.h"

class GameOverScene : public SceneBase
{
public:
	GameOverScene();
	~GameOverScene();
	void Update() override;
	void Draw() override;
};
