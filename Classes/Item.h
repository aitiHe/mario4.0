#ifndef _ITEM_H_
#define _ITEM_H_

#include "cocos2d.h"
#include "GameEnum.h"
using namespace cocos2d;

class CCItem : public Node
{
public:
	CCItem(ItemType _itemtype);
	~CCItem();
	Sprite *itemBody;   // 主要是block层的item对应的精灵
	ItemType itemType;    // Item的类型

	static CCItem* create(ItemType _itemtype);

private:
	bool itemInit();
};

// 小玩意，会移动的，可供主角站立的，丰富游戏性的小道具
class CCGadget : public Node
{
public:
	CCGadget();
	virtual ~CCGadget();

	// 下面两个方向上的更新，可能因人而异
	virtual void gadgetUpdateH();
	virtual void gadgetUpdateV();

	// 纯虚函数
	virtual void launchGadget() = 0;

	float getMoveOffset();
	float getJumpOffset();

	void setStartPos(Point pos);
	Point getStartPos();

	void setStartFace(int face);

	// 获取当前物件的矩形，为了检测主角是否在此物件上
	Rect getGadgetRect();

	// 
	Size getGadgetSize();

	GadgetState getGadgetState();

	// 检测状态
	virtual void checkGadgetState();

protected:
	Sprite *gadgetBody;
	Size bodySize;
	GadgetType gadgetType;  //小玩意的类型
	GadgetState gadgetState;  // 小玩意的状态

	float moveOffset;
	float ccMoveOffset;

	float jumpOffset;
	float ccJumpOffset;

	Point startPos;  // 起初位置

	// 第一次移动的方向，有0和1选项
	// 在LR中0代表左，1代表右
	// 在UD中0代表上，1代表下
	int startFace;

};

// 左右移动的梯子
class CCGadgetLadderLR : public CCGadget
{
public:
	CCGadgetLadderLR(float dis);
	~CCGadgetLadderLR();

	void gadgetUpdateH();

	void launchGadget();

	void setLRdis(float dis);

	void update(float dt);

private:
	float leftSide;    // 左边界
	float rightSide;   // 右边界
	float lrDis;       // 左右距离
};

class CCGadgetLadderUD : public CCGadget
{
public:
	CCGadgetLadderUD(float dis);
	~CCGadgetLadderUD();

	void gadgetUpdateV();

	void launchGadget();

	void setUDdis(float dis);

	void update(float dt);

private:
	float upSide;      // 上边界
	float downSide;    // 下边界
	float udDis;       // 上下距离
};

#endif