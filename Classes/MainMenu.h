#ifndef _MAINMENU_H_
#define _MAINMENU_H_

#include "cocos2d.h"
using namespace cocos2d;

class MainMenu : public Layer
{
public:
	MainMenu();
	~MainMenu();

    MenuItem *pStartGame;
    MenuItem *pQuit;
    MenuItem *pAbout;
    MenuItem *pSetting;

	Menu *pMenu;

	void menuCallBackForStartGame(Ref *pSender);
	void menuCallBackForAbout(Ref *pSender);
	void menuQuit(Ref *pSender);
	void menuSetting(Ref *pSender);

	bool init();


	virtual bool onTouchBegan(Touch *pTouch, Event *pEvent);
	virtual void onTouchMoved(Touch *pTouch, Event *pEvent);
	virtual void onTouchEnded(Touch *pTouch, Event *pEvent);
	virtual void onTouchCancelled(Touch *pTouch, Event *pEvent);


	CREATE_FUNC(MainMenu);

private:

};


#endif
