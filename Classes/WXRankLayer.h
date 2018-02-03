#ifndef _RANK_LAYER_H_
#define _RANK_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "ServerDataSwap.h"
#include "GlobalData.h"
USING_NS_CC;
class WXRankLayer :public Layer, public ServerDataDelegateProtocol
{
public:
	WXRankLayer();
	~WXRankLayer();

	bool init();
	virtual void onEnterTransitionDidFinish();
	static WXRankLayer* create();

	void getRankData(int type);
	void initRandSeed();
	time_t getNowTime();
	long long getNowTimeMs();
	bool isBeforeToday(time_t sec);
	long long getTodayLeftSec();
private:
	void onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onRank(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onAddCount(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void jump(cocos2d::Node *node, float dt, bool repeat = false, float intrval = 0);
	void jellyJump(cocos2d::Node *node, float dt, bool repeat = false, float intrval = 0, int tag = 0);
	void petJump(cocos2d::Node *node, float dt, bool repeat = false, float intrval = 0, int tag = 0, cocos2d::ActionInterval *ac = nullptr);
	void jelly(cocos2d::Node *node, bool repeat = false, float intrval = 0, bool delay = false, int tag = 0);
	void delayShowData(float dt);
	void onSuccess();
	void onErr(int errcode);

	void updateFightCount(float dt);
private:
	cocos2d::ui::ScrollView* srollView;
	cocos2d::ui::Button *tag0btn;
	cocos2d::ui::Button *tag1btn;
	cocos2d::ui::Button *tag2btn;
	cocos2d::ui::Text* text6;
	cocos2d::ui::Widget* rankcatabox0;
	cocos2d::ui::Widget* rankcatabox1;
	cocos2d::ui::Widget* hintdown;
	int selectrank;
	int myfightingpower;
	Node* m_csbnode;
	Node* myFihgtNode;
	cocos2d::ui::Text* countext;
	cocos2d::ui::Text* exptext;
	cocos2d::ui::Text* ranktext;
	bool isLastFight;
	cocos2d::ui::Widget* m_particle;
};

class RankItem :public Sprite, public ServerDataDelegateProtocol
{
public:
	RankItem();
	~RankItem();

	bool init(RankData *data, int type);
	static RankItem* create(RankData *data, int type = 0);

private:
	void onFight(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onSuccess();
	void onErr(int errcode);
	void shake(cocos2d::Node * node, float scaleLarge, float scaleSmall);
	void shake(cocos2d::Node * node);
private:
	RankData *m_data;
};
#endif