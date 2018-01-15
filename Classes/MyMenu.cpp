#include "MyMenu.h"

bool MyMenu::init()
{
	return initWithArray(Vector<MenuItem*>());
}
 
MyMenu::MyMenu()
{
	m_szTouchLimitNode = NULL;
	m_bTouchLimit = false;
	m_isdraging = false;
}

MyMenu::~MyMenu()
{
	m_szTouchLimitNode = NULL;
}

bool MyMenu::initWithArray(const Vector<MenuItem*>& arrayOfItems)
{
    if (Layer::init())
    {
        _enabled = true;
        // menu in the center of the screen
        Size s = Director::getInstance()->getWinSize();
 
        this->ignoreAnchorPointForPosition(true);
        setAnchorPoint(Vec2(0.5f, 0.5f));
        this->setContentSize(s);
 
        setPosition(Vec2(s.width/2, s.height/2));
 
        int z=0;
 
        for (auto& item : arrayOfItems)
        {
            this->addChild(item, z);
            z++;
        }
 
        _selectedItem = nullptr;
        _state = Menu::State::WAITING;
 
        // enable cascade color and opacity on menus
        setCascadeColorEnabled(true);
        setCascadeOpacityEnabled(true);
 
 
        auto touchListener = EventListenerTouchOneByOne::create();
        touchListener->setSwallowTouches(false);
 
		touchListener->onTouchBegan = CC_CALLBACK_2(MyMenu::onTouchBegan, this);
		touchListener->onTouchMoved = CC_CALLBACK_2(MyMenu::onTouchMoved, this);
		touchListener->onTouchEnded = CC_CALLBACK_2(MyMenu::onTouchEnded, this);
		touchListener->onTouchCancelled = CC_CALLBACK_2(MyMenu::onTouchCancelled, this);
 
        _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
 
        return true;
    }
    return false;
}
 
MyMenu* MyMenu::createWithArray( const Vector<MenuItem*>& arrayOfItems )
{
	auto ret = new MyMenu();
    if (ret && ret->initWithArray(arrayOfItems))
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
 
    return ret;
}
 
MyMenu* MyMenu::createWithItem( MenuItem* item )
{
	return MyMenu::create(item, nullptr);
}
 
MyMenu* MyMenu::createWithItems( MenuItem *item, va_list args )
{
	Vector<MenuItem*> items;
    if( item )
    {
        items.pushBack(item);
        MenuItem *i = va_arg(args, MenuItem*);
        while(i)
        {
            items.pushBack(i);
            i = va_arg(args, MenuItem*);
        }
    }
 
	return MyMenu::createWithArray(items);
}
 
MyMenu* MyMenu::create( MenuItem* item, ... )
{
    va_list args;
    va_start(args,item);
 
	MyMenu *ret = MyMenu::createWithItems(item, args);
 
    va_end(args);
 
    return ret;
}

MyMenu* MyMenu::create()
{
	return MyMenu::create(nullptr, nullptr);
}

bool MyMenu::onTouchBegan(Touch* touch, Event* event)
{
	startY = touch->getLocation().y;
	auto camera = Camera::getVisitingCamera();
	if (_state != Menu::State::WAITING || !_visible || !_enabled || !camera)
	{
		return false;
	}

	for (Node *c = this->_parent; c != nullptr; c = c->getParent())
	{
		if (c->isVisible() == false)
		{
			return false;
		}
	}

	if (isInTouchLimit(touch))
	{
		return false;
	}
	
	_selectedItem = this->getItemForTouch(touch, camera);
	if (_selectedItem)
	{
		_state = Menu::State::TRACKING_TOUCH;
		_selectedWithCamera = camera;
		_selectedItem->selected();

		return true;
	}
	return false;
}


void MyMenu::onTouchEnded(Touch* touch, Event* event)
{
	//CCASSERT(_state == Menu::State::TRACKING_TOUCH, "[Menu ccTouchEnded] -- invalid state");
	if (_state != Menu::State::TRACKING_TOUCH)
		return;
	if (!m_isdraging)
	{
		this->retain();
		if (_selectedItem)
		{
			_selectedItem->unselected();
			_selectedItem->activate();
		}
		_state = Menu::State::WAITING;
		_selectedWithCamera = nullptr;
		this->release();
	}
	else
	{
		onTouchCancelled(touch, event);
		m_isdraging = false;
	}
}

void MyMenu::onTouchCancelled(Touch* touch, Event* event)
{
	//CCASSERT(_state == Menu::State::TRACKING_TOUCH, "[Menu ccTouchCancelled] -- invalid state");
	if (_state != Menu::State::TRACKING_TOUCH)
		return;
	this->retain();
	if (_selectedItem)
	{
		_selectedItem->unselected();
	}
	_state = Menu::State::WAITING;
	this->release();
}

void MyMenu::onTouchMoved(Touch* touch, Event* event)
{
	//CCASSERT(_state == Menu::State::TRACKING_TOUCH, "[Menu ccTouchMoved] -- invalid state");
	if (_state != Menu::State::TRACKING_TOUCH)
		return;

	if (fabsf(startY - touch->getLocation().y) > 20)
		m_isdraging = true;


	MenuItem *currentItem = this->getItemForTouch(touch, _selectedWithCamera);
	if (currentItem != _selectedItem)
	{
		if (_selectedItem)
		{
			_selectedItem->unselected();
		}
		_selectedItem = currentItem;
		if (_selectedItem)
		{
			_selectedItem->selected();
		}
	}
}
void MyMenu::setTouchlimit(cocos2d::Node *node)
{
	m_szTouchLimitNode = node;
	m_bTouchLimit = true;
}

bool MyMenu::isInTouchLimit(Touch* touch)
{
	if (m_bTouchLimit)
	{
		Vec2 touchLocation = touch->getLocation();

		Vec2 local = m_szTouchLimitNode->convertToNodeSpace(touchLocation);
		Rect r = m_szTouchLimitNode->getBoundingBox();
		r.origin = Vec2::ZERO;

		if (!r.containsPoint(local))
		{
			return true;
		}
	}
	return false;
}