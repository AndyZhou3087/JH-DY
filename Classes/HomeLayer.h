
/********************************************************************
*“家”界面
*********************************************************************/
#ifndef _HOME_LAYER_H_
#define _HOME_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"

#include "Building.h"
USING_NS_CC;
class HomeLayer:public Layer
{
public:
	HomeLayer();
	~HomeLayer();

	virtual bool init();

	CREATE_FUNC(HomeLayer);

	/****************************
	更新建筑物图标，没有建造显示一推木材
	*****************************/
	void updateBuilding();

	void showNewerGuide(int step);
	/****************************
	新手引导
	*****************************/
	void checkNewerGuide();

	void delayShowNewerGuide(float dt);

private:
	/****************************
	点击建筑物
	*****************************/
	void onclick(Ref* pSender);

	/****************************
	点击仓库
	*****************************/
	void onStorageRoom(Ref* pSender);

	/****************************
	点击围墙
	*****************************/
	void onFence(Ref* pSender);
	
	/****************************
	建筑物JSON数据
	*****************************/
	void loadJsonData();

private:
	Building* m_storageroom;//仓库
	Building* m_fence;//围墙
	std::vector<Building*> Vec_Buildings;//建筑物vec
	cocos2d::ui::Widget* m_badfurnace;//没有建造暖炉之前显示的图标
	cocos2d::ui::Widget* m_badbookshelf;//没有建造书架之前显示的图标
};
#endif

