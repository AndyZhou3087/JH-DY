#include "FactionMemberLayer.h"
#include "CommonFuncs.h"
#include "HintBox.h"
#include "WaitingProgress.h"
#include "Const.h"
#include "GameScene.h"
#include "PromotionLayer.h"
#include "FactionMainLayer.h"
#include "FactionComfirmLayer.h"
#include "MD5.h"
#include "GameDataSave.h"
#include "FactionKickComfirmLayer.h"
#include "FactionCreateLayer.h"

const std::string positionstr[] = { "", "帮主", "副帮主", "长老", "帮众" };
FactionMemberLayer::FactionMemberLayer()
{
	f_action = F_NONE;
}


FactionMemberLayer::~FactionMemberLayer()
{
	f_action = F_NONE;
}


FactionMemberLayer* FactionMemberLayer::create(FactionListData *fldata)
{
	FactionMemberLayer *pRet = new FactionMemberLayer();
	if (pRet && pRet->init(fldata))
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

bool FactionMemberLayer::init(FactionListData *fldata)
{
	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 160));
	this->addChild(color);

	Node* csbnode = CSLoader::createNode("factionMemberLayer.csb");
	this->addChild(csbnode);

	cocos2d::ui::Widget *backbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(FactionMemberLayer::onBack, this));

	cocos2d::ui::Button* modifybtn = (cocos2d::ui::Button*)csbnode->getChildByName("moditybtn");
	modifybtn->addTouchEventListener(CC_CALLBACK_2(FactionMemberLayer::onModity, this));

	cocos2d::ui::Button* actionbtn = (cocos2d::ui::Button*)csbnode->getChildByName("actionbtn");
	actionbtn->addTouchEventListener(CC_CALLBACK_2(FactionMemberLayer::onAction, this));
	cocos2d::ui::Text* actiontext = (cocos2d::ui::Text*)actionbtn->getChildByName("text");
	if (GlobalData::mytitle == 1)
	{
		actionbtn->setPositionX(220);
		actiontext->setString(CommonFuncs::gbk2utf("解散帮派"));
		modifybtn->setVisible(true);
	}

	cocos2d::ui::Widget *sliserContriBtn = (cocos2d::ui::Widget*)csbnode->getChildByName("sliverbtn");
	sliserContriBtn->addTouchEventListener(CC_CALLBACK_2(FactionMemberLayer::onContribution, this));
	sliserContriBtn->setTag(0);

	cocos2d::ui::Widget *goldContriBtn = (cocos2d::ui::Widget*)csbnode->getChildByName("goldbtn");
	goldContriBtn->addTouchEventListener(CC_CALLBACK_2(FactionMemberLayer::onContribution, this));
	goldContriBtn->setTag(1);

	cocos2d::ui::Text* namelbl = (cocos2d::ui::Text*)csbnode->getChildByName("name");
	namelbl->setString(fldata->factionname);
	
	std::string str;
	int lv = fldata->lv + 1;
	lvlbl = (cocos2d::ui::Text*)csbnode->getChildByName("lv");
	str = StringUtils::format("%d", lv);
	lvlbl->setString(str);

	countlbl = (cocos2d::ui::Text*)csbnode->getChildByName("count");
	str = StringUtils::format("%d/%d", fldata->membercount, fldata->maxcount);
	countlbl->setString(str);

	explbl = (cocos2d::ui::Text*)csbnode->getChildByName("exp");
	str = StringUtils::format("%d/%d", fldata->exp, lv*lv*fldata->maxcount*100);
	explbl->setString(str);

	std::string descstr = StringUtils::format("%s", fldata->desc.c_str());
	cocos2d::ui::Text* desclbl = (cocos2d::ui::Text*)csbnode->getChildByName("desc");
	desclbl->setString(descstr);

	srollView = (cocos2d::ui::ScrollView*)csbnode->getChildByName("scrollview");
	srollView->setScrollBarEnabled(false);
	srollView->setBounceEnabled(true);

	m_fldata = fldata;

	int curday = GlobalData::getDayOfYear();
	int contributionday = GameDataSave::getInstance()->getContributionDay();

	if (curday != contributionday)
	{
		GameDataSave::getInstance()->setContributionDay(curday);
		GameDataSave::getInstance()->setSliverContribution(0);
		GameDataSave::getInstance()->setGoldContribution(0);
	}

	getFactionMemberData();

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void FactionMemberLayer::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
}

void FactionMemberLayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}

void FactionMemberLayer::onAction(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		FactionComfirmLayer* fclayer = FactionComfirmLayer::create(m_fldata);
		this->addChild(fclayer);
	}
}

void FactionMemberLayer::onModity(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		FactionCreateLayer* fclayer = FactionCreateLayer::create(1, m_fldata);
		g_gameLayer->addChild(fclayer, 5);
		this->removeFromParentAndCleanup(true);
	}
}


void FactionMemberLayer::onContribution(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Node* btnnode = (Node*)pSender;
		int tag = btnnode->getTag();
		int contribution = 0;
		bool isok = false;
		if (tag == 0)
		{
			int c = GameDataSave::getInstance()->getSliverContribution();
            if (c >= 100)
            {
                HintBox* hbox = HintBox::create(CommonFuncs::gbk2utf("银两贡献超额，每日限额100银两！"));
                this->addChild(hbox);
                return;
            }
			if (StorageRoom::getCountById("80") < 10)
			{
				HintBox* hbox = HintBox::create(CommonFuncs::gbk2utf("银两不足！"));
				this->addChild(hbox);
			}
			else
			{
				contribution = 10;
				usetypecontribution = 0;
				isok = true;
			}
		}
		else
		{
			int c = GameDataSave::getInstance()->getGoldContribution();
            if (c >= 100)
            {
                HintBox* hbox = HintBox::create(CommonFuncs::gbk2utf("元宝贡献超额，每日限额100元宝！"));
                this->addChild(hbox);
                return;
            }

			int mygold = GlobalData::getMyGoldCount();

			if (mygold >= 10)
			{
				if (GlobalData::getMD5MyGoldCount().compare(md5(mygold)) != 0)
				{
					GlobalData::dataIsModified = true;
					GlobalData::setMyGoldCount(0);
					HintBox* hint = HintBox::create(CommonFuncs::gbk2utf("发现有作弊行为，金元宝清零作为处罚！！"));
					this->addChild(hint);
					return;
				}
				contribution = 100;
				usetypecontribution = 1;
				isok = true;
			}
			else
			{
				HintBox* hbox = HintBox::create(CommonFuncs::gbk2utf("金元宝不足！"));
				this->addChild(hbox);
			}
		}
		if (isok)
		{
			f_action = F_CONTRIB;
			WaitingProgress* waitbox = WaitingProgress::create("加载中...");
			Director::getInstance()->getRunningScene()->addChild(waitbox, 1, "waitbox");

			ServerDataSwap::init(this)->contributionFaction(m_fldata->id, contribution, g_hero->getHeadID());

		}
	}
}

void FactionMemberLayer::getFactionMemberData()
{
	WaitingProgress* waitbox = WaitingProgress::create("加载中...");
	Director::getInstance()->getRunningScene()->addChild(waitbox, 1, "waitbox");
	ServerDataSwap::init(this)->getFactionMembers(m_fldata->id);
}

void FactionMemberLayer::delayShowData(float dt)
{
	srollView->removeAllChildrenWithCleanup(true);
	int size = GlobalData::vec_factionMemberData.size();

	int itemheight = 78;
	int innerheight = itemheight * size;
	int contentheight = srollView->getContentSize().height;
	if (innerheight < contentheight)
		innerheight = contentheight;
	srollView->setInnerContainerSize(Size(srollView->getContentSize().width, innerheight));

	for (unsigned int i = 0; i < GlobalData::vec_factionMemberData.size(); i++)
	{
		FactionMemberItem* node = FactionMemberItem::create(&GlobalData::vec_factionMemberData[i]);
		node->setPosition(Vec2(srollView->getContentSize().width/2, innerheight - itemheight / 2 - i * itemheight));
		std::string nodestr = StringUtils::format("fmitem%d", i);
		srollView->addChild(node,0, nodestr);
	}

	updateUi();

	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
}

void FactionMemberLayer::updateUi()
{
	int lv = 0;
	std::string str;
	for (int i = 99; i >= 0; i--)
	{
		if (m_fldata->exp >= i*i*(20+(i-1)*5) * 100)
		{
			lv = i;
			break;
		}
	}
	m_fldata->lv = lv;

	lv = lv + 1;

	bool ismaxlv = false;
	if (lv > 5)
	{
		lv = 5;
		m_fldata->lv = 4;
		ismaxlv = true;
	}
	str = StringUtils::format("%d", lv);
	lvlbl->setString(str);
	m_fldata->maxcount = 20 + (lv-1) * 5;
	str = StringUtils::format("%d/%d", m_fldata->membercount, m_fldata->maxcount);
	countlbl->setString(str);

	str = StringUtils::format("%d/%d", m_fldata->exp, lv*lv*m_fldata->maxcount * 100);
	if (ismaxlv)
		str = "-/-";
	explbl->setString(str);


	FactionMainLayer* fmlayer = (FactionMainLayer*)g_gameLayer->getChildByName("factionmainlayer");
	if (fmlayer != NULL)
	{
		fmlayer->updateLvAndMember();
	}

}

void FactionMemberLayer::onSuccess()
{
	if (f_action == F_CONTRIB)
	{
		Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
		f_action = F_NONE;

		if (GlobalData::factionExp > 0)
			m_fldata->exp = GlobalData::factionExp;

		getFactionMemberData();
		if (usetypecontribution == 0)
		{
			int c = GameDataSave::getInstance()->getSliverContribution();
			GameDataSave::getInstance()->setSliverContribution(c+10);
			StorageRoom::use("80", 10);
		}
		else if (usetypecontribution == 1)
		{
			int c = GameDataSave::getInstance()->getGoldContribution();
			GameDataSave::getInstance()->setGoldContribution(c + 10);
			GlobalData::setMyGoldCount(GlobalData::getMyGoldCount() - 10);
			int usegold = GlobalData::getUseGold() + 10;
			GlobalData::setUseGold(usegold);
		}
	}
	else
	{
		this->scheduleOnce(schedule_selector(FactionMemberLayer::delayShowData), 0.1f);
	}
}

void FactionMemberLayer::onErr(int errcode)
{
	f_action = F_NONE;
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
	HintBox * box = HintBox::create(CommonFuncs::gbk2utf("数据获取异常，请检查网络连接！！"));
	this->addChild(box);
}

FactionMemberItem::FactionMemberItem()
{
	f_action = F_NONE;
}
FactionMemberItem::~FactionMemberItem()
{

}

bool FactionMemberItem::init(FactionMemberData *data)
{
	if (!Sprite::initWithSpriteFrameName("ui/blank.png"))
		return false;

	m_data = data;
	Node* csbnode = CSLoader::createNode("factionmemberNode.csb");
	csbnode->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height / 2));
	this->addChild(csbnode);

	std::string itemstr;
	if (data->id % 2 == 0)
	{
		itemstr = "ui/factionmemberitem0.png";
	}
	if (itemstr.length() > 0)
	{
		cocos2d::ui::ImageView* item = (cocos2d::ui::ImageView*)csbnode->getChildByName("item");
		item->loadTexture(itemstr, cocos2d::ui::TextureResType::PLIST);
	}

	std::string headstr = StringUtils::format("ui/fhero%d.png", m_data->herotype);
	/*cocos2d::ui::ImageView* headimg = (cocos2d::ui::ImageView*)csbnode->getChildByName("headimg");
	headimg->loadTexture(headstr, cocos2d::ui::TextureResType::PLIST);
	headimg->setScale(0.32f);*/

	cocos2d::ui::Text* nicknamelbl = (cocos2d::ui::Text*)csbnode->getChildByName("name");
	nicknamelbl->setString(data->nickname);

	cocos2d::ui::Text* lvlbl = (cocos2d::ui::Text*)csbnode->getChildByName("lv");
	std::string str = StringUtils::format("%d", data->herolv + 1);
	lvlbl->setString(str);

	contributionlbl = (cocos2d::ui::Text*)csbnode->getChildByName("contribution");
	str = StringUtils::format("%d", data->contribution);
	contributionlbl->setString(str);

	postionlbl = (cocos2d::ui::Text*)csbnode->getChildByName("postion");
	postionlbl->setString(CommonFuncs::gbk2utf(positionstr[data->position].c_str()));

	actionbtn = (cocos2d::ui::Button*)csbnode->getChildByName("actionbtn");
	actionbtn->addTouchEventListener(CC_CALLBACK_2(FactionMemberItem::onAction, this));
	actionbtn->setSwallowTouches(false);

	cocos2d::ui::Text* actionbtnlabel = (cocos2d::ui::Text*)actionbtn->getChildByName("text");

	refusebtn = (cocos2d::ui::Button*)csbnode->getChildByName("refusebtn");
	refusebtn->addTouchEventListener(CC_CALLBACK_2(FactionMemberItem::onRefuse, this));
	refusebtn->setSwallowTouches(false);

	modifybtn = (cocos2d::ui::Button*)csbnode->getChildByName("modifybtn");
	modifybtn->addTouchEventListener(CC_CALLBACK_2(FactionMemberItem::onModify, this));
	modifybtn->setSwallowTouches(false);

	if (atoi(GlobalData::getMyID().c_str()) == data->userid && g_hero->getHeadID() == data->herotype)
	{
		actionbtn->setVisible(false);
		modifybtn->setVisible(false);
	}
	else
	{
		if (GlobalData::mytitle == 0 || GlobalData::mytitle == 4)
		{
			actionbtn->setVisible(false);
			modifybtn->setVisible(false);
		}
		else
		{
			if (data->position == 0)
			{
				actionbtnlabel->setString(CommonFuncs::gbk2utf("同意加入"));
				modifybtn->setVisible(false);
				refusebtn->setVisible(true);
			}
			else if (data->position <= GlobalData::mytitle)
			{
				actionbtn->setVisible(false);
				modifybtn->setVisible(false);
			}
			else
			{
				actionbtnlabel->setString(CommonFuncs::gbk2utf("逐出"));
			}
		}
	}

	return true;
}

FactionMemberItem* FactionMemberItem::create(FactionMemberData *data)
{
	FactionMemberItem *pRet = new FactionMemberItem();
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
void FactionMemberItem::onAction(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		cocos2d::ui::Button* actionbtn = (cocos2d::ui::Button*)pSender;
		cocos2d::ui::Text* actionbtnlabel = (cocos2d::ui::Text*)actionbtn->getChildByName("text");
		if (actionbtnlabel->getString().compare(CommonFuncs::gbk2utf("同意加入")) == 0)
		{
			WaitingProgress* waitbox = WaitingProgress::create("处理中...");
			Director::getInstance()->getRunningScene()->addChild(waitbox, 1, "waitbox");
			ServerDataSwap::init(this)->joinFaction(m_data->factionid, m_data->userid, m_data->herotype);
		}
		else if (actionbtnlabel->getString().compare(CommonFuncs::gbk2utf("逐出")) == 0)
		{
			FactionKickComfirmLayer* player = FactionKickComfirmLayer::create(this, m_data);
			g_gameLayer->addChild(player, 5);
		}
	}
}

void FactionMemberItem::onModify(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		PromotionLayer* player = PromotionLayer::create(m_data, this);
		g_gameLayer->addChild(player, 5, "promotionlayer");
	}
}

void FactionMemberItem::onRefuse(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		f_action = F_REFUSE;
		WaitingProgress* waitbox = WaitingProgress::create("加载中...");
		Director::getInstance()->getRunningScene()->addChild(waitbox, 1, "waitbox");
		ServerDataSwap::init(this)->refuseFaction(m_data->factionid, m_data->userid, m_data->herotype);
	}
}

void FactionMemberItem::onSuccess()
{
	FactionMemberLayer* mlayer = (FactionMemberLayer*)g_gameLayer->getChildByName("factionmemberlayer");

	if (f_action == F_REFUSE)
	{
		removeItem();
	}
	else
	{
		cocos2d::ui::Text* actionbtnlabel = (cocos2d::ui::Text*)actionbtn->getChildByName("text");
		if (actionbtnlabel->getString().compare(CommonFuncs::gbk2utf("同意加入")) == 0)
		{
			Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
			m_data->position = 4;
			modifybtn->setVisible(true);
			refusebtn->setVisible(false);
			actionbtnlabel->setString(CommonFuncs::gbk2utf("逐出"));
			postionlbl->setString(CommonFuncs::gbk2utf("帮众"));

			for (unsigned int i = 0; i < GlobalData::vec_factionListData.size(); i++)
			{
				if (GlobalData::vec_factionListData[i].id == m_data->factionid)
				{
					GlobalData::vec_factionListData[i].membercount++;
					break;
				}
			}
			mlayer->updateUi();
		}
	}

	f_action = F_NONE;
}

void FactionMemberItem::removeItem()
{
	FactionMemberLayer* mlayer = (FactionMemberLayer*)g_gameLayer->getChildByName("factionmemberlayer");

	for (unsigned int i = 0; i < GlobalData::vec_factionMemberData.size(); i++)
	{
		if (GlobalData::vec_factionMemberData[i].userid == m_data->userid && GlobalData::vec_factionMemberData[i].herotype == m_data->herotype)
		{
			GlobalData::vec_factionMemberData.erase(GlobalData::vec_factionMemberData.begin() + i);
			break;
		}
	}
	if (mlayer != NULL)
	{
		mlayer->updateUi();
		mlayer->delayShowData(0);
	}
}

void FactionMemberItem::onErr(int errcode)
{
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
	HintBox * box = HintBox::create(CommonFuncs::gbk2utf("操作失败，请检查网络设置稍后重试！！"));
	Director::getInstance()->getRunningScene()->addChild(box);

	f_action = F_NONE;
}

void FactionMemberItem::updatePosition(int position)
{
	postionlbl->setString(CommonFuncs::gbk2utf(positionstr[position].c_str()));
}
