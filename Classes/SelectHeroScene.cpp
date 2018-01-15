#include "SelectHeroScene.h"
#include "GlobalData.h"
#include "Const.h"
#include "HintBox.h"
#include "CommonFuncs.h"
#include "GameDataSave.h"
#include "BuyDetailsLayer.h"
#include "SoundManager.h"
#include "StoryScene.h"
#include "GameScene.h"
#include "StartScene.h"
#include "ComfirmSaveLayer.h"
#include "GameDataSave.h"

USING_NS_CC;

SelectHeroScene* g_SelectHeroScene = NULL;
SelectHeroScene::SelectHeroScene()
{
	_lastSelect = 1;
	isMoving = false;
}
SelectHeroScene::~SelectHeroScene()
{
	g_SelectHeroScene = NULL;
}

Scene* SelectHeroScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
	g_SelectHeroScene = SelectHeroScene::create();

    // add layer as a child to scene
	scene->addChild(g_SelectHeroScene);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool SelectHeroScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	GlobalData::loadUnlockHeroData();

	Node* csbnode = CSLoader::createNode("selectHeroLayer.csb");
	this->addChild(csbnode);

	cocos2d::ui::Widget* startbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("startbtn");
	startbtn->addTouchEventListener(CC_CALLBACK_2(SelectHeroScene::onStart, this));

	cocos2d::ui::Widget* backbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(SelectHeroScene::onBack, this));

	for (int i = 0; i < 4; i++)
	{
		std::string str = StringUtils::format("heroimg%d", i + 1);
		heroimg[i] = (cocos2d::ui::ImageView*)csbnode->getChildByName(str);
		heroimg[i]->setTag(i + 1);
		heroimg[i]->addTouchEventListener(CC_CALLBACK_2(SelectHeroScene::onSelect, this));
		heroimg[i]->setLocalZOrder(i);

		str = StringUtils::format("select_%d", i + 1);
		selectimg[i] = (cocos2d::ui::ImageView*)heroimg[i]->getChildByName(str);
		selectimg[i]->setPositionX(heroimg[i]->getContentSize().width / 2);
		heronameimg[i] = (cocos2d::ui::Widget*)selectimg[i]->getChildByName("sheroname");
		heronameimg[i]->setPositionX(selectimg[i]->getContentSize().width / 2);

		str = StringUtils::format("lock_%d", i + 1);
		lock[i] = (cocos2d::ui::ImageView*)heroimg[i]->getChildByName(str);
		lock[i]->setVisible(!GlobalData::getUnlocHero(i));
	}
	int defaultindex = 0;
	std::string str = StringUtils::format("images/shero%d.jpg", defaultindex + 1);
	heroimg[defaultindex]->loadTexture(str, cocos2d::ui::TextureResType::LOCAL);
	heroimg[defaultindex]->setContentSize(Sprite::create(str)->getContentSize());

	str = StringUtils::format("images/select.png");
	selectimg[defaultindex]->loadTexture(str, cocos2d::ui::TextureResType::LOCAL);
	selectimg[defaultindex]->setContentSize(Sprite::create(str)->getContentSize());
	selectimg[defaultindex]->getChildByName("sheroname")->setPositionX(selectimg[defaultindex]->getContentSize().width / 2);

    return true;
}

void SelectHeroScene::onExit()
{
	Layer::onExit();
}

void SelectHeroScene::onSelect(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);
		Node* btnnode = (Node*)pSender;
		int tag = btnnode->getTag();

		if (!GlobalData::getUnlocHero(tag - 1))
		{
			Layer* layer = BuyDetailsLayer::create(tag);
			layer->setTag(tag - 1);
			this->addChild(layer, 0, "buyherolayer");
			return;
		}
		if (_lastSelect == tag || isMoving)
			return;

		isMoving = true;
		std::string str = StringUtils::format("images/shero%d.jpg", tag);
		heroimg[tag - 1]->loadTexture(str, cocos2d::ui::TextureResType::LOCAL);
		heroimg[tag - 1]->setContentSize(Sprite::create(str)->getContentSize());

		str = StringUtils::format("images/select.png");
		selectimg[tag - 1]->loadTexture(str, cocos2d::ui::TextureResType::LOCAL);
		selectimg[tag - 1]->setContentSize(Sprite::create(str)->getContentSize());

		selectimg[tag - 1]->setPositionX(heroimg[tag - 1]->getContentSize().width / 2);

		heronameimg[tag - 1]->setPositionX(selectimg[tag - 1]->getContentSize().width / 2);

		str = StringUtils::format("images/shero%d_s.jpg", _lastSelect);
		heroimg[_lastSelect - 1]->loadTexture(str, cocos2d::ui::TextureResType::LOCAL);
		heroimg[_lastSelect - 1]->setContentSize(Sprite::create(str)->getContentSize());
		heroimg[_lastSelect - 1]->setLocalZOrder(_lastSelect - 1);

		str = StringUtils::format("images/unselect.png");
		selectimg[_lastSelect - 1]->loadTexture(str, cocos2d::ui::TextureResType::LOCAL);
		selectimg[_lastSelect - 1]->setContentSize(Sprite::create(str)->getContentSize());
		selectimg[_lastSelect - 1]->setPositionX(heroimg[_lastSelect - 1]->getContentSize().width / 2);
		heronameimg[_lastSelect - 1]->setPositionX(selectimg[_lastSelect - 1]->getContentSize().width / 2);
		
		int headsize = sizeof(heroimg) / sizeof(heroimg[0]);

		int leftperwidth = (720 - selectimg[tag - 1]->getContentSize().width) /(headsize - 1);
		for (int i = 0; i < headsize; i++)
		{
			int movex = 0;
			if (i < tag - 1)
			{
				movex = leftperwidth / 2 + i*leftperwidth;
			}
			else if (i == tag - 1)
			{
				movex = (tag - 1) * leftperwidth + heroimg[tag - 1]->getContentSize().width / 2;
			}
			else
			{
				movex = (tag - 1)* leftperwidth + heroimg[tag - 1]->getContentSize().width + leftperwidth / 2 + (i - tag) * leftperwidth;
			}
			heroimg[i]->runAction(MoveTo::create(0.1f, Vec2(movex, heroimg[i]->getPositionY())));
			
		}
		this->scheduleOnce(schedule_selector(SelectHeroScene::clickMoveFinish), 0.1f);
		_lastSelect = tag;
	}
}

void SelectHeroScene::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Director::getInstance()->replaceScene(StartScene::createScene());
	}
}

void SelectHeroScene::onStart(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		std::string uid = GlobalData::getUId();
		if (uid.size() <= 0)
		{
			enterNextScene();
			return;
		}

		bool isExit = false;
		std::vector<std::string> vec_ids = GlobalData::getSaveListId();
		
		for (unsigned int i = 0; i < vec_ids.size(); i++)
		{
			std::string saveuid = vec_ids[i];
			if (saveuid.length() > 0)
			{
				int heroid = GameDataSave::getInstance()->getHeroIdByUid(saveuid);
				if (heroid == _lastSelect)
				{
					isExit = true;
					break;
				}
			}
		}
		if (isExit)
		{
			this->addChild(ComfirmSaveLayer::create());
		}
		else
		{
			enterNextScene();
		}
	}
}

void SelectHeroScene::enterNextScene()
{
	int systime = GlobalData::getSysSecTime();
	std::string uidstr = StringUtils::format("%d", systime);
	GlobalData::setUId(uidstr);
	GameDataSave::getInstance()->setHeroId(_lastSelect);

	GlobalData::setCurHeroIdToSaveList();

	std::string defaultStorageStr = GlobalData::getDefaultStorage(_lastSelect);
	GameDataSave::getInstance()->setStorageData(defaultStorageStr);

	Scene* scene = StoryScene::createScene(0);

	Director::getInstance()->replaceScene(scene);
}

void SelectHeroScene::unlockSucc(int index)
{
	this->removeChildByName("buyherolayer");
	lock[index]->setVisible(false);
	GlobalData::setUnlockHero(index, true);
}

void SelectHeroScene::clickMoveFinish(float dt)
{
	isMoving = false;
}

