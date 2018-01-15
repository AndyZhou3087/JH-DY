#ifndef _SEPCIAL_NPC_LAYER_H_
#define _SEPCIAL_NPC_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "UIScroll.h"
USING_NS_CC;

class SepcialNpcLayer :public Layer
{
public:
	SepcialNpcLayer();
	~SepcialNpcLayer();

	bool init(std::string addrid);
	virtual void onEnterTransitionDidFinish();
	static SepcialNpcLayer* create(std::string addrid);


private:
	void onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onFight(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

private:
	Node* m_csbnode;
	std::string m_addrstr;
};
#endif

