#include "PlayScene.h"
#include "Camera.h"
#include "Player.h"
#include "Enemy.h"
#include "Object.h"
#include "Config.h"

// 戦車同士の衝突関係
static void ResolveCollision(
    VECTOR& posA, float radiusA,
    VECTOR& posB, float radiusB,
    Object* object)
{
    VECTOR delta = VSub(posA, posB);
    float dist = sqrtf(delta.x * delta.x + delta.z * delta.z);
    float minDist = radiusA + radiusB;

    if (dist >= minDist) return;

    // 重なり防止
    if (dist < 0.0001f) {
        delta = VGet(0.01f, 0.0f, 0.0f);
        dist = 0.01f;
    }

    float overlap = minDist - dist;
    VECTOR pushDir = VNorm(delta);

    VECTOR move = VScale(pushDir, overlap * 0.5f);
    VECTOR newA = VAdd(posA, move);
    VECTOR newB = VSub(posB, move);

    // 壁判定確認用
    bool canMoveA = !object->CheckHit(newA.x, newA.z, radiusA);
    bool canMoveB = !object->CheckHit(newB.x, newB.z, radiusB);

    if (canMoveA && canMoveB) {
        posA = newA;
        posB = newB;
    }
    else if (canMoveA) {
        posA = VAdd(posA, VScale(pushDir, overlap));
    }
    else if (canMoveB) {
        posB = VSub(posB, VScale(pushDir, overlap));
    }
}

PlayScene::PlayScene() :timer(0)
{
    Enemy::SetInvincible(false);

    camera = new Camera();                      // cameraの初期化
    object = new Object(Player::GetStage());    // objectの初期化
    player = new Player(object, &enemies);      // playerの初期化
    object->GetEnemySpawnPos(enemies, player);  // Enemy生成と初期化
}

PlayScene::~PlayScene()
{
    camera = nullptr;
    player = nullptr;
    //e  = nullptr;
    object = nullptr;
}

void PlayScene::Update()
{
    // 各自Update
    {
        camera->Update();
        player->Update();
        for (auto e : enemies)
            e->Update();
        object->Update();
    }

    PlayerEnemyCollision();
    EnemyCollision();
    PlayerDeath();
    StageClear();
}

// プレイヤーと敵の当たり判定処理
void PlayScene::PlayerEnemyCollision()
{
    for (Enemy* e : enemies)
    {
        if (!e || !e->IsAlive()) continue;

        VECTOR pPos = player->GetPosition(); // プレイヤーの判定を取得
        VECTOR ePos = e->GetPosition();      // 敵の判定を取得

        ResolveCollision(
            pPos, Config::Player_Half,
            ePos, Config::Enemy_Half,
            object
        );

        player->SetPosition(pPos);
        e->SetPosition(ePos);
    }
}

// 敵同士の当たり判定処理
void PlayScene::EnemyCollision()
{
    for (size_t i = 0; i < enemies.size(); i++)
    {
        for (size_t j = i + 1; j < enemies.size(); j++)
        {
            Enemy* a = enemies[i];
            Enemy* b = enemies[j];

            if (!a || !b) continue;
            if (!a->IsAlive() || !b->IsAlive()) continue;

            VECTOR posA = a->GetPosition();      // 敵Aの判定を取得
            VECTOR posB = b->GetPosition();      // 敵Bの判定を取得

            ResolveCollision(
                posA, Config::Enemy_Half,
                posB, Config::Enemy_Half,
                object
            );
            a->SetPosition(posA);
            b->SetPosition(posB);
        }
    }
}

// プレイヤー死亡処理
void PlayScene::PlayerDeath()
{
    // プレイヤー死亡後の処理
    if (!player->IsAlive()) {
        // プレイヤー死亡時点で敵を無敵にする
        Enemy::SetInvincible(true);

        if (timer < 0) timer = 0;
        else {
            timer++;
            if (timer >= Config::DEATH_WAIT) {
                // 残機を1減らす
                player->Player::DecreaseLives();

                // 残機が残っていたら準備画面へ
                if (player->Player::GetLives() > 0) {
                    SceneManager::ChangeScene("READY");
                }
                // 残機がないならリザルト画面へ
                else {
                    SceneManager::ChangeScene("CLEAR");
                }
            }
        }
    }
}

// ステージクリア処理
void PlayScene::StageClear()
{
    bool allDead = true;     // 敵全滅チェック
    int StageEnemyCount = 0; // ステージ内の敵のキル数

    for (auto e : enemies)
    {
        if (e) {
            // 倒した敵を加算
            StageEnemyCount++;
            // 全滅が確認出来たらfalse
            if (e->IsAlive())
                allDead = false;
        }
    }

    if (allDead && !stageClear) {
        stageClear = true;
        stageClearTimer = 0;

        // ステージ分の撃破数を加算
        Player::AddEnemyKill(StageEnemyCount);

        // プレイヤーを無敵可
        Player::SetInvincible(true);

        // プレイヤー操作停止
        Player::SetControlEnabled(false);

        // プレイヤー弾削除
        player->ClearBullets();

        // 敵弾削除
        for (auto e : enemies) {
            if (e) e->ClearBullets();
        }
    }

    // 敵全滅後の待機
    if (stageClear) {
        stageClearTimer++;
        if (stageClearTimer >= Config::STAGE_WAIT) {
            Player::SetInvincible(false);    // 敵全滅後プレイヤー無敵
            Player::SetControlEnabled(true); // 敵全滅後プレイヤーの操作無効化

            // 最大ステージ判定
            if (Player::GetStage() >= MAX_STAGE) {
                // 最大ステージをクリアしたらリザルトへ
                SceneManager::ChangeScene("CLEAR");
            }
            else {
                // ステージがあれば次のステージへ
                Player::NextStage();
                SceneManager::ChangeScene("READY");
            }
        }
    }
}

void PlayScene::Draw()
{
    // 画面クリア
    ClearDrawScreen();

    if (!camera || !player || !object) return;

    Draw3D();   // 3D描画
    DrawUI();   // UI描画

    // グリット線表示(デバック用)
    /*
    for (int i = -Grid_Half; i <= Grid_Half; i++)
    {
        float pos = i * Grid_Size;

        // Z方向に線表示
        DrawLine3D(
            VGet(pos, 0.0f, -Grid_Half * Grid_Size),
            VGet(pos, 0.0f, Grid_Half * Grid_Size),
            // X軸の原点のみ「赤」
            (i == 0) ? xAxisColor : GridColor
        );

        // X方向に線表示
        DrawLine3D(
            VGet(-Grid_Half * Grid_Size, 0.0f, pos),
            VGet( Grid_Half * Grid_Size, 0.0f, pos),
            // Z軸の原点のみ「青」
            (i == 0) ? zAxisColor : GridColor
        );
    }

   //DrawString(0, 0, "PLAY SCENE", GetColor(255, 255, 255));
   //DrawString(100, 400, "Push [G]Key To GameOverScene", GetColor(255, 255, 255));

   //DrawString(100, 500, "Push [C]Key To Clear", GetColor(255, 255, 255));
   */
}

// 3D描画
void PlayScene::Draw3D()
{
    SetUseZBuffer3D(TRUE);
    SetWriteZBuffer3D(TRUE);

    player->Draw();        // プレイヤーのモデルを表示
    player->DrawBullets(); // プレイヤーの弾

    for (auto e : enemies) {
        e->Draw();          // 敵のモデルを表示
        e->DrawBullets();   // 敵の弾
    }
    object->Draw();  // ブロック系統のモデルを表示
}

// UI描画
void PlayScene::DrawUI()
{
    int aliveEnemyCount = 0;

    for (auto e : enemies)
        if (e && e->IsAlive())
            aliveEnemyCount++;

    SetUseZBuffer3D(FALSE);
    SetWriteZBuffer3D(FALSE);

    // UI用カメラ ※ これがないとUIが表示されない
    SetCameraPositionAndTarget_UpVecY(
        VGet(0.0f, 0.0f, 0.0f),
        VGet(0.0f, 0.0f, 0.0f)
    );

    // UI表示
    {
        // ==== 現在のステージの数 ====
        SetFontSize(40);
        DrawFormatString(20, 20, GetColor(255, 255, 255),
            "STAGE：%d", Player::GetStage());
        // ======== 残機の数 ========
        SetFontSize(30);
        DrawFormatString(1120, 20, GetColor(255, 255, 255),
            "残機： %d", Player::GetLives());
        // ======= 残り敵の数 =======
        SetFontSize(30);
        DrawFormatString(20, 60, GetColor(255, 255, 255),
            "残り敵数 : %d", aliveEnemyCount);
    }
}