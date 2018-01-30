#include "MixSuggestLayer.h"
#include "CommonFuncs.h"
#include "Const.h"
#include "SoundManager.h"
#include "WaitingProgress.h"
#include "GameDataSave.h"
#include "HintBox.h"
#include "MD5.h"

MixSuggestLayer::MixSuggestLayer()
{
	startime0 = 0;
	startime1 = 1;
	startime2 = 0;
}


MixSuggestLayer::~MixSuggestLayer()
{
	GlobalData::g_gameStatus = GAMESTART;
}

void MixSuggestLayer::initData()
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

bool MixSuggestLayer::init()
{
	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 150));
	this->addChild(color);

	csbroot = CSLoader::createNode("mixSuggestLayer.csb");
	this->addChild(csbroot);


	desc0 = (cocos2d::ui::Text*)csbroot->getChildByName("desc0");
	desc1 = (cocos2d::ui::Text*)csbroot->getChildByName("desc1");
	desc = (cocos2d::ui::Text*)csbroot->getChildByName("desc");

	for (int i = 0; i < 4; i++)
	{
		std::string str = StringUtils::format("mixbox_%d", i);

		imgbox[i] = (cocos2d::ui::ImageView*)csbroot->getChildByName(str);
		img[i] = (cocos2d::ui::ImageView*)csbroot->getChildByName(str)->getChildByName("img");

		str = StringUtils::format("name%d", i);
		name[i] = (cocos2d::ui::Text*)csbroot->getChildByName(str);
	}

	freebtn = (cocos2d::ui::Button*)csbroot->getChildByName("freebtn");
	freebtn->addTouchEventListener(CC_CALLBACK_2(MixSuggestLayer::onFree, this));
	freebtn->setEnabled(false);

	silverbtn = (cocos2d::ui::Button*)csbroot->getChildByName("silverbtn");
	silverbtn->addTouchEventListener(CC_CALLBACK_2(MixSuggestLayer::onSilver, this));

	goldbtn = (cocos2d::ui::Button*)csbroot->getChildByName("goldbtn");
	goldbtn->addTouchEventListener(CC_CALLBACK_2(MixSuggestLayer::onGold, this));

	cocos2d::ui::Button* backbtn = (cocos2d::ui::Button*)csbroot->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(MixSuggestLayer::onBack, this));

	freetimelbl0 = (cocos2d::ui::Text*)csbroot->getChildByName("freetime0");
	freetimelbl0->setVisible(false);

	freetimelbl1 = (cocos2d::ui::Text*)csbroot->getChildByName("freetime1");
	freetimelbl1->setVisible(false);

	freetimelbl2 = (cocos2d::ui::Text*)csbroot->getChildByName("freetime2");
	freetimelbl2->setVisible(false);


	mixname = (cocos2d::ui::Text*)csbroot->getChildByName("mixtitle");

	std::string suggestgfstr = GameDataSave::getInstance()->getSuggestMixGf();

	if (suggestgfstr.length() > 0)
		loadMixGfUi(GlobalData::map_MixGfData[suggestgfstr]);

	startime0 = GameDataSave::getInstance()->getFreeMixTime(0);
	startime1 = GameDataSave::getInstance()->getFreeMixTime(1);
	startime2 = GameDataSave::getInstance()->getFreeMixTime(2);

	getServerTime();

	GlobalData::g_gameStatus = GAMEPAUSE;
	auto m_listener = EventListenerTouchOneByOne::create();
	m_listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};
	m_listener->setSwallowTouches(false);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(m_listener, this);

	return true;
}

void MixSuggestLayer::setStar(int num, bool isboss)
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

void MixSuggestLayer::getServerTime()
{
	WaitingProgress* waitbox = WaitingProgress::create("加载中...");
	Director::getInstance()->getRunningScene()->addChild(waitbox, 1, "waitbox");
	ServerDataSwap::init(this)->getServerTime();
}

void MixSuggestLayer::setStageNum(int stage)
{
	std::string numstr = StringUtils::format("%d", stage);
	m_stagenumlbl->setString(numstr);
	if (m_isboss)
		m_stagenumlbl->setVisible(false);
}

void MixSuggestLayer::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
}

void MixSuggestLayer::onExit()
{
	Layer::onExit();
}

void MixSuggestLayer::hilight()
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


void MixSuggestLayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}


void MixSuggestLayer::disable()
{
	m_stageIcon->stopAllActions();
	m_node->setScale(1);
	std::string stagestr = "stage0.png";
	m_stageIcon->loadTexture(stagestr, cocos2d::ui::TextureResType::PLIST);
	m_stageIcon->setContentSize(Sprite::createWithSpriteFrameName(stagestr)->getContentSize());
}

void MixSuggestLayer::onFree(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		freebtn->setEnabled(false);
		freetimelbl0->setVisible(true);
		startime0 = severtime;
		GameDataSave::getInstance()->setFreeMixTime(0, startime0);
		randMixGf(0);
	}
}

void MixSuggestLayer::nomal()
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


void MixSuggestLayer::onSilver(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		cocos2d::ui::ImageView* text = (cocos2d::ui::ImageView*)silverbtn->getChildByName("text");
		if (severtime - startime1 >= 86400)
		{
			silverbtn->getChildByName("silver")->setVisible(false);
			silverbtn->getChildByName("silvercount")->setVisible(false);
			freetimelbl1->setVisible(false);
			//text->setPositionY(45);
			//text->setFontSize(50);
			text->loadTexture("ui/freelabel.png", cocos2d::ui::TextureResType::PLIST);
			startime1 = severtime;
			GameDataSave::getInstance()->setFreeMixTime(1, startime1);
			randMixGf(1);
		}
		else
		{

			silverbtn->getChildByName("silver")->setVisible(true);
			silverbtn->getChildByName("silvercount")->setVisible(true);
			freetimelbl1->setVisible(true);
			//text->setPositionY(50);
			//text->setFontSize(42);
			text->loadTexture("ui/freshlabel.png", cocos2d::ui::TextureResType::PLIST);

			if (StorageRoom::getCountById("80") < 50)
			{
				HintBox* hbox = HintBox::create(CommonFuncs::gbk2utf("银两不足！"));
				this->addChild(hbox);
			}
			else
			{
				StorageRoom::use("80", 50);
				randMixGf(1);
			}
		}
	}
}

void MixSuggestLayer::showLock(int starnum)
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


void MixSuggestLayer::onGold(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		cocos2d::ui::ImageView* text = (cocos2d::ui::ImageView*)goldbtn->getChildByName("text");
		if (severtime - startime2 >= 86400)
		{
			goldbtn->getChildByName("gold")->setVisible(false);
			goldbtn->getChildByName("goldcount")->setVisible(false);
			freetimelbl2->setVisible(false);
			//text->setPositionY(45);
			//text->setFontSize(50);
			text->loadTexture("ui/freelabel.png", cocos2d::ui::TextureResType::PLIST);
			startime2 = severtime;

			GameDataSave::getInstance()->setFreeMixTime(2, startime2);
			randMixGf(2);
		}
		else
		{
			goldbtn->getChildByName("gold")->setVisible(true);
			goldbtn->getChildByName("goldcount")->setVisible(true);
			freetimelbl2->setVisible(true);
			//text->setPositionY(50);
			//text->setFontSize(42);
			text->loadTexture("ui/freshlabel.png", cocos2d::ui::TextureResType::PLIST);

			int mygold = GlobalData::getMyGoldCount();

			if (mygold >= 50)
			{
				if (GlobalData::getMD5MyGoldCount().compare(md5(mygold)) != 0)
				{
					GlobalData::dataIsModified = true;
					GlobalData::setMyGoldCount(0);
					HintBox* hint = HintBox::create(CommonFuncs::gbk2utf("发现有作弊行为，金元宝清零作为处罚！！"));
					this->addChild(hint);
					return;
				}

				GlobalData::setMyGoldCount(GlobalData::getMyGoldCount() - 50);
				randMixGf(2);
			}
			else
			{
				HintBox* hbox = HintBox::create(CommonFuncs::gbk2utf("金元宝不足！"));
				this->addChild(hbox);
			}
		}
	}
}

void MixSuggestLayer::removeLock()
{
	if (lockNode != NULL)
	{
		lockNode->removeFromParentAndCleanup(true);
		lockNode = NULL;
	}
}


void MixSuggestLayer::onSuccess()
{
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");

	severtime = GlobalData::servertime;

	this->unschedule(schedule_selector(MixSuggestLayer::updateServerTime));
	this->schedule(schedule_selector(MixSuggestLayer::updateServerTime), 1.0f);

	if (severtime - startime0 >= 86400)
	{
		freebtn->setEnabled(true);
		freetimelbl0->setVisible(false);
	}

	if (severtime - startime1 >= 86400)
	{
		cocos2d::ui::ImageView* text = (cocos2d::ui::ImageView*)silverbtn->getChildByName("text");
		silverbtn->getChildByName("silver")->setVisible(false);
		silverbtn->getChildByName("silvercount")->setVisible(false);
		freetimelbl1->setVisible(false);
		//text->setPositionY(45);
		//text->setFontSize(50);
		text->loadTexture("ui/freelabel.png", cocos2d::ui::TextureResType::PLIST);
	}
	if (severtime - startime2 >= 86400)
	{
		cocos2d::ui::ImageView* text = (cocos2d::ui::ImageView*)goldbtn->getChildByName("text");
		goldbtn->getChildByName("gold")->setVisible(false);
		goldbtn->getChildByName("goldcount")->setVisible(false);
		freetimelbl2->setVisible(false);
		//text->setPositionY(45);
		//text->setFontSize(50);
		text->loadTexture("ui/freelabel.png", cocos2d::ui::TextureResType::PLIST);
	}
}

void MixSuggestLayer::onErr(int errcode)
{
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");

}

void MixSuggestLayer::updateServerTime(float dt)
{
	severtime++;

	if (severtime - startime0 >= 86400)
	{
		freebtn->setEnabled(true);
		freetimelbl0->setVisible(false);
	}
	else
	{
		freebtn->setEnabled(false);
		freetimelbl0->setVisible(true);

		int lefttime = 86400 - (severtime - startime0);
		std::string str = StringUtils::format("%02d:%02d:%02d", lefttime / 3600, lefttime % 3600 / 60, lefttime % 3600 % 60);
		freetimelbl0->setString(str);
	}

	if (severtime - startime1 >= 86400)
	{
		cocos2d::ui::ImageView* text = (cocos2d::ui::ImageView*)silverbtn->getChildByName("text");
		silverbtn->getChildByName("silver")->setVisible(false);
		silverbtn->getChildByName("silvercount")->setVisible(false);
		freetimelbl1->setVisible(false);
		//text->setPositionY(45);
		//text->setFontSize(50);
		text->loadTexture("ui/freelabel.png", cocos2d::ui::TextureResType::PLIST);
	}
	else
	{
		cocos2d::ui::ImageView* text = (cocos2d::ui::ImageView*)silverbtn->getChildByName("text");
		silverbtn->getChildByName("silver")->setVisible(true);
		silverbtn->getChildByName("silvercount")->setVisible(true);
		freetimelbl1->setVisible(true);
		//text->setPositionY(50);
		//text->setFontSize(42);
		text->loadTexture("ui/freshlabel.png", cocos2d::ui::TextureResType::PLIST);

		int lefttime = 86400 - (severtime - startime1);
		std::string str = StringUtils::format("%02d:%02d:%02d后免费", lefttime / 3600, lefttime % 3600 / 60, lefttime % 3600 % 60);
		freetimelbl1->setString(CommonFuncs::gbk2utf(str.c_str()));
	}

	if (severtime - startime2 >= 86400)
	{
		cocos2d::ui::ImageView* text = (cocos2d::ui::ImageView*)goldbtn->getChildByName("text");
		goldbtn->getChildByName("gold")->setVisible(false);
		goldbtn->getChildByName("goldcount")->setVisible(false);
		freetimelbl2->setVisible(false);
		//text->setPositionY(45);
		//text->setFontSize(50);
		text->loadTexture("ui/freelabel.png", cocos2d::ui::TextureResType::PLIST);
	}
	else
	{
		cocos2d::ui::ImageView* text = (cocos2d::ui::ImageView*)goldbtn->getChildByName("text");
		goldbtn->getChildByName("gold")->setVisible(true);
		goldbtn->getChildByName("goldcount")->setVisible(true);
		freetimelbl2->setVisible(true);
		//text->setPositionY(50);
		//text->setFontSize(42);

		int lefttime = 86400 - (severtime - startime2);
		std::string str = StringUtils::format("%02d:%02d:%02d后免费", lefttime / 3600, lefttime % 3600 / 60, lefttime % 3600 % 60);
		freetimelbl2->setString(CommonFuncs::gbk2utf(str.c_str()));
		text->loadTexture("ui/freshlabel.png", cocos2d::ui::TextureResType::PLIST);
	}
}


void MixSuggestLayer::jumpDown(cocos2d::Node *node, float dt) {
	if (nullptr == node) {
		return;
	}

	const float originY = node->getPositionY();
	node->setPositionY(originY + dt);

	ActionInterval *action = Sequence::create(
		MoveBy::create(0.2, Vec2(0, -dt - 10)),
		MoveBy::create(0.2, Vec2(0, 20)),
		MoveBy::create(0.1, Vec2(0, -18)),
		MoveBy::create(0.1, Vec2(0, 13)),
		MoveBy::create(0.1, Vec2(0, -5)),


		ScaleTo::create(0.1, 1.02, 0.98, 1),
		ScaleTo::create(0.1, 0.98, 1, 1),
		ScaleTo::create(0.1, 1.02, 0.98, 1),
		ScaleTo::create(0.1, 1, 1, 1),
		NULL);

	node->runAction(action);
}

void MixSuggestLayer::randMixGf(int type)
{
	int count = GameDataSave::getInstance()->getMixGfCountByType(type);

	std::map<int, std::vector<MixGfData>> map_bylvMixGf;
	std::map<std::string, MixGfData>::iterator it;

	for (it = GlobalData::map_MixGfData.begin(); it != GlobalData::map_MixGfData.end(); ++it)
	{
		int lv = GlobalData::map_MixGfData[it->first].lv;
		map_bylvMixGf[lv].push_back(GlobalData::map_MixGfData[it->first]);
	}
	int r[][4] = { { 60, 100, 100, 100 }, { 40, 70, 98, 100 }, { 0, 50, 95, 100 } };

	int lvrand = GlobalData::createRandomNum(100);

	int index = 0;
	for (int i = 0; i < 4; i++)
	{
		if (lvrand < r[type][i])
		{
			index = i;
			break;
		}
	}
	if (count > 0 && count % 10 == 0)
	{
		if (type == 0)
			index = 1;
		else if (type == 1)
			index = 2;
		else if (type == 2)
			index = 3;
	}

	int size = map_bylvMixGf[index + 1].size();
	int gfrand = GlobalData::createRandomNum(size);

	MixGfData mixdata = map_bylvMixGf[index + 1][gfrand];

	GameDataSave::getInstance()->setMixGfCountByType(type, count + 1);
	GameDataSave::getInstance()->setSuggestMixGf(mixdata.id);

	loadMixGfUi(mixdata);
}

void MixSuggestLayer::shake(Node * node, float scaleLarge, float scaleSmall) {
	if (NULL == node) {
		return;
	}

	CCActionInterval * actionScaleLarge = CCScaleTo::create(0.1, scaleLarge, scaleLarge, 1);
	CCActionInterval * actionScaleSmall = CCScaleTo::create(0.1, scaleSmall, scaleSmall, 1);
	CCActionInterval * actionScaleNormal = CCScaleTo::create(0.1, 1, 1, 1);
	node->runAction(CCSequence::create(actionScaleLarge, actionScaleSmall, actionScaleNormal, NULL));
}

void MixSuggestLayer::loadMixGfUi(MixGfData mixdata)
{
	std::string suggestgfstr = GameDataSave::getInstance()->getSuggestMixGf();
	mixname->setString(GlobalData::map_MixGfData[suggestgfstr].name);
	std::string str = StringUtils::format("ui/%s.png", mixdata.mastergf.c_str());
	img[0]->loadTexture(str, cocos2d::ui::Widget::TextureResType::PLIST);
	name[0]->setString(GlobalData::map_allResource[mixdata.mastergf].cname);

	std::string sstr = StringUtils::format("ui/qubox%d.png", GlobalData::map_wgngs[mixdata.mastergf].qu);
	imgbox[0]->loadTexture(sstr, cocos2d::ui::TextureResType::PLIST);

	int secsize = mixdata.vec_secgf.size();
	for (int i = 0; i < 3; i++)
	{
		if (i < secsize)
		{
			str = StringUtils::format("ui/%s.png", mixdata.vec_secgf[i].c_str());
			sstr = StringUtils::format("ui/qubox%d.png", GlobalData::map_wgngs[mixdata.vec_secgf[i]].qu);
			name[i + 1]->setString(GlobalData::map_allResource[mixdata.vec_secgf[i]].cname);
		}
		else
		{
			str = "ui/mixsectext.png";
			sstr = "ui/buildsmall.png";
			name[i + 1]->setString("");
		}
		img[i + 1]->loadTexture(str, cocos2d::ui::Widget::TextureResType::PLIST);
		imgbox[i+1]->loadTexture(sstr, cocos2d::ui::TextureResType::PLIST);
	}
	updateDesc();
}

void MixSuggestLayer::updateDesc()
{
	std::string mixid = GameDataSave::getInstance()->getSuggestMixGf();
	if (mixid.length() > 0)
	{
		MixGfData mixdata = GlobalData::map_MixGfData[mixid];
		desc0->setVisible(false);
		desc1->setVisible(false);
		std::string atkstr;
		std::string dfstr;
		std::string critstr;
		std::string dodgestr;
		std::string maxhpstr;
		float atk = mixdata.atkpercent;
		float df = mixdata.dfpercent;
		float crit = mixdata.critpercent;
		float dodge = mixdata.dodgepercent;
		float maxhp = mixdata.hppercent;

		std::string desstr;
		if (atk > 0.0f)
		{
			atkstr = StringUtils::format("+%.2f%%", atk);
			desstr.append(CommonFuncs::gbk2utf("大幅增加攻击"));
		}
		else if (atk < 0.0f)
		{
			atkstr = StringUtils::format("%.2f%%", atk);
			desstr.append(CommonFuncs::gbk2utf("大幅降低攻击"));
		}

		if (df > 0.0f)
		{
			dfstr = StringUtils::format("+%.2f%%", df);

			if (desstr.length() > 0)
				desstr.append(CommonFuncs::gbk2utf("，"));
			desstr.append(CommonFuncs::gbk2utf("大幅增加防御"));
		}
		else if (df < 0.0f)
		{
			dfstr = StringUtils::format("%.2f%%", df);

			if (desstr.length() > 0)
				desstr.append(CommonFuncs::gbk2utf("，"));
			desstr.append(CommonFuncs::gbk2utf("大幅降低防御"));
		}

		if (crit > 0.0f)
		{
			critstr = StringUtils::format("+%.2f%%", crit);

			if (desstr.length() > 0)
				desstr.append(CommonFuncs::gbk2utf("，"));
			desstr.append(CommonFuncs::gbk2utf("大幅增加暴击率"));
		}
		else if (crit < 0.0f)
		{
			critstr = StringUtils::format("%.2f%%", crit);

			if (desstr.length() > 0)
				desstr.append(CommonFuncs::gbk2utf("，"));
			desstr.append(CommonFuncs::gbk2utf("大幅降低暴击率"));
		}

		if (dodge > 0.0f)
		{
			dodgestr = StringUtils::format("+%.2f%%", dodge);

			if (desstr.length() > 0)
				desstr.append(CommonFuncs::gbk2utf("，"));
			desstr.append(CommonFuncs::gbk2utf("大幅增加闪避率"));
		}
		else if (dodge < 0.0f)
		{
			dodgestr = StringUtils::format("%.2f%%", dodge);

			if (desstr.length() > 0)
				desstr.append(CommonFuncs::gbk2utf("，"));
			desstr.append(CommonFuncs::gbk2utf("大幅降低闪避率"));
		}

		if (maxhp > 0.0f)
		{
			maxhpstr = StringUtils::format("+%.2f%%", maxhp);

			if (desstr.length() > 0)
				desstr.append(CommonFuncs::gbk2utf("，"));
			desstr.append(CommonFuncs::gbk2utf("大幅增加最大气血。"));
		}
		else if (maxhp < 0.0f)
		{
			maxhpstr = StringUtils::format("%.2f%%", maxhp);

			if (desstr.length() > 0)
				desstr.append(CommonFuncs::gbk2utf("，"));
			desstr.append(CommonFuncs::gbk2utf("大幅降低最大气血。"));
		}
		desc->setString(desstr);
		desc->setVisible(true);
	}
	else
	{
		desc0->setVisible(true);
		desc1->setVisible(true);
		desc->setVisible(false);
	}
}


void MixSuggestLayer::shake(Node * node) {
	if (NULL == node) {
		return;
	}

	node->runAction(CCSequence::create(
		MoveBy::create(0.02, Vec2(0, 15)),
		MoveBy::create(0.02, Vec2(0, -27)),
		MoveBy::create(0.02, Vec2(0, 22)),
		MoveBy::create(0.02, Vec2(0, -14)),
		MoveBy::create(0.02, Vec2(0, 4)),
		NULL));
}
