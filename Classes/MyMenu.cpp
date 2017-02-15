#include "MyMenu.h"
bool MyMenu::init()
{
	return initWithArray(Vector<MenuItem*>());
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
