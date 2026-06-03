#include "PlayScene.h"
#include "Camera.h"
#include "Player.h"
#include "Enemy.h"
#include "Object.h"
#include "Config.h"

// =========================================
// 円同士の衝突解決（XZ平面のみで処理）
// posA / posB : それぞれの中心座標
// radiusA / radiusB : 判定半径
// =========================================
static void ResolveCollision(
    VECTOR& posA, float radiusA,
    VECTOR& posB, float radiusB,
    Object* object)
{
    // 2点間の差分ベクトル
    VECTOR delta = VSub(posA, posB);

    // 距離（XZ平面のみを使用）
    float dist = sqrtf(delta.x * delta.x + delta.z * delta.z);

    // 衝突判定距離（半径の合計）
    float minDist = radiusA + radiusB;

    // 衝突していなければ処理しない
    if (dist >= minDist) return;

    // =========================================
    // 完全に重なった場合の対策
    // (距離0だと正規化できないため微小値を入れる)
    // =========================================
    if (dist < 0.0001f) {
        delta = VGet(0.01f, 0.0f, 0.0f);
        dist = 0.01f;
    }

    float overlap = minDist - dist;    // めり込み量
    VECTOR pushDir = VNorm(delta);     // 押し出し方向（正規化）

    // =========================================
    // 両者が均等に押し出されるように半分ずつ移動
    // =========================================
    VECTOR move = VScale(pushDir, overlap * 0.5f);
    VECTOR newA = VAdd(posA, move);
    VECTOR newB = VSub(posB, move);

    // =========================================
    // 壁との衝突チェック
    // (移動後の位置が有効か確認)
    // =========================================
    bool canMoveA = !object->CheckHit(newA.x, newA.z, radiusA);
    bool canMoveB = !object->CheckHit(newB.x, newB.z, radiusB);
    
    // 両方動けるならそのまま適用
    if (canMoveA && canMoveB) { posA = newA; posB = newB;}
    // 片方だけ動ける場合はその側のみ押し出す
    else if (canMoveA) posA = VAdd(posA, VScale(pushDir, overlap));
    else if (canMoveB) posB = VSub(posB, VScale(pushDir, overlap));
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

// =========================================
// プレイヤーと敵の当たり判定処理
// =========================================
void PlayScene::PlayerEnemyCollision()
{
    for (Enemy* e : enemies)
    {
        // 無効又は、死亡している敵は無視
        if (!e || !e->IsAlive()) continue;

        VECTOR pPos = player->GetPosition(); // プレイヤーの現在位置を取得
        VECTOR ePos = e->GetPosition();      // 敵の現在位置を取得

        // 衝突解決処理
        ResolveCollision(
            pPos, Config::Player_Half,
            ePos, Config::Enemy_Half,
            object
        );

        // 計算結果を反映
        player->SetPosition(pPos);
        e->SetPosition(ePos);
    }
}

// =========================================
// 敵同士の当たり判定処理
// =========================================
void PlayScene::EnemyCollision()
{
    for (size_t i = 0; i < enemies.size(); i++)
    {
        for (size_t j = i + 1; j < enemies.size(); j++)
        {
            Enemy* a = enemies[i];
            Enemy* b = enemies[j];

            // 無効又は、死亡している敵は無視
            if (!a || !b) continue;
            if (!a->IsAlive() || !b->IsAlive()) continue;

            VECTOR posA = a->GetPosition(); // 敵Aの現在位置を取得
            VECTOR posB = b->GetPosition(); // 敵Bの現在位置を取得
            
            // 衝突解決
            ResolveCollision(
                posA, Config::Enemy_Half,
                posB, Config::Enemy_Half,
                object
            );

            // 結果を反映
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
            if (timer >= DEATH_WAIT) {
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
        if (stageClearTimer >= STAGE_WAIT) {
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