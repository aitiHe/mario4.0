#ifndef _GAMELAYER_H_
#define _GAMELAYER_H_

#include "cocos2d.h"
#include "AudioEngine.h"
//ben#include "touch_dispatcher\Touch.h"
using namespace cocos2d;

// ���������Ϸ����ͼ�㣬���е���Ϸ���ݶ�����һ��ͼ���н���
// �������Ҫ����һ�ŵ�ͼ������һ������
// �����ͼ��֮�ϸ�����ײ���
// ���۶���ͼ��֮�ϸ���ʵʱ����
// ����һЩ������λ�ñ仯��صı�������Щ��������ģ�������Ч��

class CCGameMap;
class Hero;
class CCGameLayer : public Layer
{
public:

	
	Layer *mainLayer;  // ����Ϸͼ��
	CCGameMap *mainMap;  // ����Ϸ��ͼ
	Hero *hero;        // ��Ϸ�е�����
	Point birthPoint;  // ���ǵĳ�����
	Size heroSize;     // ���ǵĳߴ�
	Size mapSize;      // ��ͼ�ĳߴ�
	Point heroAnchor;  // �����ߵ���Ļ�ĸõ���ͼ�Ϳ�ʼ����
	static float mapMaxH;       // ˮƽ��������Ϸ����ͼmainMap������X����������ֹ��ͼ���˳���Ļ
	Point mapBeginPos; // ��ͼ��λ������Ϸͼ���е�λ��

	// ģ������Ч����ر���
	float ccMoveOffset;  // ÿ֡Ҫ�ƶ��ľ���
	float ccMoveDelta;   // Ϊ��ģ������ٶ�Ч�����ƶ�����
	float ccJumpOffset;  // 
	float moveDelta;     
	float moveOffset;  // ˮƽ�ƶ�ƫ����
	float JumpOffset;  // ��ֱ��Ծ��ƫ����

	Point currentPos;  // ���ǵ�ǰ��λ��
	bool isKeyDownA;  // ��ʶA���Ƿ񱻰���
	bool isKeyDownD;  // ��ʶD���Ƿ񱻰���
	bool isKeyDownJump;  // ��ʶ��Ծ���Ƿ񱻰���
	bool isSky;        // ��ʶ�����Ƿ��ڿ���״̬

	//CCMultiTouchMenu     *pMenu;  // ��Ϸ�����ϵ�UI���Զ�����
	Menu *pMenu;
	MenuItemImage      *pLeftKey;
	MenuItemImage      *pRightKey;
	MenuItemImage      *pJump;
	MenuItemImage      *pFire;
	MenuItemImage      *pMSet;

	Sprite *pGameOverBack;  // ������������ʾ�ĵı���
	Sprite *pPassSuccess;
	Sprite *pPassFailure;
	MenuItemImage *pRetry;  // ���Ե�ǰ��
	MenuItemImage *pNext;   // ��һ��
	MenuItemImage *pBackToMenu;  // ���ص����˵��İ�ť

	Sprite *controlUI;

	Sprite *pBackKeyImage;
	SpriteFrame *pBackKeyNormal;
	SpriteFrame *pBackKeyLeft;
	SpriteFrame *pBackKeyRight;
	Sprite *pJumpImage;
	Sprite *pFireImage;
	SpriteFrame *pAB_Normal;
	SpriteFrame *pAB_Selected;

	Point backKeyPos;
	Point leftKeyPos;
	Point rightKeyPos;
	Point jumpKeyPos;
	Point fireKeyPos;
	Point MSetKeyPos;  // ���ü�

	void pauseGameLayer();  // ��ͣ��Ϸ
	void resumeGameLayer();

	void menuMSet(Ref *pSender);
	void menuCallBackLeftKey(Ref *pSender);
	void menuCallBackRightKey(Ref *pSender);
	void menuCallBackJumpKey(Ref *pSender);
	void menuCallBackFireKey(Ref *pSender);
	void menuCallBackBackToMenu(Ref *pSender);
	void menuRetry(Ref *pSender);  // ���Ա���
	void menuNext(Ref *pSender);   // ��һ��
	
	void showPassAll();  // ͨȫ��֮��Ķ���

	void initSetMenu();  // ��ʼ�����ý���
	LayerColor *pColor;
	Sprite *pSetMenu;  // ���ý���ı���
	MenuItemImage *pResume;  //  ������Ϸ
	MenuItemImage *pReStart;  // ���¿�ʼ
	MenuItemImage *pSelectMenu;  // ѡ�ؽ���
	void menuResume(Ref *pSender);
	void menuReStart(Ref *pSender);
	void menuSelectMenu(Ref *pSender);

	Sprite *pBulletBorderFireBall;          // �����߿�
	Sprite *pBulletBorderArrow;
	MenuItemSprite *pMenuFireBall;  // ����ѡ��ť
	MenuItemSprite *pMenuArrow;     // ����ѡ��ť

	Point FireBallPos;
	Point arrowPos;

	void menuCallBackFireBall(Ref *pSender);
	void menuCallBackArrow(Ref *pSender);

	void stopForPassFailure();
	void reShowPassFailure();
	
	void stopForPassSuccess();
	void reShowPassSuccess();
	void showHeroJump();  // չʾͨ�ض����е��������Ծ

	void reSetKeyNo();

	bool isPass;

	bool heroFireable;  // Ϊ�˿����ӵ������Ƶ�ȣ�����һ�����䰲ȫ��
	void reSetHeroFireable();

	CCGameLayer();
	~CCGameLayer();

	bool init();
	void initHeroAndMap();
	void initcontrolUI();
	void initBulletUI();


	void loadMap();

	void updateControl();
	bool isLeftKeyDown;
	bool isRightKeyDown;
	bool isJumpKeyDown;
	bool isFireKeyDown;

	Rect leftKeyRect;
	Rect rightKeyRect;
	Rect jumpKeyRect;
	Rect fireKeyRect;
	void initRect();

	void update(float dt);
	void updateContent(float dt);
	void updateDelta(float dt);
	void collistionV();  // ˮƽ�������ײ���
	void collistionH();  // ��ֱ�������ײ���
	void setSceneScrollPosition();  // �������ǵĵ�ǰλ���ƶ����������ʵ�λ��

	static float getMapMaxH();


	virtual void onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event);
	virtual void onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event);
	virtual void onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event);
	virtual void onTouchesCancelled(const std::vector<Touch*>&touches, Event *unused_event);

	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);

	//void processWin32KeyPress(UINT message, WPARAM wParam, LPARAM lParam);

	void menuCloseCallBack(Ref *pSender);

	void toMainMenu();
	
	CREATE_FUNC(CCGameLayer);
};

#endif
