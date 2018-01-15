#ifndef _UISCROLL_H_
#define _UISCROLL_H_
#include "cocos2d.h"
#include "cocos-ext.h"
#include "cocostudio/CocoStudio.h"
USING_NS_CC;
USING_NS_CC_EXT;
class UIScroll :public Node, public ScrollViewDelegate
{
public:
	UIScroll();
	~UIScroll();

	bool init(float width, float height);

	static UIScroll* create(float width, float height);
	void addEventText(std::string text, int fontsize = 26, Color3B color3b = Color3B(0,0,0));
	void addEventLabel(Label* label);
	void clean();

private:
	float m_width;
	float m_height;
	Layer* m_containerLayer;
	ScrollView *scrollView;
	void scrollViewDidScroll(ScrollView* view);

	void onTouchEnded(Touch*touch, Event *unused_event);
	bool onTouchBegan(Touch * touch, Event *unused_event);//开始状态
	void onTouchMoved(Touch *touch, Event *unused_event);//移动状态
};
#endif

