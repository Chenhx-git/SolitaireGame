#ifndef __CARD_VIEW_H__
#define __CARD_VIEW_H__

#include "cocos2d.h"
#include <functional> // 必须引入
#include "../utils/GameDefines.h"

class CardView : public cocos2d::Node {
public:
    // 修改 Create 参数，增加 ID
    static CardView* create(int id, int face, int suit);
    virtual bool init(int id, int face, int suit);

    // --- 1. 数据接口 ---
    int getCardId() const { return _id; } // 新增 ID 获取
    int getFace() const { return _face; }
    int getSuit() const { return _suit; }

    // --- 2. 状态接口 ---
    void setFaceUp(bool isFaceUp);
    bool isFaceUp() const { return _isFaceUp; }
    void setClickable(bool clickable) { _isClickable = clickable; }

    // --- 3. 交互接口 (关键) ---
    // 定义点击回调类型：参数是卡牌ID
    using CardClickCallback = std::function<void(int)>;

    // 注册回调函数
    void setOnCardClickCallback(const CardClickCallback& callback);

    // --- 4. 动画接口 ---
    // 播放匹配平移动画
    void playMatchAnimation(cocos2d::Vec2 targetPos, std::function<void()> onComplete = nullptr);
    // 播放回退动画
    void playUndoAnimation(cocos2d::Vec2 targetPos, std::function<void()> onComplete = nullptr);

private:
    // 内部处理触摸
    bool _onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);

    int _id; // 唯一ID，对应 Model 中的 ID
    int _face;
    int _suit;
    bool _isFaceUp = false;
    bool _isClickable = true;

    cocos2d::Sprite* _bgSprite = nullptr;
    cocos2d::Node* _contentNode = nullptr;

    // 保存回调函数
    CardClickCallback _onClickCallback = nullptr;
};

#endif // __CARD_VIEW_H__