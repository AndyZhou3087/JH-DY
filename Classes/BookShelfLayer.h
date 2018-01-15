#ifndef _BOOKSHELF_LAYER_H_
#define _BOOKSHELF_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

typedef struct
{
	std::string strid;
	int type;
	int maxvalue;
}BookData;
class BookShelfLayer :public Layer
{
public:
	BookShelfLayer();
	~BookShelfLayer();

	virtual bool init();
	virtual void onEnterTransitionDidFinish();
	CREATE_FUNC(BookShelfLayer);
	void updateContent(int category);
private:
	void onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onCategory(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	Node* m_csbnode;
	void onclick(Ref* pSender);
	cocos2d::ui::ScrollView* scrollview;
	int getCountByType(int type);
	void loadBookData();
	static bool larger_callback(BookData a, BookData b);
private:
	int typerow[4];
	std::map<int, std::vector<BookData>> map_bookData;
	std::vector<cocos2d::ui::Button*> vec_categoryBtn;//锻造台分类按钮
	int lastCategoryindex;
};
#endif

