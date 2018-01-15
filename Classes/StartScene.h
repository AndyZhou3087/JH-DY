#ifndef __START_SCENE_H__
#define __START_SCENE_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ServerDataSwap.h"
#include "ui/CocosGUI.h"
class StartScene : public cocos2d::Layer, public ServerDataDelegateProtocol
{
public:
	StartScene();
	~StartScene();
    static cocos2d::Scene* createScene();

	virtual void onExit();
    virtual bool init();
	void onSuccess();
	void onErr(int errcode);
	CREATE_FUNC(StartScene);

private:
	void onNewStart(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onContinue(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onLoadSaved(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onSet(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onQQ(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onLogo(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	void showQQ();
private:
	void checkServerData(float dt);
	cocos2d::ui::Widget* m_continuebtn;
	int clicklogocount;
	bool isdouserdata;
	cocos2d::ui::Text* qq1;
	cocos2d::ui::Text* qq2;
	cocos2d::ui::Text* qqtitle;
	cocos2d::ui::ImageView* startimg;
	cocos2d::ui::ImageView* continueimg;
	cocos2d::ui::ImageView* roleimg;
	cocos2d::ui::ImageView* setimg;
};

#endif
