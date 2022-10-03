#include "SetMusic.h"
#include "MainMenu.h"
#include "AudioEngine.h"
#include <string>
using namespace std;


CCSetMusic::CCSetMusic()
{
	// Ä¬ÈÏµÄÒôÀÖºÍÒôÐ§¶¼ÊÇ¿ª×Å
	bMusic = true;
	bSoundEffect = true;
}

CCSetMusic::~CCSetMusic()
{

}

bool CCSetMusic::init()
{
	Sprite *pBg = Sprite::create("bg.png");
	Point origin = Director::getInstance()->getVisibleOrigin();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	
	pBg->setPosition(Vec2(origin.x+visibleSize.width/2, origin.y+visibleSize.height/2));
	this->addChild(pBg, 0);

	Sprite *pSetBg = Sprite::create("Set_Music.png");
	pSetBg->setAnchorPoint(Vec2(129.0/248, 71.0/132));
	pSetBg->setPosition(Vec2(origin.x+visibleSize.width/2, origin.y+visibleSize.height/2));
	this->addChild(pSetBg, 1);

	MenuItemImage *pBack = MenuItemImage::create("backA.png", "backB.png",
    CC_CALLBACK_1(CCSetMusic::menuBackMainMenu,this));
	pBack->setPosition(Vec2(origin.x+20, origin.y+visibleSize.height-20));
	Menu *pMenu = Menu::create(pBack, NULL);
	pMenu->setAnchorPoint(Vec2::ZERO);
	pMenu->setPosition(Vec2::ZERO);
	this->addChild(pMenu, 2);

	this->initSwitch();

	return true;
}

void CCSetMusic::menuBackMainMenu( Ref *pSender )
{
	Scene *pScene = Scene::create();
	pScene->addChild(MainMenu::create());
	Director::getInstance()->replaceScene(pScene);
}

void CCSetMusic::initSwitch()
{
	Point origin = Director::getInstance()->getVisibleOrigin();
	Size visibleSize = Director::getInstance()->getVisibleSize();

	pMusicOn = SpriteFrame::create("music_on.png", Rect(0, 0, 70, 63));
	pMusicOn->retain();
	pMusicOff = SpriteFrame::create("music_off.png", Rect(0, 0, 70, 63));
	pMusicOff->retain();
	pEffectOn = SpriteFrame::create("sound_effect_on.png", Rect(0, 0, 70, 63));
	pEffectOn->retain();
	pEffectOff = SpriteFrame::create("sound_effect_off.png", Rect(0, 0, 70, 63));
	pEffectOff->retain();

	pMusic = Sprite::createWithSpriteFrame(pMusicOn);
	pMusic->setPosition(Vec2(origin.x+visibleSize.width/2+80, origin.y+visibleSize.height/2-40));
	this->addChild(pMusic, 3);

	pSoundEffect = Sprite::createWithSpriteFrame(pEffectOn);
	pSoundEffect->setPosition(Vec2(origin.x+visibleSize.width/2+80, origin.y+visibleSize.height/2+40));
	this->addChild(pSoundEffect, 3);

	MenuItemImage *pMusicMenu = MenuItemImage::create("switchBg.png", "switchBg.png",
		CC_CALLBACK_1(CCSetMusic::menuMusic,this));
	pMusicMenu->setPosition(Vec2(origin.x+visibleSize.width/2+80, origin.y+visibleSize.height/2-40));
	MenuItemImage *pEffectMenu = MenuItemImage::create("switchBg.png", "switchBg.png",
		CC_CALLBACK_1(CCSetMusic::menuEffect,this));
	pEffectMenu->setPosition(Vec2(origin.x+visibleSize.width/2+80, origin.y+visibleSize.height/2+40));

	Menu *pMenu = Menu::create(pMusicMenu, pEffectMenu, NULL);
	pMenu->setAnchorPoint(Vec2::ZERO);
	pMenu->setPosition(Vec2::ZERO);
	this->addChild(pMenu, 1);
}

void CCSetMusic::menuMusic( Ref *pSender )
{
	if (bMusic)
	{
		bMusic = false;
        pMusic->setSpriteFrame(pMusicOff);//
        ////音乐先不管
		//CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.0f);
		UserDefault::getInstance()->setStringForKey("Music", "off");
	}else
	{
		bMusic = true;
		pMusic->setSpriteFrame(pMusicOn);
        ////音乐先不管
		//CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(1.0f);
		UserDefault::getInstance()->setStringForKey("Music", "on");
	}
}

void CCSetMusic::menuEffect( Ref *pSender )
{
	if (bSoundEffect)
	{
		bSoundEffect = false;
		pSoundEffect->setSpriteFrame(pEffectOff);
        ////音乐先不管
		//CocosDenshion::SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.0f);
		UserDefault::getInstance()->setStringForKey("SoundEffect", "off");
	}else
	{
		bSoundEffect = true;
		pSoundEffect->setSpriteFrame(pEffectOn);
        ////音乐先不管
		//CocosDenshion::SimpleAudioEngine::sharedEngine()->setEffectsVolume(1.0f);
		UserDefault::getInstance()->setStringForKey("SoundEffect", "on");
	}
}

void CCSetMusic::onEnter()
{
	Layer::onEnter();

	string str = UserDefault::getInstance()->getStringForKey("Music");
	if (str == "on")
	{
		bMusic = true;
		pMusic->setSpriteFrame(pMusicOn);
        ////音乐先不管
		//CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(1.0f);
	}else if (str == "off")
	{
		bMusic = false;
		pMusic->setSpriteFrame(pMusicOff);
        ////音乐先不管
		//CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.0f);
	}

	str = UserDefault::getInstance()->getStringForKey("SoundEffect");
	if (str == "on")
	{
		bSoundEffect = true;
		pSoundEffect->setSpriteFrame(pEffectOn);
        ////音乐先不管
		//CocosDenshion::SimpleAudioEngine::sharedEngine()->setEffectsVolume(1.0f);
	}else if (str == "off")
	{
		bSoundEffect = false;
		pSoundEffect->setSpriteFrame(pEffectOff);
        ////音乐先不管
		//CocosDenshion::SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.0f);
	}
}

void CCSetMusic::onExit()
{


	Layer::onExit();
}

