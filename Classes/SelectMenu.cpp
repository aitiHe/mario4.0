#include "SelectMenu.h"
#include "MainMenu.h"
#include "Global.h"
#include "GameLayer.h"
#include <string>
#include <stdio.h>
using namespace std;


bool CCSelectMenu::init()
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF( !Layer::init() );

		m_nCurPage = 1;
		Size visibleSize = Director::getInstance()->getVisibleSize();
		Point origin = Director::getInstance()->getVisibleOrigin();

		pLayer = Layer::create();
		//CCString *seleteName = NULL;
        char seleteName[64]={0};
		for (int i = 1; i <= Global::getGlobalInstance()->getTotalLevels(); ++i)
		{
			snprintf(seleteName,64,"select%d.jpg", i);
			Sprite *pSprite = Sprite::create(seleteName);
			pSprite->setPosition(Vec2(visibleSize.width * (i-0.5f), visibleSize.height/2 + 10));
			pLayer->addChild(pSprite, 0, i);
		}

		pLayer->setContentSize(Size(480*8, 320));
		this->addChild(pLayer, 1);

		Sprite *pBg = Sprite::create("bg.png");
		pBg->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2));
		this->addChild(pBg, 0);

		pNewGame = MenuItemImage::create("newgameA.png", "newgameB.png",
			CC_CALLBACK_1(CCSelectMenu::menuBegin,this));
		pNewGame->setPosition(Vec2(origin.x + visibleSize.width/2, 30));

		MenuItemImage *pBack = MenuItemImage::create("backA.png", "backB.png",
            CC_CALLBACK_1(CCSelectMenu::menuBack,this));
		pBack->setPosition(Vec2(origin.x + 20, origin.y + visibleSize.height - 20));

		Menu *pMenu = Menu::create(pNewGame, pBack, NULL);
		pMenu->setAnchorPoint(Vec2::ZERO);
		pMenu->setPosition(Vec2::ZERO);
		this->addChild(pMenu, 2);

		m_pLevel = Label::createWithSystemFont("Level: 1", "Arial", 20);
		m_pLevel->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height - 20));
		this->addChild(m_pLevel, 3);

		bRet = true;
	}while(0);

	return bRet;

}

void CCSelectMenu::menuCloseCallback(Ref* pSender)
{

}

void CCSelectMenu::onEnter()
{
	Layer::onEnter();
	//以后更改
	//在那个类中实现触屏回调函数
	//Director::getInstance()->getTouchDispatcher()->addTargetedDelegate(this, 1, false);

	//CCString *ccStr = NULL;
    char ccStr[256]={0};
	string str;
	Sprite *pSp = NULL;
	for (int i = 2; i <= 8; ++i)
	{
        snprintf(ccStr, 256, "Level%d", i);
		//ccStr = CCString::createWithFormat("Level%d", i);
		str = UserDefault::getInstance()->getStringForKey(ccStr);
		if (str == "no")
		{
			LayerColor *pColor = LayerColor::create(Color4B(0, 0, 0, 200));
			pSp = (Sprite*)pLayer->getChildByTag(i);
			pColor->isIgnoreAnchorPointForPosition();
			pColor->setAnchorPoint(Vec2(0.5, 0.5));
			pColor->setPosition(pSp->getPosition());
			pColor->setContentSize(pSp->getContentSize());
			pLayer->addChild(pColor, pSp->getLocalZOrder() + 1);
		}
	}
}

void CCSelectMenu::onExit()
{
	//以后更改
	//Director::getInstance()->getTouchDispatcher()->removeDelegate(this);
	Layer::onExit();
}


bool CCSelectMenu::onTouchBegan(Touch *pTouch, Event *pEvent)
{
	m_touchPoint = Director::getInstance()->convertToGL(pTouch->getLocation());
	return true;
}

void CCSelectMenu::onTouchMoved(Touch *pTouch, Event *pEvent)
{

}

void CCSelectMenu::onTouchEnded(Touch *pTouch, Event *pEvent)
{
	Point endPoint = Director::getInstance()->convertToGL(pTouch->getLocation());
	float distance = endPoint.x - m_touchPoint.x;
	if(fabs(distance) > 5)
	{
		adjustView(distance);
	}
}

void CCSelectMenu::onTouchCancelled(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	Point endPoint = Director::getInstance()->convertToGL(pTouch->getLocation());
	float distance = endPoint.x - m_touchPoint.x;
	if(fabs(distance) > 5)
	{
		adjustView(distance);
	}
}

void CCSelectMenu::adjustView(float offset)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();

	if (offset<0)
	{
		++m_nCurPage;
	}else
	{
		--m_nCurPage;
	}

	if (m_nCurPage <1)
	{
		m_nCurPage = 1;
	}
	if (m_nCurPage > 8)
	{
		m_nCurPage = 8;
	}

	Point  adjustPos = Vec2(origin.x - visibleSize.width * (m_nCurPage-1), 0);
	pLayer->runAction(MoveTo::create(0.2f, adjustPos));

    char ccStr[64]={0};
    snprintf(ccStr, 64, "Level: %d", m_nCurPage);
	//char *ccStr = CCString::createWithFormat("Level: %d", m_nCurPage);
    
	m_pLevel->setString(ccStr);
    memset(ccStr, 0, 64);
    snprintf(ccStr, 64, "Level%d", m_nCurPage);
	//ccStr = CCString::createWithFormat("Level%d", m_nCurPage);
	string str = UserDefault::getInstance()->getStringForKey(ccStr);
	if (str == "no")
	{
		pNewGame->setEnabled(false);
	}else
	{
		pNewGame->setEnabled(true);
	}
	    
}

void CCSelectMenu::menuBegin( Ref *pSender )
{
	Global::getGlobalInstance()->setCurrentLevel(m_nCurPage);
	CCGameLayer *pGameLayer = CCGameLayer::create();

	Scene *pScene = Scene::create();
	pScene->addChild(pGameLayer);
	Director::getInstance()->replaceScene(pScene);
}

void CCSelectMenu::menuBack( Ref *pSender )
{
	MainMenu *pMainMenu = MainMenu::create();
	Scene *pScene = Scene::create();
	pScene->addChild(pMainMenu);
	Director::getInstance()->replaceScene(pScene);
}
