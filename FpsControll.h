#ifndef FPS_CONTROLLER_HPP_
#define FPS_CONTROLLER_HPP_

#include <list>

class FpsController final {
public:
    FpsController() = delete;
    explicit FpsController(int target_fps);

    //! @brief 処理が早すぎる場合，FPSを一定にするために待つ．
    void Wait();

    //! @brief 60Hz以上のモニター使用時に処理が詰まって画面がちらつかないように，
    //! 描画処理をスキップするかどうかを判定する．
    //! @return 処理が詰まって描画を飛ばしたいときに
    //! true を返す．その後フラグを false にする．
    bool SkipDrawScene();

private:
    //! @brief 現在の時刻を記録する関数．
    //! @param[in] now_time 現在の時刻(ミリ秒)
    void RegisterTime(int now_time);

    //! @brief どれだけ待てばよいか返す関数．
    //! @param[out] wait_time 待つべき時間 (ミリ秒)．
    //! @return コマ落ちしている場合は false．
    bool CheckNeedSkipDrawScreen(int* wait_time) const;

    //! @brief 目標のFPSが正しいかどうかを判定する関数．
    //! @return 負の値，または60より大きい値であれば false．
    bool TargetFpsIsValid() const;

    const int kTargetFpsValue;

    //! 1フレーム当たりにかかる時間(ミリ秒)
    const int kOneFrameTime;

    //! リストに2秒分のフレームごとにかかった時間を記録するため，
    //! リストの最大サイズを決める．
    const int kListMax;

    //! 1フレームごとにかかった時間を記録するリスト．
    std::list<int> time_list_;

    //! コマ落ちを実装するためのフラグ．
    //! trueであれば 1フレーム描画を飛ばし，その後フラグを折る
    bool need_skip_draw_screen_;
};

#endif  //  FPS_CONTROLLER_HPP_
