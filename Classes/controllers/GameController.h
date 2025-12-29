#ifndef __GAME_CONTROLLER_H__
#define __GAME_CONTROLLER_H__

#include "cocos2d.h"
#include "../models/GameModel.h"
#include "../views/CardView.h"
#include "../managers/UndoManager.h"
#include <map>
#include <set> // 【新增】必须引入 set

class GameController : public cocos2d::Ref {
public:
    static GameController* create(cocos2d::Scene* scene);
    ~GameController();
    bool init(cocos2d::Scene* scene);

    void onUndoBtnClick();
    void registerCardView(int id, CardView* view);
    void setDiscardPilePosition(cocos2d::Vec2 pos) { _discardPilePos = pos; }
    void setInitialTopCard(int cardId);
    void startGame();

    // 【新增】标记哪些牌属于手牌区(Stack)
    void markAsStackCard(int cardId);

private:
    GameController();

    // 核心处理函数
    void _handleCardClick(int cardId);
    bool _tryMatch(int cardId);
    bool _tryFlip(int cardId);
    void _performMoveToDiscard(int cardId, bool isFlipAction);

    cocos2d::Scene* _sceneCtx;
    GameModel* _gameModel;
    UndoManager* _undoManager;

    std::map<int, CardView*> _views;

    // 【新增】存储手牌区的ID集合
    std::set<int> _stackCardIds;

    cocos2d::Vec2 _discardPilePos;
};

#endif // __GAME_CONTROLLER_H__