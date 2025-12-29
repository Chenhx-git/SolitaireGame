#include "MatchCommand.h"
#include "../controllers/GameController.h" // 引入控制器

USING_NS_CC;

MatchCommand::MatchCommand(CardView* card, Vec2 startPos, Vec2 endPos, int startZ, int endZ)
    : _card(card), _startPos(startPos), _startZOrder(startZ), _endZOrder(endZ), _prevTopCard(nullptr)
{
    if (_card) _card->retain();
    _prevTopCard = GameController::getInstance()->getTopCard();
    if (_prevTopCard) _prevTopCard->retain();

    // 【关键修改】终点位置 强制使用 Controller 里的底牌堆位置
    // 这样保证所有牌都整齐地叠在一起
    _endPos = GameController::getInstance()->getDiscardPilePosition();
}

MatchCommand::~MatchCommand() {
    if (_card) _card->release();
    if (_prevTopCard) _prevTopCard->release();
}

void MatchCommand::execute() {
    if (!_card) return;

    // 1. 视觉操作
    _card->setLocalZOrder(_endZOrder);
    _card->stopAllActions();

    // 飞向目标 (贝塞尔曲线或平移都可以，这里用平移)
    auto move = MoveTo::create(0.2f, _endPos);
    _card->runAction(EaseCubicActionOut::create(move));

    // 2. 逻辑操作：更新控制器的底牌为“我”
    // 注意：GameController 的 setTopCard 只是简单的赋值，不含逻辑
    GameController::getInstance()->setTopCard(_card);
}

void MatchCommand::undo() {
    if (!_card) return;

    // === 回退逻辑 ===

    // 1. 视觉回退
    _card->setLocalZOrder(_startZOrder);
    _card->stopAllActions();

    auto move = MoveTo::create(0.2f, _startPos);
    _card->runAction(EaseCubicActionOut::create(move));

    // 2. 【关键修复】逻辑回退：把底牌恢复成“前任”
    if (_prevTopCard) {
        GameController::getInstance()->setTopCard(_prevTopCard);
    }
}