#include "AppDelegate.h"
// #include "HelloWorldScene.h" // 注释了
#include "views/GameScene.h"

USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(1080, 2080);
static cocos2d::Size smallResolutionSize = cocos2d::Size(540, 1040); // 电脑上显示的小窗口尺寸

AppDelegate::AppDelegate() {}
AppDelegate::~AppDelegate() {}

void AppDelegate::initGLContextAttrs() {
    GLContextAttrs glContextAttrs = { 8, 8, 8, 8, 24, 8, 0 };
    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
    // 1. 初始化导演 (Director)
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();

    // 2. 如果没有视图，创建一个
    if (!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        // 电脑上用小窗口 (540x1040)，方便调试
        glview = GLViewImpl::createWithRect("SolitaireGame", cocos2d::Rect(0, 0, smallResolutionSize.width, smallResolutionSize.height));
#else
        glview = GLViewImpl::create("SolitaireGame");
#endif
        director->setOpenGLView(glview);
    }

    // 3. 开启 FPS 显示 (调试用)
    director->setDisplayStats(true);
    director->setAnimationInterval(1.0f / 60);

    // 4. 设置设计分辨率 (1080x2080) - 宽度固定策略
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::FIXED_WIDTH);

    // 5. 启动游戏场景
    // 目前删掉了 HelloWorld，还没有写新场景，先留空。
    // 5. 启动游戏场景
    auto scene = GameScene::createScene(); // 创建我们的游戏场景
    director->runWithScene(scene);         // 运行它

    return true;
}

void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();
}

void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();
}