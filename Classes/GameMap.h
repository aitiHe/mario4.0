#ifndef _GAMEMAP_H_
#define _GAMEMAP_H_

#include "cocos2d.h"
#include "GameEnum.h"
using namespace cocos2d;

class CCItem;
class CCEnemy;
class CCGadget;

class CCGameMap : public TMXTiledMap
{
	// 只读变量，四个游戏中要用到的图层
	// 云层用于移动

	CC_PROPERTY_READONLY(TMXLayer*, cloudLayer, CloudLayer);
	// 剩余的四个图层是为了碰撞检测
	CC_PROPERTY_READONLY(TMXLayer*, blockLayer, BlockLayer);
	CC_PROPERTY_READONLY(TMXLayer*, pipeLayer, PipeLayer);
	CC_PROPERTY_READONLY(TMXLayer*, landLayer, LandLayer);
	CC_PROPERTY_READONLY(TMXLayer*, trapLayer, TrapLayer);
	// 对象层
	CC_PROPERTY_READONLY(TMXObjectGroup*, objectLayer, ObjectLayer);
	// 金币层
	CC_PROPERTY_READONLY(TMXLayer*, coinLayer, CoinLayer);
	// 旗杆层
	CC_PROPERTY_READONLY(TMXLayer*, flagpoleLayer, FlagpoleLayer);
public:
	CCGameMap();
	~CCGameMap();

	//// 与地图信息相关
	Size tileSize; // 地图中的图块尺寸
	Size mapSize;  // 地图中图块的个数

	// 这两个点是在过关成功后展示动画用到的
	Point flagPoint;  // 小旗子的点
	Sprite *pFlag;
	void showFlagMove();  // 展示旗子从旗杆上落下的动画
	Point finalPoint;  // 最后马里奥要移动到堡垒门口的点
	bool isBossMap;  // 标识是否为Boss关

	void createNewBulletForBoss(Point pos, EnemyType _enemyType); // Boss发出的一个子弹



	// 普通的吃了会变大的蘑菇出生点
	// 这种蘑菇点会根据当前马里奥的身型大小来判断顶出来的是蘑菇还是子弹
	// 记录的蘑菇点是基于图块坐标系下的
	PointArray *pMushroomPointArray;

	Sprite *pAddLifeMushroom;  // 奖人蘑菇精灵
	Point addLifePoint;  // 每一关中都有一个奖人的蘑菇点

	// 地图上保存的所有怪物的信息数组
	Vector<Node*> EnemyArray;

	// 地图上所有的子弹的信息
    Vector<Node*> BulletArray;

	// 地图上所有的小物件数组信息
    Vector<Node*> GadgetArray;
	// 当前主角在那个物件上
	CCGadget *heroInGadget;

	// 记录当前屏幕中的怪物数组
	//Array *pEnemyInScreen;
	bool isHeroInGadget(Point heroPos, float &gadgetLevel);

	Sprite *pMushSprite;  //当前被顶出的蘑菇精灵
	Point mushTileCoord;  // 当前蘑菇所在的位置

	CCItem *pItem; // 当前被顶的闪烁的问号

	SpriteFrame *_brokenCoin;  // 带问号的金币被顶开后要被替换掉的纹理样式

	// 当前需要被重设的金币砖块位置
	Point resetCoinPoint;

	// 每张地图中马里奥的出生点，GL坐标系下的坐标，CCGameLayer获取后直接可用的
	Point marioBirthPos;
	Point getMarioBirthPos();

	static CCGameMap* create(const char *tmxFile);

	// 返回地图指定位置上的图块类型，用作碰撞检测
	TileType tileTypeforPos(Point tileCoord);

	// 地图上的block图层上的某些砖块是可以被主角顶的
	void breakBlock(Point tileCoord, BodyType _bodyType);

	// 展示block图层上的某些砖块被大马里奥顶碎成四块的动画
	void showBlockBroken(Point tileCoord);
	// 展示闪烁的问号被顶出金币的动画
	void showJumpUpBlinkCoin(Point tileCoord);
	// 展示砖块被小马里奥顶一下的效果
	void showBlockJump(Point tileCoord);
	// 无论是马里奥大或是小，金币被顶出来的过程中会伴随着金币的砖块上移动作，类似于showJumpBlock
	void showCoinJump(Point tileCoord);
	// 在展示完了金币砖块/蘑菇砖块的上移动作后，要重新设置金币砖块是被顶过的状态纹理
	void resetCoinBlockTexture();
	// 生成一只蘑菇，根据当前的马里奥状态来判断要生成的是变大蘑菇还是子弹蘑菇
	void showNewMushroom(Point tileCoord, BodyType _bodyType);
	// 生成一只奖人蘑菇
	void showAddLifeMushroom(Point tileCoord);

	// 判断马里奥是否吃到蘑菇
	bool isMarioEatMushroom(Point tileCoord);
	// 判断是否迟到奖人蘑菇
	bool isMarioEatAddLifeMushroom(Point tileCoord);

	// 启动地图上的所有怪物，开启动画
	void launchEnemyInMap();

	// 启动地图上所有的物件
	void launchGadgetInMap();

	void update(float dt);
	// 停止怪物和子弹的更新，然后再停止自己的更新
	void stopUpdateForHeroDie();

	void pauseGameMap();  // 暂停地图上的各种会动的
	void resumeGameMap();  // 重新动起来

	// 主角逐个的与active状态中的怪物进行相碰检测，然后表现出受伤逻辑
	void enemyVSHero();

	// 游戏图层坐标系与tilemap图块坐标系之间的转换
	Point positionToTileCoord(Point pos);
	Point tilecoordToPosition(Point tileCoord);

	// 产生一个新的子弹，并启动起来
	void createNewBullet();

	// 子弹与怪物的碰撞检测
	void bulletVSEnemy();

	static CCGameMap *_gameMap;
	static CCGameMap* getGameMap();
private:
	void deleteOneMushPointFromArray(Point tileCoord);
	void clearItem();
	void clearSpriteArray();
	void extraInit();
	void initObjects();
	bool itemCoordArrayContains(Point tileCoord);
	bool mushroomPointContains(Point tileCoord);
	PointArray *pItemCoordArray;
	Vector<Node*> spriteArray;

	// 最后Boss关准备的变量
	Point bridgeTileStartPos;
	int bridgeTileNums;
	CCEnemy *pBossEnemy;
	void initBridgeArray();
	Sprite *pPrincess;  // 公主精灵

	// 用于顶砖块随机生成怪物的临时变量
	Point enemyTilePos;
	CCEnemy *pRandomEnemy;
	void randomShowEnemy();
	void randomLaunchEnemy();
};

#endif
