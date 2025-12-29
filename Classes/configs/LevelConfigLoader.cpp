#include "LevelConfigLoader.h"
#include "cocos2d.h"
// 引入 rapidjson 库 (Cocos 自带的 JSON 解析库)
#include "json/rapidjson.h"
#include "json/document.h"

USING_NS_CC;

LevelConfig LevelConfigLoader::loadLevel(int levelId) {
    LevelConfig config;
    config.levelId = levelId;

    // 1. 拼接文件路径，例如 "levels/level_1.json"
    std::string filename = StringUtils::format("levels/level_%d.json", levelId);

    // 2. 从硬盘读取文件内容
    std::string content = FileUtils::getInstance()->getStringFromFile(filename);

    if (content.empty()) {
        CCLOG("Error: 没找到关卡文件 -> %s", filename.c_str());
        return config;
    }

    // 3. 解析 JSON
    rapidjson::Document doc;
    doc.Parse(content.c_str());

    if (doc.HasParseError()) {
        CCLOG("Error: JSON 格式写错了 -> %s", filename.c_str());
        return config;
    }

    // 4. 读取主牌区数据 (Playfield)
    if (doc.HasMember("Playfield") && doc["Playfield"].IsArray()) {
        const rapidjson::Value& arr = doc["Playfield"];
        for (rapidjson::SizeType i = 0; i < arr.Size(); i++) {
            const rapidjson::Value& item = arr[i];
            CardConfigData card;
            card.face = item["CardFace"].GetInt();
            card.suit = item["CardSuit"].GetInt();
            card.x = item["Position"]["x"].GetFloat();
            card.y = item["Position"]["y"].GetFloat();
            config.playfieldCards.push_back(card);
        }
    }

    // 5. 读取备用牌堆数据 (Stack)
    if (doc.HasMember("Stack") && doc["Stack"].IsArray()) {
        const rapidjson::Value& arr = doc["Stack"];
        for (rapidjson::SizeType i = 0; i < arr.Size(); i++) {
            const rapidjson::Value& item = arr[i];
            CardConfigData card;
            card.face = item["CardFace"].GetInt();
            card.suit = item["CardSuit"].GetInt();
            card.x = item["Position"]["x"].GetFloat();
            card.y = item["Position"]["y"].GetFloat();
            config.stackCards.push_back(card);
        }
    }

    CCLOG("成功加载关卡 %d ! 主牌区: %d 张, 备用堆: %d 张",
        levelId, (int)config.playfieldCards.size(), (int)config.stackCards.size());

    return config;
}