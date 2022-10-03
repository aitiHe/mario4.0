#include "GameLayer.h"
#include "GameMap.h"
#include "Hero.h"
#include "AnimationManager.h"
#include "Item.h"
#include "MainMenu.h"
#include "Global.h"
#include "AudioEngine.h"
#include "SelectMenu.h"

float CCGameLayer::mapMaxH;

CCGameLayer::CCGameLayer():
mainMap(NULL),
hero(NULL)
{
	mapMaxH = 0.0f;
	mapBeginPos = Vec2(0.0f, 96.0f);

	birthPoint.x = 180.0f;
	birthPoint.y = 32.0f;

	isKeyDownA = false;
	isKeyDownD = false;
	isSky = false;
	heroFireable = true;

	moveOffset = 0.0f;
	moveDelta = 0.0f;
	JumpOffset = 0.0f;

	ccMoveDelta = 0.05f;
	ccMoveOffset = 2.0f;
	ccJumpOffset = 0.3f;

	currentPos.x = 0.0f;
	currentPos.y = 0.0f;

	Size winsize = Director::getInstance()->getWinSize();
	heroAnchor = Vec2(winsize.width/2 - 80, winsize.height/2);

	backKeyPos = Vec2(84, 48);
	leftKeyPos = Vec2(40, 48);
	rightKeyPos = Vec2(128, 48);
	jumpKeyPos = Vec2(432, 35);
	fireKeyPos = Vec2(353, 35);
	MSetKeyPos = Vec2(260, 33);

	FireBallPos = Vec2(winsize.width - 70, winsize.height - 20);
	arrowPos = Vec2(winsize.width - 30, winsize.height - 20);

	mainLayer = Layer::create();
	mainLayer->retain();

	isPass = false;

	isLeftKeyDown = false;
	isRightKeyDown = false;
	isJumpKeyDown = false;
	isFireKeyDown = false;
}

CCGameLayer::~CCGameLayer()
{
	//Director::getInstance()->getOpenGLView()->SetWin32KeyLayer(NULL);

	// ������ļ���ָ��ȫ������ͨ��create���ɵģ��Ѿ�autorelease��������������������ֶ�ɾ��
	//CC_SAFE_DELETE(hero);
	//CC_SAFE_DELETE(mainMap);
	//CC_SAFE_DELETE(mainLayer);

	this->unscheduleUpdate();
	this->unscheduleAllCallbacks();
}

bool CCGameLayer::init()
{
	//CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("OnLand.ogg", true);

	this->initHeroAndMap();

	this->initcontrolUI();

	this->initBulletUI();

	this->initRect();

	this->initSetMenu();

	//Director::getInstance()->getOpenGLView()->SetWin32KeyLayer(this);
	//this->registerWithTouchDispatcher();

	//������
	//Director::getInstance()->getTouchDispatcher()->addStandardDelegate(this, kCCMenuHandlerPriority);
	auto touchMore = EventListenerTouchAllAtOnce::create();
	touchMore->onTouchesBegan = CC_CALLBACK_2(CCGameLayer::onTouchesBegan, this);
	touchMore->onTouchesMoved = CC_CALLBACK_2(CCGameLayer::onTouchesMoved, this);
	touchMore->onTouchesEnded = CC_CALLBACK_2(CCGameLayer::onTouchesEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchMore, this);

	//�����¼�
	auto keyboardListen = EventListenerKeyboard::create();
	keyboardListen->onKeyPressed = CC_CALLBACK_2(CCGameLayer::onKeyPressed, this);
	keyboardListen->onKeyReleased = CC_CALLBACK_2(CCGameLayer::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListen, this);

	//this->setTouchEnabled(true);
	this->scheduleUpdate();

	return true;
}

void CCGameLayer::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	switch (keyCode)
	{
	case cocos2d::EventKeyboard::KeyCode::KEY_A:
		isLeftKeyDown = true;
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_D:
		isRightKeyDown = true;
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_J:
		isJumpKeyDown = true;
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_K:
		isFireKeyDown = true;
		break;
	default:
		break;
	}
}

void CCGameLayer::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	switch (keyCode)
	{
	case cocos2d::EventKeyboard::KeyCode::KEY_A:
		isLeftKeyDown = false;
		isKeyDownA = false;
		moveOffset = 0.0f;
		moveDelta = 0.0f;
		hero->setHeroState(eNormalLeft);
		pBackKeyImage->setSpriteFrame(pBackKeyNormal);
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_D:
		isRightKeyDown = false;
		isKeyDownD = false;
		moveOffset = 0.0f;
		moveDelta = 0.0f;
		hero->setHeroState(eNormalRight);
		pBackKeyImage->setSpriteFrame(pBackKeyNormal);
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_J:
		isJumpKeyDown = false;
		pJumpImage->setSpriteFrame(pAB_Normal);
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_K:
		isFireKeyDown = false;
		pFireImage->setSpriteFrame(pAB_Normal);
		break;
	default:
		break;
	}
}

void CCGameLayer::initSetMenu()
{
	pColor = LayerColor::create(Color4B(0, 0, 0, 100));
	this->addChild(pColor, this->getChildrenCount());
	pColor->setVisible(false);

	pSetMenu = Sprite::create("Set_Menu.png");
	pSetMenu->setAnchorPoint(Vec2(129.0/248, 71.0/132));
	Point origin = Director::getInstance()->getVisibleOrigin();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	pSetMenu->setPosition(Vec2(origin.x+visibleSize.width/2, origin.y+visibleSize.height/2));
	this->addChild(pSetMenu, this->getChildrenCount()+1);
	pSetMenu->setVisible(false);

	pResume = MenuItemImage::create("resume_n.png", "resume_s.png",
		 CC_CALLBACK_1(CCGameLayer::menuResume,this));
	pResume->setPosition(Vec2(origin.x+visibleSize.width/2, origin.y+visibleSize.height/2+40));
	pResume->setVisible(false);
	pResume->setEnabled(false);

	pReStart = MenuItemImage::create("restart_n.png", "restart_s.png",
		CC_CALLBACK_1(CCGameLayer::menuReStart,this));
	pReStart->setPosition(Vec2(origin.x+visibleSize.width/2, origin.y+visibleSize.height/2));
	pReStart->setVisible(false);
	pReStart->setEnabled(false);

	pSelectMenu = MenuItemImage::create("select_n.png", "select_s.png",
		CC_CALLBACK_1(CCGameLayer::menuSelectMenu,this));
	pSelectMenu->setPosition(Vec2(origin.x+visibleSize.width/2, origin.y+visibleSize.height/2-40));
	pSelectMenu->setVisible(false);
	pSelectMenu->setEnabled(false);

	pMenu->addChild(pResume, pMenu->getChildrenCount());
	pMenu->addChild(pReStart, pMenu->getChildrenCount());
	pMenu->addChild(pSelectMenu, pMenu->getChildrenCount());
}

void CCGameLayer::menuMSet( Ref *pSender )
{
	this->pauseGameLayer();
}

void CCGameLayer::menuResume( Ref *pSender )
{
	this->resumeGameLayer();
}

void CCGameLayer::menuReStart( Ref *pSender )
{
	Scene *pScene = Scene::create();
	pScene->addChild(CCGameLayer::create());
	Director::getInstance()->replaceScene(pScene);
}

void CCGameLayer::menuSelectMenu( Ref *pSender )
{
	Scene *pScene = Scene::create();
	pScene->addChild(CCSelectMenu::create());
	Director::getInstance()->replaceScene(pScene);
}

void CCGameLayer::initRect()
{
	leftKeyRect = Rect(leftKeyPos.x - pLeftKey->getContentSize().width/2,
		leftKeyPos.y - pLeftKey->getContentSize().height/2,
		pLeftKey->getContentSize().width,
		pLeftKey->getContentSize().height);

	rightKeyRect = Rect(rightKeyPos.x - pRightKey->getContentSize().width/2,
		rightKeyPos.y - pRightKey->getContentSize().height/2,
		pRightKey->getContentSize().width,
		pRightKey->getContentSize().height);

	jumpKeyRect = Rect(jumpKeyPos.x - pJump->getContentSize().width/2,
		jumpKeyPos.y - pJump->getContentSize().height/2,
		pJump->getContentSize().width,
		pJump->getContentSize().height);

	fireKeyRect = Rect(fireKeyPos.x - pFire->getContentSize().width/2,
		fireKeyPos.y - pFire->getContentSize().height/2,
		pFire->getContentSize().width,
		pFire->getContentSize().height);
}

void CCGameLayer::initHeroAndMap()
{
	char temp[20] = { 0 };
	sprintf(temp, "MarioMap%d.tmx", Global::getGlobalInstance()->getCurrentLevel());
	mainMap = CCGameMap::create(temp);

	mapSize = Size(mainMap->getMapSize().width * mainMap->getTileSize().width, 
		mainMap->getMapSize().height * mainMap->getTileSize().height);
	mainMap->setPosition(Vec2(0, 0));
	mainLayer->addChild(mainMap);
	hero = Hero::create();
	hero->setBodyType(Global::getGlobalInstance()->currentHeroType);
	hero->setAnchorPoint(Vec2(0.5f, 0.0f));
	hero->setPosition(mainMap->getMarioBirthPos());
	heroSize = hero->getCurrentSize();
	mainLayer->addChild(hero);
	mainLayer->setPosition(mapBeginPos);
	this->addChild(mainLayer);
}

void CCGameLayer::loadMap()
{

}

void CCGameLayer::initcontrolUI()
{
	controlUI = Sprite::create("controlUI.png");
	controlUI->setAnchorPoint(Vec2(0, 0));
	this->addChild(controlUI);

	Size winSize = Director::getInstance()->getWinSize();

	pGameOverBack = Sprite::create("gameover.png");
	pGameOverBack->setPosition(Vec2(winSize.width/2, winSize.height/2 + 50));
	this->addChild(pGameOverBack);
	pGameOverBack->setVisible(false);

	pPassFailure = Sprite::create("PassFailure.png");
	pPassFailure->setPosition(Vec2(winSize.width/2, winSize.height/2 + 50));
	this->addChild(pPassFailure);
	pPassFailure->setVisible(false);

	pPassSuccess = Sprite::create("PassSuccess.png");
	pPassSuccess->setPosition(Vec2(winSize.width/2, winSize.height/2 + 50));
	this->addChild(pPassSuccess);
	pPassSuccess->setVisible(false);


	winSize = Director::getInstance()->getWinSize();

	pBackKeyImage = Sprite::create("backKeyImage.png");
	pBackKeyImage->setPosition(backKeyPos);
	this->addChild(pBackKeyImage);

	pBackKeyNormal = SpriteFrame::create("backKeyImage.png", Rect(0, 0, 72, 72));
	pBackKeyNormal->retain();
	pBackKeyLeft = SpriteFrame::create("backKeyLeft.png", Rect(0, 0, 72, 72));
	pBackKeyLeft->retain();
	pBackKeyRight = SpriteFrame::create("backKeyRight.png", Rect(0, 0, 72, 72));
	pBackKeyRight->retain();

	pAB_Normal = SpriteFrame::create("AB_normal.png", Rect(0, 0, 72, 50));
	pAB_Normal->retain();
	pAB_Selected = SpriteFrame::create("AB_select.png", Rect(0, 0, 72, 50));
	pAB_Selected->retain();

	pJumpImage = Sprite::createWithSpriteFrame(pAB_Normal);
	pJumpImage->setPosition(jumpKeyPos);
	this->addChild(pJumpImage, 3);

	pFireImage = Sprite::createWithSpriteFrame(pAB_Normal);
	pFireImage->setPosition(fireKeyPos);
	this->addChild(pFireImage, 3);

	pLeftKey = MenuItemImage::create("leftright.png", "leftright.png",
                                     [&](Ref* sender){
        isKeyDownA = false;
        moveOffset = 0.0f;
        moveDelta = 0.0f;
        hero->setHeroState(eNormalLeft);
        pBackKeyImage->setSpriteFrame(pBackKeyNormal);
    });
	pRightKey = MenuItemImage::create("leftright.png", "leftright.png",
		 CC_CALLBACK_1(CCGameLayer::menuCallBackRightKey,this));
	pJump = MenuItemImage::create("AB_normal.png", "AB_select.png",
		CC_CALLBACK_1(CCGameLayer::menuCallBackJumpKey,this));
	pFire = MenuItemImage::create("AB_normal.png", "AB_select.png",
		CC_CALLBACK_1(CCGameLayer::menuCallBackFireKey,this));
	pMSet = MenuItemImage::create("M_n.png", "M_s.png",
		CC_CALLBACK_1(CCGameLayer::menuMSet,this));

	pBackToMenu = MenuItemImage::create("backToMenu.png", "backToMenu.png",
		 CC_CALLBACK_1(CCGameLayer::menuCallBackBackToMenu,this));
	pBackToMenu->setEnabled(false);
	pBackToMenu->setVisible(false);

	pNext = MenuItemImage::create("nextlevel_normal.png", "nextlevel_select.png",
		CC_CALLBACK_1(CCGameLayer::menuNext,this));
	pNext->setPosition(Vec2(winSize.width/2, winSize.height/2 + 40));
	pNext->setVisible(false);
	pNext->setEnabled(false);

	pRetry = MenuItemImage::create("retry_normal.png", "retry_select.png",
		CC_CALLBACK_1(CCGameLayer::menuRetry,this));
	pRetry->setPosition(Vec2(winSize.width/2, winSize.height/2));
	pRetry->setVisible(false);
	pRetry->setEnabled(false);

	pLeftKey->setPosition(leftKeyPos);
	pRightKey->setPosition(rightKeyPos);
	pJump->setPosition(jumpKeyPos);
	pFire->setPosition(fireKeyPos);
	pMSet->setPosition(MSetKeyPos);
	pBackToMenu->setPosition(Vec2(winSize.width/2, winSize.height/2 + 20));

	pMenu = Menu::create(pNext, pRetry, pMSet, NULL);
	pMenu->setAnchorPoint(Vec2::ZERO);
	pMenu->setPosition(Vec2(0, 0));

	this->addChild(pMenu, 100);
}

void CCGameLayer::initBulletUI()
{
	pBulletBorderArrow = Sprite::create("bulletBorder.png");
	pBulletBorderArrow->setPosition(arrowPos);
	//pBulletBorderArrow->setScale(2.0f);

	pBulletBorderFireBall = Sprite::create("bulletBorder.png");
	pBulletBorderFireBall->setPosition(FireBallPos);

	Sprite *pArrow = Sprite::create("arrowBullet.png");
	
	pMenuArrow = MenuItemSprite::create(pArrow, pArrow,pArrow,
		 CC_CALLBACK_1(CCGameLayer::menuCallBackArrow,this));
	pMenuArrow->setPosition(arrowPos);
	pMenu->addChild(pMenuArrow);

	Sprite *pFireBall = Sprite::create("fireBall.png");
	//pFireBall->setScale(2.0f);
	pMenuFireBall = MenuItemSprite::create(pFireBall, pFireBall,pFireBall,
		 CC_CALLBACK_1(CCGameLayer::menuCallBackFireBall,this));
	pMenuFireBall->setPosition(FireBallPos);
	pMenu->addChild(pMenuFireBall);
}

void CCGameLayer::menuCallBackLeftKey(Ref *pSender)
{
	isKeyDownA = false;
	moveOffset = 0.0f;
	moveDelta = 0.0f;
	hero->setHeroState(eNormalLeft);
	pBackKeyImage->setSpriteFrame(pBackKeyNormal);
}

void CCGameLayer::menuCallBackRightKey(Ref *pSender)
{
	isKeyDownD = false;
	moveOffset = 0.0f;
	moveDelta = 0.0f;
	hero->setHeroState(eNormalRight);
	pBackKeyImage->setSpriteFrame(pBackKeyNormal);
}

void CCGameLayer::menuCallBackJumpKey(Ref *pSender)
{
	
}

void CCGameLayer::menuCallBackFireKey(Ref *pSender)
{

}

void CCGameLayer::menuCallBackArrow(Ref *pSender)
{
	if (hero->isBulletable())
	{
		Global::getGlobalInstance()->setCurrentBulletType(eBullet_arrow);
		hero->setBulletType(eBullet_arrow);
	}
}

void CCGameLayer::menuCallBackFireBall(Ref *pSender)
{
	if (hero->isBulletable())
	{
		Global::getGlobalInstance()->setCurrentBulletType(eBullet_common);
		hero->setBulletType(eBullet_common);
	}
}

void CCGameLayer::menuCallBackBackToMenu(Ref *pSender)
{
	this->toMainMenu();
}

void CCGameLayer::onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event)
{
	//for (CCSetIterator it = pTouches->begin(); it != pTouches->end(); ++it)
	//{
	//	Touch *pTouch = (Touch*)(*it);
	//	Point touchPos = pTouch->getLocation();
	//	if (leftKeyRect.containsPoint(touchPos))
	//	{
	//		isLeftKeyDown = true;
	//	}
	//	if (rightKeyRect.containsPoint(touchPos))
	//	{
	//		isRightKeyDown = true;
	//	}
	//	if (jumpKeyRect.containsPoint(touchPos))
	//	{
	//		isJumpKeyDown = true;
	//	}
	//	if (fireKeyRect.containsPoint(touchPos))
	//	{
	//		isFireKeyDown = true;
	//	}
	//}
}

void CCGameLayer::onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event)
{
	//for (CCSetIterator it = pTouches->begin(); it != pTouches->end(); ++it)
	//{
	//	Touch *pTouch = (Touch*)(*it);
	//	Point touchPos = pTouch->getLocation();
	//	if (leftKeyRect.containsPoint(touchPos))
	//	{
	//		isLeftKeyDown = false;
	//		isKeyDownA = false;
	//		moveOffset = 0.0f;
	//		moveDelta = 0.0f;
	//		hero->setHeroState(eNormalLeft);
	//		pBackKeyImage->setSpriteFrame(pBackKeyNormal);
	//	}
	//	if (rightKeyRect.containsPoint(touchPos))
	//	{
	//		isRightKeyDown = false;
	//		isKeyDownD = false;
	//		moveOffset = 0.0f;
	//		moveDelta = 0.0f;
	//		hero->setHeroState(eNormalRight);
	//		pBackKeyImage->setSpriteFrame(pBackKeyNormal);
	//	}
	//	if (jumpKeyRect.containsPoint(touchPos))
	//	{
	//		isJumpKeyDown = false;
	//		pJumpImage->setSpriteFrame(pAB_Normal);
	//	}
	//	if (fireKeyRect.containsPoint(touchPos))
	//	{
	//		isFireKeyDown = false;
	//		pFireImage->setSpriteFrame(pAB_Normal);
	//	}
	//}
}

void CCGameLayer::onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event)
{
	//if (isLeftKeyDown)
	//{
	//	bool flag = false;
	//	for (CCSetIterator it = pTouches->begin(); it != pTouches->end(); ++it)
	//	{
	//		Touch *pTouch = (Touch*)(*it);
	//		Point touchPos = pTouch->getLocation();

	//		if (leftKeyRect.containsPoint(touchPos))
	//		{
	//			flag = true;
	//		}
	//	}
	//	if (flag == false)
	//	{
	//		isLeftKeyDown = false;
	//		isKeyDownA = false;
	//		moveOffset = 0.0f;
	//		moveDelta = 0.0f;
	//		hero->setHeroState(eNormalLeft);
	//		pBackKeyImage->setSpriteFrame(pBackKeyNormal);
	//	}
	//}

	//if (isRightKeyDown)
	//{
	//	bool flag = false;
	//	for (CCSetIterator it = pTouches->begin(); it != pTouches->end(); ++it)
	//	{
	//		Touch *pTouch = (Touch*)(*it);
	//		Point touchPos = pTouch->getLocation();

	//		if (rightKeyRect.containsPoint(touchPos))
	//		{
	//			flag = true;
	//		}
	//	}
	//	if (flag == false)
	//	{
	//		isRightKeyDown = false;
	//		isKeyDownD = false;
	//		moveOffset = 0.0f;
	//		moveDelta = 0.0f;
	//		hero->setHeroState(eNormalRight);
	//		pBackKeyImage->setSpriteFrame(pBackKeyNormal);
	//	}
	//}

	//if (isJumpKeyDown)
	//{
	//	bool flag = false;
	//	for (CCSetIterator it = pTouches->begin(); it != pTouches->end(); ++it)
	//	{
	//		Touch *pTouch = (Touch*)(*it);
	//		Point touchPos = pTouch->getLocation();

	//		if (jumpKeyRect.containsPoint(touchPos))
	//		{
	//			flag = true;
	//		}
	//	}
	//	if (flag == false)
	//	{
	//		isJumpKeyDown = false;
	//		pJumpImage->setSpriteFrame(pAB_Normal);
	//	}
	//}

	//if (isFireKeyDown)
	//{
	//	bool flag = false;
	//	for (CCSetIterator it = pTouches->begin(); it != pTouches->end(); ++it)
	//	{
	//		Touch *pTouch = (Touch*)(*it);
	//		Point touchPos = pTouch->getLocation();

	//		if (fireKeyRect.containsPoint(touchPos))
	//		{
	//			flag = true;
	//		}
	//	}
	//	if (flag == false)
	//	{
	//		isFireKeyDown = false;
	//		pFireImage->setSpriteFrame(pAB_Normal);
	//	}
	//}
}

void CCGameLayer::onTouchesCancelled(const std::vector<Touch*>&touches, Event *unused_event)
{

}

//void CCGameLayer::registerWithTouchDispatcher()
//{
//	Layer::registerWithTouchDispatcher();
//	Director::getInstance()->getTouchDispatcher()->addStandardDelegate(this, 0);
//}

void CCGameLayer::menuCloseCallBack(Ref *pSender)
{
	this->toMainMenu();
}

//void CCGameLayer::processWin32KeyPress(UINT message, WPARAM wParam, LPARAM lParam)
//{
//	if (hero->isHeroDied())
//	{
//		return ;
//	}
//	switch (message)
//	{
//	case WM_KEYDOWN:
//		if (wParam == 65)
//		{
//			isKeyDownA = true;
//			moveOffset = -ccMoveOffset;
//			moveDelta = -ccMoveDelta;
//			hero->setHeroState(eLeft);
//		}
//		else if (wParam == 68)
//		{
//			isKeyDownD = true;
//			moveOffset = ccMoveOffset;
//			moveDelta = ccMoveDelta;
//			hero->setHeroState(eRight);
//		}
//		else if (wParam == 32)
//		{
//			hero->gadgetable = false;
//			if (!isSky)
//			{
//				CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("Jump.wma");
//				JumpOffset = 6.0f;
//				isSky = true;
//				hero->isFlying = true;
//			}
//		}
//		else if (wParam == 73)
//		{
//			// �����ӵ�
//			if (hero->isBulletable())
//			{
//				if (heroFireable)
//				{
//					mainMap->createNewBullet();
//					hero->fireAction();
//					heroFireable = false;
//					DelayTime *pDelay = DelayTime::create(0.5f);
//					this->runAction(Sequence::create(pDelay, 
//						CallFunc::create(this, callfunc_selector(CCGameLayer::reSetHeroFireable)), NULL));
//
//				}
//			}
//		}
//		break;
//	case WM_KEYUP:
//		if (wParam == 65)
//		{
//			isKeyDownA = false;
//			moveOffset = 0.0f;
//			moveDelta = 0.0f;
//			hero->setHeroState(eNormalLeft);
//		}
//		else if (wParam == 68)
//		{
//			isKeyDownD = false;
//			moveOffset = 0.0f;
//			moveDelta = 0.0f;
//			hero->setHeroState(eNormalRight);
//		}
//		break;
//	}
//}




void CCGameLayer::updateControl()
{
	if (!hero->isHeroDied())
	{
		if (/*pLeftKey->isSelected()*/ isLeftKeyDown )
		{
			isKeyDownA = true;
			moveOffset = -ccMoveOffset;
			moveDelta = -ccMoveDelta;
			hero->setHeroState(eLeft);
			pBackKeyImage->setSpriteFrame(pBackKeyLeft);
		}
		else if (/*pRightKey->isSelected()*/ isRightKeyDown )
		{
			isKeyDownD = true;
			moveOffset = ccMoveOffset;
			moveDelta = ccMoveDelta;
			hero->setHeroState(eRight);
			pBackKeyImage->setSpriteFrame(pBackKeyRight);
		}
		if (/*pJump->isSelected()*/ isJumpKeyDown )
		{
			hero->gadgetable = false;
			if (!isSky)
			{
				//CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("Jump.ogg");
				JumpOffset = 6.0f;
				isSky = true;
				hero->isFlying = true;				
			}
			pJumpImage->setSpriteFrame(pAB_Selected);
		}
		if (/*pFire->isSelected()*/ isFireKeyDown )
		{
			// �����ӵ�
			if (hero->isBulletable())
			{
				if (heroFireable)
				{
					//CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("RengHuoQiu.ogg");
					mainMap->createNewBullet();
					hero->fireAction();
					heroFireable = false;
					DelayTime *pDelay = DelayTime::create(0.5f);
					this->runAction(Sequence::create(pDelay, 
						CallFunc::create(CC_CALLBACK_0(CCGameLayer::reSetHeroFireable,this)), NULL));

				}
			}
			pFireImage->setSpriteFrame(pAB_Selected);
		}
	}

}

void CCGameLayer::toMainMenu()
{
	Scene *pScene = Scene::create();
	pScene->addChild(MainMenu::create());
	Director::getInstance()->replaceScene(pScene);
}

void CCGameLayer::stopForPassFailure()
{
	if (Global::getGlobalInstance()->getCurrentLifeNum() == 0)
	{
		//CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic(true);
		//CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("GameOver.ogg");
		//Global::getGlobalInstance()->setWhyToMainMenu(efor_GameOver);
		pGameOverBack->setVisible(true);
	}
	else
	{
		//Global::getGlobalInstance()->setWhyToMainMenu(efor_PassFailure);
		pPassFailure->setVisible(true);
	}

	mainMap->stopUpdateForHeroDie();
	this->unscheduleUpdate();
	this->reSetKeyNo();

	DelayTime *pDelay = DelayTime::create(3);
	this->runAction(Sequence::create(pDelay,
		CallFunc::create( CC_CALLBACK_0(CCGameLayer::reShowPassFailure,this)), NULL));
}

void CCGameLayer::reSetKeyNo()
{
	//pFire->setEnabled(false);
	//pJump->setEnabled(false);
	pMenuFireBall->setEnabled(false);
	pMenuArrow->setEnabled(false);
}

void CCGameLayer::reShowPassFailure()
{	
	if (Global::getGlobalInstance()->getCurrentLifeNum() == 0)
	{
		this->toMainMenu();
	}
	else
	{
		pRetry->setVisible(true);
		pRetry->setEnabled(true);
	}
	//pBackToMenu->setEnabled(true);
	//pBackToMenu->setVisible(true);
}

void CCGameLayer::stopForPassSuccess()
{
	int level = Global::getGlobalInstance()->getCurrentLevel();
    char str[64]={0};
    snprintf(str, 64, "Level%d", level + 1);
	//CCString *str = CCString::createWithFormat("Level%d", level + 1);
	UserDefault::getInstance()->setStringForKey(str, "yes");

	
	mainMap->stopUpdateForHeroDie();
	this->unscheduleUpdate();

	DelayTime *pDelay = DelayTime::create(3);
	this->runAction(Sequence::create(pDelay,
		CallFunc::create(CC_CALLBACK_0(CCGameLayer::reShowPassSuccess,this)), NULL));

}

void CCGameLayer::reShowPassSuccess()
{
	pPassSuccess->setVisible(true);
	//pBackToMenu->setEnabled(true);
	//pBackToMenu->setVisible(true);

	
	if (Global::getGlobalInstance()->getCurrentLevel() == Global::getGlobalInstance()->getTotalLevels())
	{
		this->showPassAll();
	}else
	{
		pNext->setVisible(true);
		pNext->setEnabled(true);
	}


}

void CCGameLayer::showHeroJump()
{
	hero->reSetForSuccess();
	JumpTo *pJump = JumpTo::create(1.0f, mainMap->finalPoint, 32, 3);
	hero->runAction(pJump);
}

void CCGameLayer::update(float dt)
{
	// �Ƿ������ж�
	if (hero->isHeroDied())
	{
		this->stopForPassFailure();
		return ;
	}
	// �Ƿ�ͨ���ж�
	if (isPass)
	{
		this->stopForPassSuccess();
		return ;
	}

	this->updateControl();

	currentPos = hero->getPosition();
	heroSize = hero->getContentSize();

	if (hero->isGadgetable())
	{
		currentPos.x += moveOffset + mainMap->heroInGadget->getMoveOffset();
		currentPos.y += JumpOffset + mainMap->heroInGadget->getJumpOffset();
	}
	else
	{
		currentPos.x += moveOffset;
		currentPos.y += JumpOffset;
	}
	if (isSky)
	{
		switch (hero->face)
		{
		case eLeft:
			hero->setHeroState(eJumpLeft);
			break;
		case eRight:
			hero->setHeroState(eJumpRight);
			break;
		default:
			break;
		}
	}
   // currentPos.x+=2;
    
	hero->setPosition(currentPos);
	setSceneScrollPosition();
	collistionV();
	collistionH();
}

void CCGameLayer::updateContent(float dt)
{
	currentPos = hero->getPosition();
	currentPos.x = currentPos.x + moveOffset + moveDelta;
	hero->setPosition(currentPos);
	setSceneScrollPosition();
	collistionH();
}

void CCGameLayer::updateDelta(float dt)
{
	//if (isKeyDownA)
	//{
	//	moveDelta -= ccMoveDelta;
	//}
	//else if (isKeyDownD)
	//{
	//	moveDelta += ccMoveDelta;
	//}
}

// ˮƽ������ײ���
// �����ڰ���д�Լһ���ӵ�bugӦ����ˮƽ��ײ�����⣬�ѽ��
void CCGameLayer::collistionH()
{
	Point currentPos = hero->getPosition();
	// ���жϲ��������Ƴ���Ļ�����
	if ( (currentPos.x - heroSize.width/2 - mapMaxH) <= 0 )
	{
		Point pp = Vec2(mapMaxH + heroSize.width/2, currentPos.y);
		hero->setPosition(pp);
		return ;
	}

	bool flag = false;
	// �Ҳ��ж�
	Point rightCollision = Vec2(currentPos.x + heroSize.width/2, currentPos.y /* + heroSize.height/2*/);
	Point rightTileCoord = mainMap->positionToTileCoord(rightCollision);
	if (mainMap->isMarioEatMushroom(rightTileCoord))
	{
		hero->changeForGotMushroom();
	}
	if (mainMap->isMarioEatAddLifeMushroom(rightTileCoord))
	{
		hero->changeForGotAddLifeMushroom();
	}
	Point rightPos = mainMap->tilecoordToPosition(rightTileCoord);
	rightPos = Vec2(rightPos.x - heroSize.width/2, currentPos.y);

	TileType tileType = mainMap->tileTypeforPos(rightTileCoord);
	switch (tileType)
	{
	case eTile_Block:
	case eTile_Pipe:
	case eTile_Land:
		hero->setPosition(rightPos);
		flag = true;
		break;
	case eTile_Flagpole:
		{
			isPass = true;
			mainMap->showFlagMove();
			this->showHeroJump();
			return ;
			break;
		}
	case eTile_Coin:
		//CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("EatCoin.ogg");
		mainMap->getCoinLayer()->removeTileAt(rightTileCoord);
		break;
	default:
		break;
	}

	// ���ǵ������ײ����
	Point leftCollision = Vec2(currentPos.x - heroSize.width/2, currentPos.y /* + heroSize.height/2*/);
	Point leftTileCoord = mainMap->positionToTileCoord(leftCollision);
	if (mainMap->isMarioEatMushroom(leftTileCoord))
	{
		hero->changeForGotMushroom();
	}
	if (mainMap->isMarioEatAddLifeMushroom(leftTileCoord))
	{
		hero->changeForGotAddLifeMushroom();
	}
	Point leftPos = mainMap->tilecoordToPosition(leftTileCoord);
	leftPos = Vec2(leftPos.x + heroSize.width/2 + mainMap->getTileSize().width, currentPos.y);
	tileType = mainMap->tileTypeforPos(leftTileCoord);
	switch (tileType)
	{
	case eTile_Block:
	case eTile_Pipe:
	case eTile_Land:
		hero->setPosition(leftPos);
		flag = true;
		break;
	case eTile_Coin:
		//CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("EatCoin.ogg");
		mainMap->getCoinLayer()->removeTileAt(leftTileCoord);
		break;
	case eTile_Flagpole:
		{
			isPass = true;
			mainMap->showFlagMove();
			this->showHeroJump();
			return ;
			break;
		}
	default:
		break;
	}
}

// ��ֱ������ײ���
void CCGameLayer::collistionV()
{
	Point currentPos = hero->getPosition();

	// �������ж�����µĽŵ��Ƿ��Ѿ��ӽ���ͼ�±�Ե��5���ش����Դ�Ϊdie
	if (currentPos.y <= 0)
	{
		hero->setHeroDie(true);
		hero->setPosition(Vec2(currentPos.x, 1));
		hero->dieForTrap();  // ִ��һ�ζ�������ʾ������������
		//CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("DiaoRuXianJingSi.ogg");
		return ;
	}

	// �������ж�������Ƿ�ͷ��Խ����Ϸ�������ϲ�
	if (currentPos.y > mapSize.height - heroSize.height - 2)
	{
		JumpOffset = 0.0f;
		hero->setPosition(Vec2(currentPos.x, mapSize.height - heroSize.height - 2));
		isSky = false;

		return ;
	}

	// ����ͷ������ײ���
	for (int heroIdx = 6; heroIdx <= heroSize.width - 6; ++heroIdx)
	{
		Point upCollision = Vec2(currentPos.x - heroSize.width/2 + heroIdx, currentPos.y + heroSize.height);
		Point upTileCoord = mainMap->positionToTileCoord(upCollision);
		// �ж��Ƿ�Ե�Ģ��
		if (mainMap->isMarioEatMushroom(upTileCoord))
		{
			hero->changeForGotMushroom();
		}
		if (mainMap->isMarioEatAddLifeMushroom(upTileCoord))
		{
			hero->changeForGotAddLifeMushroom();
		}
		Point upPos = mainMap->tilecoordToPosition(upTileCoord);
		upPos = Vec2(currentPos.x, upPos.y - heroSize.height);
		TileType tileType = mainMap->tileTypeforPos(upTileCoord);
		bool flagUp = false;
		switch (tileType)
		{
		case eTile_Block:
		case eTile_Land:
			if (JumpOffset > 0)
			{
				mainMap->breakBlock(upTileCoord, hero->getCurrentBodyType());
				JumpOffset = 0.0f;
				hero->setPosition(upPos);
				flagUp = true;
			}
			break;
		case eTile_Coin:
			//CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("EatCoin.ogg");
			mainMap->getCoinLayer()->removeTileAt(upTileCoord);
			break;
		default:
			break;
		}
		if (flagUp)
		{
			JumpOffset -= ccJumpOffset;
			return ;
		}
	}

	float heroLeftSide = currentPos.x - heroSize.width/2;

	for (int heroIdx = 4; heroIdx <= heroSize.width - 4; ++heroIdx)
	{
		Point downCollision = Vec2(heroLeftSide + heroIdx, currentPos.y);
		Point downTileCoord = mainMap->positionToTileCoord(downCollision);
		if (mainMap->isMarioEatMushroom(downTileCoord))
		{
			hero->changeForGotMushroom();
		}
		if (mainMap->isMarioEatAddLifeMushroom(downTileCoord))
		{
			hero->changeForGotAddLifeMushroom();
		}
		downTileCoord.y += 1;
		Point downPos = mainMap->tilecoordToPosition(downTileCoord);
		downPos = Vec2(currentPos.x , downPos.y + mainMap->getTileSize().height);
		
		TileType tileType = mainMap->tileTypeforPos(downTileCoord);
		bool flagDown = false;
		switch (tileType)
		{
		case eTile_Flagpole:
			{
				isPass = true;
				mainMap->showFlagMove();
				this->showHeroJump();
				return ;
				break;
			}
		case eTile_Coin:
			//CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("EatCoin.ogg");
			mainMap->getCoinLayer()->removeTileAt(downTileCoord);
			break;
		//case eTile_Trap:
		//	hero->setPosition(Vec2(currentPos.x, 16.0f));
		//	hero->setHeroDie(true);
		//	return ;
		//	break;
		case eTile_Land:
		case eTile_Pipe:
		case eTile_Block:
			{
				if (JumpOffset < 0)
				{
					hero->setGadgetable(false);
					JumpOffset = 0.0f;
					hero->setPosition(downPos);
					isSky = false;
					hero->isFlying = false;
					switch (hero->face)
					{
					case eLeft:
						if (isKeyDownA)
						{
							hero->setHeroState(eLeft);
						}
						else
						{
							hero->setHeroState(eNormalLeft);
						}
						break;
					case eRight:
						if (isKeyDownD)
						{
							hero->setHeroState(eRight);
						}
						else
						{
							hero->setHeroState(eNormalRight);
						}
						break;
					default:
						break;
					}
				}
				flagDown = true;
			}
			break;
		default:
			break;
		}
		if (flagDown) 
		{
			return ;
		}

		// ��������Ƿ�վ��������
		float gadgetLevel = 0.0f;
		if (mainMap->isHeroInGadget(downCollision, gadgetLevel))
		{
			JumpOffset = 0.0f;
			downPos = Vec2(currentPos.x, gadgetLevel);
			hero->setPosition(downPos);
			hero->setGadgetable(true);
			isSky = false;
			hero->isFlying = false;
			switch (hero->face)
			{
			case eLeft:
				if (isKeyDownA)
				{
					hero->setHeroState(eLeft);
				}
				else
				{
					hero->setHeroState(eNormalLeft);
				}
				break;
			case eRight:
				if (isKeyDownD)
				{
					hero->setHeroState(eRight);
				}
				else
				{
					hero->setHeroState(eNormalRight);
				}
				break;
			default:
				break;
			}
			return ;
		}
		else
		{
			hero->setGadgetable(false);
		}
	}

	JumpOffset -= ccJumpOffset;
}

void CCGameLayer::setSceneScrollPosition()
{
	Point pos = hero->getPosition();
	Size winsize = Director::getInstance()->getWinSize();

	float x = MAX(pos.x, heroAnchor.x);
	float y = MAX(pos.y, heroAnchor.y);

	x = MIN(x, mapSize.width - winsize.width/2 - 80);
	y = MIN(y, mapSize.height - winsize.height/2);
	Point actualPosition = Vec2(x, y);

	Point viewPoint = heroAnchor-actualPosition;
	
	// ʹ�õ�ͼ���ܵ��˻�ȥ
	if (fabsf(viewPoint.x) <= mapMaxH)
	{
		return ;
	} 
	else
	{
		mainLayer->setPosition(viewPoint);
		mapMaxH = fabsf(mainLayer->getPosition().x);
	}
}

float CCGameLayer::getMapMaxH()
{
	return mapMaxH;
}

void CCGameLayer::reSetHeroFireable()
{
	heroFireable = true;
}

void CCGameLayer::menuRetry( Ref *pSender )
{
	Scene *pScene = Scene::create();
	pScene->addChild(CCGameLayer::create());
	Director::getInstance()->replaceScene(pScene);
}

void CCGameLayer::menuNext( Ref *pSender )
{
	Global::getGlobalInstance()->currentLevelPlusOne();
	Scene *pScene = Scene::create();
	pScene->addChild(CCGameLayer::create());
	Director::getInstance()->replaceScene(pScene);
}

void CCGameLayer::showPassAll()
{
    Label *pPassAll = Label::createWithSystemFont("You Pass All", "Arial", 40);
	Point origin = Director::getInstance()->getVisibleOrigin();
	Size visibleSize = Director::getInstance()->getVisibleSize();

	pPassAll->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height));
	this->addChild(pPassAll, this->getChildrenCount());
	MoveTo *pTo = MoveTo::create(0.3f, Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2));
	DelayTime *pDelay = DelayTime::create(2.0f);
	pPassAll->runAction(Sequence::create(pTo, pDelay,
		CallFunc::create( CC_CALLBACK_0(CCGameLayer::toMainMenu,this)), NULL));
}

void CCGameLayer::pauseGameLayer()
{
	mainMap->pauseGameMap();
	this->unscheduleUpdate();

	//pLeftKey->setEnabled(false);
	//pRightKey->setEnabled(false);
	//pJump->setEnabled(false);
	//pFire->setEnabled(false);
	pMenuArrow->setEnabled(false);
	pMenuFireBall->setEnabled(false);

	pColor->setVisible(true);
	pSetMenu->setVisible(true);
	pResume->setVisible(true);
	pResume->setEnabled(true);
	pReStart->setVisible(true);
	pReStart->setEnabled(true);
	pSelectMenu->setVisible(true);
	pSelectMenu->setEnabled(true);
}

void CCGameLayer::resumeGameLayer()
{
	mainMap->resumeGameMap();
	this->scheduleUpdate();

	//pLeftKey->setEnabled(true);
	//pRightKey->setEnabled(true);
	//pJump->setEnabled(true);
	//pFire->setEnabled(true);
	pMenuArrow->setEnabled(true);
	pMenuFireBall->setEnabled(true);

	pColor->setVisible(false);
	pSetMenu->setVisible(false);
	pResume->setVisible(false);
	pResume->setEnabled(false);
	pReStart->setVisible(false);
	pReStart->setEnabled(false);
	pSelectMenu->setVisible(false);
	pSelectMenu->setEnabled(false);
}






