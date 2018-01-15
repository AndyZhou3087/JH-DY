#include "RepairLayer.h"
#include "GlobalData.h"
#include "CommonFuncs.h"
#include "Const.h"
#include "StorageRoom.h"
#include "SoundManager.h"
#include "GameScene.h"
#include "HintBox.h"
#include "MyMenu.h"
#include "ResDetailsLayer.h"
#include "RepairOKLayer.h"
#include "CannotTouchLayer.h"

RepairLayer::RepairLayer()
{
	resIsEnough = true;
	useluckstone = 0;
	succrnd = 100;
	clickindex = -1;
}


RepairLayer::~RepairLayer()
{

}


RepairLayer* RepairLayer::create(int type)
{
	RepairLayer *pRet = new RepairLayer();
	if (pRet && pRet->init(type))
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

bool RepairLayer::init(int type)
{
	m_type = type;
	Node* csbnode = CSLoader::createNode("repairLayer.csb");
	this->addChild(csbnode);

	cocos2d::ui::Widget *backbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(RepairLayer::onBack, this));

	failSprite = (cocos2d::ui::Widget*)csbnode->getChildByName("strengthfail");

	failSprite->setVisible(false);

	m_actionBtn = (cocos2d::ui::Button*)csbnode->getChildByName("actionbtn");
	m_actionBtn->addTouchEventListener(CC_CALLBACK_2(RepairLayer::onAction, this));
	m_actionBtn->setTag(type);
	m_actionBtn->setEnabled(false);

	cocos2d::ui::Widget* desclbl = (cocos2d::ui::Button*)csbnode->getChildByName("desc");
	if (m_type == 2)
	{
		m_actionBtn->setTitleText(CommonFuncs::gbk2utf("强化"));
		desclbl->setVisible(true);
	}

	succrandtext = (cocos2d::ui::Text*)csbnode->getChildByName("succrndtext");
	succrandtext->setVisible(false);

	repairbox = (cocos2d::ui::Widget*)csbnode->getChildByName("repairbox");

	repairResBoxImg = (cocos2d::ui::ImageView*)csbnode->getChildByName("repair");

	repairResImg = (cocos2d::ui::ImageView*)repairResBoxImg->getChildByName("img");
	repairResImg->addTouchEventListener(CC_CALLBACK_2(RepairLayer::onResItem, this));
	repairResImg->setEnabled(false);

	for (int i = 0; i < 3; i++)
	{
		std::string str = StringUtils::format("box%d", i + 1);
		needResbox[i] = (cocos2d::ui::Widget*)csbnode->getChildByName(str);
		needResbox[i]->setVisible(false);
		needResImg[i] = (cocos2d::ui::ImageView*)csbnode->getChildByName(str)->getChildByName("img");
		needResImg[i]->addTouchEventListener(CC_CALLBACK_2(RepairLayer::onResItem, this));

		needResCountLbl[i] = (cocos2d::ui::Text*)csbnode->getChildByName(str)->getChildByName("count");

	}

	updataUI();

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void RepairLayer::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
}



void RepairLayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}

void RepairLayer::onAction(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Node* node = (Node*)pSender;

		PackageData* data = (PackageData*)repairResImg->getUserData();
		if (node->getTag() == 1)
		{
			if (data->goodvalue >= 100)
			{
				HintBox* layer = HintBox::create(CommonFuncs::gbk2utf("无需修复！！"));
				this->addChild(layer);
				return;
			}
			else if (!resIsEnough)
			{
				HintBox* layer = HintBox::create(CommonFuncs::gbk2utf("资源不足！！"));
				this->addChild(layer);
				return;
			}
			else
			{
				showAnim();
			}
		}
		else
		{
			if (data->slv >= 20)
			{
				HintBox* layer = HintBox::create(CommonFuncs::gbk2utf("强化已最高，无需强化！！"));
				this->addChild(layer);
				return;
			}
			else if (!resIsEnough)
			{
				HintBox* layer = HintBox::create(CommonFuncs::gbk2utf("资源不足！！"));
				this->addChild(layer);
				return;
			}
			else
			{
				showAnim();
			}
		}
	}
}

void RepairLayer::onRepairItem(cocos2d::Ref* pSender)
{

	Node* node = (Node*)pSender;
	PackageData* pdata = (PackageData*)node->getUserData();

	if (pdata->strid.compare("82") == 0)
	{
		if (clickindex < 0)
		{
			HintBox* hint = HintBox::create(CommonFuncs::gbk2utf("请先选择装备！！"));
			this->addChild(hint);
			return;
		}
		if (useluckstone >= pdata->count)
			return;

		useluckstone++;

		succrandtext->setVisible(true);;
		succrnd += 5;
		if (succrnd > 100)
			succrnd = 100;
		std::string strrnd = StringUtils::format("成功率%d%%", succrnd);
		succrandtext->setString(CommonFuncs::gbk2utf(strrnd.c_str()));

		std::string usestr = StringUtils::format("%d", useluckstone);
		needResCountLbl[2]->setString(usestr);

		for (unsigned int i = 0; i < vec_RepairData.size(); i++)
		{
			std::string name = StringUtils::format("pitem%d", i);
			this->removeChildByName(name);
		}
		updataUI();
		return;
	}
	else
	{
		failSprite->setVisible(false);
		clickindex = node->getTag();
		if (useluckstone > 0)
		{
			useluckstone = 0;
			for (unsigned int i = 0; i < vec_RepairData.size(); i++)
			{
				std::string name = StringUtils::format("pitem%d", i);
				this->removeChildByName(name);
			}
			updataUI();
		}
	}

	for (int i = 0; i < 3; i++)
	{
		needResbox[i]->setVisible(false);
		needResbox[i]->setOpacity(255);
	}
	resIsEnough = true;

	std::string qustr = StringUtils::format("ui/qubox%d.png", GlobalData::map_equips[pdata->strid].qu);
	repairResBoxImg->loadTexture(qustr, cocos2d::ui::TextureResType::PLIST);
	repairResBoxImg->setContentSize(Sprite::createWithSpriteFrameName(qustr)->getContentSize());

	std::string repairResStr = StringUtils::format("ui/%s.png", pdata->strid.c_str());
	repairResImg->loadTexture(repairResStr, cocos2d::ui::TextureResType::PLIST);
	repairResImg->setContentSize(Sprite::createWithSpriteFrameName(repairResStr)->getContentSize());
	repairResImg->setTag(0);
	repairResImg->setUserData(pdata);
	repairResImg->setEnabled(true);

	if (m_type == 1)
	{
		repairneedres = GlobalData::map_equips[pdata->strid].repair_res;
	}
	else
	{
		repairneedres.clear();
		succrnd = 100;
		int nextslv = pdata->slv + 1;
		if (nextslv > 20)
			nextslv = 20;

		if (nextslv > 5)
		{
			succrnd = 100 - (nextslv - 5) * 6;
		}

		std::string strrnd = StringUtils::format("成功率%d%%", succrnd);
		succrandtext->setString(CommonFuncs::gbk2utf(strrnd.c_str()));

		std::string res1 = StringUtils::format("%d", 12 * 1000 + nextslv);
		repairneedres.push_back(res1);

		std::string res2 = StringUtils::format("%d", 81 * 1000 + nextslv);
		repairneedres.push_back(res2);

		std::string res3 = StringUtils::format("%d", 82 * 1000 + 0);
		repairneedres.push_back(res3);

		succrandtext->setVisible(true);
	}


	int ressize = repairneedres.size();
	int resImgStartIndex = 0;

	if (ressize == 2)
	{
		resImgStartIndex = 1;
	}
	for (int i = 0; i < ressize; i++)
	{
		int needrescount = atoi(repairneedres[i].c_str()) % 1000;
		int needresid = atoi(repairneedres[i].c_str()) / 1000;

		needResbox[resImgStartIndex + i]->setVisible(true);
		std::string str = StringUtils::format("ui/%d.png", needresid);
		needResImg[resImgStartIndex + i]->loadTexture(str, cocos2d::ui::TextureResType::PLIST);
		needResImg[resImgStartIndex + i]->setContentSize(Sprite::createWithSpriteFrameName(str)->getContentSize());
		std::string resid = StringUtils::format("%d", needresid);
		int mycount = getMyPackageResCount(resid);
		needResImg[resImgStartIndex + i]->setTag(needresid);
		if (m_type == 2 && i == ressize - 1)
			str = StringUtils::format("%d", useluckstone);
		else
			str = StringUtils::format("%d/%d", mycount, needrescount);
		if (mycount < needrescount)
		{
			resIsEnough = false;
			needResCountLbl[resImgStartIndex + i]->setTextColor(Color4B(204, 4, 5, 255));
		}
		else
		{
			needResCountLbl[resImgStartIndex + i]->setTextColor(Color4B(255, 255, 255, 255));
		}
		needResCountLbl[resImgStartIndex + i]->setString(str);
	}
	m_actionBtn->setEnabled(true);
}

void RepairLayer::onResItem(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Node* node = (Node*)pSender;
		ResDetailsLayer::whereClick = 1;
		if (node->getTag() == 0)
		{
			PackageData* pdata = (PackageData*)node->getUserData();
			if (pdata != NULL && pdata->strid.length() > 0)
				this->addChild(ResDetailsLayer::create((PackageData*)node->getUserData()));
		}
		else
		{
			std::string resid = StringUtils::format("%d", node->getTag());
			this->addChild(ResDetailsLayer::createByResId(resid));
		}
			
	}
}

void RepairLayer::showAnim()
{
	m_actionBtn->setEnabled(false);

	CannotTouchLayer* layer = CannotTouchLayer::create();
	g_gameLayer->addChild(layer, 5, "notouchlayer");

	for (int i = 0; i < 3; i++)
	{
		ActionInterval* ac1 = Spawn::create(FadeOut::create(1.5f), EaseSineIn::create(MoveTo::create(1.0f, Vec2(repairResBoxImg->getPosition()))), NULL);
		needResbox[i]->runAction(Sequence::create(ac1, NULL));
	}
	
	repairbox->runAction(Sequence::create(EaseSineIn::create(RotateTo::create(1.5f, 720 * 2)), CallFuncN::create(CC_CALLBACK_1(RepairLayer::finishAnim, this)), NULL));
}

int RepairLayer::getMyPackageResCount(std::string strid)
{
	int count = 0;
	for (unsigned int i = 0; i < MyPackage::vec_packages.size(); i++)
	{
		if (MyPackage::vec_packages[i].strid.compare(strid) == 0)
		{
			count += MyPackage::vec_packages[i].count;
		}
	}
	return count;
}

void RepairLayer::finishAnim(Ref* pSender)
{
	float delay = 0.05f;
	Vec2 pos[] = { Vec2(360, 895), Vec2(140, 530), Vec2(570, 530) };

	for (int i = 0; i < 3; i++)
	{
		needResbox[i]->setPosition(pos[i]);
	}
	
	PackageData* data = (PackageData*)repairResImg->getUserData();
	if (data->goodvalue < 100)
		data->goodvalue = 100;
	if (m_type == 2)
	{
		int r = GlobalData::createRandomNum(100);
		if (r < succrnd)
		{
			float dt = 0.5f;
			data->slv += 1;
			this->scheduleOnce(schedule_selector(RepairLayer::delayShowOkLayer), dt);
			delay += dt;

			for (unsigned int i = 0; i < GlobalData::vec_achiveData.size(); i++)
			{
				if (GlobalData::vec_achiveData[i].type == A_9)
				{
					if (GlobalData::vec_achiveData[i].vec_para[0].compare(data->strid) == 0 && GlobalData::vec_achiveData[i].finish != -1)
					{
						GlobalData::vec_achiveData[i].finish = data->slv;
						GlobalData::saveAchiveData();
					}
				}
			}
		}
		else
		{
			dofail();
		}
	}

	for (unsigned int i = 0; i < vec_RepairData.size(); i++)
	{
		std::string name = StringUtils::format("pitem%d", i);
		this->removeChildByName(name);
	}
	repairbox->setRotation(0);
	if (useluckstone > 0)
	{
		MyPackage::cutone("82", useluckstone);
		useluckstone = 0;
	}
	for (unsigned int i = 0; i < repairneedres.size(); i++)
	{
		int needrescount = atoi(repairneedres[i].c_str()) % 1000;
		int needresid = atoi(repairneedres[i].c_str()) / 1000;
		std::string strid = StringUtils::format("%d", needresid);
		MyPackage::cutone(strid, needrescount);
	}

	updataUI();
	this->scheduleOnce(schedule_selector(RepairLayer::delayCanClick), delay);
}

void RepairLayer::updataUI()
{
	vec_RepairData.clear();
	vec_repairItem.clear();

	if (g_hero->getAtrByType(H_WEAPON)->count > 0)
		vec_RepairData.push_back(g_hero->getAtrByType(H_WEAPON));
	if (g_hero->getAtrByType(H_ARMOR)->count > 0)
		vec_RepairData.push_back(g_hero->getAtrByType(H_ARMOR));

	for (int i = 0; i < MyPackage::getSize(); i++)
	{
		if (MyPackage::vec_packages[i].type == WEAPON || MyPackage::vec_packages[i].type == PROTECT_EQU)
			vec_RepairData.push_back(&MyPackage::vec_packages[i]);
		if (m_type == 2 && MyPackage::vec_packages[i].type == RES_2 && MyPackage::vec_packages[i].strid.compare("82") == 0)
		{
			vec_RepairData.push_back(&MyPackage::vec_packages[i]);
		}
	}

	for (unsigned int i = 0; i < vec_RepairData.size(); i++)
	{
		std::string boxstr = "ui/buildsmall.png";
		PackageData* tmpdata = vec_RepairData[i];
		if (tmpdata->type == WEAPON || tmpdata->type == PROTECT_EQU)
		{
			boxstr = StringUtils::format("ui/qubox%d.png", GlobalData::map_equips[tmpdata->strid].qu);
		}
		else if (tmpdata->type == N_GONG || tmpdata->type == W_GONG)
		{
			boxstr = StringUtils::format("ui/qubox%d.png", GlobalData::map_wgngs[tmpdata->strid].qu);
		}

		Sprite * box = Sprite::createWithSpriteFrameName(boxstr);

		MenuItemSprite* boxItem = MenuItemSprite::create(
			box,
			box,
			box,
			CC_CALLBACK_1(RepairLayer::onRepairItem, this));
		boxItem->setUserData(vec_RepairData[i]);
		boxItem->setTag(i);
		boxItem->setPosition(Vec2(130 + i % 4 * 150, 350 + i / 4 * 150));
		MyMenu* menu = MyMenu::create();
		menu->addChild(boxItem);
		menu->setPosition(Vec2(0, 0));
		std::string name = StringUtils::format("pitem%d", i);
		this->addChild(menu, 0, name);
		vec_repairItem.push_back(boxItem);

		std::string str = StringUtils::format("ui/%s.png", tmpdata->strid.c_str());
		Sprite * res = Sprite::createWithSpriteFrameName(str);
		res->setPosition(Vec2(box->getContentSize().width / 2, box->getContentSize().height / 2));
		box->addChild(res);

		Label * goodlbl = Label::createWithTTF("", "fonts/STXINGKA.TTF", 15);//Label::createWithSystemFont("", "", 15);
		if (m_type == 2)
		{
			if (tmpdata->strid.compare("82") == 0)
			{
				str = StringUtils::format("%d", tmpdata->count - useluckstone);
			}
			else
			{
				str = StringUtils::format("强化+%d", tmpdata->slv);
				//goodlbl->setTextColor(Color4B(216, 151, 82, 255));
			}
		}
		else
			str = StringUtils::format("耐久度%d", tmpdata->goodvalue);
		goodlbl->setString(CommonFuncs::gbk2utf(str.c_str()));
		goodlbl->setAnchorPoint(Vec2(1, 0));
		goodlbl->setPosition(Vec2(box->getContentSize().width - 10, 8));
		box->addChild(goodlbl);

		str = StringUtils::format("%s", GlobalData::map_allResource[tmpdata->strid].cname.c_str());
		Label * namelbl = Label::createWithTTF(str, "fonts/STXINGKA.TTF", 24);
		namelbl->setColor(Color3B(0, 0, 0));
		namelbl->setPosition(Vec2(box->getContentSize().width / 2, -10));
		box->addChild(namelbl);
	}
}

void RepairLayer::delayShowOkLayer(float dt)
{
	RepairOKLayer* layer = RepairOKLayer::create((PackageData*)repairResImg->getUserData(), m_type);
	this->addChild(layer);

	if (m_type == 2)
	{
		onRepairItem(vec_repairItem[clickindex]);
	}
	m_actionBtn->setEnabled(true);
}

void RepairLayer::delayCanClick(float dt)
{
	g_gameLayer->removeChildByName("notouchlayer");
}

void RepairLayer::dofail()
{
	failSprite->runAction(Sequence::create(Show::create(), FadeIn::create(0.5f), NULL));
	if (vec_RepairData[clickindex]->slv < 10)
	{
		vec_RepairData[clickindex]->slv--;
	}
	else
	{
		if (g_hero->getAtrByType(H_WEAPON) == vec_RepairData[clickindex])
		{
			PackageData data;
			data.count = 0;
			g_hero->setAtrByType(H_WEAPON, data);
			return;
		}
		if (g_hero->getAtrByType(H_ARMOR) == vec_RepairData[clickindex])
		{
			PackageData data;
			data.count = 0;
			g_hero->setAtrByType(H_ARMOR, data);
			return;
		}

		for (int i = 0; i < MyPackage::getSize(); i++)
		{
			std::string strid = MyPackage::vec_packages[i].strid;
			if (strid.compare(vec_RepairData[clickindex]->strid) == 0)
			{
				MyPackage::cutone(MyPackage::vec_packages[i]);
			}
		}
	}
}

void RepairLayer::onExit()
{
	Layer::onExit();
}