#ifndef _REVIVE_LAYER_H_
#define _REVIVE_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

class ReviveLayer :public Layer
{
public:
	ReviveLayer();
	~ReviveLayer();

	virtual bool init();
	static ReviveLayer* create();
	static void doRevive();
	static void reviveOk();
private:
	Node* m_csbnode;
	cocos2d::ui::Text* m_revivepricelbl;
	cocos2d::ui::Text* m_revivedesc;
	int revivecount;
	cocos2d::ui::Button* m_closebtn;
	cocos2d::ui::Button* m_sharebtn;
	cocos2d::ui::Button* m_cancelbtn;
	cocos2d::ui::Text* m_sharetext;
	cocos2d::ui::Button* m_freeRevivebtn;
private:
	void onCancel(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onRevive(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onShare(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onFreeRevive(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onHelp(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	static void shareCallback(int platform, int stCode, std::string& errorMsg);
	void checkGoldCount(float dt);
	void checkShareReviveCount(float dt);
	void checkFreeReviveCount(float dt);
	int needgold;
};
#endif

