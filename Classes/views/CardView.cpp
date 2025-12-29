#include "CardView.h"
#include <string>

USING_NS_CC;

CardView* CardView::create(int id, int face, int suit) {
    CardView* pRet = new(std::nothrow) CardView();
    if (pRet && pRet->init(id, face, suit)) {
        pRet->autorelease();
        return pRet;
    }
    else {
        delete pRet;
        return nullptr;
    }
}

bool CardView::init(int id, int face, int suit) {
    if (!Node::init()) {
        return false;
    }

    _id = id;
    _face = face;
    _suit = suit;
    _isFaceUp = false;
    _isClickable = true;

    // --- 1. 构建卡牌外观 ---
    // 【核心逻辑】无论正面背面，底图永远是这张通用的背景图
    _bgSprite = Sprite::create("cards/card_general.png");

    if (!_bgSprite) {
        // 如果连 card_general 都没有，画个红框报错
        auto errLayer = LayerColor::create(Color4B::RED, 100, 140);
        this->addChild(errLayer);
        this->setContentSize(Size(100, 140));
    }
    else {
        this->addChild(_bgSprite);
        Size size = _bgSprite->getContentSize();
        this->setContentSize(size);
        _bgSprite->setPosition(size.width / 2, size.height / 2);

        // 内容节点：装着数字和花色
        _contentNode = Node::create();
        _contentNode->setPosition(Vec2::ZERO);
        _bgSprite->addChild(_contentNode);

        // --- 绘制花色和数字 ---
        bool isRed = (suit == 1 || suit == 3);
        std::string colorStr = isRed ? "red" : "black";

        std::string faceStr = std::to_string(face);
        if (face == 1) faceStr = "A";
        if (face == 11) faceStr = "J";
        if (face == 12) faceStr = "Q";
        if (face == 13) faceStr = "K";

        std::string suitName;
        switch (suit) {
        case 0: suitName = "spade"; break;
        case 1: suitName = "heart"; break;
        case 2: suitName = "club"; break;
        case 3: suitName = "diamond"; break;
        }

        // 小数字
        std::string numPath = StringUtils::format("cards/number/small_%s_%s.png", colorStr.c_str(), faceStr.c_str());
        auto numSprite = Sprite::create(numPath);
        if (numSprite) {
            numSprite->setAnchorPoint(Vec2(0, 1));
            numSprite->setPosition(Vec2(10, size.height - 10));
            _contentNode->addChild(numSprite);
        }

        // 小花色
        std::string suitPath = StringUtils::format("cards/suits/%s.png", suitName.c_str());
        auto smallSuitSprite = Sprite::create(suitPath);
        if (smallSuitSprite) {
            smallSuitSprite->setAnchorPoint(Vec2(0.5, 1));
            smallSuitSprite->setScale(0.5f);
            smallSuitSprite->setPosition(Vec2(23, size.height - 50));
            _contentNode->addChild(smallSuitSprite);
        }

        // 大图
        std::string bigPath = StringUtils::format("cards/number/big_%s_%s.png", colorStr.c_str(), faceStr.c_str());
        auto bigSprite = Sprite::create(bigPath);
        if (bigSprite) {
            bigSprite->setPosition(Vec2(size.width / 2, size.height / 2));
            _contentNode->addChild(bigSprite);
        }
    }

    // 初始化时，根据默认状态设置可见性
    if (_contentNode) _contentNode->setVisible(_isFaceUp);

    // --- 2. 初始化交互监听 ---
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(CardView::_onTouchBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

void CardView::setOnCardClickCallback(const CardClickCallback& callback) {
    _onClickCallback = callback;
}

bool CardView::_onTouchBegan(Touch* touch, Event* event) {
    if (!this->isVisible() || !_isClickable) return false;

    Vec2 localPoint = this->convertToNodeSpace(touch->getLocation());
    Rect rect = Rect(0, 0, _contentSize.width, _contentSize.height);

    if (rect.containsPoint(localPoint)) {
        if (_onClickCallback) {
            _onClickCallback(_id);
        }
        return true;
    }
    return false;
}

void CardView::setFaceUp(bool isFaceUp) {
    // 即使状态没变，为了保险也可以执行一次，或者保留这个判断
    // if (_isFaceUp == isFaceUp) return; 
    _isFaceUp = isFaceUp;

    if (!_bgSprite || !_contentNode) return;

    if (_isFaceUp) {
        // 正面：显示数字和花色
        // _bgSprite->setTexture("cards/card_general.png"); // 不需要重复设，底图一直都是它
        _contentNode->setVisible(true);
    }
    else {
        // 背面：隐藏数字和花色，剩下的就是一张白板
        // 这样就不需要 card_back.png 了
        _contentNode->setVisible(false);
    }
}

void CardView::playMatchAnimation(Vec2 targetPos, std::function<void()> onComplete) {
    this->setLocalZOrder(100);
    auto move = MoveTo::create(0.3f, targetPos);
    auto ease = EaseCubicActionOut::create(move);

    Action* act = nullptr;
    if (onComplete) {
        act = Sequence::create(ease, CallFunc::create(onComplete), nullptr);
    }
    else {
        act = ease;
    }
    this->stopAllActions();
    this->runAction(act);
}

void CardView::playUndoAnimation(Vec2 targetPos, std::function<void()> onComplete) {
    this->setLocalZOrder(100);
    auto move = MoveTo::create(0.3f, targetPos);
    auto ease = EaseCubicActionOut::create(move);

    Action* act = nullptr;
    if (onComplete) {
        act = Sequence::create(ease, CallFunc::create(onComplete), nullptr);
    }
    else {
        act = ease;
    }
    this->stopAllActions();
    this->runAction(act);
}