#ifndef _PAYSELECT_LAYER_H_
#define _PAYSELECT_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "GlobalData.h"
USING_NS_CC;

class PaySelectLayer :public Layer
{
public:
	PaySelectLayer();
	~PaySelectLayer();

	bool init(int payindex);
	static PaySelectLayer* create(int payindex);

private:
	int m_payindex;
private:
	void onCancel(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onPay(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
};
#endif

