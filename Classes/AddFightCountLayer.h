#ifndef _ADDFIGHTCOUNT_LAYER_H_
#define _ADDFIGHTCOUNT_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "ServerDataSwap.h"
#include "GlobalData.h"
USING_NS_CC;
class AddFightCountLayer :public Layer, public ServerDataDelegateProtocol
{
public:
	AddFightCountLayer();
	~AddFightCountLayer();
	void on1sTimer();
	bool init(int matchtype);
	static AddFightCountLayer* create(int matchtype);
	void loadTime();
private:
	void initBg();
	void onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onAddCount(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void saveTime();
	void onSuccess();
	void onErr(int errcode);
	void initA();
private:
	Node* m_csbnode;
	int buycount;
	int m_matchtype;
};
#endif