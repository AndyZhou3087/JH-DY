#include "HelpMainLayer.h"
#include "CommonFuncs.h"
#include "Const.h"
#include "SoundManager.h"
#include "GlobalData.h"
#include "json.h"
#include "HelpDescLayer.h"
#include "ShopLayer.h"

HelpMainLayer::HelpMainLayer()
{
	isChangePause = false;
}


HelpMainLayer::~HelpMainLayer()
{
	if (GlobalData::g_gameStatus == GAMEPAUSE && isChangePause)
		GlobalData::g_gameStatus = GAMESTART;
}


HelpMainLayer* HelpMainLayer::create()
{
	HelpMainLayer *pRet = new HelpMainLayer();
	if (pRet && pRet->init())
	{
		pRet->autorelease();
	}
	else
	{
		delete pRet;
		pRet = NULL;
	}
	return pRet;
}

bool HelpMainLayer::init()
{
	Node* csbnode = CSLoader::createNode("helpLayer.csb");
	this->addChild(csbnode);

	cocos2d::ui::Widget *backbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(HelpMainLayer::onBack, this));

	cocos2d::ui::Widget *shopbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("shopbtn");
	shopbtn->addTouchEventListener(CC_CALLBACK_2(HelpMainLayer::onShop, this));

	m_srollView = (cocos2d::ui::ScrollView*)csbnode->getChildByName("scrollview");
	m_srollView->setScrollBarEnabled(false);
	m_srollView->setBounceEnabled(true);
	//m_srollView->setEnabled(false);
	m_srollView->setTouchEnabled(false);
	loadHelpText();

	int itemh = 70;
	int innerheight = vec_helpdata.size() * itemh;
	int contentheight = m_srollView->getContentSize().height;
	if (innerheight < contentheight)
		innerheight = contentheight;
	m_srollView->setInnerContainerSize(Size(650, innerheight));

	for (unsigned int i = 0; i < vec_helpdata.size(); i++)
	{
		std::string str = StringUtils::format("%s.%s", vec_helpdata[i].id.c_str(), vec_helpdata[i].title.c_str());
		cocos2d::ui::Text* titletxt = cocos2d::ui::Text::create(str, "fonts/STXINGKA.TTF", 30);
		titletxt->setTouchEnabled(true);
		titletxt->setAnchorPoint(Vec2(0, 0.5));
		titletxt->setColor(Color3B(8, 70, 127));
		titletxt->setPosition(Vec2(50, innerheight - 30 - i* itemh));
		titletxt->addTouchEventListener(CC_CALLBACK_2(HelpMainLayer::onTextClick, this));
		titletxt->setTag(i);
		m_srollView->addChild(titletxt);

		cocos2d::ui::ImageView* line = cocos2d::ui::ImageView::create("ui/qqline0.png", cocos2d::ui::Widget::TextureResType::PLIST);
		line->setAnchorPoint(Vec2(0, 0.5));
		line->setTouchEnabled(true);
		line->setPosition(Vec2(0,-2));
		line->setScale9Enabled(true);
		line->setColor(Color3B(8, 70, 127));
		line->setContentSize(Size(titletxt->getContentSize().width, 3));
		line->addTouchEventListener(CC_CALLBACK_2(HelpMainLayer::onTextClick, this));
		line->setTag(i);
		titletxt->addChild(line);
	}

	if (GlobalData::g_gameStatus == GAMESTART)
	{
		isChangePause = true;
		GlobalData::g_gameStatus = GAMEPAUSE;
	}

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void HelpMainLayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}

void HelpMainLayer::onShop(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->addChild(ShopLayer::create());
	}
}

void HelpMainLayer::onTextClick(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);
		Node* node = (Node*)pSender;
		int index = node->getTag();
		this->addChild(HelpDescLayer::create(vec_helpdata[index].title, vec_helpdata[index].content));
	}
}

void HelpMainLayer::loadHelpText()
{
	rapidjson::Document doc = ReadJsonFile("data/help.json");
	rapidjson::Value& allData = doc["h"];
	for (unsigned int i = 0; i < allData.Size(); i++)
	{
		rapidjson::Value& oneData = allData[i];
		helpData data;
		if (oneData.IsObject())
		{
			rapidjson::Value& value = oneData["id"];
			data.id = value.GetString();

			value = oneData["title"];
			data.title = value.GetString();

			value = oneData["content"];
			data.content = value.GetString();
			vec_helpdata.push_back(data);
		}
	}
}

