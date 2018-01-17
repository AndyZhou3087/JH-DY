#include "TimeGiftLayer.h"
#include "CommonFuncs.h"
#include "SoundManager.h"
#include "GlobalData.h"
#include "Const.h"
#include "ShopLayer.h"

TimeGiftLayer::TimeGiftLayer()
{

}


TimeGiftLayer::~TimeGiftLayer()
{

}

bool TimeGiftLayer::init(int goodsId)
{
	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 200));
	this->addChild(color);

	Node* csbnode = CSLoader::createNode("timeGfitLayer.csb");
	this->addChild(csbnode);

	int herocount = sizeof(heroprice) / sizeof(heroprice[0]);

	cocos2d::ui::Button* buybtn = (cocos2d::ui::Button*)csbnode->getChildByName("buybtn");
	buybtn->addTouchEventListener(CC_CALLBACK_2(TimeGiftLayer::onbuy, this));

	cocos2d::ui::Button* closebtn = (cocos2d::ui::Button*)csbnode->getChildByName("closebtn");
	closebtn->addTouchEventListener(CC_CALLBACK_2(TimeGiftLayer::onClose, this));

	cocos2d::ui::Text* price = (cocos2d::ui::Text*)csbnode->getChildByName("price");

	std::string str = StringUtils::format("%d元", GlobalData::vec_goods[goodsId - herocount].price);
	price->setString(CommonFuncs::gbk2utf(str.c_str()));

	m_timelbl = (cocos2d::ui::Text*)csbnode->getChildByName("time");

	checkTimeGift(0);
	this->schedule(schedule_selector(TimeGiftLayer::checkTimeGift), 1.0f);

	int rgoldcount = 100;
	std::vector<std::string> vec_rewardres = GlobalData::vec_goods[goodsId - herocount].vec_res;

	Vec2 pos[] = { Vec2(300, 730), Vec2(444, 730), Vec2(588, 730), Vec2(345, 567), Vec2(523, 567) };
	//int startx = 75;
	//int spacex = 140;
	//int starty = 620;
	//if (vec_rewardres.size() == 2)
	//{
	//	startx = 160;
	//	spacex = 200;
	//}

	int ressize = vec_rewardres.size() + 1;
	for (int i = 0; i < ressize; i++)
	{
		Sprite * box = Sprite::createWithSpriteFrameName("ui/timegiftframe.png");
		box->setPosition(pos[i]);
		this->addChild(box);


		std::string resstr;
		std::string strcount;
		std::string namstr;
		if (i == 0)
		{
			resstr = "ui/gd0.png";
			strcount = StringUtils::format("x%d", rgoldcount);
			namstr = CommonFuncs::gbk2utf("金元宝");
		}
		else
		{
			std::string resid = vec_rewardres[i - 1];
			int intresid = atoi(resid.c_str());
			resstr = StringUtils::format("ui/%d.png", intresid / 1000);
			strcount = StringUtils::format("x%d", intresid % 1000);
			std::string ridstr = StringUtils::format("%d", intresid / 1000);
			namstr = GlobalData::map_allResource[ridstr].cname;
		}
		Sprite* res = Sprite::createWithSpriteFrameName(resstr);
		res->setPosition(Vec2(box->getContentSize().width / 2, box->getContentSize().width / 2));
		box->addChild(res);

		Label * coutlbl = Label::createWithTTF(strcount, "fonts/STXINGKA.TTF", 25);//Label::createWithSystemFont(strcount, "", 25);
		coutlbl->setAnchorPoint(Vec2(0.5, 0.5));
		coutlbl->setColor(Color3B(0, 0, 0));
		coutlbl->setPosition(Vec2(box->getPositionX(), box->getPositionY() - box->getContentSize().height / 2 - 40));
		this->addChild(coutlbl);

		Label * namelbl = Label::createWithTTF(namstr, "fonts/STXINGKA.TTF", 25);
		namelbl->setColor(Color3B(0, 0, 0));
		namelbl->setPosition(Vec2(box->getPositionX(), box->getPositionY()-box->getContentSize().height/2-15));
		this->addChild(namelbl);
	}

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	this->setTag(goodsId);
	return true;
}

TimeGiftLayer* TimeGiftLayer::create(int goodsId)
{
	TimeGiftLayer *pRet = new TimeGiftLayer();
	if (pRet && pRet->init(goodsId))
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

void TimeGiftLayer::onbuy(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		ShopLayer::beginPay(this->getTag());
	}
}

void TimeGiftLayer::onClose(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}

void TimeGiftLayer::checkTimeGift(float dt)
{
	int lefttime = GlobalData::getTimeGiftLeftTime();

	int hour = lefttime / 3600;
	int min = lefttime % 3600 / 60;
	int sec = lefttime % 3600 % 60;
	std::string timestr = StringUtils::format("%02d:%02d:%02d", hour, min, sec);
	m_timelbl->setString(timestr);

}
