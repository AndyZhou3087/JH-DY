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

class DivideLayer :public Layer
{
public:
	DivideLayer();
	~DivideLayer();

	bool init(BuildActionData* bdata);
	static DivideLayer* create(BuildActionData* bdata);
	void showContent();
private:

	void removeSelf(float dt);
private:
	BuildActionData* m_bdata;
	Node* m_csbnode;
	cocos2d::ui::Widget* m_dividetext;
	cocos2d::ui::Text* m_desctext;
	LayerColor* m_colorlayer;
};
#endif

