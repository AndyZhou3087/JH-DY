#ifndef _HSLJRANK_LAYER_H_
#define _HSLJRANK_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "ServerDataSwap.h"
#include "GlobalData.h"
USING_NS_CC;
class HSLJRankLayer :public Layer, public ServerDataDelegateProtocol
{
public:
	HSLJRankLayer();
	~HSLJRankLayer();

	bool init();
	virtual void onEnterTransitionDidFinish();
	static HSLJRankLayer* create();

	void getRankData();
	void setStar(int num, bool isboss = false);
	void hilight();
	void showLock(int starnum);
	void removeLock();
	HSLJRankData myrankdata;

private:
	void onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	void delayShowData(float dt);
	void onSuccess();
	void onErr(int errcode);

	void initData();
	void disable();
	void nomal();
	void setStageNum(int stage);


private:
	cocos2d::ui::ScrollView* srollView;
	Node* m_csbnode;
	Node* m_node;
	cocos2d::ui::ImageView* m_stageIcon;
	cocos2d::ui::TextBMFont* m_stagenumlbl;
	cocos2d::ui::Widget* m_star[3];
	cocos2d::ui::Widget* m_starbg[3];
	int m_starnum;
	bool m_isboss;
	Sprite* lockNode;
};

class HSLJRankItem :public Sprite
{
public:
	HSLJRankItem();
	~HSLJRankItem();

	bool init(HSLJRankData *data);
	static HSLJRankItem* create(HSLJRankData *data);

private:
private:
	HSLJRankData *m_data;
};
#endif