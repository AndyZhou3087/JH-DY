#ifndef __MYMENU_H_
#define __MYMENU_H_
#include "cocos2d.h"
USING_NS_CC;
class MyMenu:public Menu
{
public:
	MyMenu();
	~MyMenu();
    bool init();
	virtual bool onTouchBegan(Touch* touch, Event* event);
	virtual void onTouchMoved(Touch *touch, Event *event);
	virtual void onTouchEnded(Touch *touch, Event *event);
	virtual void onTouchCancelled(Touch *touch, Event *event);
    /** initializes a Menu with a NSArray of MenuItem objects */
    bool initWithArray(const Vector<MenuItem*>& arrayOfItems);
	static MyMenu* createWithArray(const Vector<MenuItem*>& arrayOfItems);
    static MyMenu* createWithItem(MenuItem* item);
	static MyMenu* create();
    /** creates a Menu with MenuItem objects */
    static MyMenu* createWithItems(MenuItem *firstItem, va_list args);
	static MyMenu* create(MenuItem* item, ...)	CC_REQUIRES_NULL_TERMINATION;
	void setTouchlimit(cocos2d::Node* node);
private:
	bool isInTouchLimit(Touch* touch);
private:
	Node* m_szTouchLimitNode;
	bool m_bTouchLimit;
	bool m_isdraging;
	float startY;
};
#endif
