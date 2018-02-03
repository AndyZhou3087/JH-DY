/********************************************************************
* 分解层
*********************************************************************/
#ifndef _DIVIDE_SCENE_H_
#define _DIVIDE_SCENE_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "GlobalData.h"
USING_NS_CC;

class WXDivideLayer :public Layer
{
public:
	WXDivideLayer();
	~WXDivideLayer();

	bool init(BuildActionData* bdata);
	static WXDivideLayer* create(BuildActionData* bdata);
	void showContent();
	 long long getNowTimeMs();
	bool isBeforeToday(time_t sec);
	long long getTodayLeftSec();
private:
	void initRandSeed();
	time_t getNowTime();

	void removeSelf(float dt);
private:
	BuildActionData* m_bdata;
	Node* m_csbnode;
	cocos2d::ui::Widget* m_dividetext;
	cocos2d::ui::Text* m_desctext;
	LayerColor* m_colorlayer;
};
#endif

