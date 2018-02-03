#ifndef _CHALLENGERESULT_LAYER_H_
#define _CHALLENGERESULT_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "ServerDataSwap.h"
#include "GlobalData.h"
USING_NS_CC;
class WXPlayerChallengeResultLayer :public Layer, public ServerDataDelegateProtocol
{
public:
	WXPlayerChallengeResultLayer();
	~WXPlayerChallengeResultLayer();

	bool init(RankData* fightPlayerData, int win);
	static WXPlayerChallengeResultLayer* create(RankData* fightPlayerData, int win);
	void setStar(int num, bool isboss = false);
	void hilight();
	void disable();

private:
	void onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void initData();
	void onSuccess();
	void onErr(int errcode);
	void showRank(float dt);
	void delayShowRank(float dt);
	void nomal();
	void setStageNum(int stage);
	void showLock(int starnum);
	void removeLock();
private:
	Node* m_csbnode;
	float _myrank;
	float _visualmyrank;
	cocos2d::ui::TextBMFont* ranknum;
	Node* m_node;
	cocos2d::ui::ImageView* m_stageIcon;
	cocos2d::ui::TextBMFont* m_stagenumlbl;
	cocos2d::ui::Widget* m_star[3];
	cocos2d::ui::Widget* m_starbg[3];
	int m_starnum;
	bool m_isboss;
	Sprite* lockNode;
};
#endif