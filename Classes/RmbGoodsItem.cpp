#include "RmbGoodsItem.h"
#include "CommonFuncs.h"
#include "ShopLayer.h"
#include "SoundManager.h"
RmbGoodsItem::RmbGoodsItem()
{
	isDraging = false;
}


RmbGoodsItem::~RmbGoodsItem()
{
}

bool RmbGoodsItem::init(GoodsData* gdata)
{
	if (!Sprite::initWithSpriteFrameName("ui/blank.png"))
		return false;

	Node* csbnode = CSLoader::createNode("rmbGoodsNode.csb");//物品节点
	csbnode->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height / 2));
	this->addChild(csbnode);

	Node* bgnode = csbnode->getChildByName("itembg");

	icon = (cocos2d::ui::ImageView*)bgnode->getChildByName("icon");
	nameTxt = (cocos2d::ui::Text*)bgnode->getChildByName("name");

	//图标
	std::string imagepath = StringUtils::format("ui/%s.png", gdata->icon.c_str());
	icon->loadTexture(imagepath, cocos2d::ui::TextureResType::PLIST);
	icon->setContentSize(Sprite::createWithSpriteFrameName(imagepath)->getContentSize());

	nameTxt->setString(gdata->name);
	std::string pricestr = StringUtils::format("%d元", gdata->price);

	cocos2d::ui::Button* bgbtn = (cocos2d::ui::Button*)csbnode->getChildByName("itembg");//整块节点击
	bgbtn->addTouchEventListener(CC_CALLBACK_2(RmbGoodsItem::onItem, this));
	bgbtn->setSwallowTouches(false);

	cocos2d::ui::Button* buybtn = (cocos2d::ui::Button*)bgnode->getChildByName("buybtn");//整块节点击
	buybtn->addTouchEventListener(CC_CALLBACK_2(RmbGoodsItem::onBuyBtn, this));
	buybtn->setSwallowTouches(false);
	buybtn->setTitleText(CommonFuncs::gbk2utf(pricestr.c_str()));
	return true;
}

RmbGoodsItem* RmbGoodsItem::create(GoodsData* gdata)
{
	RmbGoodsItem *pRet = new RmbGoodsItem();
	if (pRet && pRet->init(gdata))
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
void RmbGoodsItem::onBuyBtn(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	onItem(pSender, type);
}

void RmbGoodsItem::onItem(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	Node* node = (Node*)pSender;
	if (type == ui::Widget::TouchEventType::BEGAN)
	{
		startPos = node->convertToWorldSpace(this->getParent()->getPosition());
	}
	else if (type == ui::Widget::TouchEventType::MOVED)
	{
		Vec2 pos = node->convertToWorldSpace(this->getParent()->getPosition());
		if (fabsf(pos.y - startPos.y) > 20)
			isDraging = true;
	}
	else if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (!isDraging)
		{
			SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);
			//购买
			ShopLayer::beginPay(this->getTag());
		}
		isDraging = false;
	}
}