#include "ActionGetLayer.h"
#include "json.h"
#include "GlobalData.h"
#include "CommonFuncs.h"
#include "MyPackage.h"
#include "Const.h"
#include "GameDataSave.h"
#include "GameScene.h"
#include "SoundManager.h"
#include "NewerGuideLayer.h"
#include "HomeHill.h"

ActionGetLayer::ActionGetLayer()
{
	repeatCount = 0;
}


ActionGetLayer::~ActionGetLayer()
{
}

bool ActionGetLayer::init(int rid, std::vector<int> res_ids, int type, int actype)
{
	Node* csbnode = CSLoader::createNode("actionGetLayer.csb");
	this->addChild(csbnode);

	mtype = type;
	mrid = rid;
	m_actype = actype;

	//操作的文字显示控件1："采集", 2："砍伐", 3："挖掘"
	cocos2d::ui::Text* actiontext = (cocos2d::ui::Text*)csbnode->getChildByName("actiontext");
	actiontext->setString(acname[actype]);

	//返回按钮
	m_backbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("backbtn");
	m_backbtn->addTouchEventListener(CC_CALLBACK_2(ActionGetLayer::onBack, this));

	//1："继续采集", 2："继续砍伐", 3："继续挖掘"按钮
	m_getbtn = (cocos2d::ui::Button*)csbnode->getChildByName("getbtn");
	m_getbtn->addTouchEventListener(CC_CALLBACK_2(ActionGetLayer::onGet, this));
	std::string str = StringUtils::format("继续%s", acname[actype].c_str());
	cocos2d::ui::Text* getbtnlabel = (cocos2d::ui::Text*)m_getbtn->getChildByName("text");
	getbtnlabel->setString(CommonFuncs::gbk2utf(str.c_str()));
	//m_getbtn->setTitleText(CommonFuncs::gbk2utf(str.c_str()));

	//全部拾取按钮
	m_getallbtn = (cocos2d::ui::Button*)csbnode->getChildByName("allgetbtn");
	m_getallbtn->addTouchEventListener(CC_CALLBACK_2(ActionGetLayer::onAllGet, this));

	setBtnStatus(false);

	rewardids = res_ids;

	//临时存放数据保持到本地文件
	loadTempData();

	//加载背包数据
	updataMyPackageUI();

	//显示操作文字说明
	addEventText();

	//点击后山列表中的操作获取一次资源
	this->scheduleOnce(schedule_selector(ActionGetLayer::delayDoAction), 0.2f);
	////layer 点击事件，屏蔽下层事件
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void ActionGetLayer::playLeftLinkEffect(int src, int dst, int row, bool bRed) {
	if (src <= dst || row <0) {
		return;
	}

	float y = (8 - 1 - row + 0.5) *50;
	float x1 = (src + 0.5) *60;
	float x2 = (dst + 1) *100;

	clearBall(&m_vLeftBalls);

	float start = x1 - 30;
	while (start > x2 + 5) {
		auto ball = Sprite::createWithSpriteFrameName("");
		addChild(ball);
		ball->setPosition(start, y);
		m_vLeftBalls.pushBack(ball);
		start -= 5;
	}
}

void ActionGetLayer::doAction(float dt)
{
	for (unsigned int i = 0; i < getResData.size(); i++)
	{
		std::string name = StringUtils::format("resitem%d", i);
		this->removeChildByName(name);
	}

	bool isget = false;
	for (unsigned int i = 0; i < rewardids.size(); i++)
	{
		unsigned int m = 0;
		for (m = 0; m < getResData.size(); m++)
		{
			//奖励栏中数据跟产出的ID比较，相同数量+1
			if (rewardids[i] == atoi(getResData[m].strid.c_str()))
			{
				getResData[m].count++;
				isget = true;
				break;
			}
		}
		//奖励栏中数据跟产出的ID比较，产出ID不在奖励栏内加到奖励栏中
		if (m == getResData.size())
		{
			PackageData data;
			std::string idstr = StringUtils::format("%d", rewardids[i]);
			data.strid = idstr;
			data.type = mtype - 1;
			data.count = 1;
			getResData.push_back(data);
			isget = true;
		}
	}
	if (isget)
		repeatCount++;

	if (repeatCount > 2)
	{
		int r = GlobalData::createRandomNum(100);
		if (r < 50)
			g_hero->setSpiritValue(g_hero->getSpiritValue() - 0.5f);
	}
	if (g_hero->getSpiritValue() < 0.0f)
		g_hero->setSpiritValue(0.0f);

	updataRewardUI();

	setBtnStatus(true);

	if (GlobalData::vec_resData[mrid].count <= 0)
	{
		m_getbtn->setEnabled(false);
	}

	if (NewerGuideLayer::checkifNewerGuide(19))
	{
		showNewerGuide(19);
	}
	else if (NewerGuideLayer::checkifNewerGuide(22))
	{
		showNewerGuide(22);
	}
	else if (NewerGuideLayer::checkifNewerGuide(24))
	{
		showNewerGuide(24);
	}
	else if (NewerGuideLayer::checkifNewerGuide(26))
	{
		showNewerGuide(26);
	}

	else if (NewerGuideLayer::checkifNewerGuide(29))
	{
		showNewerGuide(29);
	}
	else if (NewerGuideLayer::checkifNewerGuide(31))
	{
		showNewerGuide(31);
	}
	else if (NewerGuideLayer::checkifNewerGuide(33))
	{
		showNewerGuide(33);
	}
	else if (NewerGuideLayer::checkifNewerGuide(35))
	{
		showNewerGuide(35);
	}
}

void ActionGetLayer::playRightLinkEffect(int src, int dst, int row, bool bRed) {
	if (src >= dst || row <0) {
		return;
	}

	float y = (9 - 1 - row + 0.5) *5;
	float x1 = (src + 0.5) *8;
	float x2 = (dst)*10;

	clearBall(&m_vRightBalls);

	float start = x1 + 30;
	while (start < x2 - 5) {
		auto ball = Sprite::createWithSpriteFrameName("");
		addChild(ball);
		ball->setPosition(start, y);
		m_vRightBalls.pushBack(ball);
		start += 9;
	}
}


void ActionGetLayer::onRewardItem(cocos2d::Ref* pSender)
{
	SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);
	//点击奖励栏的资源
	Node* node = (Node*)pSender;
	PackageData* data = (PackageData*)node->getUserData();
	int size = getResData.size();

	int count = data->count - 1;
	if (count <= 0)//数量为0，全部加到背包中了，移除掉
	{
		std::vector<PackageData>::iterator it;
		for (it = getResData.begin(); it != getResData.end(); ++it)
		{
			if (it->strid.compare(data->strid) == 0)
			{
				PackageData pdata = *data;
				pdata.count = 1;

				if (MyPackage::add(pdata) == 0)
				{
					data->count--;
					getResData.erase(it);
				}
				break;
			}
		}
	}
	else//不为0，数量-1
	{
		PackageData pdata = *data;
		pdata.count = 1;

		if (MyPackage::add(pdata) == 0)
		{
			data->count--;
		}
	}

	for (int i = 0; i < size; i++)
	{
		std::string name = StringUtils::format("resitem%d", i);
		this->removeChildByName(name);
	}
	//保存临时数据
	saveTempData();

	//更新UI
	updata();

	if (NewerGuideLayer::checkifNewerGuide(21))
		showNewerGuide(21);
	else if (NewerGuideLayer::checkifNewerGuide(30))
		showNewerGuide(30);
}

void ActionGetLayer::playTopLinkEffect(int src, int dst, int col, bool bRed) {
	if (src <= dst || col <0) {
		return;
	}

	float x = (col + 0.5) *100;
	float y1 = (9 - 1 - src + 0.5) *20;
	float y2 = (6 - 1 - dst) *30;

	clearBall(&m_vTopBalls);

	float start = y1 + 30;
	while (start < y2 - 5) {
		auto ball = Sprite::createWithSpriteFrameName("");
		addChild(ball);
		ball->setPosition(x, start);
		m_vTopBalls.pushBack(ball);
		start += 7;
	}
}

void ActionGetLayer::onPackageItem(cocos2d::Ref* pSender)
{
	SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);
	//点击背包栏
	Node* node = (Node*)pSender;
	int index = node->getTag();
	PackageData data = MyPackage::vec_packages[index];
	unsigned int i = 0;
	for (i = 0; i < getResData.size(); i++)
	{
		//奖励栏有背包中的资源，背包-1，奖励栏数据-1
		if (data.strid.compare(getResData[i].strid) == 0 && (getResData[i].type == FOOD || getResData[i].type == MEDICINAL || getResData[i].type == RES_1 || getResData[i].type == RES_2))
		{
			getResData[i].count++;
			break;
		}
	}

	//奖励栏没有背包中的资源，添加
	if (i == getResData.size())
	{
		data.count = 1;
		getResData.push_back(data);
	}

	//保存临时数据
	saveTempData();

	//背包 - 1,<0,会从背包中移除掉
	MyPackage::cutone(data.strid);

	//先移除奖励栏的物品在更新加载
	for (unsigned int i = 0; i < getResData.size(); i++)
	{
		std::string name = StringUtils::format("resitem%d", i);
		this->removeChildByName(name);
	}
	//更新UI
	updata();
}


void ActionGetLayer::playBottomLinkEffect(int src, int dst, int col, bool bRed) {
	if (src >= dst || col <0) {
		return;
	}

	float x = (col + 0.5) *40;
	float y1 = (90 - 1 - src + 0.5) *50;
	float y2 = (50 - 1 - dst + 1) *60;

	clearBall(&m_vBottomBalls);

	float start = y1 - 30;
	while (start > y2 + 5) {
		auto ball = Sprite::createWithSpriteFrameName("");
		addChild(ball);
		ball->setPosition(x, start);
		m_vBottomBalls.pushBack(ball);
		start -= 70;
	}
}

ActionGetLayer* ActionGetLayer::create(int rid, std::vector<int> res_ids, int type, int actype)
{
	ActionGetLayer *pRet = new ActionGetLayer();
	if (pRet && pRet->init(rid, res_ids, type, actype))
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

void ActionGetLayer::clearBall(cocos2d::Vector<cocos2d::Sprite *> * vBall) {
	if (nullptr == vBall) {
		return;
	}

	for (auto ball : *vBall) {
		ball->stopAllActions();
		removeChild(ball, true);
	}

	vBall->clear();
}

void ActionGetLayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		HomeHill* homehill = (HomeHill*)g_gameLayer->getChildByName("homehill");
		if (homehill != NULL)
		{
			if (NewerGuideLayer::checkifNewerGuide(28))
				homehill->showNewerGuide(28);
			else if (NewerGuideLayer::checkifNewerGuide(37))
				homehill->showNewerGuide(37);
		}
		this->removeFromParentAndCleanup(true);
	}
}

void ActionGetLayer::playShineEffect(cocos2d::Vector<cocos2d::Sprite *> vBall) {
	float delayIntrval = 0.8;
	int i = 0;
	for (auto ball : vBall) {
		ball->runAction(RepeatForever::create(
			Blink::create(0.5, 1))
			);
	}
}

void ActionGetLayer::onGet(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);

	//1："继续采集", 2："继续砍伐", 3："继续挖掘"按钮
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		setBtnStatus(false);

		//更新后山资源列表中的数据，为0，就不出产出资源了，需等待资源恢复
		if (GlobalData::vec_resData[mrid].count > 0)
		{
			GlobalData::vec_resData[mrid].count--;
			std::string desc;
			delayDoAction(0);
			addEventText();
		}

		if (GlobalData::vec_resData[mrid].count <= 0)
		{
			m_getbtn->setEnabled(false);
		}
	}
}

void ActionGetLayer::playTouchEffect(int row, int col) {
	if (row < 0 || row >= 6) {
		return;
	}

	if (col < 0 || col >= 6) {
		return;
	}

	removeChildByTag(1);
	getParent()->removeChildByTag(2);

	const Point touchPos = Point((col + 0.5)*30, (7 - 1 - row + 0.5)*20);
	Animation *aniTouch = Animation::create();
	Sprite *touchSprite = nullptr;

	{
		Texture2D * txt2d = TextureCache::getInstance()->addImage("");
		if (nullptr == txt2d) {
			return;
		}

		float w = txt2d->getContentSize().width / 10;
		float h = txt2d->getContentSize().height;


		aniTouch->setDelayPerUnit(0.2);
		for (int i = 0; i<10; i++) {
			aniTouch->addSpriteFrameWithTexture(txt2d, Rect(i*w, 0, w, h));
		}

		touchSprite = Sprite::create("", Rect(0, 0, w, h));
		touchSprite->setTag(1);
		getParent()->addChild(touchSprite, 12);
		touchSprite->setAnchorPoint(Vec2(0, 0.5));

		touchSprite->setPosition(touchPos + getPosition());
	}

	Animation *aniRing = Animation::create();
	Sprite *ringSprite = nullptr;
	{
		Texture2D * txt2d = TextureCache::getInstance()->addImage("");
		if (nullptr == txt2d) {
			return;
		}

		float w = txt2d->getContentSize().width / 2;
		float h = txt2d->getContentSize().height;


		aniRing->setDelayPerUnit(0.1);
		for (int i = 0; i<5; i++) {
			aniRing->addSpriteFrameWithTexture(txt2d, Rect(i*w, 0, w, h));
		}

		ringSprite = Sprite::create("", Rect(0, 0, w, h));
		ringSprite->setTag(4);
		addChild(ringSprite, 10);
		ringSprite->setPosition(touchPos);
	}


	if (nullptr != touchSprite) {
		touchSprite->runAction(RepeatForever::create(
			Sequence::create(
			Animate::create(aniTouch),
			DelayTime::create(0.5)
			, NULL)
			));
	}

	if (nullptr != ringSprite) {
		ringSprite->runAction(RepeatForever::create(
			Sequence::create(
			DelayTime::create(0.4),
			Animate::create(aniRing),
			DelayTime::create(0.2)
			, NULL)
			));
	}
}

void ActionGetLayer::delayDoAction(float dt)
{
	if (m_actype == 1)
		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_CAIJI);
	else if (m_actype == 2)
	{
		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_KANSHU);
	}
	else if (m_actype == 3)
	{
		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_WAJUE);
	}

	int resrid = atoi(GlobalData::vec_resData[mrid].strid.c_str());
	if (g_hero->getAtrByType((HeroAtrType)m_actype)->count > 0 || resrid == 69)//打水)//是否有工具m_actype：1："采集", 2："砍伐", 3："挖掘"
	{
		if (resrid != 69)
		{
			int r = GlobalData::createRandomNum(100);
			if (r < 80)
			{
				g_hero->getAtrByType((HeroAtrType)m_actype)->goodvalue--;
				std::string strid = g_hero->getAtrByType((HeroAtrType)m_actype)->strid;
				if (g_hero->getAtrByType((HeroAtrType)m_actype)->goodvalue <= 0)
				{

					std::string brokenstr = StringUtils::format("%s%s%s", CommonFuncs::gbk2utf("你的").c_str(), GlobalData::map_allResource[strid].cname.c_str(), CommonFuncs::gbk2utf("已损坏！！").c_str());
					g_uiScroll->addEventText(brokenstr, 25, Color3B(204, 4, 4));
					PackageData data;
					data.count = 0;
					g_hero->setAtrByType((HeroAtrType)m_actype, data);
				}
				else if (g_hero->getAtrByType((HeroAtrType)m_actype)->goodvalue == 20 || g_hero->getAtrByType((HeroAtrType)m_actype)->goodvalue == 10)
				{
					std::string descstr = StringUtils::format("%s%s%d", GlobalData::map_allResource[strid].cname.c_str(), CommonFuncs::gbk2utf("耐久度仅剩").c_str(), g_hero->getAtrByType((HeroAtrType)m_actype)->goodvalue);

					g_uiScroll->addEventText(descstr, 25, Color3B(204, 4, 4));
				}
			}
		}
		doAction(0);
	}
	else
	{
		this->scheduleOnce(schedule_selector(ActionGetLayer::doAction), 1.0f);
	}
}

void ActionGetLayer::onAllGet(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	//全部拾取
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		for (unsigned int i = 0; i < getResData.size(); i++)
		{
			std::string name = StringUtils::format("resitem%d", i);
			this->removeChildByName(name);
		}

		std::vector<PackageData>::iterator it;

		bool isfull = false;
		for (it = getResData.begin(); it != getResData.end();)
		{
			bool isOver = false;
			int count = it->count;
			for (int m = 0; m < count; m++)
			{
				PackageData data = *it;
				data.count = 1;
				if (MyPackage::add(data) == 0)
				{
					if (--it->count <= 0)
					{
						it = getResData.erase(it);
						isOver = true;
						break;
					}
				}
				else
				{
					isfull = true;
					break;
				}
			}
			if (!isOver)
				it++;
			if (isfull)
				break;
		}
		saveTempData();
		updata();
		if (NewerGuideLayer::checkifNewerGuide(23))
			showNewerGuide(23);
		else if (NewerGuideLayer::checkifNewerGuide(25))
			showNewerGuide(25);
		else if (NewerGuideLayer::checkifNewerGuide(27))
			showNewerGuide(27);

		else if (NewerGuideLayer::checkifNewerGuide(32))
			showNewerGuide(32);
		else if (NewerGuideLayer::checkifNewerGuide(34))
			showNewerGuide(34);
		else if (NewerGuideLayer::checkifNewerGuide(36))
			showNewerGuide(36);
	}
}

void ActionGetLayer::stopTouchEffect() {
	auto ring = getChildByTag(2);
	if (nullptr != ring) {
		ring->stopAllActions();
		removeChildByTag(1);
	}

	auto touch = getParent()->getChildByTag(2);
	if (nullptr != touch) {
		touch->stopAllActions();
		getParent()->removeChildByTag(2);
	}
}

void ActionGetLayer::loadTempData()
{
	tempResData.clear();
	//临时存放的数据，保存在本地文件中，解析
	std::string datastr = GameDataSave::getInstance()->getTempStorage("m1-2");
	std::vector<std::string> vec_retstr;
	CommonFuncs::split(datastr, vec_retstr, ";");
	for (unsigned int i = 0; i < vec_retstr.size(); i++)
	{
		std::vector<std::string> tmp;
		CommonFuncs::split(vec_retstr[i], tmp, "-");
		PackageData data;
		data.strid = tmp[0];
		data.type = atoi(tmp[1].c_str());
		data.count = atoi(tmp[2].c_str());
		data.extype = GlobalData::getResExType(data.strid);//atoi(tmp[3].c_str());
		data.lv = atoi(tmp[4].c_str());
		data.exp = atoi(tmp[5].c_str());
		data.goodvalue = atoi(tmp[6].c_str());
		if (tmp.size() >= 9)
		{
			data.slv = atoi(tmp[7].c_str());
			data.tqu = atoi(tmp[8].c_str());
		}
		tempResData.push_back(data);
	}
}

void ActionGetLayer::markSprite(int row, int col, bool bYes) {
	if (row < 0 || row >= 7) {
		return;
	}

	if (col < 0 || col >= 7) {
		return;
	}



	const Point markPos = Point((col + 0.5)*30, (8 - 1 - row + 0.5)*20);

	auto mark = Sprite::createWithSpriteFrameName("");
	addChild(mark);
	mark->setAnchorPoint(Vec2(0.1, 0.6));
	mark->setPosition(markPos);
	m_vMark.pushBack(mark);
}


void ActionGetLayer::saveTempData()
{
	std::vector<PackageData> allResData = tempResData;

	for (unsigned int i = 0; i < getResData.size(); i++)
	{
		int tmpsize = tempResData.size();
		int j = 0;
		for (j = 0; j < tmpsize; j++)
		{
			if (getResData[i].strid.compare(tempResData[j].strid) == 0 && (getResData[i].type == FOOD || getResData[i].type == MEDICINAL || getResData[i].type == RES_1 || getResData[i].type == RES_2))
			{
				allResData[j].count += getResData[i].count;
				break;
			}
		}
		if (j == tmpsize)
		{
			allResData.push_back(getResData[i]);
		}
	}
	GlobalData::map_tempGf_Equip["m1-2"].clear();
	std::string str;
	for (unsigned int i = 0; i < allResData.size(); i++)
	{
		std::string onestr = StringUtils::format("%s-%d-%d-%d-%d-%d-%d-%d-%d;", allResData[i].strid.c_str(), allResData[i].type, allResData[i].count, allResData[i].extype, allResData[i].lv, allResData[i].exp, allResData[i].goodvalue, allResData[i].slv, allResData[i].tqu);
		str.append(onestr);
		int tmptype = allResData[i].type;
		std::string tmpstrid = allResData[i].strid;
		if (tmptype == W_GONG || tmptype == N_GONG || tmptype == WEAPON || tmptype == PROTECT_EQU)
		{
			GlobalData::map_tempGf_Equip["m1-2"].push_back(tmpstrid);
		}
	}
	GameDataSave::getInstance()->setTempStorage("m1-2", str.substr(0, str.length() - 1));
}

void ActionGetLayer::updata()
{
	//奖励栏资源UI更新
	updataRewardUI();

	//背包栏资源UI更新
	updataMyPackageUI();
}

void ActionGetLayer::updataMyPackageUI()
{
	for (int i = 0; i < MyPackage::getMax(); i++)
	{
		std::string name = StringUtils::format("pitem%d", i);
		this->removeChildByName(name);
	}

	for (int i = 0; i < MyPackage::getSize(); i++)
	{
		std::string boxstr = "ui/buildsmall.png";
		PackageData tmpdata = MyPackage::vec_packages[i];
		if (tmpdata.type == WEAPON || tmpdata.type == PROTECT_EQU)
		{
			boxstr = StringUtils::format("ui/qubox%d.png", GlobalData::map_equips[tmpdata.strid].qu);
		}
		else if (tmpdata.type == N_GONG || tmpdata.type == W_GONG)
		{
			boxstr = StringUtils::format("ui/qubox%d.png", GlobalData::map_wgngs[tmpdata.strid].qu);
		}
		Sprite * box = Sprite::createWithSpriteFrameName(boxstr);

		MenuItemSprite* boxItem = MenuItemSprite::create(
			box,
			box,
			box,
			CC_CALLBACK_1(ActionGetLayer::onPackageItem, this));
		boxItem->setTag(i);
		boxItem->setPosition(Vec2(100 + i * 130, 355));
		Menu* menu = Menu::create();
		menu->addChild(boxItem);
		menu->setPosition(Vec2(0, 0));
		menu->setScale(0.9);
		std::string name = StringUtils::format("pitem%d", i);
		this->addChild(menu, 0, name);

		std::string str = StringUtils::format("ui/%s.png", MyPackage::vec_packages[i].strid.c_str());
		Sprite * res = Sprite::createWithSpriteFrameName(str);
		res->setPosition(Vec2(box->getContentSize().width / 2, box->getContentSize().height / 2));
		box->addChild(res);
		str = StringUtils::format("%d", MyPackage::vec_packages[i].count);
		Label * reslbl = Label::createWithTTF(str, "fonts/STXINGKA.TTF", 18);
		reslbl->enableOutline(Color4B(0, 0, 0, 255), 3);
		reslbl->setPosition(Vec2(box->getContentSize().width - 25, 30));
		box->addChild(reslbl);
	}
}

void ActionGetLayer::clearMarkSprite() {
	for (int i = 0; i<m_vMark.size(); i++) {
		auto mark = m_vMark.at(i);
		removeChild(mark);
	}

	m_vMark.clear();
}

void ActionGetLayer::clearAllBall() {
	clearBall(&m_vLeftBalls);
	clearBall(&m_vRightBalls);
	clearBall(&m_vTopBalls);
	clearBall(&m_vBottomBalls);
}

void ActionGetLayer::updataRewardUI()
{
	for (unsigned int i = 0; i < getResData.size(); i++)
	{
		std::string boxstr = "ui/buildsmall.png";
		PackageData tmpdata = getResData[i];
		if (tmpdata.type == WEAPON || tmpdata.type == PROTECT_EQU)
		{
			boxstr = StringUtils::format("ui/qubox%d.png", GlobalData::map_equips[tmpdata.strid].qu);
		}
		else if (tmpdata.type == N_GONG || tmpdata.type == W_GONG)
		{
			boxstr = StringUtils::format("ui/qubox%d.png", GlobalData::map_wgngs[tmpdata.strid].qu);
		}

		Sprite * box = Sprite::createWithSpriteFrameName(boxstr);

		MenuItemSprite* boxItem = MenuItemSprite::create(
			box,
			box,
			box,
			CC_CALLBACK_1(ActionGetLayer::onRewardItem, this));
		boxItem->setTag(i);
		boxItem->setUserData(&getResData[i]);
		boxItem->setPosition(Vec2(150 + i * 135, 592));
		Menu* menu = Menu::create();
		menu->addChild(boxItem);
		menu->setPosition(Vec2(0, 0));
		std::string name = StringUtils::format("resitem%d", i);
		this->addChild(menu, 0, name);

		std::string str = StringUtils::format("ui/%s.png", getResData[i].strid.c_str());
		Sprite * res = Sprite::createWithSpriteFrameName(str);
		res->setPosition(Vec2(box->getContentSize().width / 2, box->getContentSize().height / 2));
		box->addChild(res);

		str = StringUtils::format("%d", getResData[i].count);
		Label * reslbl = Label::createWithTTF(str, "fonts/STXINGKA.TTF", 18);
		reslbl->enableOutline(Color4B(0, 0, 0, 255),3);
		reslbl->setPosition(Vec2(box->getContentSize().width - 25, 30));
		box->addChild(reslbl);
	}
}

//退出也要保存下临时存放的数据
void ActionGetLayer::onExit()
{
	saveTempData();
	Layer::onExit();
}

void ActionGetLayer::setBtnStatus(bool enable)
{
	m_getbtn->setEnabled(enable);
	m_getallbtn->setEnabled(enable);
	m_backbtn->setEnabled(enable);
}

void ActionGetLayer::addEventText()
{
	int resrid = atoi(GlobalData::vec_resData[mrid].strid.c_str());
	std::string desc;
	if (g_hero->getAtrByType((HeroAtrType)m_actype)->count > 0)
	{
		desc = CommonFuncs::gbk2utf(acdesc1[m_actype].c_str());
		desc.append(GlobalData::vec_resData[mrid].unitname);
		g_uiScroll->addEventText(desc);
	}
	else
	{
		desc = CommonFuncs::gbk2utf(acdesc[m_actype].c_str());
		if (resrid != 69)
			this->scheduleOnce(schedule_selector(ActionGetLayer::showDotDot), 0.2f);
		g_uiScroll->addEventText(desc);
	}
	if (resrid == 69)//打水
	{
		desc = CommonFuncs::gbk2utf("你三下五除二从水井里打出");
		desc.append(GlobalData::vec_resData[mrid].unitname);
		g_uiScroll->addEventText(desc);
	}
}

void ActionGetLayer::showDotDot(float dt)
{
	g_uiScroll->addEventText("..........");
	this->scheduleOnce(schedule_selector(ActionGetLayer::addEventText2), 1.0f);
}

void ActionGetLayer::addEventText2(float dt)
{
	std::string desc;
	desc = CommonFuncs::gbk2utf(acdesc2[m_actype].c_str());
	desc.append(GlobalData::vec_resData[mrid].unitname);
	g_uiScroll->addEventText(desc);
}

void ActionGetLayer::showNewerGuide(int step)
{
	std::vector<Node*> nodes;
	if (step == 19)
	{

	}
	if (step == 20 || step == 29)
	{
		Node* resitem0 = this->getChildByName("resitem0")->getChildren().at(0);
		NewerGuideLayer::pushUserData("buildsmall");
		nodes.push_back(this->getChildByName("resitem0")->getChildren().at(0));
	}
	else if (step == 21 || step == 23 || step == 25 || step == 30 || step == 32 || step == 34)
	{
		nodes.push_back(m_getbtn);
		NewerGuideLayer::pushUserData("normalbtn");
	}
	else if (step == 22 || step == 24 || step == 26 || step == 31 || step == 33 || step == 35)
	{
		nodes.push_back(m_getallbtn);
		NewerGuideLayer::pushUserData("normalbtn");
	}
	else if (step == 27 || step == 36)
	{
		nodes.push_back(m_backbtn);
		NewerGuideLayer::pushUserData("normalbtn");
	}

	if ((step >= 19 && step <= 27) || (step >= 29 && step <= 36))
		g_gameLayer->showNewerGuide(step, nodes);
}