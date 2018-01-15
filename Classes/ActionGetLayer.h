/****************************
资源采集界面
****************************/
#ifndef _ACTIONGET_LAYER_H_
#define _ACTIONGET_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "MyPackage.h"
USING_NS_CC;
class ActionGetLayer :public Layer
{
public:
	ActionGetLayer();
	~ActionGetLayer();

	/****************************
	初始化
	@param rid 资源列表中的id
	@param res_ids 获取产出的资源id,可能会有产出多种资源
	@param type 资源列表中类型StorageType
	@param actype 用于工具类，1："采集", 2："砍伐", 3："挖掘"
	****************************/
	bool init(int rid, std::vector<int> res_ids, int type, int actype);
	virtual void onExit();
	static ActionGetLayer* create(int rid, std::vector<int> res_ids, int type, int actype);

	/****************************
	更新背包栏资源
	*****************************/
	void updataMyPackageUI();

	/****************************
	更新奖励栏资源
	*****************************/
	void updataRewardUI();

	/****************************
	新手引导
	*****************************/
	void showNewerGuide(int step);

private:
	/****************************
	点击1："继续采集", 2："继续砍伐", 3："继续挖掘"按钮回调
	*****************************/
	void onGet(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	/****************************
	点击返回按钮回调
	*****************************/
	void onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	/****************************
	点击全部拾取按钮回调
	*****************************/
	void onAllGet(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	/****************************
	更新背包，奖励栏资源
	*****************************/
	void updata();

	/****************************
	点击奖励栏添加到背包栏
	*****************************/
	void onRewardItem(cocos2d::Ref* pSender);

	/****************************
	点击背包栏添加到奖励栏
	*****************************/
	void onPackageItem(cocos2d::Ref* pSender);

	/****************************
	获取资源
	*****************************/
	void doAction(float dt);

	/****************************
	没有拾取的资源保存到临时存放地点
	*****************************/
	void saveTempData();

	/****************************
	加载临时存放地点的数据
	*****************************/
	void loadTempData();

	/****************************
	设置按钮状态
	@param 是否可点击
	*****************************/
	void setBtnStatus(bool enable);

	/****************************
	显示操作文字
	*****************************/
	void addEventText();

	/****************************
	显示.....(没有工具是操作要延迟一会)
	*****************************/
	void showDotDot(float dt);

	/****************************
	显示操作文字2
	*****************************/
	void addEventText2(float dt);

	/****************************
	延迟获取资源
	*****************************/
	void delayDoAction(float dt);

private:
	int mrid;//后山资源列表中的ID
	std::vector<int> rewardids;//1："采集", 2："砍伐", 3："挖掘"产出的ID
	std::vector<PackageData> getResData;//操作（1："采集", 2："砍伐", 3："挖掘"）获取的资源数据（奖励栏中的资源数据）
	std::vector<PackageData> tempResData;//临时存放的资源数据
	int mtype;//类型
	int m_actype;//工具类型 1："采集", 2："砍伐", 3："挖掘"
	cocos2d::ui::Button* m_getbtn;//继续操作按钮
	cocos2d::ui::Button* m_getallbtn;//全部拾取按钮
	cocos2d::ui::Widget* m_backbtn;//返回按钮
	int repeatCount;//重复操作次数
};
#endif

