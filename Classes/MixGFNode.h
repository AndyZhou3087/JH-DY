/********************************************************************
*功法合成栏
*********************************************************************/
#ifndef _MIXGFNODE_H_
#define _MIXGFNODE_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "Hero.h"
USING_NS_CC;


class MixGFNode :public Node
{
public:
	MixGFNode();
	~MixGFNode();

	virtual bool init();
	virtual void onEnterTransitionDidFinish();
	virtual void onExit();
	CREATE_FUNC(MixGFNode);

	/****************************
	刷新map_carryData 列表
	*****************************/
	void refreshCarryData();
	bool getRandomBoolean(float rate);
	bool getRandomBoolean();
	int getRandomNum(int range);
	int getRandomNum(int rangeStart, int rangeEnd);
	/****************************
	保存数据
	*****************************/
	static void saveData();

	void showTalkGuide();

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
	点击每个物品栏回调
	*****************************/
	void onMix(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void shake(cocos2d::Node * node, float scaleLarge, float scaleSmall);
	void shake(cocos2d::Node * node);
	/****************************
	点击提示旁的人物，推荐组合
	*****************************/
	void onSuggest(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	/****************************
	添加功法数据
	*****************************/
	void addGFData();

	/****************************
	加载组合好的功法
	*****************************/
	void loadMixSuccGF();

	//装备的滚动控件
	cocos2d::ui::ScrollView* m_scrollView;

	/****************************
	选中装备装上或卸下
	@param 点击的item
	*****************************/
	void onItem(Ref* pSender);
	void jump(cocos2d::Node *node, float dt, bool repeat = false, float intrval = 0);
	void jellyJump(cocos2d::Node *node, float dt, bool repeat = false, float intrval = 0, int tag = 0);
	void petJump(cocos2d::Node *node, float dt, bool repeat = false, float intrval = 0, int tag = 0, cocos2d::ActionInterval *ac = nullptr);
	void jelly(cocos2d::Node *node, bool repeat = false, float intrval = 0, bool delay = false, int tag = 0);
	/****************************
	更新可选功法
	*****************************/
	void updateGFScroll();

	/****************************
	更新组合功法说明
	*****************************/
	void updateDesc();


	void jumpDown(cocos2d::Node *node, float dt);

	bool isPhone();

private:
	Node* csbroot;
	cocos2d::ui::Widget* heroselectbg;
	cocos2d::ui::ImageView* imgbtn[4];
	cocos2d::ui::ImageView* img[4];
	EventListenerTouchOneByOne* m_listener;
	cocos2d::ui::Widget* mixokbtn;
	cocos2d::ui::Text* mixtitle;
	cocos2d::ui::ImageView* suggestbtn;
	cocos2d::ui::ImageView* shade;
	int lastclickindex;
	Sprite* m_select;
	Sprite* m_secselect[3];
	PackageData* lastselectitem;
	std::vector<PackageData> vec_myhasgf;
	PackageData* masterGFData;
	std::map<int, std::vector<PackageData*>> map_secgfdata;
	cocos2d::ui::Text* desc0;
	cocos2d::ui::Text* desc1;
	cocos2d::ui::Text* desc;

};
#endif

