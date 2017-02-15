#ifndef __MYMENU_H_
#define __MYMENU_H_
#include "cocos2d.h"
USING_NS_CC;
class MyMenu:public Menu
{
public:
    bool init();
 
    /** initializes a Menu with a NSArray of MenuItem objects */
    bool initWithArray(const Vector<MenuItem*>& arrayOfItems);
	static MyMenu* createWithArray(const Vector<MenuItem*>& arrayOfItems);
    static MyMenu* createWithItem(MenuItem* item);
	static MyMenu* create();
    /** creates a Menu with MenuItem objects */
    static MyMenu* createWithItems(MenuItem *firstItem, va_list args);
	static MyMenu* create(MenuItem* item, ...)	CC_REQUIRES_NULL_TERMINATION;
private:
 
};
#endif
