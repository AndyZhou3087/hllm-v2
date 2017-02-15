#include "Line.h"

Line::Line()
{
}

Line::~Line()
{
}

Line* Line::createWithType(LineType type, float x, float y, int row, int col)
{
	Line* pLine = new Line;
	pLine->initWithType(type, x, y, row, col);
	pLine->autorelease();
	return pLine;
}

Line* Line::initWithType(LineType type, float x, float y, int row, int col)
{
	char filename[25] = { 0 };
	
	if (type == LINE_LEFT || type == LINE_RIGHT)
		strcpy(filename, "heng1.png");
	else
		strcpy(filename, "shu1.png");
	Sprite::initWithSpriteFrameName(filename);
	
	_type = type;
	_x = x;
	_y = y;
	_row = row;
	_col = col;

	schedule(schedule_selector(Line::update));

	return this;
}

void Line::update(float dt)
{
}

void Line::StartRun()
{
	Vector<SpriteFrame*> vec;
	SpriteFrame* frame;
	string frameStr;
	for (int i = 1; i <= 6; i++)
	{
		if (_type == LINE_LEFT || _type == LINE_RIGHT)
			frameStr = StringUtils::format("heng%d.png", i);
		else
			frameStr = StringUtils::format("shu%d.png", i);
		frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameStr);
		vec.pushBack(frame);
	}
	Animation *animation = Animation::createWithSpriteFrames(vec, 0.08f);
	animation->setRestoreOriginalFrame(true);
	Animate *animate = Animate::create(animation);
	this->setVisible(true);
	this->runAction(Sequence::create(animate, CallFunc::create(CC_CALLBACK_0(Line::HideMe, this)), NULL));
	//this->runAction(Sequence::create(DelayTime::create(0.6f), CallFunc::create(this, callfunc_selector(Line::HideMe)), NULL));
}

void Line::HideMe()
{
	this->setVisible(false);
}
