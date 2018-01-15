#include "GetVipRewardLayer.h"
#include "CommonFuncs.h"
#include "SoundManager.h"
#include "GlobalData.h"
#include "Const.h"
#include "GoldGoodsItem.h"
#include "WaitingProgress.h"
#include "LoginRewardLayer.h"
#include "GameScene.h"

GetVipRewardLayer::GetVipRewardLayer()
{
	rgoldcount = 0;
}


GetVipRewardLayer::~GetVipRewardLayer()
{

}

bool GetVipRewardLayer::init()
{
	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 200));
	this->addChild(color);

	Node* csbnode = CSLoader::createNode("getVipRewardLayer.csb");
	this->addChild(csbnode);

	cocos2d::ui::Button* closebtn = (cocos2d::ui::Button*)csbnode->getChildByName("closebtn");
	closebtn->addTouchEventListener(CC_CALLBACK_2(GetVipRewardLayer::onClose, this));

	std::string namestr;

	int golditemcount = sizeof(goldcount) / sizeof(goldcount[0]);

	std::vector<std::string> vec_rewardres;
	for (unsigned int i = 0; i < GlobalData::vec_buyVipIds.size(); i++)
	{
		for (unsigned int m = 0; m < GlobalData::vec_goods.size(); m++)
		{
			std::string vipid = GlobalData::vec_goods[m].icon;
			if (GlobalData::vec_buyVipIds[i].compare(vipid) == 0)
			{
				vec_rgoodsindex.push_back(m);
				namestr.append(GlobalData::vec_goods[m].name);
				namestr.append(CommonFuncs::gbk2utf("、"));
				rgoldcount += vipgoldcount[m - golditemcount];

				for (unsigned int n = 0; n < GlobalData::vec_goods[m].vec_res.size(); n++)
				{
					int intres = atoi(GlobalData::vec_goods[m].vec_res[n].c_str());
					int intid = intres / 1000;
					int count = intres % 1000;
					bool isfind = false;
					for (unsigned int j = 0; j < vec_rewardres.size(); j++)
					{
						int intres1 = atoi(vec_rewardres[j].c_str());
						int intid1 = intres1 / 1000;
						int count1 = intres1 % 1000;
						if (intid == intid1)
						{
							std::string newstr = StringUtils::format("%d", intid * 1000 + count + count1);
							vec_rewardres[j] = newstr;
							isfind = true;
							break;
						}
					}
					if (!isfind)
					{
						vec_rewardres.push_back(GlobalData::vec_goods[m].vec_res[n]);
					}
				}
			}
		}
	}
	namestr = namestr.substr(0, namestr.length() - 3);

	std::string descstr = StringUtils::format("%s%s%s", CommonFuncs::gbk2utf("获取").c_str(), namestr.c_str(), CommonFuncs::gbk2utf("福利：").c_str());
	cocos2d::ui::Text* desctext = (cocos2d::ui::Text*)csbnode->getChildByName("desc");
	desctext->setString(descstr);

	int startx = 75;
	int spacex = 140;
	int starty = 620;
	if (vec_rewardres.size() == 2)
	{
		startx = 160;
		spacex = 200;
	}

	int ressize = vec_rewardres.size() + 1;
	for (int i = 0; i < ressize; i++)
	{
		Sprite * box = Sprite::createWithSpriteFrameName("ui/buildsmall.png");
		box->setPosition(Vec2(startx + i*spacex, starty));
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
		coutlbl->setAnchorPoint(Vec2(1, 0.5));
		coutlbl->setColor(Color3B(255, 255, 255));
		coutlbl->setPosition(Vec2(box->getPositionX() + box->getContentSize().width / 2 - 10, 580));
		this->addChild(coutlbl);

		Label * namelbl = Label::createWithTTF(namstr, "fonts/STXINGKA.TTF", 26);
		namelbl->setColor(Color3B(0, 0, 0));
		namelbl->setPosition(Vec2(box->getPositionX(), 530));
		this->addChild(namelbl);
	}
	WaitingProgress* waitbox = WaitingProgress::create("获取月卡中...");
	Director::getInstance()->getRunningScene()->addChild(waitbox, 1, "waitbox");
	ServerDataSwap::init(this)->isGetVip(GlobalData::vec_buyVipIds);
	GlobalData::vec_buyVipIds.clear();
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

GetVipRewardLayer* GetVipRewardLayer::create()
{
	GetVipRewardLayer *pRet = new GetVipRewardLayer();
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

void GetVipRewardLayer::onClose(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);

		if (GlobalData::continueLoginDays > 0)
		{
			LoginRewardLayer* llayer = LoginRewardLayer::create();
			g_gameLayer->addChild(llayer, 100);
		}
	}
}

void GetVipRewardLayer::getVipGoods()
{
	GlobalData::setMyGoldCount(GlobalData::getMyGoldCount() + rgoldcount);

	for (unsigned int m = 0; m < vec_rgoodsindex.size();m++)
		GoldGoodsItem::addBuyGoods(&GlobalData::vec_goods[vec_rgoodsindex[m]]);

}
void GetVipRewardLayer::onSuccess()
{
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
	getVipGoods();
}

void GetVipRewardLayer::onErr(int errcode)
{
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
	this->removeFromParentAndCleanup(true);
}