#include "RaffleResultLayer.h"
#include "Const.h"
#include "SoundManager.h"
#include "AnalyticUtil.h"
#include "CommonFuncs.h"

RaffleResultLayer::RaffleResultLayer()
{
}


RaffleResultLayer::~RaffleResultLayer()
{

}

RaffleResultLayer* RaffleResultLayer::create()
{
	RaffleResultLayer *pRet = new RaffleResultLayer();
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

bool RaffleResultLayer::init()
{
	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 200));
	this->addChild(color);

	Node* csbnode = CSLoader::createNode("raffleResultLayer.csb");
	this->addChild(csbnode);

	cocos2d::ui::Widget* okbtn = (cocos2d::ui::Button*)csbnode->getChildByName("okbtn");
	okbtn->addTouchEventListener(CC_CALLBACK_2(RaffleResultLayer::onOK, this));

	cocos2d::ui::Text* stagelbl = (cocos2d::ui::Text*)csbnode->getChildByName("stagelbl");
	stagelbl->setString(GlobalData::myRaffleData.mywinstage);

	std::string str;
	cocos2d::ui::Text* wingoldlbl = (cocos2d::ui::Text*)csbnode->getChildByName("wingold");
	str = StringUtils::format("%d", GlobalData::myRaffleData.mywingold);
	wingoldlbl->setString(str);
	
	wingold = GlobalData::myRaffleData.mywingold;

	std::string rankstr[] = { "一", "二", "三" };
	int rankpercent[] = { 50, 30, 20 };

	int rank = GlobalData::myRaffleData.mywinrank;
	if (rank >= 1 && rank <= 3)
	{
		cocos2d::ui::Text* ranklbl = (cocos2d::ui::Text*)csbnode->getChildByName("ranklbl");
		str = StringUtils::format("%s", rankstr[rank-1].c_str());
		ranklbl->setString(CommonFuncs::gbk2utf(str.c_str()));

		cocos2d::ui::Text* poolnumlbl = (cocos2d::ui::Text*)csbnode->getChildByName("poolnum");
		str = StringUtils::format("%d", GlobalData::myRaffleData.mywingold * 100 / rankpercent[rank-1]);
		poolnumlbl->setString(str);
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

void RaffleResultLayer::onOK(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		GlobalData::setMyGoldCount(GlobalData::getMyGoldCount() + wingold);
		this->removeFromParentAndCleanup(true);
	}
}