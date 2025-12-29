#include "GameController.h"
#include "../services/RuleService.h"

USING_NS_CC;

GameController* GameController::create(Scene* scene) {
    GameController* controller = new GameController();
    if (controller && controller->init(scene)) {
        controller->autorelease();
        return controller;
    }
    delete controller;
    return nullptr;
}

GameController::GameController()
    : _gameModel(nullptr), _undoManager(nullptr), _sceneCtx(nullptr) {
}

GameController::~GameController() {
    if (_gameModel) delete _gameModel;
    if (_undoManager) delete _undoManager;
}

bool GameController::init(Scene* scene) {
    _sceneCtx = scene;

    _gameModel = new GameModel();
    _undoManager = new UndoManager();

    return true;
}

void GameController::registerCardView(int id, CardView* view) {
    if (!view) return;

    _views[id] = view;

    view->setOnCardClickCallback([this](int clickedId) {
        this->_handleCardClick(clickedId);
        });

    CardData data(id, view->getFace(), view->getSuit());
    _gameModel->playFieldCards.push_back(data);
}

// 【新增】标记手牌
void GameController::markAsStackCard(int cardId) {
    _stackCardIds.insert(cardId);
}

void GameController::startGame() {
    // 游戏开始
}

// ========================================================
// 核心交互逻辑 (逻辑修正版)
// ========================================================

void GameController::_handleCardClick(int cardId) {
    if (_views.find(cardId) == _views.end()) return;
    CardView* view = _views[cardId];

    // 逻辑 A: 如果是手牌区的牌 -> 执行"替换/翻牌" (需求1)
    // 只要它在 stack 列表里，且不是当前的底牌(防止误点废牌堆)，就直接飞
    if (_stackCardIds.find(cardId) != _stackCardIds.end()) {
        if (_gameModel->topCard.id != cardId) {
            _tryFlip(cardId);
        }
    }
    // 逻辑 B: 否则(桌面牌) -> 执行"消除/匹配" (需求2)
    else {
        // 只有正面的牌才能消除
        if (view->isFaceUp()) {
            _tryMatch(cardId);
        }
    }
}

bool GameController::_tryMatch(int cardId) {
    CardView* clickView = _views[cardId];

    CardData clickData(cardId, clickView->getFace(), clickView->getSuit());
    CardData topData = _gameModel->topCard;

    // 只有点数差1才能消除
    if (RuleService::canMatch(clickData, topData)) {
        _performMoveToDiscard(cardId, false);
        return true;
    }
    else {
        // 抖动提示错误
        clickView->runAction(Sequence::create(
            MoveBy::create(0.05f, Vec2(-10, 0)),
            MoveBy::create(0.05f, Vec2(20, 0)),
            MoveBy::create(0.05f, Vec2(-10, 0)),
            nullptr
        ));
        return false;
    }
}

bool GameController::_tryFlip(int cardId) {
    // 手牌直接飞，不需要规则判断
    _performMoveToDiscard(cardId, true);
    return true;
}

void GameController::_performMoveToDiscard(int cardId, bool isFlipAction) {
    CardView* cardView = _views[cardId];

    // 1. 保存状态 (Undo用)
    Vec2 oldPos = cardView->getPosition();
    int oldZ = cardView->getLocalZOrder();
    bool oldFaceUp = cardView->isFaceUp();
    CardData oldTopCard = _gameModel->topCard;

    // 【关键】检查它是不是手牌，如果是，飞走后就要从手牌区除名
    bool wasStackCard = (_stackCardIds.find(cardId) != _stackCardIds.end());
    if (wasStackCard) {
        _stackCardIds.erase(cardId);
    }

    // 2. 记录 Undo
    _undoManager->pushAction([=]() {
        // A. 恢复 Model
        _gameModel->topCard = oldTopCard;

        // B. 恢复 View
        if (cardView) {
            cardView->setFaceUp(oldFaceUp);
            cardView->playUndoAnimation(oldPos, [=]() {
                cardView->setLocalZOrder(oldZ);
                });
        }

        // C. 恢复身份 (如果是手牌，Undo回来后它又变回手牌了)
        if (wasStackCard) {
            this->markAsStackCard(cardId);
        }
        });

    // 3. 更新 Model
    _gameModel->topCard = CardData(cardId, cardView->getFace(), cardView->getSuit());

    // 4. 更新 View
    if (isFlipAction) {
        cardView->setFaceUp(true);
    }
    cardView->playMatchAnimation(_discardPilePos);
}

void GameController::onUndoBtnClick() {
    if (_undoManager && _undoManager->hasHistory()) {
        _undoManager->undo();
    }
}

void GameController::setInitialTopCard(int cardId) {
    if (_views.find(cardId) != _views.end()) {
        CardView* view = _views[cardId];
        _gameModel->topCard = CardData(cardId, view->getFace(), view->getSuit());
    }
}