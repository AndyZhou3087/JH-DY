/********************************************************************
*角色装备栏
*********************************************************************/
#ifndef _HEROPROPER_H_
#define _HEROPROPER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "Hero.h"
USING_NS_CC;


class HeroProperNode :public Node
{
public:
	HeroProperNode();
	~HeroProperNode();

	virtual bool init();
	virtual void onEnterTransitionDidFinish();
	virtual void onExit();
	CREATE_FUNC(HeroProperNode);

	/****************************
	刷新map_carryData 列表
	*****************************/
	void refreshCarryData();

	/****************************
	显示新手引导
	*****************************/
	void showNewerGuide(int step);

	/****************************
	选择装备
	*****************************/
	void selectCarryData();

	/****************************
	刷新装备栏中的功法，功法组合时选择中的会清除掉，
	@param 功法类型
	*****************************/
	void refreshGF(HeroAtrType atrype);

	/****************************
	移除组合标签
	*****************************/
	void removeMixTag();

private:

	/****************************
	点击“确定”按钮回调
	*****************************/
	void onOK(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	/****************************
	点击每个物品栏回调
	*****************************/
	void onImageClick(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	/****************************
	弹出装备选择框
	@param 选中的类型
	*****************************/
	void showSelectFrame(HeroAtrType index);

	//装备的滚动控件
	cocos2d::ui::ScrollView* m_scrollView;

	/****************************
	选中装备装上或卸下
	@param 点击的item
	*****************************/
	void onItem(Ref* pSender);

	/****************************
	选择另外一种类型的移除掉之前的item
	*****************************/
	void removeitem();

	/****************************
	将装备添加到map_carryData 列表
	@param 装备类型
	*****************************/
	void addCarryData(HeroAtrType index);

	/****************************
	更新属性面板
	*****************************/
	void updataProperpanel(int atrypeindex, PackageData pdata);

	/****************************
	更新背包栏数据
	*****************************/
	void updataMyPackageUI();

	/****************************
	带上装备
	@param 装备类型
	@param 装备数据
	*****************************/
	void takeon(HeroAtrType atrype, PackageData pdata);

	/****************************
	卸下装备
	@param 装备类型
	@return 是否卸下成功，背包满了就失败
	*****************************/
	bool takeoff(HeroAtrType atrype);

	/****************************
	检查性别，合成功法根据性别可以装备
	@param 性别
	@return >0可装备 只适合0--自宫，-1-男，-2-女 -3男女
	*****************************/
	int checkSex(int sex);

	/****************************
	检查是否能带上装备
	@return true:能装备上，false:不能装备上
	*****************************/
	bool isCanTakeOn();

private:
	Node* csbroot;
	EventListenerTouchOneByOne* m_listener;
	cocos2d::ui::ImageView* propeImages[8];//8种装备类型
	cocos2d::ui::ImageView* imgbtn[8];//8种装备框
	cocos2d::ui::Text* lvtext[8];//8种装备等级或者耐久度
	cocos2d::ui::Widget* heroselectbg;//选择装备背景
	cocos2d::ui::Widget* amountdesc;//坐骑说明
	cocos2d::ui::Widget* heroppoint;//装备背景上的三角形
	cocos2d::ui::Text* title;//标题
	int lastclickindex;//上次选中装备栏类型index
	bool isout;//是否在外面
	std::map<HeroAtrType, std::vector<PackageData>> map_carryData;//装备栏数据
	Sprite* m_select;//选中标识
	PackageData* m_lastSelectedData;//上次选中中的装备数据
	int m_step;//新手引导索引
	HeroAtrType m_select_atrype;
	Node* m_select_itemnode;
	PackageData* m_select_udata;
};
#endif

