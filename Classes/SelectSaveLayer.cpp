#include "SelectSaveLayer.h"
#include "GlobalData.h"
#include "SoundManager.h"
#include "GameDataSave.h"
#include "Utility.h"
#include "Const.h"
#include "SelectSaveComfirmLayer.h"
#include "CommonFuncs.h"
bool SelectSaveLayer::init()
{
	
    if (!Layer::init()) 
	{
		return false;
    }

	Node* csbnode = CSLoader::createNode("selectSavedLayer.csb");
	this->addChild(csbnode);

	cocos2d::ui::Button* backbtn = (cocos2d::ui::Button*)csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(SelectSaveLayer::onBack, this));

	std::vector<std::string> saveids = GlobalData::getSaveListId();
	for (int i = 0; i < 4; i++)
	{
		std::string nodestr = StringUtils::format("save%d", i);
		m_saveNode[i] = csbnode->getChildByName(nodestr);

		cocos2d::ui::Button* selectbtn = (cocos2d::ui::Button*)m_saveNode[i]->getChildByName("btn");

		cocos2d::ui::Widget* itembg = (cocos2d::ui::Widget*)selectbtn->getChildByName("saveitembg");

		cocos2d::ui::ImageView* headimg = (cocos2d::ui::ImageView*)itembg->getChildByName("head");
		std::string headstr = StringUtils::format("ui/tophero%d.png", i+1);
		headimg->loadTexture(headstr, cocos2d::ui::TextureResType::PLIST);

		cocos2d::ui::Widget* textnode = (cocos2d::ui::Widget*)selectbtn->getChildByName("textnode");

		if (saveids[i].size() <= 0)
		{
			itembg->setOpacity(150);
			textnode->setVisible(false);
			m_saveNode[i]->getChildByName("nodatatext")->setVisible(true);
			
		}
		else
		{
			selectbtn->addTouchEventListener(CC_CALLBACK_2(SelectSaveLayer::onSelect, this));
			selectbtn->setTag(i);
			textnode->setVisible(true);
			cocos2d::ui::Text* nametxt = (cocos2d::ui::Text*)textnode->getChildByName("name");
			nametxt->setString(CommonFuncs::gbk2utf(heroname[i].c_str()));

			std::string str = StringUtils::format("%sherolv", saveids[i].c_str());
			int lv = loadIntDataByKey(str, 0);
			str = StringUtils::format("%d", lv + 1);
			cocos2d::ui::Text* lvtxt = (cocos2d::ui::Text*)textnode->getChildByName("lv");
			lvtxt->setString(str);

			str = StringUtils::format("%slivedays", saveids[i].c_str());
			int daycount = loadIntDataByKey(str, 0);
			str = StringUtils::format("%d", daycount);
			cocos2d::ui::Text* daytxt = (cocos2d::ui::Text*)textnode->getChildByName("day");
			daytxt->setString(str);
		}
	}

	//////layer 点击事件，屏蔽下层事件
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};
	//点击任何位置移除掉
	listener->onTouchEnded = [=](Touch *touch, Event *event)
	{
		return;
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    return true;
}

void SelectSaveLayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		removSelf();
	}
}

void SelectSaveLayer::onSelect(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Node* node = (Node*)pSender;
		SelectSaveComfirmLayer* layer = SelectSaveComfirmLayer::create(node->getTag());
		this->addChild(layer);
	}
}

void SelectSaveLayer::removSelf()
{
	this->removeFromParentAndCleanup(true);
}
