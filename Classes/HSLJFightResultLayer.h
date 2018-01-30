#ifndef _HSLJFIGHTRESULT_LAYER_H_
#define _HSLJFIGHTRESULT_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "ServerDataSwap.h"
#include "GlobalData.h"
USING_NS_CC;
class HSLJFightResultLayer :public Layer, public ServerDataDelegateProtocol
{
public:
	HSLJFightResultLayer();
	~HSLJFightResultLayer();

	bool init(int myfinalhero, int win);
	static HSLJFightResultLayer* create(int myfinalhero, int win);
	bool isBeforeToday(time_t sec);
 long long getTodayLeftSec();
private:
	void onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void initRandSeed();
	time_t getNowTime();
	long long getNowTimeMs();

	void onSuccess();
	void onErr(int errcode);

	void shake(cocos2d::Node * node, float scaleLarge, float scaleSmall);
	void shake(cocos2d::Node * node);
	void showRank(float dt);
	void showResult();

	bool getRandomBoolean(float rate);
	bool getRandomBoolean();
	int getRandomNum(int range);
	int getRandomNum(int rangeStart, int rangeEnd);
private:
	Node* m_csbnode;
	float _myrank;
	float _visualmyrank;
	cocos2d::ui::TextBMFont* ranknum;
	cocos2d::ui::ImageView* winicon;
	cocos2d::ui::Text* wintext;
	cocos2d::ui::Text* rankup;
};
#endif