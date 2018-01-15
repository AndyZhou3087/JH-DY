#ifndef _CHALLENGERESULT_LAYER_H_
#define _CHALLENGERESULT_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "ServerDataSwap.h"
#include "GlobalData.h"
USING_NS_CC;
class PlayerChallengeResultLayer :public Layer, public ServerDataDelegateProtocol
{
public:
	PlayerChallengeResultLayer();
	~PlayerChallengeResultLayer();

	bool init(RankData* fightPlayerData, int win);
	static PlayerChallengeResultLayer* create(RankData* fightPlayerData, int win);

private:
	void onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	void onSuccess();
	void onErr(int errcode);
	void showRank(float dt);
	void delayShowRank(float dt);
private:
	Node* m_csbnode;
	float _myrank;
	float _visualmyrank;
	cocos2d::ui::TextBMFont* ranknum;
};
#endif