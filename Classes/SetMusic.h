#ifndef _SETMUSIC_H_
#define _SETMUSIC_H_

#include "cocos2d.h"
using namespace cocos2d;

class CCSetMusic : public Layer
{
public:
	CCSetMusic();
	~CCSetMusic();

	bool init();
	void initSwitch();
	
	CREATE_FUNC(CCSetMusic);

	void menuBackMainMenu(Ref *pSender);
	void menuMusic(Ref *pSender);
	void menuEffect(Ref *pSender);

	virtual void onEnter();
	virtual void onExit();
private:
	bool bSoundEffect;
	bool bMusic;
	Sprite *pMusic;
	Sprite *pSoundEffect;
	SpriteFrame *pMusicOn;
	SpriteFrame *pMusicOff;
	SpriteFrame *pEffectOn;
	SpriteFrame *pEffectOff;
};

#endif