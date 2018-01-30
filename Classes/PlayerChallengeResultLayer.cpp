#include "PlayerChallengeResultLayer.h"
#include "CommonFuncs.h"
#include "GameScene.h"
#include "HintBox.h"
#include "WaitingProgress.h"
#include "Const.h"
#include "SoundManager.h"
#include "GameDataSave.h"
#include "RankLayer.h"

PlayerChallengeResultLayer::PlayerChallengeResultLayer()
{
	GlobalData::isPlayerChallenging = false;
	GameDataSave::getInstance()->setPlayerChallengeData("");
}


PlayerChallengeResultLayer::~PlayerChallengeResultLayer()
{

}
void PlayerChallengeResultLayer::initData()
{
	m_starnum = 0;
	m_stageIcon = (cocos2d::ui::ImageView*)m_node->getChildByName("image");
	std::string str;
	for (int i = 0; i < 3; i++)
	{
		str = StringUtils::format("star%d", i);
		m_star[i] = (cocos2d::ui::Widget*)m_node->getChildByName(str);
		str = StringUtils::format("star%dbg", i);
		m_starbg[i] = (cocos2d::ui::Widget*)m_node->getChildByName(str);
	}
	m_stagenumlbl = (cocos2d::ui::TextBMFont*)m_node->getChildByName("stagenum");
}


PlayerChallengeResultLayer* PlayerChallengeResultLayer::create(RankData* fightPlayerData, int win)
{
	PlayerChallengeResultLayer *pRet = new PlayerChallengeResultLayer();
	if (pRet && pRet->init(fightPlayerData, win))
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
void PlayerChallengeResultLayer::setStar(int num, bool isboss)
{
	m_starnum = num;
	m_isboss = isboss;
	std::string stagestr;
	if (num > 0)
	{
		stagestr = "stage1.png";
		for (int i = 0; i < 3; i++)
		{
			m_starbg[i]->setVisible(true);
		}
		for (int i = 0; i < num; i++)
		{
			m_star[i]->setVisible(true);
		}
	}
	else
	{
		stagestr = "stage0.png";
		for (int i = 0; i < 3; i++)
		{
			m_star[i]->setVisible(false);
			m_starbg[i]->setVisible(false);
		}
	}
	m_stageIcon->loadTexture(stagestr, cocos2d::ui::TextureResType::PLIST);
	m_stageIcon->setContentSize(Sprite::createWithSpriteFrameName(stagestr)->getContentSize());
	if (isboss)
	{
		int index = num > 0 ? 1 : 0;
		std::string bossstr = StringUtils::format("sboss%d.png", index);
		m_stageIcon->loadTexture(bossstr, cocos2d::ui::TextureResType::PLIST);
		m_stageIcon->setContentSize(Sprite::createWithSpriteFrameName(bossstr)->getContentSize());
	}
}

bool PlayerChallengeResultLayer::init(RankData* fightPlayerData, int win)
{
	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 160));
	this->addChild(color);

	m_csbnode = CSLoader::createNode("playerChallengeResultLayer.csb");
	m_csbnode->setPositionY(100);
	this->addChild(m_csbnode);

	cocos2d::ui::Widget *backbtn = (cocos2d::ui::Widget*)m_csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(PlayerChallengeResultLayer::onBack, this));

	std::string str = StringUtils::format("ui/fhero%d.png", g_hero->getHeadID());
	cocos2d::ui::ImageView* headimg = (cocos2d::ui::ImageView*)m_csbnode->getChildByName("heroimg");
	headimg->loadTexture(str, cocos2d::ui::TextureResType::PLIST);

	str = StringUtils::format("ui/wintitle%d.png", win);
	cocos2d::ui::ImageView* wintitle = (cocos2d::ui::ImageView*)m_csbnode->getChildByName("wintitle");
	wintitle->loadTexture(str, cocos2d::ui::TextureResType::PLIST);

	str = StringUtils::format("ui/winicon%d.png", win);
	cocos2d::ui::ImageView* winicon = (cocos2d::ui::ImageView*)m_csbnode->getChildByName("winicon");
	winicon->loadTexture(str, cocos2d::ui::TextureResType::PLIST);

	str = StringUtils::format("ui/winbox%d.png", win);
	cocos2d::ui::ImageView* winbox = (cocos2d::ui::ImageView*)m_csbnode->getChildByName("winbox");
	winbox->loadTexture(str, cocos2d::ui::TextureResType::PLIST);

	char* arr[] = { "再接再厉", "排名上升" };
	cocos2d::ui::Text* wintext = (cocos2d::ui::Text*)m_csbnode->getChildByName("wintext");
	wintext->setString(CommonFuncs::gbk2utf(arr[win]));

	cocos2d::ui::Text* explbl = (cocos2d::ui::Text*)m_csbnode->getChildByName("explbl");

	ranknum = (cocos2d::ui::TextBMFont*)m_csbnode->getChildByName("ranknum");
	str = StringUtils::format("%d", GlobalData::myrank);

	ranknum->setString(str);

	int lastrank = GlobalData::myrank;
	cocos2d::ui::Text* rankup = (cocos2d::ui::Text*)m_csbnode->getChildByName("rankup");
	rankup->setVisible(false);
	if (win == 1)
	{
		explbl->setString("+3");
		if (fightPlayerData->rank < GlobalData::myrank)
		{
			//str = StringUtils::format("%d", fightPlayerData->rank);
			//ranknum->setString(str);
			_myrank = fightPlayerData->rank;
			_visualmyrank = GlobalData::myrank;
			std::string upstr = StringUtils::format("%d", GlobalData::myrank - fightPlayerData->rank);
			rankup->setString(upstr);
			rankup->setVisible(true);
			GlobalData::myrank = fightPlayerData->rank;
			this->scheduleOnce(schedule_selector(PlayerChallengeResultLayer::delayShowRank), 1.0f);
		}
		GlobalData::myFihgtexp += 3;
	}
	else
	{
		explbl->setString("-1");
		GlobalData::myFihgtexp -= 1;
		rankup->setVisible(false);
		//winicon->setVisible(false);
		winicon->setPositionY(266);
	}

	GlobalData::myFihgtCount--;
	
	
	ServerDataSwap::init(NULL)->getChallengeResult(lastrank, fightPlayerData->playerid, fightPlayerData->herotype, fightPlayerData->rank, win);
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}


void PlayerChallengeResultLayer::setStageNum(int stage)
{
	std::string numstr = StringUtils::format("%d", stage);
	m_stagenumlbl->setString(numstr);
	if (m_isboss)
		m_stagenumlbl->setVisible(false);
}
void PlayerChallengeResultLayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
		RankLayer* rlayer = (RankLayer*)g_gameLayer->getChildByName("ranklayer");
		if (rlayer != NULL)
			rlayer->getRankData(2);
	}
}

void PlayerChallengeResultLayer::onSuccess()
{
	
}

void PlayerChallengeResultLayer::hilight()
{
	m_stageIcon->stopAllActions();
	m_node->setScale(1);
	std::string stagestr;
	if (m_isboss)
	{
		stagestr = "sboss1.png";
	}
	else
	{
		stagestr = "stage2.png";
	}
	m_stageIcon->loadTexture(stagestr, cocos2d::ui::TextureResType::PLIST);
	m_stageIcon->setContentSize(Sprite::createWithSpriteFrameName(stagestr)->getContentSize());
}

void PlayerChallengeResultLayer::onErr(int errcode)
{
	HintBox * box = HintBox::create(CommonFuncs::gbk2utf("数据获取异常，请检查网络连接！！"));
	this->addChild(box);
}

void PlayerChallengeResultLayer::disable()
{
	m_stageIcon->stopAllActions();
	m_node->setScale(1);
	std::string stagestr = "stage0.png";
	m_stageIcon->loadTexture(stagestr, cocos2d::ui::TextureResType::PLIST);
	m_stageIcon->setContentSize(Sprite::createWithSpriteFrameName(stagestr)->getContentSize());
}

void PlayerChallengeResultLayer::nomal()
{
	m_stageIcon->stopAllActions();
	m_node->setScale(1);
	std::string stagestr;
	if (m_isboss)
	{
		stagestr = "sboss1.png";
	}
	else
	{
		stagestr = "stage1.png";
	}
	m_stageIcon->loadTexture(stagestr, cocos2d::ui::TextureResType::PLIST);
	m_stageIcon->setContentSize(Sprite::createWithSpriteFrameName(stagestr)->getContentSize());
}

void PlayerChallengeResultLayer::showRank(float dt)
{

	float step = (_myrank - _visualmyrank) * 0.2f;
	if (fabs(step) < 0.21)
	{
		step = 0.21 * ((step > 0) ? 1 : -1);
	}
	_visualmyrank += step;

	if (fabs(_myrank - _visualmyrank) < 1.0f) {
		_visualmyrank = _myrank;
		this->unschedule(schedule_selector(PlayerChallengeResultLayer::showRank));
	}

	std::string rankstr = StringUtils::format("%d", (int)_visualmyrank);
	ranknum->setString(rankstr);

}

void PlayerChallengeResultLayer::showLock(int starnum)
{
	if (lockNode == NULL)
	{
		lockNode = Sprite::createWithSpriteFrameName("lock.png");
		lockNode->setPosition(Vec2(m_node->getContentSize().width / 2, 10));
		m_node->addChild(lockNode, 2);

		Sprite* lockbox = Sprite::createWithSpriteFrameName("unlockbox.png");
		lockbox->setPosition(Vec2(lockNode->getContentSize().width / 2, lockNode->getContentSize().height - 60));
		lockNode->addChild(lockbox);

		Sprite* star = Sprite::createWithSpriteFrameName("star1.png");
		star->setPosition(Vec2(lockbox->getContentSize().width / 2, lockbox->getContentSize().height / 2));
		lockbox->addChild(star);

		std::string desc = StringUtils::format("%3d     开启", starnum);
		Label* stardesclbl = Label::createWithBMFont("fonts/tips.fnt", CommonFuncs::gbk2utf(desc.c_str()));
		stardesclbl->setPosition(Vec2(lockbox->getContentSize().width / 2, lockbox->getContentSize().height / 2));
		stardesclbl->setScale(0.6f);
		lockbox->addChild(stardesclbl);
	}
}

void PlayerChallengeResultLayer::delayShowRank(float dt)
{
	this->schedule(schedule_selector(PlayerChallengeResultLayer::showRank), 1.0f / 30);

}

void PlayerChallengeResultLayer::removeLock()
{
	if (lockNode != NULL)
	{
		lockNode->removeFromParentAndCleanup(true);
		lockNode = NULL;
	}
}
