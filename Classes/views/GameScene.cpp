#pragma execution_character_set("utf-8")

#include "GameScene.h"
#include "CardView.h"
#include "../configs/LevelConfigLoader.h"
#include "../controllers/GameController.h"
#include <algorithm> // 必须引用，用于 std::reverse

USING_NS_CC;

Scene* GameScene::createScene() {
    return GameScene::create();
}

GameScene::~GameScene() {
    if (_controller) {
        _controller->release();
        _controller = nullptr;
    }
}

bool GameScene::init() {
    if (!Scene::init()) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // =================================================================
    // 1. 背景绘制 (上棕下紫)
    // =================================================================
    float bottomHeight = visibleSize.height * 0.25f;

    // 上半部分 (棕色)
    auto topBg = LayerColor::create(Color4B(160, 120, 60, 255), visibleSize.width, visibleSize.height - bottomHeight);
    topBg->setPosition(origin.x, origin.y + bottomHeight);
    this->addChild(topBg);

    // 下半部分 (紫色)
    auto bottomBg = LayerColor::create(Color4B(140, 40, 140, 255), visibleSize.width, bottomHeight);
    bottomBg->setPosition(origin.x, origin.y);
    this->addChild(bottomBg);

    // =================================================================
    // 2. 初始化逻辑
    // =================================================================
    _controller = GameController::create(this);
    _controller->retain();

    LevelConfig config = LevelConfigLoader::loadLevel(1);

    // 【关键】反转手牌数据，解决"顺序反了"的问题
    std::reverse(config.stackCards.begin(), config.stackCards.end());

    // 确定底部牌堆位置 (在紫色区域中心)
    float pileY = origin.y + bottomHeight / 2;
    Vec2 stockPos(visibleSize.width / 2 - 150, pileY);
    Vec2 discardPos(visibleSize.width / 2 + 150, pileY);

    _controller->setDiscardPilePosition(discardPos);

    int cardIdCounter = 0;

    // --- 生成主牌区 (桌面) ---
    for (const auto& cardData : config.playfieldCards) {
        int id = cardIdCounter++;
        auto card = CardView::create(id, cardData.face, cardData.suit);
        card->setPosition(Vec2(cardData.x, cardData.y));
        this->addChild(card);

        // 主牌区初始化翻开
        card->setFaceUp(true);

        _controller->registerCardView(id, card);
    }

    // --- 生成备用牌堆 (底部手牌) ---
    int initialTopCardId = -1;
    int totalStackCards = config.stackCards.size();
    float stackOffsetX = 35.0f; // 手牌横向间距

    for (int i = 0; i < totalStackCards; ++i) {
        auto cardData = config.stackCards[i];
        int id = cardIdCounter++;
        auto card = CardView::create(id, cardData.face, cardData.suit);
        this->addChild(card);

        _controller->registerCardView(id, card);

        if (i == totalStackCards - 1) {
            // === 初始底牌 (最右侧的一张) ===
            card->setPosition(discardPos);
            card->setFaceUp(true);
            card->setLocalZOrder(100);

            initialTopCardId = id;
            // 它是废牌堆的牌，不用 markAsStackCard
        }
        else {
            // === 备用手牌 (左侧横排) ===
            card->setFaceUp(true); // 正面朝上
            card->setLocalZOrder(i);

            // 横向排列计算
            float startX = stockPos.x - ((totalStackCards - 2) * stackOffsetX) / 2.0f;
            card->setPosition(Vec2(startX + i * stackOffsetX, stockPos.y));

            // 【关键】标记为手牌，点击时直接替换底牌
            _controller->markAsStackCard(id);
        }
    }

    // 3. 设置初始状态
    if (initialTopCardId != -1) {
        _controller->setInitialTopCard(initialTopCardId);
    }

    _controller->startGame();

    // =================================================================
    // 4. UI: 回退按钮 (修复了之前的语法错误)
    // =================================================================
    auto undoLabel = Label::createWithSystemFont("回退", "Microsoft YaHei", 36);
    undoLabel->enableShadow(Color4B::BLACK, Size(2, -2), 2);

    // 创建按钮：Lambda 只包含点击逻辑，不要在这里面设置位置
    auto undoItem = MenuItemLabel::create(undoLabel, [this](Ref* sender) {
        if (_controller) {
            _controller->onUndoBtnClick();
        }
        }); // <--- 这里闭合 Lambda

    // 在 Lambda 外面设置属性
    undoItem->setAnchorPoint(Vec2(1, 0));
    undoItem->setPosition(Vec2(visibleSize.width - 20, origin.y + 20));

    auto menu = Menu::create(undoItem, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1000);

    return true;
}