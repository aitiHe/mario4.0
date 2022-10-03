#ifndef _ABOUTMENU_H_
#define _ABOUTMENU_H_

#include "cocos2d.h"
using namespace cocos2d;

class CCAbout : public Layer
{
public:
	CCAbout();
	~CCAbout();

	bool init();
	CREATE_FUNC(CCAbout);

	void menuBack(Ref *pSender);
private:

};

#endif