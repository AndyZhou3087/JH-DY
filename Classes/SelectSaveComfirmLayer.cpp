#include "SelectSaveComfirmLayer.h"
#include "GlobalData.h"
#include "SoundManager.h"
#include "GameDataSave.h"
#include "GameScene.h"
#include "CommonFuncs.h"
#include "Const.h"
bool SelectSaveComfirmLayer::init(int index)
{
	
    if (!Layer::init()) 
	{
		return false;
    }

	Node* csbnode = CSLoader::createNode("selectSaveComfirmLayer.csb");
	this->addChild(csbnode);

	cocos2d::ui::Button* okbtn = (cocos2d::ui::Button*)csbnode->getChildByName("okbtn");
	okbtn->addTouchEventListener(CC_CALLBACK_2(SelectSaveComfirmLayer::onOk, this));
	okbtn->setTag(index);
	cocos2d::ui::Button* cancelbtn = (cocos2d::ui::Button*)csbnode->getChildByName("cancelbtn");
	cancelbtn->addTouchEventListener(CC_CALLBACK_2(SelectSaveComfirmLayer::onCancel, this));

	cocos2d::ui::Text* nametxt = (cocos2d::ui::Text*)csbnode->getChildByName("name");
	nametxt->setString(CommonFuncs::gbk2utf(heroname[index].c_str()));

	cocos2d::ui::ImageView* headimg = (cocos2d::ui::ImageView*)csbnode->getChildByName("headimg");
	std::string headstr = StringUtils::format("ui/tophero%d.png", index + 1);
	headimg->loadTexture(headstr, cocos2d::ui::TextureResType::PLIST);

	//////layer 点击事件，屏蔽下层事件
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};
	//点击任何位置移除掉
	listener->onTouchEnded = [=](Touch *touch, Event *event)
	{
		return;
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    return true;
}

SelectSaveComfirmLayer* SelectSaveComfirmLayer::create(int index)
{
	SelectSaveComfirmLayer *pRet = new SelectSaveComfirmLayer();
	if (pRet && pRet->init(index))
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


void SelectSaveComfirmLayer::onOk(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Node* node = (Node*)pSender;
		int tag = node->getTag();
		std::string suid = GlobalData::getSaveListId().at(tag);
		GlobalData::setUId(suid);

		Scene* scene = GameScene::createScene();

		Director::getInstance()->replaceScene(scene);
	}
}

void SelectSaveComfirmLayer::onCancel(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{	
		removSelf();
	}
}

void SelectSaveComfirmLayer::removSelf()
{
	this->removeFromParentAndCleanup(true);
}

