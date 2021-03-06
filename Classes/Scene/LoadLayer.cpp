/*****************************************************************************
*  Copyright (C) 2017 奇巴图 642701967@qq.com                                  
*                                                                            
*  此文件属于软件学院2017c++大项目泡泡堂选题的项目文件.                              
*                                                                            
*  此项目是开源项目, 在期末答辩之后, 我们可能会在假期里对一些因时间不够未完成的功能进       
*  行补充, 以及适配windows平台, 在未来如果技术允许的情况下, 会酌情开发ios版本和anroid   
*  版本, 期待您能够为这个开源项目提供宝贵意见, 帮助我们做得更好, 如果能够贡献宝贵的代      
*  码那就更令人开心了.                                                          
*                                                                            
*  本项目遵守MIT开源协议, 这也就是说, 您需要遵守的唯一条件就是在修改后的代码或者发行       
*  包包含原作者的许可信息. 除非获得原作者的特殊许可, 在任何项目(包括商业项目)里使用        
*  本项目的文件都需要包含作者的许可.                                               
*                                                                            
*  如果对项目有疑问或者其他建议, 欢迎联系13167211978@163.com, 1061152718@qq.com,   
*  15750659557@163.com我们期待能和您互相交流合作, 学习更多的知识.                                      
*                                                                            
*  另外注意: 此项目需要您自行配置cocos环境,安装boost库, 如果遇到相关问题的话, 欢迎将     
*  错误日志发给我们, 您的帮助将有助于改善游戏的体验.                                  
*                                                                            
*  @file     LoadLayer.cpp                                                  
*  @brief    游戏所需资源加载缓存类                                       
*  Details.                                                                
*                                                                            
*  @author   奇巴图                                                            
*  @email    642701967@qq.com                                               
*  @version  4.0.1.5(版本号)                                                  
*  @date      2017/06/02                                        
*  @license  Massachusetts Institute of Technology License (MIT)             
*                                                                            
*----------------------------------------------------------------------------
*  Remark         : Description                                              
*----------------------------------------------------------------------------
*  Change History :                                                          
*  <Date>     | <Version> | <Author>       | <Description>                   
*----------------------------------------------------------------------------
*  2017/06/02 | 4.0.1.5   | 奇巴图          | Create file                     
*----------------------------------------------------------------------------
*                                                                            
*****************************************************************************/
#include "LoadLayer.h"

USING_NS_CC;

Scene* LoadLayer::createScene(){
    auto scene = Scene::create();
    auto layer = LoadLayer::create();
    scene->addChild(layer);
    
    return scene;
}

bool LoadLayer::init(){
    if (!Layer::init()) {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    auto bg = Sprite::create("SkyBg.png");
    this->addChild(bg);
    bg->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("animation.plist");
    auto barSprite = Sprite::create("ProgressBar.png");
    auto barBgSprite = Sprite::create("LoadingBar.png");
    barBgSprite->setPosition(Vec2(visibleSize.width/2,visibleSize.height/3));
    _progress = ProgressTimer::create(barSprite);
    _progress->setPercentage(0.0f);
    _progress->setScale(1.0f);
    _progress->setMidpoint(Point(0.0f, 0.5f));
    _progress->setBarChangeRate(Point(1.0f, 0.0f));
    _progress->setType(ProgressTimer::Type::BAR);
    _progress->setPosition(visibleSize.width/2,visibleSize.height/3);
    
    Label *label = Label::createWithTTF("Loading...", "fonts/Marker Felt.ttf", 40);
    Size size = Director::getInstance()->getVisibleSize();
    label->setPosition(Vec2(size.width / 2, size.height / 2));
    
    
    this->addChild(label);
    this->addChild(barBgSprite);
    this->addChild(_progress);
    
    return true;
}

void LoadLayer::onEnterTransitionDidFinish(){
    Layer::onEnterTransitionDidFinish();
    ValueMap map = FileUtils::getInstance()->getValueMapFromFile("loadLayer.plist");
    ValueVector spriteSheets = map.at("SpriteSheets").asValueVector();
    ValueVector musics = map.at("BGM").asValueVector();
    _sourceCount = spriteSheets.size() + musics.size();
    _progressInterval = 100 / _sourceCount;
    loadSpriteSheets(spriteSheets);
    loadMusic(musics);
    
}

void LoadLayer::loadMusic(ValueVector musicFiles){
    for (Value &v : musicFiles) {
        progressUpdate();
    }
}

void LoadLayer::loadSpriteSheets(ValueVector spriteSheets){
    for (Value &v : spriteSheets) {
        SpriteFrameCache::getInstance()->addSpriteFramesWithFile(v.asString().c_str());
        progressUpdate();
    }
}

void LoadLayer::progressUpdate(){
    if(--_sourceCount){
        _progress->setPercentage(100.0f-(_progressInterval * _sourceCount));
    }
    else{
        auto pft = ProgressFromTo::create(2.0f, 0, 100);
        auto callFunc = CallFunc::create([=]{
            auto delay = DelayTime::create(0.2f);
            auto callFunc = CallFunc::create([]{
                auto transition = TransitionSlideInB::create(2, GameMenu::createScene());
                Director::getInstance()->replaceScene(transition);
            });
            auto action = Sequence::create(delay,callFunc, NULL);
            this->runAction(action);
        });
        auto action = Sequence::create(pft,callFunc, NULL);
        _progress->runAction(action);
    }
}


