
/********************************************************************
*地点显示界面
*********************************************************************/
#ifndef _GOWHERE_LAYER_H_
#define _GOWHERE_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

/****************************
地点操作类型
*****************************/
typedef enum
{
	GOWHERE = 0,//准备去
	ARRIVE,//到达
	ONWAY_JUMP//中途奇遇（跳崖）
}WHERELAYER_TYPE;

class WXGoWhereLayer :public Layer
{
public:
	WXGoWhereLayer();
	~WXGoWhereLayer();

	virtual void onEnterTransitionDidFinish();
	/****************************
	初始化
	@param 地点id
	@param 地点操作类型
	@param 目的地距离，像素单位
	*****************************/
	bool init(std::string addrname, WHERELAYER_TYPE type, float distance = 0.0f);
	static WXGoWhereLayer* create(std::string addrid, WHERELAYER_TYPE type, float distance = 0.0f);
	bool getRandomBoolean(float rate);
	bool getRandomBoolean();
	int getRandomNum(int range);
	int getRandomNum(int rangeStart, int rangeEnd);
	/****************************
	新手引导
	*****************************/
	void checkNewerGuide();

	/****************************
	新手引导
	*****************************/
	void showNewerGuide(int step);
	void shake(cocos2d::Node * node, float scaleLarge, float scaleSmall);
	void shake(cocos2d::Node * node);
private:

	/****************************
	点击“算了”按钮回调
	*****************************/
	void onClose(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	/****************************
	点击“出发”按钮回调
	*****************************/
	void onGO(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void petJump(cocos2d::Node *node, float dt, bool repeat = false, float intrval = 0, int tag = 0, cocos2d::ActionInterval *ac = nullptr);
	void jelly(cocos2d::Node *node, bool repeat = false, float intrval = 0, bool delay = false, int tag = 0);
	/****************************
	点击“物品存放”按钮回调
	*****************************/
	void onST(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	/****************************
	点击“进入”按钮回调
	*****************************/
	void onComeIn(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void jump(cocos2d::Node *node, float dt, bool repeat = false, float intrval = 0);
	void jellyJump(cocos2d::Node *node, float dt, bool repeat = false, float intrval = 0, int tag = 0);

	/****************************
	临时存放点按钮红点提示，定时刷新
	*****************************/
	void checkRedPoint(float dt);
	void jumpDown(cocos2d::Node *node, float dt);

	bool isPhone();
private:
	std::string m_addrstr;//地点ID
	WHERELAYER_TYPE m_type;
	static float m_distance;
	cocos2d::ui::Widget* m_stredpoint;//临时物品红点

	cocos2d::ui::Button* m_gobtn;//出发按钮
	cocos2d::ui::Button* m_enterbtn;//进入按钮
	cocos2d::ui::Button* m_stbtn;//临时存放按钮
	cocos2d::ui::Text* closebtnlabel;
	cocos2d::ui::Text* enterbtnlabel;
};
#endif

