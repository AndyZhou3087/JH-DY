#include "MatchFightResultLayer.h"
#include "CommonFuncs.h"
#include "GameScene.h"
#include "HintBox.h"
#include "WaitingProgress.h"
#include "Const.h"
#include "SoundManager.h"
#include "GameDataSave.h"
#include "RankLayer.h"
#include "HSLJMainLayer.h"

MatchFightResultLayer::MatchFightResultLayer()
{
	GameDataSave::getInstance()->setHsljMatchPlayer("");
}


MatchFightResultLayer::~MatchFightResultLayer()
{
	GlobalData::isPlayerChallenging = false;
}


MatchFightResultLayer* MatchFightResultLayer::create(int myfinalhero, int win)
{
	MatchFightResultLayer *pRet = new MatchFightResultLayer();
	if (pRet && pRet->init(myfinalhero, win))
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

bool MatchFightResultLayer::init(int myfinalhero, int win)
{
	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 200));
	this->addChild(color);

	m_csbnode = CSLoader::createNode("playerChallengeResultLayer.csb");
	m_csbnode->setPositionY(100);
	this->addChild(m_csbnode);

	cocos2d::ui::Widget *backbtn = (cocos2d::ui::Widget*)m_csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(MatchFightResultLayer::onBack, this));

	cocos2d::ui::Text *title = (cocos2d::ui::Text*)m_csbnode->getChildByName("title");
	title->setString(CommonFuncs::gbk2utf("华山论剑"));

	std::string str = StringUtils::format("ui/tophero%d.png", myfinalhero);
	cocos2d::ui::ImageView* headimg = (cocos2d::ui::ImageView*)m_csbnode->getChildByName("heroimg");
	headimg->loadTexture(str, cocos2d::ui::TextureResType::PLIST);

	str = StringUtils::format("ui/wintitle%d.png", win);
	cocos2d::ui::ImageView* wintitle = (cocos2d::ui::ImageView*)m_csbnode->getChildByName("wintitle");
	wintitle->loadTexture(str, cocos2d::ui::TextureResType::PLIST);

	winicon = (cocos2d::ui::ImageView*)m_csbnode->getChildByName("winicon");
	winicon->setVisible(false);

	str = StringUtils::format("ui/winbox%d.png", win);
	cocos2d::ui::ImageView* winbox = (cocos2d::ui::ImageView*)m_csbnode->getChildByName("winbox");
	winbox->loadTexture(str, cocos2d::ui::TextureResType::PLIST);

	wintext = (cocos2d::ui::ImageView*)m_csbnode->getChildByName("wintext");
	wintext->setVisible(false);

	cocos2d::ui::Text* explbl = (cocos2d::ui::Text*)m_csbnode->getChildByName("explbl");

	ranknum = (cocos2d::ui::TextBMFont*)m_csbnode->getChildByName("ranknum");
	ranknum->setVisible(false);

	int myherocount = 0;
	std::vector<std::string> vec_ids = GlobalData::getSaveListId();

	std::vector<std::string> vec_userid;
	for (unsigned int i = 0; i < vec_ids.size(); i++)
	{
		std::string saveuid = vec_ids[i];
		if (saveuid.length() > 0)
			myherocount++;
	}

	int curexp = 0;

	rankup = (cocos2d::ui::Text*)m_csbnode->getChildByName("rankup");
	rankup->setVisible(false);
	if (win == 1)
	{
		curexp = 10 + GlobalData::vec_matchPlayerData.size();
		str = StringUtils::format("+%d", curexp);
	}
	else
	{
		curexp = -(10 + myherocount);
		str = StringUtils::format("%d", curexp);
	}
	explbl->setString(str);
	
	
	WaitingProgress* waitbox = WaitingProgress::create("数据处理中...");
	Director::getInstance()->getRunningScene()->addChild(waitbox, 1, "waitbox");

	ServerDataSwap::init(this)->getMatchFightResult(GlobalData::matchPlayerInfo.playerid, curexp);
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}


void MatchFightResultLayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		HSLJMainLayer* alayer = HSLJMainLayer::create();
		g_gameLayer->addChild(alayer, 5, "hsljmainlayer");
		this->removeFromParentAndCleanup(true);
	}
}

void MatchFightResultLayer::onSuccess()
{
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
	showResult();
}

void MatchFightResultLayer::onErr(int errcode)
{
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");

	HintBox * box = HintBox::create(CommonFuncs::gbk2utf("数据获取异常，请检查网络连接！！"));
	this->addChild(box);
}


void MatchFightResultLayer::showResult()
{
	std::string str1;
	std::string str2;
	std::string str3;

	int up = GlobalData::myMatchInfo.afterrank - GlobalData::myMatchInfo.beforerank;
	if (up <= 0)
	{
		str1 = "ui/wintext1.png";
		str2 = "ui/winicon1.png";
		str3 = StringUtils::format("%d", -up);
	}
	else
	{
		str1 = "ui/wintext-1.png";
		str2 = "ui/winicon0.png";
		str3 = StringUtils::format("%d", up);
	}
	wintext->setVisible(true);
	wintext->loadTexture(str1, cocos2d::ui::TextureResType::PLIST);
	winicon->setVisible(true);
	winicon->loadTexture(str2, cocos2d::ui::TextureResType::PLIST);
	rankup->setVisible(true);
	rankup->setString(str3);

	ranknum->setVisible(true);
	_myrank = GlobalData::myMatchInfo.afterrank;
	_visualmyrank = GlobalData::myMatchInfo.beforerank;

	showRank(0);
}

void MatchFightResultLayer::showRank(float dt)
{

	float step = (_myrank - _visualmyrank) * 0.2f;
	if (fabs(step) < 0.21)
	{
		step = 0.21 * ((step > 0) ? 1 : -1);
	}
	_visualmyrank += step;

	if (fabs(_myrank - _visualmyrank) < 1.0f) {
		_visualmyrank = _myrank;
		this->unschedule(schedule_selector(MatchFightResultLayer::showRank));
	}

	std::string rankstr = StringUtils::format("%d", (int)_visualmyrank);
	ranknum->setString(rankstr);

}