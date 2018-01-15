/********************************************************************
*功法提示
*********************************************************************/
#ifndef _MIXSUGGESTLAYER_H_
#define _MIXSUGGESTLAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "Hero.h"
#include "ServerDataSwap.h"
USING_NS_CC;


class MixSuggestLayer :public Layer, public ServerDataDelegateProtocol
{
public:
	MixSuggestLayer();
	~MixSuggestLayer();

	virtual bool init();
	virtual void onEnterTransitionDidFinish();
	virtual void onExit();
	CREATE_FUNC(MixSuggestLayer);
	void getServerTime();
private:

	/****************************
	点击“免费”按钮回调
	*****************************/
	void onFree(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	/****************************
	点击“银两刷新”
	*****************************/
	void onSilver(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	/****************************
	点击“金币刷新”
	*****************************/
	void onGold(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	/****************************
	点击返回
	*****************************/
	void onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	void onSuccess();
	void onErr(int errcode);

	void updateServerTime(float dt);

	void randMixGf(int type);

	void loadMixGfUi(MixGfData mixdata);

	void updateDesc();

private:
	cocos2d::ui::ImageView* imgbox[4];
	cocos2d::ui::ImageView* img[4];
	cocos2d::ui::Text* name[4];
	cocos2d::ui::Button* freebtn;
	cocos2d::ui::Button* silverbtn;
	cocos2d::ui::Button* goldbtn;
	cocos2d::ui::Text* freetimelbl0;
	cocos2d::ui::Text* freetimelbl1;
	cocos2d::ui::Text* freetimelbl2;
	cocos2d::ui::Text* mixname;
	Node *csbroot;
	int severtime;
	int startime0;
	int startime1;
	int startime2;

	cocos2d::ui::Text* desc0;
	cocos2d::ui::Text* desc1;
	cocos2d::ui::Text* desc;
};
#endif

