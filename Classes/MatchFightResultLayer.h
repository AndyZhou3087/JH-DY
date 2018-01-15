#ifndef _MATCHFIGHTRESULT_LAYER_H_
#define _MATCHFIGHTRESULT_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "ServerDataSwap.h"
#include "GlobalData.h"
USING_NS_CC;
class MatchFightResultLayer :public Layer, public ServerDataDelegateProtocol
{
public:
	MatchFightResultLayer();
	~MatchFightResultLayer();

	bool init(int myfinalhero, int win);
	static MatchFightResultLayer* create(int myfinalhero, int win);

private:
	void onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	void onSuccess();
	void onErr(int errcode);
	void showRank(float dt);
	void showResult();
private:
	Node* m_csbnode;
	float _myrank;
	float _visualmyrank;
	cocos2d::ui::TextBMFont* ranknum;
	cocos2d::ui::ImageView* winicon;
	cocos2d::ui::ImageView* wintext;
	cocos2d::ui::Text* rankup;
};
#endif