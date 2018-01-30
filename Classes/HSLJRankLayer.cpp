#include "HSLJRankLayer.h"
#include "CommonFuncs.h"
#include "HintBox.h"
#include "WaitingProgress.h"
#include "Const.h"
#include "SoundManager.h"
#include "HSLJMainLayer.h"
#include "GameScene.h"

HSLJRankLayer::HSLJRankLayer()
{

}


HSLJRankLayer::~HSLJRankLayer()
{
}

void HSLJRankLayer::initData()
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
HSLJRankLayer* HSLJRankLayer::create()
{
	HSLJRankLayer *pRet = new HSLJRankLayer();
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

void HSLJRankLayer::setStar(int num, bool isboss)
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

bool HSLJRankLayer::init()
{
	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 160));
	this->addChild(color);

	m_csbnode = CSLoader::createNode("HSLJRankLayer.csb");
	this->addChild(m_csbnode);

	cocos2d::ui::Widget *backbtn = (cocos2d::ui::Widget*)m_csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(HSLJRankLayer::onBack, this));


	srollView = (cocos2d::ui::ScrollView*)m_csbnode->getChildByName("scrollview");
	srollView->setScrollBarEnabled(false);
	srollView->setBounceEnabled(true);

	getRankData();

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void HSLJRankLayer::setStageNum(int stage)
{
	std::string numstr = StringUtils::format("%d", stage);
	m_stagenumlbl->setString(numstr);
	if (m_isboss)
		m_stagenumlbl->setVisible(false);
}


void HSLJRankLayer::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
}

void HSLJRankLayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}

void HSLJRankLayer::hilight()
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


void HSLJRankLayer::getRankData()
{
	WaitingProgress* waitbox = WaitingProgress::create("排名中...");
	Director::getInstance()->getRunningScene()->addChild(waitbox, 1, "waitbox");
	ServerDataSwap::init(this)->getHSLJRankData();
}

void HSLJRankLayer::disable()
{
	m_stageIcon->stopAllActions();
	m_node->setScale(1);
	std::string stagestr = "stage0.png";
	m_stageIcon->loadTexture(stagestr, cocos2d::ui::TextureResType::PLIST);
	m_stageIcon->setContentSize(Sprite::createWithSpriteFrameName(stagestr)->getContentSize());
}
void HSLJRankLayer::delayShowData(float dt)
{

	srollView->removeAllChildrenWithCleanup(true);
	int size = GlobalData::vec_hsljRankData.size();

	int itemheight = 78;
	int innerheight = itemheight * size;

	int contentheight = srollView->getContentSize().height;
	srollView->setContentSize(Size(srollView->getContentSize().width, contentheight));
	if (innerheight < contentheight)
		innerheight = contentheight;
	srollView->setInnerContainerSize(Size(srollView->getContentSize().width, innerheight));

	myrankdata.rank = 0;
	myrankdata.nickname = GlobalData::getMyNickName();
	myrankdata.wincount = GlobalData::myMatchInfo.mywincount;
	myrankdata.totalcount = GlobalData::myMatchInfo.mywincount + GlobalData::myMatchInfo.myfailcount;
	myrankdata.exp = GlobalData::myMatchInfo.myexp;

	for (unsigned int i = 0; i < GlobalData::vec_hsljRankData.size(); i++)
	{
		HSLJRankItem* node = HSLJRankItem::create(&GlobalData::vec_hsljRankData[i]);
		node->setPosition(Vec2(srollView->getContentSize().width/2, innerheight - itemheight / 2 - i * itemheight));
		srollView->addChild(node);
	}

	this->removeChildByName("mynode");


	Node* mynode = HSLJRankItem::create(&myrankdata);
	mynode->setPosition(Vec2(360, 170));
	this->addChild(mynode, 0, "mynode");

	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
}

void HSLJRankLayer::nomal()
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


void HSLJRankLayer::onSuccess()
{
	this->scheduleOnce(schedule_selector(HSLJRankLayer::delayShowData), 0.1f);

}

void HSLJRankLayer::showLock(int starnum)
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


void HSLJRankLayer::onErr(int errcode)
{
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
	HintBox * box = HintBox::create(CommonFuncs::gbk2utf("数据获取异常，请检查网络连接！！"));
	this->addChild(box);
}

void HSLJRankLayer::removeLock()
{
	if (lockNode != NULL)
	{
		lockNode->removeFromParentAndCleanup(true);
		lockNode = NULL;
	}
}

HSLJRankItem::HSLJRankItem()
{

}
HSLJRankItem::~HSLJRankItem()
{

}


bool HSLJRankItem::init(HSLJRankData *data)
{
	if (!Sprite::initWithSpriteFrameName("ui/blank.png"))
		return false;

	m_data = data;

	Node* csbnode = CSLoader::createNode("hsljRankNode.csb");//物品节点

	csbnode->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height / 2));
	this->addChild(csbnode);

	int rank = data->rank;

	std::string itemstr;

	cocos2d::ui::Text* ranknumlbl = (cocos2d::ui::Text*)csbnode->getChildByName("ranknum");
	cocos2d::ui::Text* nicknamelbl = (cocos2d::ui::Text*)csbnode->getChildByName("nickname");
	cocos2d::ui::Text* winpercentlbl = (cocos2d::ui::Text*)csbnode->getChildByName("winpercent");
	cocos2d::ui::Text* explbl = (cocos2d::ui::Text*)csbnode->getChildByName("exp");
	cocos2d::ui::Text* expnamelbl = (cocos2d::ui::Text*)csbnode->getChildByName("expname");

	if (data->nickname.compare(GlobalData::getMyNickName()) == 0)
	{
		HSLJRankLayer* rlayer = (HSLJRankLayer*)g_gameLayer->getChildByName("shljranklayer");
		rlayer->myrankdata.rank = rank;
		//itemstr = "ui/hsljmyitem.png";
		ranknumlbl->setTextColor(Color4B(192, 88, 32, 255));
		nicknamelbl->setTextColor(Color4B(192, 88, 32, 255));
		winpercentlbl->setTextColor(Color4B(192, 88, 32, 255));
		explbl->setTextColor(Color4B(192, 88, 32, 255));
		expnamelbl->setTextColor(Color4B(192, 88, 32, 255));
	}
	else if (rank % 2 == 0)
	{
		itemstr = "ui/hsljitem0.png";
	}

	if (itemstr.length() > 0)
	{
		/*cocos2d::ui::ImageView* item = (cocos2d::ui::ImageView*)csbnode->getChildByName("item");
		item->loadTexture(itemstr, cocos2d::ui::TextureResType::PLIST);*/
	}

	std::string str = StringUtils::format("%d", rank);
	ranknumlbl->setString(str);

	if (rank <= 3 && rank > 0)
	{
		ranknumlbl->setVisible(false);
		std::string rankspritename = StringUtils::format("ui/ranknum%d.png", rank);
		Sprite* ranknum = Sprite::createWithSpriteFrameName(rankspritename);
		ranknum->setPosition(ranknumlbl->getPosition());
		csbnode->addChild(ranknum);
	}
	else if (rank == 0)
	{
		ranknumlbl->setString(CommonFuncs::gbk2utf("未上榜"));
		ranknumlbl->setScale(0.7f);
	}

	nicknamelbl->setString(data->nickname);

	if (data->totalcount > 0)
		str = StringUtils::format("%d%%", data->wincount * 100 / data->totalcount);
	else
		str = "0%";
	winpercentlbl->setString(str);

	int exp = data->exp;
	str = StringUtils::format("%d", exp);
	explbl->setString(str);

	str = HSLJMainLayer::getDwStr(exp);

	expnamelbl->setString(CommonFuncs::gbk2utf(str.c_str()));

	return true;
}

HSLJRankItem* HSLJRankItem::create(HSLJRankData *data)
{
	HSLJRankItem *pRet = new HSLJRankItem();
	if (pRet && pRet->init(data))
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
