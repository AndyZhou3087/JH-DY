#include "BookShelfLayer.h"
#include <algorithm>
#include "json.h"
#include "CommonFuncs.h"
#include "Const.h"
#include "SoundManager.h"
#include "StorageRoom.h"
#include "MyMenu.h"
#include "GlobalData.h"
#include "GameScene.h"
#include "BookDetailsLayer.h"

const std::string name[] = { "武功", "内功", "武器", "防具"};

BookShelfLayer::BookShelfLayer()
{
	lastCategoryindex = 0;
}


BookShelfLayer::~BookShelfLayer()
{

}

bool BookShelfLayer::init()
{

	m_csbnode = CSLoader::createNode("bookshelfLayer.csb");
	m_csbnode->setPosition(Vec2(0, -90));
	this->addChild(m_csbnode);


	cocos2d::ui::Widget* backbtn = (cocos2d::ui::Widget*)m_csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(BookShelfLayer::onBack, this));


	scrollview = (cocos2d::ui::ScrollView*)m_csbnode->getChildByName("ScrollView");
	scrollview->setScrollBarEnabled(false);
	scrollview->setBounceEnabled(true);

	Node* categoryBtnNode = m_csbnode->getChildByName("tagnode");
	for (int i = 0; i < categoryBtnNode->getChildrenCount(); i++)
	{
		std::string btnstr = StringUtils::format("btn%d", i);
		cocos2d::ui::Button* btn = (cocos2d::ui::Button*)categoryBtnNode->getChildByName(btnstr);
		btn->setTag(i);
		btn->addTouchEventListener(CC_CALLBACK_2(BookShelfLayer::onCategory, this));
		vec_categoryBtn.push_back(btn);
	}
	vec_categoryBtn[0]->setBright(false);

	loadBookData();
	updateContent(0);

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);


	return true;
}

void BookShelfLayer::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
}

void BookShelfLayer::updateContent(int category)
{
	scrollview->removeAllChildrenWithCleanup(true);
	int textheigth = 0;
	int dataheigth = 0;
	int innerheight = scrollview->getInnerContainerSize().height;
	int atype[] = { W_GONG, N_GONG, WEAPON, PROTECT_EQU };
	int itemheight = 170;
	std::vector<int> vec_type;
	if (category == 0)
	{
		int size = sizeof(atype) / sizeof(atype[0]);
		for (int i = 0; i < size; i++)
		{
			vec_type.push_back(atype[i]);
			int count = getCountByType(atype[i]);
			if (count > 0)
			{
				int row = count % 4 == 0 ? count / 4 : (count / 4 + 1);
				typerow[i] = row;
				textheigth += 50;
			}
		}
	}
	else
	{
		vec_type.push_back(atype[category - 1]);
		int count = getCountByType(atype[category - 1]);
		if (count > 0)
		{
			int row = count % 4 == 0 ? count / 4 : (count / 4 + 1);
			typerow[0] = row;
		}
	}

	for (unsigned int i = 0; i < vec_type.size(); i++)
	{
		dataheigth += typerow[i] * itemheight;
	}

	innerheight = textheigth + dataheigth;

	int contentheight = scrollview->getContentSize().height;
	if (innerheight < contentheight)
		innerheight = contentheight;
	scrollview->setInnerContainerSize(Size(scrollview->getContentSize().width, innerheight));


	if (category == 0)
	{
		for (unsigned int i = 0; i < vec_type.size(); i++)
		{
			int icount = getCountByType(vec_type[i]);
			if (icount > 0)
			{
				int preheight = 0;
				for (unsigned int j = 0; j < i; j++)
				{
					if (getCountByType(vec_type[j])>0)
					{
						preheight += 50;
						preheight += typerow[j] * itemheight;
					}

				}
				Sprite * sepline = Sprite::createWithSpriteFrameName("ui/storagesepline.png");
				sepline->setPosition(Vec2(sepline->getContentSize().width / 2 + 40, innerheight - 40 - preheight));
				std::string sepname = StringUtils::format("sep%d", i);
				scrollview->addChild(sepline, 0, sepname);
				Label* namelbl = Label::createWithTTF(CommonFuncs::gbk2utf(name[i].c_str()), "fonts/STXINGKA.TTF", 22);
				namelbl->setColor(Color3B(0, 0, 0));
				namelbl->setPosition(Vec2(25, 25));
				sepline->addChild(namelbl);
			}
		}
	}
	for (unsigned int i = 0; i < vec_type.size(); i++)
	{
		for (unsigned int m = 0; m < map_bookData[vec_type[i]].size(); m++)
		{

			std::string boxstr = "";
			std::string datastrid = map_bookData[vec_type[i]][m].strid;
			if (vec_type[i] == WEAPON || vec_type[i] == PROTECT_EQU)
			{
				boxstr = StringUtils::format("ui/qubox%d.png", GlobalData::map_equips[datastrid].qu);
			}
			else if (vec_type[i] == N_GONG || vec_type[i] == W_GONG)
			{
				boxstr = StringUtils::format("ui/qubox%d.png", GlobalData::map_wgngs[datastrid].qu);
			}

			if (boxstr.length() > 0)
			{
				Sprite * box = Sprite::createWithSpriteFrameName(boxstr);

				MenuItemSprite* boxItem = MenuItemSprite::create(
					box,
					box,
					box,
					CC_CALLBACK_1(BookShelfLayer::onclick, this));
				boxItem->setTag(0);
				boxItem->setUserData(&map_bookData[vec_type[i]][m]);
				std::string sepname = StringUtils::format("sep%d", i);
				Node* sepline = scrollview->getChildByName(sepname);
				if (sepline != NULL)
					boxItem->setPosition(Vec2(box->getContentSize().width / 2 + 50 + m % 4 * 140, sepline->getPositionY() - itemheight/2 - m / 4 * itemheight));
				else
					boxItem->setPosition(Vec2(box->getContentSize().width / 2 + 50 + m % 4 * 140, innerheight + 10 - itemheight/2 - m / 4 * itemheight));

				MyMenu* menu = MyMenu::create();
				menu->addChild(boxItem);
				menu->setTouchlimit(scrollview);
				menu->setPosition(Vec2(0, 0));

				scrollview->addChild(menu);

				std::string str = StringUtils::format("ui/%s.png", datastrid.c_str());
				Sprite * res = Sprite::createWithSpriteFrameName(str);
				res->setPosition(Vec2(box->getContentSize().width / 2, box->getContentSize().height / 2));
				box->addChild(res);

				Label *namelbl = Label::createWithTTF(GlobalData::map_allResource[datastrid].cname, "fonts/STXINGKA.TTF", 23);
				namelbl->setColor(Color3B(0, 0, 0));
				namelbl->setPosition(Vec2(box->getContentSize().width / 2, -17));
				box->addChild(namelbl);

	
				if (g_hero->getMeHas(datastrid) == NULL)
				{
					std::string mixid = GlobalData::getMixGF();

					bool isinmixSec = false;
					if (mixid.length() > 0)
					{
						for (unsigned int n = 0; n < GlobalData::map_MixGfData[mixid].vec_secgf.size(); n++)
						{
							if (datastrid.compare(GlobalData::map_MixGfData[mixid].vec_secgf[n]) == 0)
							{
								isinmixSec = true;
								break;
							}
						}
					}
					if (!isinmixSec)
					{
						CommonFuncs::changeGray(box);
						CommonFuncs::changeGray(res);
						namelbl->setOpacity(180);
					}
				}
			}
		}
	}
}

void BookShelfLayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}

void BookShelfLayer::onclick(Ref* pSender)
{
	SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);
	Node* node = (Node*)pSender;
	BookData* bdata = (BookData*)node->getUserData();
	this->addChild(BookDetailsLayer::create(bdata));
}

int BookShelfLayer::getCountByType(int type)
{
	int count = 0;
	return map_bookData[type].size();
}

void BookShelfLayer::loadBookData()
{
	std::map<std::string, WG_NGData>::iterator it;
	for (it = GlobalData::map_wgngs.begin(); it != GlobalData::map_wgngs.end(); ++it)
	{
		WG_NGData gfdata = GlobalData::map_wgngs[it->first];
		std::string strid = gfdata.id;
		BookData bdata;
		bdata.strid = strid;
		bdata.type = gfdata.type - 1;
		bdata.maxvalue = gfdata.vec_bns[gfdata.maxlv - 1];
		map_bookData[gfdata.type - 1].push_back(bdata);
	}

	std::map<std::string, EquipData>::iterator ite;
	for (ite = GlobalData::map_equips.begin(); ite != GlobalData::map_equips.end(); ++ite)
	{
		EquipData edata = GlobalData::map_equips[ite->first];
		BookData bdata;
		bdata.strid = edata.id;
		int type = edata.type - 1;
		if (type == WEAPON)
			bdata.maxvalue = edata.atk;
		else
			bdata.maxvalue = edata.df;
		bdata.type = type;
		map_bookData[type].push_back(bdata);
	}

	std::map<int, std::vector<BookData>>::iterator bit;
	for (bit = map_bookData.begin(); bit != map_bookData.end(); ++bit)
	{
		sort(map_bookData[bit->first].begin(), map_bookData[bit->first].end(), larger_callback);
	}

}

bool BookShelfLayer::larger_callback(BookData a, BookData b)
{
	if (a.maxvalue > b.maxvalue)
		return true;
	else
		return false;
}


void BookShelfLayer::onCategory(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Node* node = (Node*)pSender;
		if (lastCategoryindex == node->getTag())
			return;

		lastCategoryindex = node->getTag();

		for (unsigned int i = 0; i < vec_categoryBtn.size(); i++)
		{
			if (node->getTag() != i)
			{
				vec_categoryBtn[i]->setBright(true);
			}
			else
			{
				vec_categoryBtn[i]->setBright(false);
			}
		}
		updateContent(node->getTag());
	}
}