#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"

// 前向声明，避免循环引用
class GameController;

class GameScene : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();
    virtual bool init();

    // 析构函数：记得释放控制器
    virtual ~GameScene();

    CREATE_FUNC(GameScene);

private:
    // 持有控制器的指针
    GameController* _controller;
};

#endif // __GAME_SCENE_H__