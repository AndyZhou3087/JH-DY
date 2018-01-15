#include "MixGFNode.h"
#include "json.h"
#include "CommonFuncs.h"
#include "Const.h"
#include "GameScene.h"
#include "GlobalData.h"
#include "StorageRoom.h"
#include "GameDataSave.h"
#include "SoundManager.h"
#include "MyMenu.h"
#include "HintBox.h"
#include "Hero.h"
#include "BuyComfirmLayer.h"
#include "MixGFDoneLayer.h"
#include "HeroProperNode.h"
#include "MixSuggestLayer.h"
#include "NewerGuide2Layer.h"

MixGFNode::MixGFNode()
{
	lastselectitem = NULL;
	masterGFData = NULL;
	lastclickindex = -1;
}


MixGFNode::~MixGFNode()
{

}

bool MixGFNode::init()
{
	csbroot = CSLoader::createNode("mixGFNode.csb");
	this->addChild(csbroot);
	for (int i = 0; i < 4; i++)
	{
		std::string str = StringUtils::format("mixbox_%d", i);

		imgbtn[i] = (cocos2d::ui::ImageView*)csbroot->getChildByName(str);
		imgbtn[i]->setTag(i);
		imgbtn[i]->addTouchEventListener(CC_CALLBACK_2(MixGFNode::onImageClick, this));

		img[i] = (cocos2d::ui::ImageView*)imgbtn[i]->getChildByName("img");

	}

	mixokbtn = (cocos2d::ui::Widget*)csbroot->getChildByName("mixbtn");
	mixokbtn->addTouchEventListener(CC_CALLBACK_2(MixGFNode::onMix, this));
	mixokbtn->setVisible(false);

	mixtitle = (cocos2d::ui::Text*)csbroot->getChildByName("mixtitle");

	heroselectbg = (cocos2d::ui::Widget*)csbroot->getChildByName("heroselectbg");

	m_scrollView = (cocos2d::ui::ScrollView*)heroselectbg->getChildByName("ScrollView");
	m_scrollView->setScrollBarEnabled(false);
	m_scrollView->setBounceEnabled(true);

	cocos2d::ui::Button* okbtn = (cocos2d::ui::Button*)heroselectbg->getChildByName("okbtn");
	okbtn->addTouchEventListener(CC_CALLBACK_2(MixGFNode::onOK, this));

	cocos2d::ui::ImageView* suggestbtn = (cocos2d::ui::ImageView*)csbroot->getChildByName("heroatrmixhint");
	suggestbtn->addTouchEventListener(CC_CALLBACK_2(MixGFNode::onSuggest, this));

	desc0 = (cocos2d::ui::Text*)csbroot->getChildByName("mixdesc");
	desc1 = (cocos2d::ui::Text*)csbroot->getChildByName("mixdesc1");
	desc = (cocos2d::ui::Text*)csbroot->getChildByName("desc");

	m_select = Sprite::createWithSpriteFrameName("ui/atrselected.png");
	m_select->setAnchorPoint(Vec2(0, 1));
	m_scrollView->addChild(m_select, 1);
	m_select->setVisible(false);

	for (int i = 0; i < 3; i++)
	{
		m_secselect[i] = Sprite::createWithSpriteFrameName("ui/mixsecselect.png");
		m_secselect[i]->setAnchorPoint(Vec2(0, 1));
		m_scrollView->addChild(m_secselect[i], 1);
		m_secselect[i]->setVisible(false);
	}

	addGFData();

	loadMixSuccGF();

	updateGFScroll();

	updateDesc();

	m_listener = EventListenerTouchOneByOne::create();
	m_listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};
	m_listener->setSwallowTouches(false);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(m_listener, this);

	return true;
}

void MixGFNode::onEnterTransitionDidFinish()
{
	Node::onEnterTransitionDidFinish();
}

void MixGFNode::onOK(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		heroselectbg->setVisible(false);
		m_listener->setSwallowTouches(false);
		this->setLocalZOrder(0);
	}
}

void MixGFNode::onImageClick(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);

		if (g_hero->getIsOut())
		{
			HintBox* layer = HintBox::create(CommonFuncs::gbk2utf("请回住所进行组合！！"));
			g_gameLayer->addChild(layer, 4);
			return;
		}
		Node* node = (Node*)pSender;
		int tag = node->getTag();
		//点击相同的一个不做操作
		if (lastclickindex == tag && heroselectbg->isVisible())
			return;

		lastclickindex = tag;
		heroselectbg->setVisible(true);
		m_listener->setSwallowTouches(true);
		this->setLocalZOrder(1);

		if (lastclickindex == 0)
		{
			if (m_select->isVisible())
				lastselectitem = masterGFData;
			else
				lastselectitem = NULL;

			std::map<int, std::vector<PackageData*>>::iterator it;
			for (it = map_secgfdata.begin(); it != map_secgfdata.end(); ++it)
			{
				if (map_secgfdata[it->first].size() > 0)
				{
					for (unsigned int m = 0; m < vec_myhasgf.size(); m++)
					{
						std::string name = StringUtils::format("resbox%d", m);
						MenuItemSprite *menuSprite = (MenuItemSprite *)m_scrollView->getChildByName(name)->getChildByName("itembox");
						if (vec_myhasgf[m].strid.compare(map_secgfdata[it->first][0]->strid) == 0)
						{
							std::string name = StringUtils::format("resbox%d", m);
							MenuItemSprite *menuSprite = (MenuItemSprite *)m_scrollView->getChildByName(name)->getChildByName("itembox");
							CommonFuncs::removeGray(menuSprite->getNormalImage());
							CommonFuncs::removeGray(menuSprite->getNormalImage()->getChildByName("res"));
						}
					}
				}
			}
		}
		else
		{
			if (m_secselect[lastclickindex - 1]->isVisible())
				lastselectitem = map_secgfdata[lastclickindex - 1][0];
			else
				lastselectitem = NULL;
			std::map<int, std::vector<PackageData*>>::iterator it;
			for (it = map_secgfdata.begin(); it != map_secgfdata.end(); ++it)
			{
				if (map_secgfdata[it->first].size() > 0)
				{
					for (unsigned int m = 0; m < vec_myhasgf.size(); m++)
					{
						std::string name = StringUtils::format("resbox%d", m);
						MenuItemSprite *menuSprite = (MenuItemSprite *)m_scrollView->getChildByName(name)->getChildByName("itembox");
						if (vec_myhasgf[m].strid.compare(map_secgfdata[it->first][0]->strid) == 0)
						{
							if (it->first == lastclickindex - 1)
							{
								CommonFuncs::removeGray(menuSprite->getNormalImage());
								CommonFuncs::removeGray(menuSprite->getNormalImage()->getChildByName("res"));
							}
							else
							{
								CommonFuncs::changeGray(menuSprite->getNormalImage());
								CommonFuncs::changeGray(menuSprite->getNormalImage()->getChildByName("res"));
							}

							break;
						}
					}
				}
			}
		}
	}
}

void MixGFNode::onMix(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);

		int selectsecsize = map_secgfdata.size();
		if (masterGFData != NULL && selectsecsize > 0)
		{
			int isok = -1;
			std::map<std::string, MixGfData>::iterator it;

			for (it = GlobalData::map_MixGfData.begin(); it != GlobalData::map_MixGfData.end(); ++it)
			{
				int index = 0;
				int secgfsize = GlobalData::map_MixGfData[it->first].vec_secgf.size();
				if (GlobalData::map_MixGfData[it->first].mastergf.compare(masterGFData->strid) == 0 && secgfsize == selectsecsize)
				{
					for (int i = 0; i < secgfsize; i++)
					{
						for (int m = 0; m < secgfsize; m++)
						{
							if (map_secgfdata[m].size() > 0 && GlobalData::map_MixGfData[it->first].vec_secgf[i].compare(map_secgfdata[m][0]->strid) == 0)
							{
								index++;
								break;
							}
						}
					}
					if (index == secgfsize)
					{
						isok = 0;
						HeroProperNode * heropropernode = (HeroProperNode*)this->getParent()->getChildByName("HeroProperNode");
						HeroAtrType heroatrtype[] = { H_WG, H_NG };
						for (int i = 0; i < 2; i++)
						{
							if (g_hero->getAtrByType(heroatrtype[i])->count > 0)
							{
								if (GlobalData::map_MixGfData[it->first].mastergf.compare(g_hero->getAtrByType(heroatrtype[i])->strid) == 0)
								{
									heropropernode->refreshGF(heroatrtype[i]);
								}
								else
								{
									for (int n = 0; n < secgfsize; n++)
									{
										if (GlobalData::map_MixGfData[it->first].vec_secgf[n].compare(g_hero->getAtrByType(heroatrtype[i])->strid) == 0)
										{
											heropropernode->refreshGF(heroatrtype[i]);
											break;
										}
									}
								}

							}
						}
						//StorageRoom::use(GlobalData::map_MixGfData[it->first].mastergf);

						for (int n = 0; n < secgfsize; n++)
						{
							StorageRoom::use(GlobalData::map_MixGfData[it->first].vec_secgf[n]);
						}
						GlobalData::setMixGF(it->first);
						mixtitle->setString(GlobalData::map_MixGfData[it->first].name);
						mixokbtn->setVisible(false);

						for (unsigned int i = 0; i < GlobalData::vec_achiveData.size(); i++)
						{
							if (GlobalData::vec_achiveData[i].type == A_8)
							{
								if (GlobalData::vec_achiveData[i].vec_para[0].compare(it->first) == 0 && GlobalData::vec_achiveData[i].finish != -1)
								{
									GlobalData::vec_achiveData[i].finish = 1;
									GlobalData::saveAchiveData();
								}
							}
						}

						break;
					}
				}
			}
			this->getParent()->addChild(MixGFDoneLayer::create(isok), 2);
			if (isok == -1)
			{
				int injury = g_hero->getInnerinjuryValue() - 10;
				if (injury < 0)
				{
					injury = 0;
				}
				g_hero->setInnerinjuryValue(injury);
				float life = g_hero->getLifeValue() - g_hero->getMaxLifeValue()*0.1;
				if (life < 0.0f)
					life = 0.0f;
				g_hero->setLifeValue(life);
			}
		}

		updateDesc();
	}
}

void MixGFNode::addGFData()
{
	//
	vec_myhasgf.clear();
	for (unsigned int m = 0; m < StorageRoom::map_storageData[W_GONG].size(); m++)
	{
		PackageData data = StorageRoom::map_storageData[W_GONG][m];
		if (data.lv >= GlobalData::map_wgngs[data.strid].maxlv - 1)
			vec_myhasgf.push_back(data);
	}

	for (unsigned int m = 0; m < StorageRoom::map_storageData[N_GONG].size(); m++)
	{
		PackageData data = StorageRoom::map_storageData[N_GONG][m];
		if (data.lv >= GlobalData::map_wgngs[data.strid].maxlv - 1)
			vec_myhasgf.push_back(data);
	}
	std::string herocarrywg;
	if (g_hero->getAtrByType(H_WG)->count > 0) //装备的外功
	{
		herocarrywg = g_hero->getAtrByType(H_WG)->strid;
		PackageData sdata = *g_hero->getAtrByType(H_WG);
		if (sdata.lv >= GlobalData::map_wgngs[sdata.strid].maxlv - 1)
			vec_myhasgf.insert(vec_myhasgf.begin(), sdata);
	}
	std::string herocarryng;
	if (g_hero->getAtrByType(H_NG)->count > 0)//装备的内功
	{
		herocarryng = g_hero->getAtrByType(H_NG)->strid;
		PackageData sdata = *g_hero->getAtrByType(H_NG);
		if (sdata.lv >= GlobalData::map_wgngs[sdata.strid].maxlv - 1)
		vec_myhasgf.insert(vec_myhasgf.begin(), sdata);
	}
	std::string mymixgf = GlobalData::getMixGF();
	if (mymixgf.length() > 0)
	{
		MixGfData mdata = GlobalData::map_MixGfData[mymixgf];
		if (herocarrywg.compare(mdata.mastergf) != 0 && herocarryng.compare(mdata.mastergf) != 0 && StorageRoom::getCountById(mdata.mastergf) <= 0)
		{
			PackageData data;
			WG_NGData gfdata = GlobalData::map_wgngs[mdata.mastergf];
			data.strid = gfdata.id;
			data.count = 1;
			data.lv = gfdata.maxlv - 1;
			data.type = gfdata.type - 1;
			data.extype = gfdata.extype;
			vec_myhasgf.insert(vec_myhasgf.begin(), data);
		}

		for (unsigned int n = 0; n < mdata.vec_secgf.size(); n++)
		{
			PackageData data;
			WG_NGData gfdata = GlobalData::map_wgngs[mdata.vec_secgf[n]];
			data.strid = gfdata.id;
			data.count = 1;
			data.lv = gfdata.maxlv - 1;
			data.type = gfdata.type - 1;
			data.extype = gfdata.extype;
			vec_myhasgf.push_back(data);
		}
	}
}

void MixGFNode::updateGFScroll()
{
	int tempsize = vec_myhasgf.size();
	int itemheight = 165;
	int row = tempsize % 4 == 0 ? tempsize / 4 : (tempsize / 4 + 1);
	int innerheight = itemheight * row;
	int contentheight = m_scrollView->getContentSize().height;
	if (innerheight < contentheight)
		innerheight = contentheight;
	m_scrollView->setInnerContainerSize(Size(650, innerheight));

	for (int i = 0; i < tempsize; i++)
	{
		std::string boxstr = "ui/buildsmall.png";
		PackageData tmpdata = vec_myhasgf[i];

		boxstr = StringUtils::format("ui/qubox%d.png", GlobalData::map_wgngs[tmpdata.strid].qu);

		Sprite * box = Sprite::createWithSpriteFrameName(boxstr);

		MenuItemSprite* boxItem = MenuItemSprite::create(
			box,
			box,
			box,
			CC_CALLBACK_1(MixGFNode::onItem, this));
		boxItem->setUserData(&vec_myhasgf[i]);
		boxItem->setTag(i);

		boxItem->setPosition(Vec2(80 + i % 4 * 145, innerheight - i / 4 * itemheight - itemheight / 2));
		MyMenu* menu = MyMenu::create();
		menu->setTouchlimit(m_scrollView);
		menu->addChild(boxItem, 0, "itembox");
		menu->setPosition(Vec2(0, 0));
		std::string name = StringUtils::format("resbox%d", i);
		m_scrollView->addChild(menu, 0, name);

		std::string rstrid = vec_myhasgf[i].strid;
		std::string str = StringUtils::format("ui/%s.png", rstrid.c_str());
		Sprite * res = Sprite::createWithSpriteFrameName(str);
		res->setPosition(Vec2(box->getContentSize().width / 2, box->getContentSize().height / 2));
		box->addChild(res, 0, "res");

		str = StringUtils::format("%s", GlobalData::map_allResource[rstrid].cname.c_str());
		Label * namelbl = Label::createWithTTF(str, "fonts/STXINGKA.TTF", 24);
		namelbl->setColor(Color3B(0, 0, 0));
		namelbl->setPosition(Vec2(box->getContentSize().width / 2, -10));
		box->addChild(namelbl);

		//Label * lvlbl = Label::createWithSystemFont("", "", 15);
		//lvlbl->setColor(Color3B(255, 255, 255));
		//lvlbl->setAnchorPoint(Vec2(1, 0));
		//lvlbl->setPosition(Vec2(box->getContentSize().width - 10, 8));
		//box->addChild(lvlbl);

		if (masterGFData != NULL)
		{
			if (masterGFData->strid.compare(tmpdata.strid) == 0)
			{
				m_select->setPosition(Vec2(boxItem->getPositionX() - boxItem->getContentSize().width / 2, boxItem->getPositionY() + boxItem->getContentSize().height / 2));
				m_select->setVisible(true);
			}
			for (unsigned int i = 0; i < map_secgfdata.size(); i++)
			{
				if (map_secgfdata[i].size() > 0 && map_secgfdata[i][0]->strid.compare(tmpdata.strid) == 0)
				{
					m_secselect[i]->setPosition(Vec2(boxItem->getPositionX() - boxItem->getContentSize().width / 2, boxItem->getPositionY() + boxItem->getContentSize().height / 2));
					m_secselect[i]->setVisible(true);
				}
			}
		}
	}
}

void MixGFNode::loadMixSuccGF()
{
	std::string mymixgf = GlobalData::getMixGF();
	if (mymixgf.length() > 0)
	{
		MixGfData mdata =  GlobalData::map_MixGfData[mymixgf];
		std::string str = StringUtils::format("ui/%s.png", mdata.mastergf.c_str());
		img[0]->loadTexture(str, cocos2d::ui::TextureResType::PLIST);
		str = StringUtils::format("ui/qubox%d.png", GlobalData::map_wgngs[mdata.mastergf].qu);
		imgbtn[0]->loadTexture(str, cocos2d::ui::TextureResType::PLIST);

		for (unsigned int m = 0; m < vec_myhasgf.size(); m++)
		{
			if (vec_myhasgf[m].strid.compare(mdata.mastergf) == 0)
			{
				masterGFData = &vec_myhasgf[m];
				break;
			}
		}
		
		for (unsigned int i = 0; i < mdata.vec_secgf.size(); i++)
		{
			str = StringUtils::format("ui/%s.png", mdata.vec_secgf[i].c_str());
			img[i + 1]->loadTexture(str, cocos2d::ui::TextureResType::PLIST);
			str = StringUtils::format("ui/qubox%d.png", GlobalData::map_wgngs[mdata.vec_secgf[i]].qu);
			imgbtn[i + 1]->loadTexture(str, cocos2d::ui::TextureResType::PLIST);


			for (unsigned int m = 0; m < vec_myhasgf.size(); m++)
			{
				if (mdata.vec_secgf[i].compare(vec_myhasgf[m].strid) == 0)
				{
					map_secgfdata[i].push_back(&vec_myhasgf[m]);
					break;
				}
			}
		}
		mixtitle->setString(GlobalData::map_MixGfData[mymixgf].name);
	}
}

void MixGFNode::onItem(Ref* pSender)
{
	SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);
	if (GlobalData::isExercising() && !GlobalData::isHasFSF())
	{
		int index = -1;
		for (unsigned int i = 0; i < GlobalData::vec_goods.size(); i++)
		{
			if (GlobalData::vec_goods[i].icon.compare("72") == 0)
			{
				index = i;
				break;
			}
		}
		if (index >= 0)
		{
			BuyComfirmLayer* layer = BuyComfirmLayer::create(&GlobalData::vec_goods[index]);
			Director::getInstance()->getRunningScene()->addChild(layer, 1000, "buycomfirmlayer");
		}
		return;
	}
	HeroProperNode * heropropernode = (HeroProperNode*)this->getParent()->getChildByName("HeroProperNode");
	Node* item = (Node*)pSender;
	int tag = item->getTag();
	PackageData* pdata = (PackageData*)item->getUserData();
	if (lastselectitem != pdata)
	{
		if (lastclickindex == 0)
		{
			m_select->setPosition(Vec2(item->getPositionX() - item->getContentSize().width / 2, item->getPositionY() + item->getContentSize().height / 2));
			m_select->setVisible(true);
			masterGFData = pdata;
			int index = -1;
			for (unsigned int i = 0; i < map_secgfdata.size(); i++)
			{
				if (map_secgfdata[i].size() > 0 && map_secgfdata[i][0]->strid.compare(pdata->strid) == 0)
				{
					index = i;
					break;
				}
			}
			if (index >= 0)
			{
				m_secselect[index]->setVisible(false);
				map_secgfdata.erase(index);
				std::string sstr = "ui/buildsmall.png";
				imgbtn[index+1]->loadTexture(sstr, cocos2d::ui::TextureResType::PLIST);
				sstr = "ui/mixsectext.png";
				img[index + 1]->loadTexture(sstr, cocos2d::ui::TextureResType::PLIST);
			}
		}
		else
		{
			if (masterGFData != NULL && masterGFData->strid.compare(pdata->strid) == 0)
			{
				m_select->setVisible(false);
				masterGFData = NULL;
				std::string sstr = "ui/buildsmall.png";
				imgbtn[0]->loadTexture(sstr, cocos2d::ui::TextureResType::PLIST);
				sstr = "ui/mixmtext.png";
				img[0]->loadTexture(sstr, cocos2d::ui::TextureResType::PLIST);
			}

			for (unsigned int i = 0; i < map_secgfdata.size(); i++)
			{
				if (map_secgfdata[i].size() > 0 && map_secgfdata[i][0]->strid.compare(pdata->strid) == 0)
					return;
			}
			map_secgfdata[lastclickindex - 1].clear();
			map_secgfdata[lastclickindex - 1].push_back(pdata);
			m_secselect[lastclickindex - 1]->setPosition(Vec2(item->getPositionX() - item->getContentSize().width / 2, item->getPositionY() + item->getContentSize().height / 2));
			m_secselect[lastclickindex - 1]->setVisible(true);
		}

		std::string sstr = StringUtils::format("ui/qubox%d.png", GlobalData::map_wgngs[pdata->strid].qu);
		imgbtn[lastclickindex]->loadTexture(sstr, cocos2d::ui::TextureResType::PLIST);
		sstr = StringUtils::format("ui/%s.png", pdata->strid.c_str());
		img[lastclickindex]->loadTexture(sstr, cocos2d::ui::TextureResType::PLIST);
		lastselectitem = pdata;

		if (pdata->type == W_GONG && g_hero->getAtrByType(H_WG)->strid.compare(pdata->strid) == 0 && g_hero->getAtrByType(H_WG)->count > 0)
			heropropernode->refreshGF(H_WG);
		else if (pdata->type == N_GONG && g_hero->getAtrByType(H_NG)->strid.compare(pdata->strid) == 0 && g_hero->getAtrByType(H_NG)->count > 0)
			heropropernode->refreshGF(H_NG);
	}
	else
	{
		if (lastclickindex == 0)
		{
			m_select->setVisible(false);
		}
		else
		{
			if (masterGFData != NULL && masterGFData->strid.compare(pdata->strid) == 0)
				return;

			m_secselect[lastclickindex - 1]->setVisible(false);
		}

		std::string sstr = "ui/buildsmall.png";
		imgbtn[lastclickindex]->loadTexture(sstr, cocos2d::ui::TextureResType::PLIST);

		if (lastclickindex == 0)
		{
			sstr = "ui/mixmtext.png";
			masterGFData = NULL;
		}
		else
		{
			sstr = "ui/mixsectext.png";
			map_secgfdata.erase(lastclickindex - 1);
		}
		img[lastclickindex]->loadTexture(sstr, cocos2d::ui::TextureResType::PLIST);

		lastselectitem = NULL;

	}

	if (masterGFData != NULL && map_secgfdata.size() > 0)
	{
		mixokbtn->setVisible(true);
	}

	std::string mixid = GlobalData::getMixGF();
	if (mixid.length() > 0)
	{
		MixGfData mdata = GlobalData::map_MixGfData[mixid];
		for (unsigned int n = 0; n < mdata.vec_secgf.size(); n++)
		{
			if (StorageRoom::getCountById(mdata.vec_secgf[n]) <= 0)
			{
				PackageData data;
				WG_NGData gfdata = GlobalData::map_wgngs[mdata.vec_secgf[n]];
				data.strid = gfdata.id;
				data.count = 1;
				data.lv = gfdata.maxlv - 1;
				data.type = gfdata.type - 1;
				data.extype = gfdata.extype;
				StorageRoom::add(data);
			}
		}
	}
	mixtitle->setString(CommonFuncs::gbk2utf("武功合成"));
	GlobalData::setMixGF("");
	updateDesc();
	heropropernode->removeMixTag();
}


void MixGFNode::saveData()
{
	//保存装备栏数据
	//std::string str;
	//for (int i = H_WEAPON; i < H_MAX; i++)
	//{
	//	PackageData* sdata = g_hero->getAtrByType((HeroAtrType)i);
	//
	//	std::string idstr = StringUtils::format("%s-%d-%d-%d-%d-%d-%d-%d-%d;", sdata->strid.c_str(), sdata->type, sdata->count, sdata->extype, sdata->lv, sdata->exp, sdata->goodvalue, sdata->slv,sdata->tqu);
	//	str.append(idstr);
	//}
	//GameDataSave::getInstance()->setHeroProperData(str.substr(0, str.length() - 1));
}

void MixGFNode::onExit()
{
	saveData();
	Node::onExit();
}

void MixGFNode::onSuggest(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);
		if (g_hero->getIsOut())
		{
			HintBox* layer = HintBox::create(CommonFuncs::gbk2utf("请回所宅查看！！"));
			g_gameLayer->addChild(layer, 4);
		}
		else
		{
			MixSuggestLayer* layer = MixSuggestLayer::create();
			g_gameLayer->addChild(layer, 4, "mixsuggestlayer");
		}
	}
}

void MixGFNode::updateDesc()
{
	std::string mixid = GlobalData::getMixGF();
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

void MixGFNode::showTalkGuide()
{
	if (NewerGuide2Layer::checkifNewerGuide(102))
	{
		std::vector<Node*> vec_node;
		for (int i = 0; i < 4;i++)
		{
			std::string childname = StringUtils::format("mixbox_%d", i);
			Node* node = csbroot->getChildByName(childname);
			node->setUserData((void*)"buildsmall");
			vec_node.push_back(node);
			Node* cnode = node->getChildByName("img");
			if (i == 0)
				cnode->setUserData((void*)"mixmtext");
			else
				cnode->setUserData((void*)"mixsectext");
			vec_node.push_back(cnode);
		}
		NewerGuide2Layer *layer = NewerGuide2Layer::create(102, vec_node);
		g_gameLayer->addChild(layer, NEWERLAYERZOER);
	}
}
