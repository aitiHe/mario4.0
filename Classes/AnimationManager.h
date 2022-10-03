#ifndef _ANIMATION_MANAGER_H__
#define _ANIMATION_MANAGER_H__

#include "cocos2d.h"
#include "GameEnum.h"
using namespace cocos2d;

class AnimationManager
{
public:
	void initAnimationMap();
	void preLoadEffectAndMusic();  // 加载音乐和音效

	Animation* getAnimation(AnimationType key);
	Animate* createAnimate(const char *key);
	Animate* createAnimate(AnimationType key);

	void setSelectLevel();
	void setMusicSwitch();

	static AnimationManager* getInstance();
	static AnimationManager *_instance;
private:
	// 涉及到主角的很规则的精灵帧动画创建函数
	Animation* createAnimationByAnimationType(AnimationType key);
};

#define sAnimationMgr AnimationManager::getInstance()
#endif