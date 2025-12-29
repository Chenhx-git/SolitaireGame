#include "FlipCommand.h"
#include "../controllers/GameController.h"

USING_NS_CC;

FlipCommand::FlipCommand(CardView* card)
    : _card(card), _prevTopCard(nullptr)
{
    if (_card) _card->retain();
    _prevTopCard = GameController::getInstance()->getTopCard();
    if (_prevTopCard) _prevTopCard->retain();

    // 记录起点和终点
    if (_card) _startPos = _card->getPosition();
    _endPos = GameController::getInstance()->getDiscardPilePosition();
}

FlipCommand::~FlipCommand() {
    if (_card) _card->release();
    if (_prevTopCard) _prevTopCard->release();
}

void FlipCommand::execute() {
    if (!_card) return;

    // 1. 状态变更
    _card->setFaceUp(true);
    _card->setLocalZOrder(100);

    // 2. 动画
    _card->stopAllActions();
    auto move = MoveTo::create(0.3f, _endPos);

    // 计算新的 ZOrder
    int newZ = (_prevTopCard ? _prevTopCard->getLocalZOrder() : 0) + 1;

    // 【关键修复】
    // 1. 创建一个局部变量 targetCard 保存指针
    // 2. 在 lambda [] 中捕获 targetCard 和 newZ，而不是捕获 [=]
    CardView* targetCard = _card;

    auto finishCallback = CallFunc::create([targetCard, newZ]() {
        // 这里 targetCard 是被复制进来的指针，不再依赖 Command 对象(this)
        if (targetCard) targetCard->setLocalZOrder(newZ);
        });

    _card->runAction(Sequence::create(EaseCubicActionOut::create(move), finishCallback, nullptr));

    // 3. 逻辑变更
    GameController::getInstance()->setTopCard(_card);
}

void FlipCommand::undo() {
    if (!_card) return;

    // === 回退功能 ===

    // 1. 状态变更
    _card->setFaceUp(false);
    _card->setLocalZOrder(100);

    // 2. 动画
    _card->stopAllActions();
    auto move = MoveTo::create(0.3f, _startPos);

    // 【关键修复】同理，不要用 [=]，使用局部变量捕获
    CardView* targetCard = _card;

    auto finishCallback = CallFunc::create([targetCard]() {
        if (targetCard) targetCard->setLocalZOrder(0); // 恢复层级
        });

    _card->runAction(Sequence::create(EaseCubicActionOut::create(move), finishCallback, nullptr));

    // 3. 逻辑恢复
    if (_prevTopCard) {
        GameController::getInstance()->setTopCard(_prevTopCard);
    }
}