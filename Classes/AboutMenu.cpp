#include "AboutMenu.h"
#include "MainMenu.h"

CCAbout::CCAbout()
{

}

CCAbout::~CCAbout()
{

}

bool CCAbout::init()
{
	Point origin = Director::getInstance()->getVisibleOrigin();
	Size visibleSize = Director::getInstance()->getVisibleSize();

	Sprite *pBg = Sprite::create("bg.png");
	pBg->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2));
	this->addChild(pBg, 0);

	Sprite *pZhy = Sprite::create("zhy.jpg");
	pZhy->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2 + 50));
	this->addChild(pZhy, 2);

	Sprite *pWxb = Sprite::create("wxb.jpg");
	pWxb->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2 - 50));
	this->addChild(pWxb, 2);

	MenuItemImage *pBack = MenuItemImage::create("backA.png", "backB.png",
		CC_CALLBACK_1(CCAbout::menuBack,this));
	pBack->setPosition(Vec2(origin.x + 20, origin.y + visibleSize.height - 20));
	Menu *pMenu = Menu::create(pBack, NULL);
	pMenu->setAnchorPoint(Vec2::ZERO);
	pMenu->setPosition(Vec2::ZERO);
	this->addChild(pMenu, 1);

	return true;
}

void CCAbout::menuBack( Ref *pSender )
{
	MainMenu *pMainMenu = MainMenu::create();
	Scene *pScene = Scene::create();
	pScene->addChild(pMainMenu);
	Director::getInstance()->replaceScene(pScene);
}
