#ifndef _GAME_ENUM_H_
#define _GAME_ENUM_H_

// 不仅表示动画的的获取类型，还表示了当前马里奥的状态
// 马里奥的状态:
// eRight：       正常状态（向右行走状态中）
// eLeft:         向左走的状态中
// eJumpLeft:     向左跳跃状态
// eJumpRight:    向右跳跃状态

enum marioDirection
{
	eLeft,
	eRight,
	eJumpLeft,
	eJumpRight,
	eNormalRight,
	eNormalLeft,
	eFireTheHole
};

enum GameElement
{
	eElement_GameLayer,
	eElement_GameMap
};

// 马里奥的身体尺寸
enum BodyType
{
	eBody_Normal,
	eBody_Small,
	eBody_Fireable
};

// 动画类型
enum AnimationType
{
	eAniLeft,
	eAniRight,
	eAniLeftSmall,
	eAniRightSmall,
	eAniLeftFire,
	eAniRightFire,
	eAniBlinkCoin,
	eAniMushroom,
	eAniflower,
	eAniTortoiseLeft,
	eAniTortoiseRight,
	eAniTortoiseFly,
	eAniRotatedFireBall,
	eAniBoomedFireBall,
	eAniFireActionR,
	eAniFireActionL,
	eAniFlyFishR,
	eAniFlyFishL,
	eAniArrowBroken,
	eAniSmallDie,
	eAniNormalDie,
	eAniFireDie,
	eAniBossMoveLeft,
	eAniBossMoveRight,
	eAniBossFireLeft,
	eAniBossFireRight,
	eAniBatteryBoom,   // 魂斗罗炮台爆破
	eAniLighting,      // 闪电动画
	eAniArrowLeft,
	eAniArrowRight,
	eAniArrowDie,
	eAniArrowActionL,
	eAniArrowActionR
};

// 游戏元素类型，闪烁的金币等
enum ItemType
{
	eBlinkCoin
};

// 地图中图块元素的种类，有以下这些是碰撞检测需要的
enum TileType
{
	eTile_Land,   // 地板
	eTile_Block,  // 金币等小块块
	eTile_Pipe,   // 管道
	eTile_Trap,   // 陷阱
	eTile_Coin,   // 金币
	eTile_NoneH,  // 代表水平方向上未检测出碰撞物
	eTile_NoneV,   // 代表垂直方向上未检测出碰撞物
	eTile_Flagpole  // 旗杆
};

// 怪物类型
enum EnemyType
{
	eEnemy_mushroom,
	eEnemy_flower,
	eEnemy_tortoise,
	eEnemy_tortoiseRound,
	eEnemy_tortoiseFly,
	eEnemy_fireString,
	eEnemy_flyFish,
	eEnemy_Boss,
	eEnemy_BossFire,  // Boss发射的子弹
	eEnemy_AddMushroom,   // 可以冒出一定量蘑菇的怪物
	eEnemy_Battery,        // 魂斗罗里的炮台
	eEnemy_BatteryBullet,           // 魂斗罗炮台发出的子弹
	eEnemy_DarkCloud,           // 乌云
	eEnemy_Lighting             // 闪电
};

// 怪物状态，有活跃，非活跃，死亡
enum EnemyState
{
	eEnemyState_active,
	eEnemyState_nonactive,
	eEnemyState_over
};

// 怪物与主角碰撞检测的结果，有怪物被踩死的，有主角被怪物碰死的，还有没有碰撞到的
enum EnemyVSHero
{
	eVS_enemyKilled,
	eVS_heroKilled,
	eVS_nonKilled
};

// 子弹类型
enum BulletType
{
	eBullet_common,
	eBullet_arrow
};

// 子弹的活动状态，刚发出去的子弹是活动的，但是打到怪物或是碰到相关障碍物就为非活动状态
enum BulletState
{
	eBulletState_active,
	eBulletState_nonactive
};

// 小玩意的类型
enum GadgetType
{
	eGadget_LadderUD,  // 上下梯子
	eGadget_LadderLR   // 左右梯子

};

// 小玩意的状态，离开屏幕后就自动销毁掉
enum GadgetState
{
	eGadgetState_active,
	eGadgetState_nonactive,
	eGadgetState_over
};

// 当从游戏界面返回到主菜单时的原因
enum ToMainMenuFor
{
	efor_PassFailure,
	efor_PassSuccess,
	efor_Non,
	efor_StartGame,
	efor_GameOver,
	efor_PassAll
};

#endif