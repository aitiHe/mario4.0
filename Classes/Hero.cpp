#include "Hero.h"
#include "AnimationManager.h"
#include "Global.h"
#include "AudioEngine.h"

Hero* Hero::_heroInstance;
Hero* Hero::getHeroInstance()
{
	return _heroInstance;
}

Hero::Hero():
mainBody(NULL),
mainTemp(NULL),
_jumpLeft(NULL),
_jumpRight(NULL),
_lifeOverSmall(NULL),
_lifeOverNormal(NULL),
_lifeOverFire(NULL),
_normalLeft(NULL),
_normalRight(NULL),
_smallJumpLeft(NULL),
_smallJumpRight(NULL),
_smallLeft(NULL),
_smallRight(NULL),
pLabelUp(NULL)
{
	norBodySize = Size(18, 32);
	smallSize = Size(14, 16);
	currentSize = smallSize;
	state = eNormalRight;
	statePre = eNormalRight;
	face = eRight;
	isFlying = false;
	bodyType = eBody_Small;
	
	Texture2D *pTexture = Director::getInstance()->getTextureCache()->addImage("walkLeft.png");
	_jumpLeft = SpriteFrame::createWithTexture(pTexture, Rect(180, 0, 18, 32));
	_jumpLeft->retain();
	_normalLeft = SpriteFrame::createWithTexture(pTexture, Rect(18*9, 0, 18, 32));
	_normalLeft->retain();

	pTexture = Director::getInstance()->getTextureCache()->addImage("WalkLeft_fire.png");
	_jumpLeftFire = SpriteFrame::createWithTexture(pTexture, Rect(180, 0, 18, 32));
	_jumpLeftFire->retain();
	_normalLeftFire = SpriteFrame::createWithTexture(pTexture, Rect(18*9, 0, 18, 32));
	_normalLeftFire->retain();

	pTexture = Director::getInstance()->getTextureCache()->addImage("WalkRight_fire.png");
	_jumpRightFire = SpriteFrame::createWithTexture(pTexture, Rect(180, 0, 18, 32));
	_jumpRightFire->retain();
	_normalRightFire = SpriteFrame::createWithTexture(pTexture, Rect(0, 0, 18, 32));
	_normalRightFire->retain();

	pTexture = Director::getInstance()->getTextureCache()->addImage("walkRight.png");
	_jumpRight = SpriteFrame::createWithTexture(pTexture, Rect(180, 0, 18, 32));
	_jumpRight->retain();
	_normalRight = SpriteFrame::createWithTexture(pTexture, Rect(0, 0, 18, 32));
	_normalRight->retain();

	pTexture = Director::getInstance()->getTextureCache()->addImage("smallWalkRight.png");
	_smallRight = SpriteFrame::createWithTexture(pTexture, Rect(0, 0, 14, 16));
	_smallRight->retain();
	_smallJumpRight = SpriteFrame::createWithTexture(pTexture, Rect(140, 0, 14, 16));
	_smallJumpRight->retain();

	pTexture = Director::getInstance()->getTextureCache()->addImage("smallWalkLeft.png");
	_smallLeft = SpriteFrame::createWithTexture(pTexture, Rect(14*9, 0, 14, 16));
	_smallLeft->retain();
	_smallJumpLeft = SpriteFrame::createWithTexture(pTexture, Rect(140, 0, 14, 16));
	_smallJumpLeft->retain();

	pTexture = Director::getInstance()->getTextureCache()->addImage("small_die.png");
	_lifeOverSmall = SpriteFrame::createWithTexture(pTexture, Rect(16, 0, 16, 18));
	_lifeOverSmall->retain();

	pTexture = Director::getInstance()->getTextureCache()->addImage("normal_die.png");
	_lifeOverNormal = SpriteFrame::createWithTexture(pTexture, Rect(24, 0, 24, 34));
	_lifeOverNormal->retain();

	pTexture = Director::getInstance()->getTextureCache()->addImage("fire_die.png");
	_lifeOverFire = SpriteFrame::createWithTexture(pTexture, Rect(24, 0, 24, 34));
	_lifeOverFire->retain();

	pTexture = Director::getInstance()->getTextureCache()->addImage("allow_walkLeft.png");
	_normalLeftArrow = SpriteFrame::createWithTexture(pTexture, Rect(0, 0, 18, 32));
	_normalLeftArrow->retain();
	_jumpLeftArrow = SpriteFrame::createWithTexture(pTexture, Rect(180, 0, 18, 32));
	_jumpLeftArrow->retain();

	pTexture = Director::getInstance()->getTextureCache()->addImage("allow_walkRight.png");
	_normalRightArrow = SpriteFrame::createWithTexture(pTexture, Rect(0, 0, 18, 32));
	_normalRightArrow->retain();
	_jumpRightArrow = SpriteFrame::createWithTexture(pTexture, Rect(180, 0, 18, 32));
	_jumpRightArrow->retain();

	isDied = false;

	_heroInstance = this;

	isSafeTime = false;

	bulletable = false;

	gadgetable = false;

	currentBulletType = eBullet_common;

	_heroInstance = this;
}

Hero::~Hero()
{
	this->unscheduleAllCallbacks();

	// �������Щָ�붼��ͨ��create���ɵģ��Ѿ�������������������Բ���Ҫ�ֶ��ͷ�
	//_jumpLeft->release();
	//_jumpRight->release();
	//_lifeOver->release();
	//_normalLeft->release();
	//_normalRight->release();
	//_smallJumpLeft->release();
	//_smallJumpRight->release();
	//_smallLeft->release();
	//_smallRight->release();


	/*	CC_SAFE_DELETE(_jumpLeft);
	CC_SAFE_DELETE(_jumpRight);	
	CC_SAFE_DELETE(_lifeOver);	
	CC_SAFE_DELETE(_normalLeft);	
	CC_SAFE_DELETE(_normalRight);	
	CC_SAFE_DELETE(_smallJumpLeft);	
	CC_SAFE_DELETE(_smallJumpRight);	
	CC_SAFE_DELETE(_smallLeft);
	CC_SAFE_DELETE(_smallRight);*/	
}

void Hero::setHeroState(marioDirection _state)
{
	if (isDied)
	{
		return ;
	}
	if (state == _state)
	{
		return;
	}
	statePre = state;
	state = _state;

	mainBody->stopAllActions();
	switch (_state)
	{
	case eFireTheHole:
		{

			break;
		}
	case eNormalRight:
		{
			if (bodyType == eBody_Normal)
			{
				if (bulletable)
				{
					switch (Global::getGlobalInstance()->getCurrentBulletType())
					{
					case eBullet_common:
						mainBody->setSpriteFrame(_normalRightFire);
						break;
					case eBullet_arrow:
						mainBody->setSpriteFrame(_normalRightArrow);
						break;
					}
				}
				else
				{
					mainBody->setSpriteFrame(_normalRight);
				}
			} 
			else
			{
				mainBody->setSpriteFrame(_smallRight);
			}
			face = eRight;
			break;
		}
	case eNormalLeft:
		{
			if (bodyType == eBody_Normal)
			{
				if (bulletable)
				{
					switch (Global::getGlobalInstance()->getCurrentBulletType())
					{
					case eBullet_common:
						mainBody->setSpriteFrame(_normalLeftFire);
						break;
					case eBullet_arrow:
						mainBody->setSpriteFrame(_normalLeftArrow);
						break;
					}
				}
				else
				{
					mainBody->setSpriteFrame(_normalLeft);
				}
			} 
			else
			{
				mainBody->setSpriteFrame(_smallLeft);
			}
			face = eLeft;
			break;
		}
	case eRight:
		{
			if (!isFlying)
			{
				if (bodyType ==  eBody_Normal)
				{
					if (bulletable)
					{
						switch (Global::getGlobalInstance()->getCurrentBulletType())
						{
						case eBullet_common:
							mainBody->runAction( RepeatForever::create(
								sAnimationMgr->createAnimate(eAniRightFire)) );
							break;
						case eBullet_arrow:
							mainBody->runAction( RepeatForever::create(
								sAnimationMgr->createAnimate(eAniArrowRight)) );
							break;
						}
					}
					else
					{
						mainBody->runAction( RepeatForever::create(
							sAnimationMgr->createAnimate(eAniRight)) );
					}
				} 
				else
				{
					mainBody->runAction( RepeatForever::create(
						sAnimationMgr->createAnimate(eAniRightSmall)) );
				}
			}
			face = eRight;
			break;
		}
	case eLeft:
		{
			if (!isFlying)
			{
				if (bodyType == eBody_Normal)
				{
					if (bulletable)
					{
						switch (Global::getGlobalInstance()->getCurrentBulletType())
						{
						case eBullet_common:
							mainBody->runAction( RepeatForever::create(
								sAnimationMgr->createAnimate(eAniLeftFire)) );
							break;
						case eBullet_arrow:
							mainBody->runAction( RepeatForever::create(
								sAnimationMgr->createAnimate(eAniArrowLeft)) );
							break;
						}
					}
					else
					{
						mainBody->runAction(RepeatForever::create(
							sAnimationMgr->createAnimate(eAniLeft)));
					}
				} 
				else
				{
					mainBody->runAction( RepeatForever::create(
						sAnimationMgr->createAnimate(eAniLeftSmall)) );
				}
				
			}
			face = eLeft;
			break;
		}
	case eJumpLeft:
		{
			if (bodyType == eBody_Normal)
			{
				if (bulletable)
				{					
					switch (Global::getGlobalInstance()->getCurrentBulletType())
					{
					case eBullet_common:
						mainBody->setSpriteFrame(_jumpLeftFire);
						break;
					case eBullet_arrow:
						mainBody->setSpriteFrame(_jumpLeftArrow);
						break;
					}
				}
				else
				{
					mainBody->setSpriteFrame(_jumpLeft);
				}
			} 
			else
			{
				mainBody->setSpriteFrame(_smallJumpLeft);
			}
			face = eLeft;
			break;
		}
	case eJumpRight:
		{
			if (bodyType == eBody_Normal)
			{
				if (bulletable)
				{
					switch (Global::getGlobalInstance()->getCurrentBulletType())
					{
					case eBullet_common:
						mainBody->setSpriteFrame(_jumpRightFire);
						break;
					case eBullet_arrow:
						mainBody->setSpriteFrame(_jumpRightArrow);
						break;
					}
				}
				else
				{
					mainBody->setSpriteFrame(_jumpRight);
				}
			} 
			else
			{
				mainBody->setSpriteFrame(_smallJumpRight);
			}
			face = eRight;
			break;
		}
	default:
		break;
	}
}

marioDirection Hero::getHeroState()
{
	return state;
}

void Hero::onEnter()
{
	Node::onEnter();
}

void Hero::onExit()
{
	Node::onExit();
}

bool Hero::heroInit()
{
	this->setContentSize(smallSize);
	// ������ص���С�������
	mainBody = Sprite::create("smallWalkRight.png", Rect(0, 0, 14, 16));
	mainBody->setAnchorPoint(Vec2(0, 0));
	this->addChild(mainBody);
	state = eNormalRight;
	return true;
}

Hero* Hero::create()
{
	Hero *pHero = new Hero();
	if (pHero && pHero->heroInit())
	{
		pHero->autorelease();
		return pHero;
	}
	CC_SAFE_DELETE(pHero);
	return NULL;
}

void Hero::setBodyType(BodyType _bodytype)
{
	bodyType = _bodytype;
	switch (_bodytype)
	{
	case eBody_Normal:
		currentSize = norBodySize;
		mainBody->setSpriteFrame(_normalRight);
		break;
	case eBody_Small:
		currentSize = smallSize;
		mainBody->setSpriteFrame(_smallRight);
		break;
	case eBody_Fireable:
		{
			bodyType = eBody_Normal;
			currentSize = norBodySize;
			bulletable = true;
			switch (Global::getGlobalInstance()->getCurrentBulletType())
			{
			case eBullet_arrow:
				mainBody->setSpriteFrame(_normalRightArrow);
				break;
			case eBullet_common:
				mainBody->setSpriteFrame(_normalRightFire);
				break;
			}

			break;
		}
	default:
		break;
	}
	this->setContentSize(currentSize);
}

Size Hero::getCurrentSize()
{
	return currentSize;
}

BodyType Hero::getCurrentBodyType()
{
	return bodyType;
}

void Hero::setSafeTime(bool _issafe)
{
	isSafeTime = _issafe;
}

bool Hero::getIsSafeTime()
{
	return isSafeTime;
}

void Hero::changeForGotAddLifeMushroom()
{
	//CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("AddLife.ogg");

	Global::getGlobalInstance()->lifeNumPlusOne();

	pLabelUp = Label::createWithSystemFont("UP1!", "Arial", 20);
	pLabelUp->setPosition(Vec2::ZERO);
	this->addChild(pLabelUp);
	JumpBy *pJump = JumpBy::create(0.5f, Vec2(0, this->getContentSize().height/2),
		this->getContentSize().height, 1);
	pLabelUp->runAction(Sequence::create(pJump, 
		CallFunc::create( CC_CALLBACK_0(Hero::clearLabelUp,this)), NULL));
}

void Hero::changeForGotMushroom()
{
	//CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("EatMushroomOrFlower.ogg");

	switch (bodyType)
	{
	case eBody_Small:
		{
			// ����һ�α����˸�Ķ���
			Global::getGlobalInstance()->currentHeroType = eBody_Normal;
			this->setHeroTypeForNormal();
			ActionInterval *pBlink = Blink::create(1, 5);
			this->runAction(pBlink);
		}
		break;
	case eBody_Normal:
		// չʾһ��up����ʾ����һ���ˣ������ǽŵ�������ͷ��Ȼ����ʧ
		{
			//pLabelUp = CCLabelTTF::create("Good!!!", "Arial", 20);
			//pLabelUp->setPosition(Vec2::ZERO);
			//this->addChild(pLabelUp);
			//JumpBy *pJump = JumpBy::create(0.5f, Vec2(0, this->getContentSize().height/2),
			//	this->getContentSize().height, 1);
			//pLabelUp->runAction(Sequence::create(pJump, 
			//	CallFunc::create(this, callfunc_selector(Hero::clearLabelUp))));
			bulletable = true;
			Global::getGlobalInstance()->currentHeroType = eBody_Fireable;
			if (!bulletable)
			{
				switch (face)
				{
				case eRight:
					mainBody->setSpriteFrame(_normalRightFire);
					break;
				case eLeft:
					mainBody->setSpriteFrame(_normalLeftFire);
					break;
				default:
					break;
				}
			}
		}
		break;
	default:
		break;
	}
}

void Hero::changeForGotEnemy()
{
	isSafeTime = true;
	ActionInterval *pDelay = DelayTime::create(3.0f);
	this->runAction(Sequence::create(pDelay, 
		CallFunc::create( CC_CALLBACK_0(Hero::reSetSafeTime,this)), NULL));

	switch (bodyType)
	{
	case eBody_Normal:
		{
			//CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("SuoXiao.ogg");
			Global::getGlobalInstance()->currentHeroType = eBody_Small;
			this->setHeroTypeForSmall();
			ActionInterval *pBlink = Blink::create(3, 15);
			this->runAction(pBlink);
			break;
		}
	case eBody_Small:
		{
			//CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("YuDaoGuaiWuSi.ogg");
			mainBody->stopAllActions();
			mainBody->setSpriteFrame(_lifeOverSmall);
			this->setHeroDie(true);
			break;
		}
	default:
		break;
	}
}

void Hero::setHeroDie(bool _die)
{
	isDied = _die;
	Global::getGlobalInstance()->currentHeroType = eBody_Small;
	Global::getGlobalInstance()->lifeNumCutOne();
}
bool Hero::isHeroDied()
{
	return isDied;
}

void Hero::dieForTrap()
{
	mainBody->stopAllActions();

	switch (bodyType)
	{
	case eBody_Small:
		mainBody->setSpriteFrame(_lifeOverSmall);
		mainBody->runAction(sAnimationMgr->createAnimate(eAniSmallDie));
		break;
	case eBody_Normal:
		if (bulletable)
		{
			switch (Global::getGlobalInstance()->getCurrentBulletType())
			{
			case eBullet_common:
				mainBody->setSpriteFrame(_lifeOverFire);
				mainBody->runAction(sAnimationMgr->createAnimate(eAniFireDie));
				break;
			case eBullet_arrow:
				_lifeOverFire = SpriteFrame::create("arrow_die.png", Rect(24, 0, 24, 32));
				mainBody->setSpriteFrame(_lifeOverFire);
				mainBody->runAction(sAnimationMgr->createAnimate(eAniArrowDie));
			}
		}
		else
		{
			mainBody->setSpriteFrame(_lifeOverNormal);
			mainBody->runAction(sAnimationMgr->createAnimate(eAniNormalDie));			
		}
		break;
	default:
		break;
	}

	ActionInterval *pMoveUp = MoveBy::create(0.6f, Vec2(0, 32));
	ActionInterval *pMoveDown = MoveBy::create(0.6f, Vec2(0, -32));
	ActionInterval *pDeley = DelayTime::create(0.2f);
	
	this->runAction(Sequence::create(pMoveUp, pDeley, pMoveDown,
		CallFunc::create( CC_CALLBACK_0(Hero::reSetNonVisible,this)), NULL));
}

void Hero::reSetNonVisible()
{
	mainBody->stopAllActions();
	this->setVisible(false);
}

void Hero::clearLabelUp()
{
	pLabelUp->removeFromParentAndCleanup(true);
	pLabelUp = NULL;
}

void Hero::setHeroTypeForSmall()
{
	this->setBodyType(eBody_Small);
	bulletable = false;
	switch (face)
	{
	case eRight:
		mainBody->setSpriteFrame(_smallRight);
		break;
	case eLeft:
		mainBody->setSpriteFrame(_smallLeft);
		break;
	default:
		break;
	}	
}

void Hero::setHeroTypeForNormal()
{
	this->setBodyType(eBody_Normal);
	switch (face)
	{
	case eRight:
		mainBody->setSpriteFrame(_normalRight);
		break;
	case eLeft:
		mainBody->setSpriteFrame(_normalLeft);
		break;
	default:
		break;
	}
}

void Hero::reSetSafeTime()
{
	isSafeTime = false;
}

bool Hero::isBulletable()
{
	return bulletable;
}

void Hero::setGadgetable(bool _gadgetable)
{
	gadgetable = _gadgetable;
}
bool Hero::isGadgetable()
{
	return gadgetable;
}

void Hero::fireAction()
{
	ActionInterval *pAction = NULL;
	this->setHeroState(eFireTheHole);
	switch (face)
	{
	case eRight:		
		{
			switch (Global::getGlobalInstance()->getCurrentBulletType())
			{
			case eBullet_common:
				pAction = sAnimationMgr->createAnimate(eAniFireActionR);
				break;
			case eBullet_arrow:
				pAction = sAnimationMgr->createAnimate(eAniArrowActionR);
				break;
			}
			break;
		}
	case eLeft:		
		{
			switch (Global::getGlobalInstance()->getCurrentBulletType())
			{
			case eBullet_common:
				pAction = sAnimationMgr->createAnimate(eAniFireActionL);
				break;
			case eBullet_arrow:
				pAction = sAnimationMgr->createAnimate(eAniArrowActionL);
				break;
			}
			break;
		}
	default:
		break;
	}
	mainBody->runAction(Sequence::create(pAction,
		CallFunc::create( CC_CALLBACK_0(Hero::reSetStateForFired,this)), NULL));
}

void Hero::reSetStateForFired()
{
	this->setHeroState(statePre);
}

void Hero::setBulletType(BulletType _type)
{	
	if (currentBulletType != _type)
	{
		currentBulletType = _type;
		switch (face)
		{
		case eRight:
			{
				switch (currentBulletType)
				{
				case eBullet_common:
					mainBody->setSpriteFrame(_normalRightFire);
					break;
				case eBullet_arrow:
					mainBody->setSpriteFrame(_normalRightArrow);
					break;
				}
				break;
			}
		case eLeft:
			{
				switch (currentBulletType)
				{
				case eBullet_common:
					mainBody->setSpriteFrame(_normalLeftFire);
					break;
				case eBullet_arrow:
					mainBody->setSpriteFrame(_normalLeftArrow);
					break;
				}
				break;
			}
		}
	}
}

BulletType Hero::getBulletType()
{
	return currentBulletType;
}

void Hero::reSetForSuccess()
{
	switch (bodyType)
	{
	case eBody_Small:
		mainBody->setSpriteFrame(_smallRight);
		break;
	case eBody_Normal:
		if (bulletable)
		{			
			switch (Global::getGlobalInstance()->getCurrentBulletType())
			{
			case eBullet_common:
				mainBody->setSpriteFrame(_normalRightFire);
				break;
			case eBullet_arrow:
				mainBody->setSpriteFrame(_normalRightArrow);
				break;
			}
		} 
		else
		{
			mainBody->setSpriteFrame(_normalRight);
		}
		break;
	default:
		break;
	}
}
