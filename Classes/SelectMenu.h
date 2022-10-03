#ifndef _SELECTMENU_H_
#define _SELECTMENU_H_

#include "cocos2d.h"
using namespace cocos2d;

class CCSelectMenu : public cocos2d::Layer
{
public:
	virtual bool init();

	void menuCloseCallback(Ref* pSender);

	CREATE_FUNC(CCSelectMenu);

public:
	virtual void onEnter();
	virtual void onExit();

	virtual bool onTouchBegan(Touch *pTouch, Event *pEvent);
	virtual void onTouchMoved(Touch *pTouch, Event *pEvent);
	virtual void onTouchEnded(Touch *pTouch, Event *pEvent);
	virtual void onTouchCancelled(Touch *pTouch, Event *pEvent);

	void menuBegin(Ref *pSender);
	void menuBack(Ref *pSender);
private:
	//根据手势滑动的距离和方向滚动图层
	void adjustView(float offset);
	Point m_touchPoint;
	int m_nCurPage;
	Layer *pLayer;
    Label *m_pLevel;
	MenuItemImage *pNewGame;
};


#endif
