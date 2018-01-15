/********************************************************************
* 建筑物UI界面
*********************************************************************/
#ifndef _BUIDINGUI_LAYER_H_
#define _BUIDINGUI_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "Building.h"
#include "GlobalData.h"
#include "ServerDataSwap.h"
USING_NS_CC;

typedef enum
{
	BUILD,//建造或者升级
	ACTION//每个建筑物自己的操作，eg:床：睡觉；药箱：制药...
}BACTIONTYPE;

class BuildingUILayer :public Layer, public ServerDataDelegateProtocol
{
public:
	BuildingUILayer();
	~BuildingUILayer();

	virtual bool init(Building* build);
	virtual void onEnterTransitionDidFinish();

	static BuildingUILayer* create(Building* build);

	/****************************
	取消闭关
	*****************************/
	void resetExercise();

	void getServerTime();

	/****************************
	延迟显示新手引导
	*****************************/
	void checkNewerGuide();

private:

	/****************************
	点击返回按钮回调
	*****************************/
	void onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	/****************************
	点击建筑物自己的操作按钮回调
	*****************************/
	void onAction(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	/****************************
	点击建筑物图标回调，显示详细信息
	*****************************/
	void onBuidingDetails(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	/****************************
	点击资源回调，显示详细信息
	*****************************/
	void onResDetails(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	/****************************
	更新建筑物等级资源UI
	****************************/
	void updataBuildRes();

	/****************************
	延迟加载建筑物自己操作的UI
	*****************************/
	void delayLoadActionUi(float dt);

	/****************************
	加载建筑物自己操作的UI
	****************************/
	void loadActionUi();

	/****************************
	添加建筑物自己操作的UI到scrollview
	****************************/
	void setActionScrollViewUI();

	/****************************
	更新建筑物自己操作的UI
	****************************/
	void updataActionRes();

	/****************************
	新手引导
	*****************************/
	void showNewerGuide(int step);

	/****************************
	更新闭关的说明
	*****************************/
	void updateExerciseDesc();

	/****************************
	更新闭关剩余时间
	*****************************/
	void updateExerciseLeftTime(float dt);

	/****************************
	点击锻造台分类按钮回调
	*****************************/
	void onCategory(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	/****************************
	锻造台分类加载
	@category 分类
	*****************************/
	void loadActionUIByCategory(int category);

	/****************************
	延迟显示分类
	*****************************/
	void delayShowCategoryActionUI(float dt);

	void onSuccess();
	void onErr(int errcode);

private:
	Building* m_build;
	Node* buildnode;//每个ITEM NODE
	Node* m_csbnode;//UI NODE
	cocos2d::ui::LoadingBar* buildbar;//建造进度条控件
	cocos2d::ui::Button* buildbtn;//建造按钮控件
	cocos2d::ui::ScrollView* scrollview;//操作滚动列表控件
	cocos2d::ui::Widget* m_backbtn;//返回按钮控件
	std::vector<cocos2d::ui::LoadingBar*> vec_actionbar;//建筑物自己的操作 进度条控件
	std::vector<cocos2d::ui::Button*> vec_actionbtn;//建造进度条控件
	std::vector<cocos2d::ui::Text*> vec_progresstext;//进度条上的文字控件，这里用来显示时间
	void onfinish(Ref* pSender, BACTIONTYPE type);//建造，建筑物自己的操作 完成

	void ondivideSucc(Ref* pSender, BACTIONTYPE type, Node* divideLayer);//分解操作 完成
	void onExercisefinish(int index);//闭关完成

	void showFinishHintText(std::string path);//显示完成后的提示文字
	void finishAnim(Ref* pSender, Node* node);//建造，建筑物自己的操作 完成后的提示动画
	std::vector<Node*> vec_actionItem;//建筑物自己操作 item node
	Label* m_loadlbl;//加载文字
	int selectActionIndex;//闭关选择的item索引
	int estarttime;//闭关开始的时间
	std::string ex_wgstrid;//闭关时装备的外功ID;
	std::string ex_ngstrid;//闭关室装备的内功ID
	std::vector<cocos2d::ui::Button*> vec_categoryBtn;//锻造台分类按钮
	std::vector<BuildActionData> vec_buildAcitonData;//建筑物操作的数据
	int lastCategoryindex;
	bool isDraging;
	Vec2 startPos;
	int exersiceTag;
	int curtime;
};
#endif

