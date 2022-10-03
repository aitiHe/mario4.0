#include "AnimationManager.h"
#include "AudioEngine.h"
#include <string>
#include "cocos2d.h"
using namespace std;
USING_NS_CC;
AnimationManager* AnimationManager::_instance = NULL;

Animation* AnimationManager::createAnimationByAnimationType(AnimationType key)
{
	Vector<SpriteFrame*> pArray;
	SpriteFrame *frame = NULL;
	Texture2D *pTexture;
	Animation *pAnimation = NULL;

	switch (key)
	{
	case eAniRight:
		{
			pTexture = Director::getInstance()->getTextureCache()->addImage("walkRight.png");
			for (int i = 0; i < 10; ++i)
			{
				frame = SpriteFrame::createWithTexture(pTexture, Rect(18*i, 0, 18, 32));
				pArray.pushBack(frame);
			}
			pAnimation = Animation::createWithSpriteFrames(pArray, 0.02f);
			break;
		}
	case eAniLeft:
		{
			pTexture = Director::getInstance()->getTextureCache()->addImage("walkLeft.png");
			for (int i = 9; i >= 0; --i)
			{
				frame = SpriteFrame::createWithTexture(pTexture, Rect(18*i, 0, 18, 32));
				pArray.pushBack(frame);
			}
			pAnimation = Animation::createWithSpriteFrames(pArray, 0.02f);
			break;
		}
	case eAniLeftSmall:
		{
			pTexture = Director::getInstance()->getTextureCache()->addImage("smallWalkLeft.png");
			for (int i = 9; i >= 0; --i)
			{
				frame = SpriteFrame::createWithTexture(pTexture, Rect(14*i, 0, 14, 16));
				pArray.pushBack(frame);
			}
			pAnimation = Animation::createWithSpriteFrames(pArray, 0.02f);
			break;
		}
	case eAniRightSmall:
		{
			pTexture = Director::getInstance()->getTextureCache()->addImage("smallWalkRight.png");
			for (int i = 0; i < 10; ++i)
			{
				frame = SpriteFrame::createWithTexture(pTexture, Rect(14*i, 0, 14, 16));
				pArray.pushBack(frame);
			}
			pAnimation = Animation::createWithSpriteFrames(pArray, 0.02f);
			break;
		}
	case eAniLeftFire:
		{
			pTexture = Director::getInstance()->getTextureCache()->addImage("WalkLeft_fire.png");
			for (int i = 9; i >= 0; --i)
			{
				frame = SpriteFrame::createWithTexture(pTexture, Rect(18*i, 0, 18, 32));
				pArray.pushBack(frame);
			}
			pAnimation = Animation::createWithSpriteFrames(pArray, 0.02f);
			break;
		}
	case eAniRightFire:
		{
			pTexture = Director::getInstance()->getTextureCache()->addImage("WalkRight_fire.png");
			for (int i = 0; i < 10; ++i)
			{
				frame = SpriteFrame::createWithTexture(pTexture, Rect(18*i, 0, 18, 32));
				pArray.pushBack(frame);
			}
			pAnimation = Animation::createWithSpriteFrames(pArray, 0.02f);
			break;
		}
	case eAniBlinkCoin:
		{
			pTexture = Director::getInstance()->getTextureCache()->addImage("coinani.png");
			for (int i = 0; i < 4; ++i)
			{
				frame = SpriteFrame::createWithTexture(pTexture, Rect(8*i, 0, 8, 14));
				pArray.pushBack(frame);
				pArray.pushBack(frame);
			}
			pAnimation = Animation::createWithSpriteFrames(pArray, 0.02f);
			break;
		}
	case eAniMushroom:
		{
			pTexture = Director::getInstance()->getTextureCache()->addImage("Mushroom0.png");
			for (int i = 0; i < 2; ++i)
			{
				frame = SpriteFrame::createWithTexture(pTexture, Rect(16*i, 0, 16, 16));
				pArray.pushBack(frame);
			}
			pAnimation = Animation::createWithSpriteFrames(pArray, 0.3f);
			break;
		}
	case eAniflower:
		{
			pTexture = Director::getInstance()->getTextureCache()->addImage("flower0.png");
			for (int i = 0; i < 2; ++i)
			{
				frame = SpriteFrame::createWithTexture(pTexture, Rect(16*i, 0, 16, 24));
				pArray.pushBack(frame);
			}
			pAnimation = Animation::createWithSpriteFrames(pArray, 0.3f);
			break;
		}
	case eAniTortoiseLeft:
		{
			pTexture = Director::getInstance()->getTextureCache()->addImage("tortoise0.png");
			for (int i = 2; i < 4; ++i)
			{
				frame = SpriteFrame::createWithTexture(pTexture, Rect(18*i, 0, 18, 24));
				pArray.pushBack(frame);
			}
			pAnimation = Animation::createWithSpriteFrames(pArray, 0.3f);
			break;
		}
	case eAniTortoiseRight:
		{
			pTexture = Director::getInstance()->getTextureCache()->addImage("tortoise0.png");
			for (int i = 4; i < 6; ++i)
			{
				frame = SpriteFrame::createWithTexture(pTexture, Rect(18*i, 0, 18, 24));
				pArray.pushBack(frame);
			}
			pAnimation = Animation::createWithSpriteFrames(pArray, 0.3f);
			break;
		}
	case eAniTortoiseFly:
		{
			pTexture = Director::getInstance()->getTextureCache()->addImage("tortoise0.png");
			for (int i = 0; i < 2; ++i)
			{
				frame = SpriteFrame::createWithTexture(pTexture, Rect(18*i, 0, 18, 24));
				pArray.pushBack(frame);
			}
			pAnimation = Animation::createWithSpriteFrames(pArray, 0.3f);
			break;
		}
	case eAniRotatedFireBall:
		{
			pTexture = Director::getInstance()->getTextureCache()->addImage("fireRight.png");
			for (int i = 0; i < 8; ++i)
			{
				frame = SpriteFrame::createWithTexture(pTexture, Rect(10*i, 3, 10, 10));
				pArray.pushBack(frame);
			}
			pAnimation = Animation::createWithSpriteFrames(pArray, 0.01f);
			break;
		}
	case eAniBoomedFireBall:
		{
			pTexture = Director::getInstance()->getTextureCache()->addImage("fireRight.png");

			frame = SpriteFrame::createWithTexture(pTexture, Rect(80, 3, 10, 10));
			pArray.pushBack(frame);
			frame = SpriteFrame::createWithTexture(pTexture, Rect(90, 0, 14, 16));
			pArray.pushBack(frame);
			frame = SpriteFrame::createWithTexture(pTexture, Rect(105, 0, 16, 16));
			pArray.pushBack(frame);

			pAnimation = Animation::createWithSpriteFrames(pArray, 0.05f);
			break;
		}
	case eAniFireActionR:
		{
			pTexture = Director::getInstance()->getTextureCache()->addImage("fireActionR.png");
			for (int i = 0; i < 6; ++i)
			{
				frame = SpriteFrame::createWithTexture(pTexture, Rect(25*i, 0, 25, 32));
				pArray.pushBack(frame);
			}
			pAnimation = Animation::createWithSpriteFrames(pArray, 0.02f);
			break;
		}
	case eAniArrowActionR:
		{
			pTexture = Director::getInstance()->getTextureCache()->addImage("arrowActionR.png");
			for (int i = 0; i < 6; ++i)
			{
				frame = SpriteFrame::createWithTexture(pTexture, Rect(25*i, 0, 25, 32));
				pArray.pushBack(frame);
			}
			pAnimation = Animation::createWithSpriteFrames(pArray, 0.02f);
			break;
		}
	case eAniFireActionL:
		{
			pTexture = Director::getInstance()->getTextureCache()->addImage("fireActionL.png");
			for (int i = 5; i >= 0; --i)
			{
				frame = SpriteFrame::createWithTexture(pTexture, Rect(25*i, 0, 25, 32));
				pArray.pushBack(frame);
			}
			pAnimation = Animation::createWithSpriteFrames(pArray, 0.02f);
			break;
		}
	case eAniArrowActionL:
		{
			pTexture = Director::getInstance()->getTextureCache()->addImage("arrowActionL.png");
			for (int i = 5; i >= 0; --i)
			{
				frame = SpriteFrame::createWithTexture(pTexture, Rect(25*i, 0, 25, 32));
				pArray.pushBack(frame);
			}
			pAnimation = Animation::createWithSpriteFrames(pArray, 0.02f);
			break;
		}
	case eAniFlyFishR:
		{
			pTexture = Director::getInstance()->getTextureCache()->addImage("flyFishRight.png");
			frame = SpriteFrame::createWithTexture(pTexture, Rect(16*4, 0, 16, 16));
			pArray.pushBack(frame);
			frame = SpriteFrame::createWithTexture(pTexture, Rect(16*5, 0, 16, 16));
			pArray.pushBack(frame);

			pAnimation = Animation::createWithSpriteFrames(pArray, 0.05f);
			break;
		}
	case eAniFlyFishL:
		{
			pTexture = Director::getInstance()->getTextureCache()->addImage("flyFishLeft.png");
			frame = SpriteFrame::createWithTexture(pTexture, Rect(16*4, 0, 16, 16));
			pArray.pushBack(frame);
			frame = SpriteFrame::createWithTexture(pTexture, Rect(16*5, 0, 16, 16));
			pArray.pushBack(frame);

			pAnimation = Animation::createWithSpriteFrames(pArray, 0.05f);
			break;
		}
	case eAniArrowBroken:
		{
			pTexture = Director::getInstance()->getTextureCache()->addImage("arrowBroken.png");
			for (int i = 0; i < 3; ++i)
			{
				frame = SpriteFrame::createWithTexture(pTexture, Rect(16*i, 0, 16, 16));
				pArray.pushBack(frame);
			}

			pAnimation = Animation::createWithSpriteFrames(pArray, 0.05f);
			break;
		}
	case eAniSmallDie:
		{
			pTexture = Director::getInstance()->getTextureCache()->addImage("small_die.png");
			for (int i = 0; i < 7; ++i)
			{
				frame = SpriteFrame::createWithTexture(pTexture, Rect(16*i, 0, 16, 18));
				pArray.pushBack(frame);
				pArray.pushBack(frame);
			}

			pAnimation = Animation::createWithSpriteFrames(pArray, 0.1f);
			break;
		}
	case eAniNormalDie:
		{
			pTexture = Director::getInstance()->getTextureCache()->addImage("normal_die.png");
			for (int i = 0; i < 7; ++i)
			{
				frame = SpriteFrame::createWithTexture(pTexture, Rect(24*i, 0, 24, 34));
				pArray.pushBack(frame);
				pArray.pushBack(frame);
			}

			pAnimation = Animation::createWithSpriteFrames(pArray, 0.1f);
			break;
		}
	case eAniFireDie:
		{
			pTexture = Director::getInstance()->getTextureCache()->addImage("fire_die.png");
			for (int i = 0; i < 7; ++i)
			{
				frame = SpriteFrame::createWithTexture(pTexture, Rect(24*i, 0, 24, 34));
				pArray.pushBack(frame);
				pArray.pushBack(frame);
			}

			pAnimation = Animation::createWithSpriteFrames(pArray, 0.1f);
			break;
		}
	case eAniBossMoveLeft:
		{
			pTexture = Director::getInstance()->getTextureCache()->addImage("boss.png");
			for (int i = 3; i >=0; --i)
			{
				frame = SpriteFrame::createWithTexture(pTexture, Rect(32*i, 0, 32, 32));
				pArray.pushBack(frame);
			}

			pAnimation = Animation::createWithSpriteFrames(pArray, 0.2f);
			break;
		}
	case eAniBossMoveRight:
		{
			pTexture = Director::getInstance()->getTextureCache()->addImage("boss.png");
			for (int i = 4; i < 8; ++i)
			{
				frame = SpriteFrame::createWithTexture(pTexture, Rect(32*i, 0, 32, 32));
				pArray.pushBack(frame);
			}

			pAnimation = Animation::createWithSpriteFrames(pArray, 0.2f);
			break;
		}
	case eAniBossFireLeft:
		{
			pTexture = Director::getInstance()->getTextureCache()->addImage("bossBullet.png");
			for (int i = 0; i < 2; ++i)
			{
				frame = SpriteFrame::createWithTexture(pTexture, Rect(24*i, 0, 24, 8));
				pArray.pushBack(frame);
			}

			pAnimation = Animation::createWithSpriteFrames(pArray, 0.1f);
			break;
		}
	case eAniBossFireRight:
		{
			pTexture = Director::getInstance()->getTextureCache()->addImage("bossBullet.png");
			for (int i = 3; i >= 2; --i)
			{
				frame = SpriteFrame::createWithTexture(pTexture, Rect(24*i, 0, 24, 8));
				pArray.pushBack(frame);
			}

			pAnimation = Animation::createWithSpriteFrames(pArray, 0.1f);
			break;
		}
	case eAniBatteryBoom:
		{
			frame = SpriteFrame::create("batteryBoom1.png", Rect(0, 0, 32, 27));
			pArray.pushBack(frame);
			frame = SpriteFrame::create("batteryBoom2.png", Rect(0, 0, 32, 18));
			pArray.pushBack(frame);
			frame = SpriteFrame::create("batteryBoom3.png", Rect(0, 0, 32, 20));
			pArray.pushBack(frame);

			pAnimation = Animation::createWithSpriteFrames(pArray, 0.1f);
			break;
		}
	case eAniLighting:
		{
			pTexture = Director::getInstance()->getTextureCache()->addImage("lighting.png");

			frame = SpriteFrame::createWithTexture(pTexture, Rect(0, 0, 16, 32));
			pArray.pushBack(frame);
			frame = SpriteFrame::createWithTexture(pTexture, Rect(16, 0, 16, 32));
			pArray.pushBack(frame);

			pAnimation = Animation::createWithSpriteFrames(pArray, 0.1f);
			break;
		}
	case eAniArrowLeft:
		{
			pTexture = Director::getInstance()->getTextureCache()->addImage("allow_walkLeft.png");
			for (int i = 9; i >= 0; --i)
			{
				frame = SpriteFrame::createWithTexture(pTexture, Rect(18*i, 0, 18, 32));
				pArray.pushBack(frame);
			}
			pAnimation = Animation::createWithSpriteFrames(pArray, 0.02f);
			break;
		}
	case eAniArrowRight:
		{
			pTexture = Director::getInstance()->getTextureCache()->addImage("allow_walkRight.png");
			for (int i = 0; i < 10; ++i)
			{
				frame = SpriteFrame::createWithTexture(pTexture, Rect(18*i, 0, 18, 32));
				pArray.pushBack(frame);
			}
			pAnimation = Animation::createWithSpriteFrames(pArray, 0.02f);
			break;
		}
	case eAniArrowDie:
		{
			pTexture = Director::getInstance()->getTextureCache()->addImage("arrow_die.png");
			for (int i = 0; i < 7; ++i)
			{
				frame = SpriteFrame::createWithTexture(pTexture, Rect(24*i, 0, 24, 34));
				pArray.pushBack(frame);
				pArray.pushBack(frame);
			}
			pAnimation = Animation::createWithSpriteFrames(pArray, 0.1f);
			break;
		}
	default:
		break;
	}
	
//	pArray->release();
	
	return pAnimation;
}

void AnimationManager::preLoadEffectAndMusic()
{
	// CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("OnLand.ogg");
	// CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("DingChuMoGuHuoHua.ogg");
	// CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("AddLife.ogg");
	// CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("EatMushroomOrFlower.ogg");
	// CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("SuoXiao.ogg");
	// CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("BossDiaoLuoQiaoXia.ogg");
	// CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("BossDie.ogg");
	// CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("BossHuoQiu.ogg");
	// CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("CaiSiGuaiWu.ogg");
	// CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("DiaoRuXianJingSi.ogg");
	// CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("DingPoZhuan.ogg");
	// CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("DingYingZhuanKuai.ogg");
	// CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("EatCoin.ogg");
	// CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("GameOver.ogg");
	// CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("HuoQiuDaDaoGuaiWu.ogg");
	// CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("Jump.ogg");
	// CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("QiZiLuoXia.ogg");
	// CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("YuDaoGuaiWuSi.ogg");
	// CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("RengHuoQiu.ogg");
}

void AnimationManager::initAnimationMap()
{
	this->preLoadEffectAndMusic();

	this->setSelectLevel();

	this->setMusicSwitch();

	char temp[20] = { 0 };

	sprintf(temp, "%d", eAniRight);	
	AnimationCache::getInstance()->addAnimation(
		createAnimationByAnimationType(eAniRight), temp);

	sprintf(temp, "%d", eAniLeft);
	AnimationCache::getInstance()->addAnimation(
		createAnimationByAnimationType(eAniLeft), temp);

	sprintf(temp, "%d", eAniLeftSmall);
	AnimationCache::getInstance()->addAnimation(
		createAnimationByAnimationType(eAniLeftSmall), temp);

	sprintf(temp, "%d", eAniRightSmall);
	AnimationCache::getInstance()->addAnimation(
		createAnimationByAnimationType(eAniRightSmall), temp);

	sprintf(temp, "%d", eAniBlinkCoin);
	AnimationCache::getInstance()->addAnimation(
		createAnimationByAnimationType(eAniBlinkCoin), temp);

	sprintf(temp, "%d", eAniMushroom);
	AnimationCache::getInstance()->addAnimation(
		createAnimationByAnimationType(eAniMushroom), temp);

	sprintf(temp, "%d", eAniflower);
	AnimationCache::getInstance()->addAnimation(
		createAnimationByAnimationType(eAniflower), temp);

	sprintf(temp, "%d", eAniTortoiseLeft);
	AnimationCache::getInstance()->addAnimation(
		createAnimationByAnimationType(eAniTortoiseLeft), temp);

	sprintf(temp, "%d", eAniTortoiseRight);
	AnimationCache::getInstance()->addAnimation(
		createAnimationByAnimationType(eAniTortoiseRight), temp);

	sprintf(temp, "%d", eAniTortoiseFly);
	AnimationCache::getInstance()->addAnimation(
		createAnimationByAnimationType(eAniTortoiseFly), temp);

	sprintf(temp, "%d", eAniRightFire);
	AnimationCache::getInstance()->addAnimation(
		createAnimationByAnimationType(eAniRightFire),  temp);

	sprintf(temp, "%d", eAniLeftFire);
	AnimationCache::getInstance()->addAnimation(
		createAnimationByAnimationType(eAniLeftFire), temp);

	sprintf(temp, "%d", eAniRotatedFireBall);
	AnimationCache::getInstance()->addAnimation(
		createAnimationByAnimationType(eAniRotatedFireBall), temp);

	sprintf(temp, "%d", eAniBoomedFireBall);
	AnimationCache::getInstance()->addAnimation(
		createAnimationByAnimationType(eAniBoomedFireBall), temp);

	sprintf(temp, "%d", eAniFireActionR);
	AnimationCache::getInstance()->addAnimation(
		createAnimationByAnimationType(eAniFireActionR), temp);

	sprintf(temp, "%d", eAniFireActionL);
	AnimationCache::getInstance()->addAnimation(
		createAnimationByAnimationType(eAniFireActionL), temp);

	sprintf(temp, "%d", eAniFlyFishR);
	AnimationCache::getInstance()->addAnimation(
		createAnimationByAnimationType(eAniFlyFishR), temp);

	sprintf(temp, "%d", eAniFlyFishL);
	AnimationCache::getInstance()->addAnimation(
		createAnimationByAnimationType(eAniFlyFishL), temp);

	sprintf(temp, "%d", eAniArrowBroken);
	AnimationCache::getInstance()->addAnimation(
		createAnimationByAnimationType(eAniArrowBroken), temp);

	sprintf(temp, "%d", eAniSmallDie);
	AnimationCache::getInstance()->addAnimation(
		createAnimationByAnimationType(eAniSmallDie), temp);

	sprintf(temp, "%d", eAniNormalDie);
	AnimationCache::getInstance()->addAnimation(
		createAnimationByAnimationType(eAniNormalDie), temp);

	sprintf(temp, "%d", eAniFireDie);
	AnimationCache::getInstance()->addAnimation(
		createAnimationByAnimationType(eAniFireDie), temp);

	sprintf(temp, "%d", eAniBossMoveLeft);
	AnimationCache::getInstance()->addAnimation(
		createAnimationByAnimationType(eAniBossMoveLeft), temp);

	sprintf(temp, "%d", eAniBossMoveRight);
	AnimationCache::getInstance()->addAnimation(
		createAnimationByAnimationType(eAniBossMoveRight), temp);

	sprintf(temp, "%d", eAniBossFireLeft);
	AnimationCache::getInstance()->addAnimation(
		createAnimationByAnimationType(eAniBossFireLeft), temp);

	sprintf(temp, "%d", eAniBossFireRight);
	AnimationCache::getInstance()->addAnimation(
		createAnimationByAnimationType(eAniBossFireRight), temp);

	sprintf(temp, "%d", eAniBatteryBoom);
	AnimationCache::getInstance()->addAnimation(
		createAnimationByAnimationType(eAniBatteryBoom), temp);

	sprintf(temp, "%d", eAniLighting);
	AnimationCache::getInstance()->addAnimation(
		createAnimationByAnimationType(eAniLighting), temp);

	sprintf(temp, "%d", eAniArrowLeft);
	AnimationCache::getInstance()->addAnimation(
		createAnimationByAnimationType(eAniArrowLeft), temp);

	sprintf(temp, "%d", eAniArrowRight);
	AnimationCache::getInstance()->addAnimation(
		createAnimationByAnimationType(eAniArrowRight), temp);

	sprintf(temp, "%d", eAniArrowDie);
	AnimationCache::getInstance()->addAnimation(
		createAnimationByAnimationType(eAniArrowDie), temp);

	sprintf(temp, "%d", eAniArrowActionL);
	AnimationCache::getInstance()->addAnimation(
		createAnimationByAnimationType(eAniArrowActionL), temp);

	sprintf(temp, "%d", eAniArrowActionR);
	AnimationCache::getInstance()->addAnimation(
		createAnimationByAnimationType(eAniArrowActionR), temp);
}

Animation* AnimationManager::getAnimation(AnimationType key)
{
	char temp[20] = { 0 };
	sprintf(temp, "%d", key);
	
	return AnimationCache::getInstance()->getAnimation(temp);
}

Animate* AnimationManager::createAnimate(const char *key)
{
	Animation *anim = AnimationCache::getInstance()->getAnimation(key);
	if (anim)
	{
		return Animate::create(anim);
	}
	return NULL;
}

Animate* AnimationManager::createAnimate(AnimationType key)
{
	Animation *anim = getAnimation(key);
	if (anim)
	{
		return Animate::create(anim);
	}
	return NULL;
}

AnimationManager* AnimationManager::getInstance()
{
	if (!_instance)
	{
		_instance = new AnimationManager();
	}
	return _instance;
}

void AnimationManager::setSelectLevel()
{
	char str[256] = {0};
	for (int i = 2; i <= 8; ++i)
	{
        snprintf(str, 256, "Level%d", i);
		// str= CCString::createWithFormat("Level%d", i);
        UserDefault::getInstance()->setStringForKey(str, "no");
	}
}

void AnimationManager::setMusicSwitch()
{
    UserDefault::getInstance()->setStringForKey("Music", "on");
    UserDefault::getInstance()->setStringForKey("SoundEffect", "on");
}

