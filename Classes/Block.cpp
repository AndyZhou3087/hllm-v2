#include "Block.h"
#define MAX_ZODER 81
Block::Block()
{
	_block = NULL;
	_frozen = NULL;
	_glock = NULL;
	_slock = NULL;
	_cloud = NULL;
	_backSprite = NULL;
	_prop = NULL;
	_focus = NULL;
	_turn = NULL;
	_blogo = NULL;
	_changemask = false;
}

Block::~Block()
{
}

Block* Block::createWithType(int type, float x, float y, int row, int col, SpecialType stype, SpecialStatus sstatus, MoveRule mr, bool needscale)
{
	Block* pBlock = new Block;
	pBlock->initWithType(type, x, y, row, col, stype, sstatus, mr, needscale);
	pBlock->autorelease();
	return pBlock;
}

Block* Block::initWithType(int type, float x, float y, int row, int col, SpecialType stype, SpecialStatus sstatus, MoveRule mr, bool needscale)
{
	_stype = stype;
	_sstatus = sstatus;
	_mr = mr;
	_type = type;
	_x = x;
	_y = y;
	_row = row;
	_col = col;
	_state = NORMALBLOCK;
	_focusing = false;
	_bp = BP_NONE;
	char filename[25] = { 0 };
	Sprite::initWithSpriteFrameName("blank.png");
	if (stype == B_NOTHING)
	{

	}	
	else
	{
		_block = Sprite::createWithSpriteFrameName("blockbg.png");
		_block->setPosition(this->getContentSize().width / 2, this->getContentSize().height / 2);
		this->addChild(_block);
		if (stype == B_NORMAL || stype == B_COVER)
		{
			sprintf(filename, "%d.png", type);
			_blogo = Sprite::createWithSpriteFrameName(filename);
			_blogo->setPosition(_block->getContentSize().width / 2, _block->getContentSize().height / 2);
			_block->addChild(_blogo);
			if (stype == B_COVER)
			{
				sprintf(filename, "t%c.png", converTolowerCase(stype));
				_cloud = Sprite::createWithSpriteFrameName(filename);
				_cloud->setPosition(this->getContentSize().width / 2, this->getContentSize().height / 2);
				addChild(_cloud);
			}
		}
		else
		{

			if (stype == B_TURN)
			{
				sprintf(filename, "%d.png", type);
				
				_blogo = Sprite::createWithSpriteFrameName(filename);
				_blogo->setPosition(_block->getContentSize().width / 2, _block->getContentSize().height / 2);
				_block->addChild(_blogo);
				_block->setVisible(false);

				sprintf(filename, "t%c.png", converTolowerCase(stype));
				_turn = Sprite::createWithSpriteFrameName(filename);
				_turn->setPosition(this->getContentSize().width / 2, this->getContentSize().height / 2);
				addChild(_turn);
			}
			else
			{
				sprintf(filename, "t%c.png", converTolowerCase(stype));
				_blogo = Sprite::createWithSpriteFrameName(filename);
				_blogo->setPosition(_block->getContentSize().width / 2, _block->getContentSize().height / 2);
				_block->addChild(_blogo);
			}
		}
		_focus = Sprite::createWithSpriteFrameName("select.png");
		_focus->setPosition(this->getContentSize().width / 2, this->getContentSize().height / 2);

		_focus->setVisible(false);

		this->addChild(_focus);

		_backSprite = Sprite::createWithSpriteFrameName("back.png");
		_backSprite->setPosition(this->getContentSize().width / 2, this->getContentSize().height / 2);
		_backSprite->setVisible(false);
		addChild(_backSprite);
	}

	if (sstatus == SS_FROZEN || sstatus == SS_GOLDLOCKFROZEN || sstatus == SS_SILVERLOCKFROZEN)
	{
		_frozen = Sprite::createWithSpriteFrameName("s1.png");
		_frozen->setPosition(this->getContentSize().width / 2, this->getContentSize().height / 2);
		addChild(_frozen);
		_state = DISABLE;
	}
	if (sstatus == SS_GOLDLOCK || sstatus == SS_GOLDLOCKFROZEN)
	{
		_glock = Sprite::createWithSpriteFrameName("s2.png");
		_glock->setPosition(this->getContentSize().width / 2, this->getContentSize().height / 2);
		addChild(_glock);
		_state = DISABLE;
	}

	if (sstatus == SS_SILVERLOCK || sstatus == SS_SILVERLOCKFROZEN)
	{
		_slock = Sprite::createWithSpriteFrameName("s3.png");
		_slock->setPosition(this->getContentSize().width / 2, this->getContentSize().height / 2);
		addChild(_slock);
		_state = DISABLE;
	}
	/*
	this->setScale(0.01f);
	float scale = 1.0f;
	if (needscale)
		scale = 0.9f;
	this->runAction(ScaleTo::create(0.8f, scale));
	*/
	schedule(schedule_selector(Block::update));

	return this;
}

void Block::update(float dt)
{

}

void Block::setFocus(bool value)
{
	if (_focusing == value)
		return;

	if (value)
	{
		_OriZorer = this->getLocalZOrder();
		_focus->setVisible(true);
		_focusing = true;
		this->setLocalZOrder(MAX_ZODER + _OriZorer);
		this->runAction(Sequence::create(EaseSineIn::create(MoveBy::create(0.05f, Vec2(10, -10))), EaseSineIn::create(MoveBy::create(0.08f, Vec2(-10, 10))), NULL));
		
		if (_stype == B_TURN)
		{
			_block->runAction(Spawn::create(Show::create(), FadeIn::create(0), OrbitCamera::create(0.16f, 1, 0, 90, -90, 0, 0), NULL));
			_block->setLocalZOrder(-1);
			_turn->runAction(Sequence::create(DelayTime::create(0.08f), OrbitCamera::create(0.08f, 1, 0, 90, -90, 0, 0), NULL));
			_turn->setLocalZOrder(-2);
		}

		if (isBack())
			hideBack();
	}
	else
	{
		_focusing = false;
		_focus->setVisible(false);
		this->setLocalZOrder(_OriZorer);
		//this->setPosition(Vec2(this->getPositionX() - 4, this->getPositionY() + 4));

		if (_stype == B_TURN)
		{
			_turn->runAction(Sequence::create(Show::create(), FadeIn::create(0), OrbitCamera::create(0.16f, 1, 0, -90, 90, 0, 0), NULL));
			_turn->setLocalZOrder(-1);
			_block->runAction(Sequence::create(DelayTime::create(0.08f), OrbitCamera::create(0.08f, 1, 0, -90, 90, 0, 0), NULL));
			_block->setLocalZOrder(-2);
		}
	}
}

void Block::state(BlockState val)
{ 
	_state = val;

	if (_state == DISABLE)
	{
		if (_stype >= B_ONE && _stype <= B_NINE)
		{
			string str = StringUtils::format("t%cd.png", converTolowerCase(_stype));
			SpriteFrame* disableSprite = SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
			_blogo->setSpriteFrame(disableSprite);
		}
	}
	else if (_state == NORMALBLOCK)
	{
		if (_stype >= B_ONE && _stype <= B_NINE)
		{
			string str = StringUtils::format("t%c.png", converTolowerCase(_stype));
			SpriteFrame* disableSprite = SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
			_blogo->setSpriteFrame(disableSprite);
		}
	}
	else
	{
		if (_cloud != NULL)
			_cloud->setVisible(false);
		
	}
}

void Block::changeType(int type)
{
	_type = type;
	string str = StringUtils::format("%d.png", type);
	_blogo->setSpriteFrame(str);
}

void Block::changeSpecialType(SpecialType stype, int type)
{
	_stype = stype;
	string str = StringUtils::format("t%c.png", converTolowerCase(stype));

	char filename[25] = { 0 };
	if (_block == NULL)
	{
		_block = Sprite::createWithSpriteFrameName("blockbg.png");
		_block->setPosition(this->getContentSize().width / 2, this->getContentSize().height / 2);
		this->addChild(_block);
	}
	if (_blogo == NULL)
	{
		if (type >= 0)
		{
			sprintf(filename, "%d.png", type);
			_blogo = Sprite::createWithSpriteFrameName(filename);
			_blogo->setPosition(this->getContentSize().width / 2, this->getContentSize().height / 2);
			_block->addChild(_blogo);
		}
	}
	if (_focus == NULL)
	{
		_focus = Sprite::createWithSpriteFrameName("select.png");
		_focus->setPosition(this->getContentSize().width / 2, this->getContentSize().height / 2);
		_focus->setVisible(false);
		this->addChild(_focus);
	}

	if (_backSprite == NULL)
	{
		_backSprite = Sprite::createWithSpriteFrameName("back.png");
		_backSprite->setPosition(this->getContentSize().width / 2, this->getContentSize().height / 2);
		_backSprite->setVisible(false);
		addChild(_backSprite);
	}
	if (_stype == B_COVER)
	{
		if (_cloud == NULL)
		{
			_cloud = Sprite::createWithSpriteFrameName(str);
			_cloud->setPosition(this->getContentSize().width / 2, this->getContentSize().height / 2);
			addChild(_cloud);

			_type = type;
			sprintf(filename, "%d.png", type);
			_blogo->setSpriteFrame(filename);
		}
	}
	else if (_stype == B_TURN)
	{
		_type = type;
		sprintf(filename, "%d.png", type);
		if (_turn == NULL)
		{
			_turn = Sprite::createWithSpriteFrameName(str);
			_turn->setPosition(this->getContentSize().width / 2, this->getContentSize().height / 2);
			_turn->setVisible(true);
			addChild(_turn);
		}
		_blogo->setSpriteFrame(filename);
		_block->setVisible(false);
	}
	else if (_stype == B_NORMAL)
	{
		_type = type;
		sprintf(filename, "%d.png", type);
		_blogo->setSpriteFrame(filename);
	}
	else
	{
		_type = -1;
		_blogo->setSpriteFrame(str);
	}
}

void Block::sstate(SpecialStatus val)
{
	_sstatus = val;
	if (val == SS_FROZEN)
	{
		if (_frozen != NULL)
		{
			_frozen->runAction(Sequence::create(FadeIn::create(0.02f), Show::create(), NULL));
			_state = DISABLE;
		}
		else
		{
			_frozen = Sprite::createWithSpriteFrameName("s1.png");
			_frozen->setPosition(this->getContentSize().width / 2, this->getContentSize().height / 2);
			addChild(_frozen);
			_state = DISABLE;
		}
		if (_glock != NULL)
		{
			_glock->runAction(Sequence::create(FadeOut::create(0.2f), Hide::create(), NULL));
			//_glock->setVisible(false);
		}
		if (_slock != NULL)
		{
			_slock->runAction(Sequence::create(FadeOut::create(0.2f), Hide::create(), NULL));
			//_slock->setVisible(false);
		}
	}
	else if (val == SS_GOLDLOCK)
	{
		if (_frozen != NULL)
		{
			_frozen->runAction(Sequence::create(FadeOut::create(0.2f), Hide::create(), NULL));
			//_frozen->setVisible(false);
		}
		if (_slock != NULL)
		{
			_slock->runAction(Sequence::create(FadeOut::create(0.2f), Hide::create(), NULL));
			//_slock->setVisible(false);
		}

		if (_glock != NULL)
		{
			_glock->runAction(Sequence::create(FadeIn::create(0.02f), Show::create(), NULL));
			_state = DISABLE;
		}
		else
		{
			_glock = Sprite::createWithSpriteFrameName("s2.png");
			_glock->setPosition(this->getContentSize().width / 2, this->getContentSize().height / 2);
			addChild(_glock);
			_state = DISABLE;
		}

	}
	else if (val == SS_SILVERLOCK)
	{
		if (_frozen != NULL)
		{
			_frozen->runAction(Sequence::create(FadeOut::create(0.2f), Hide::create(), NULL));
			//_frozen->setVisible(false);
		}
		if (_glock != NULL)
		{
			_glock->runAction(Sequence::create(FadeOut::create(0.2f), Hide::create(), NULL));
			//_glock->setVisible(false);
		}
		if (_slock != NULL)
		{
			_slock->runAction(Sequence::create(FadeIn::create(0.02f), Show::create(), NULL));
			_state = DISABLE;
		}
		else
		{
			_slock = Sprite::createWithSpriteFrameName("s3.png");
			_slock->setPosition(this->getContentSize().width / 2, this->getContentSize().height / 2);
			addChild(_slock);
			_state = DISABLE;
		}
	}
	else if (val == SS_NORMAL)
	{
		if (_frozen != NULL)
		{
			_frozen->runAction(Sequence::create(FadeOut::create(0.2f), Hide::create(), NULL));
			//_frozen->setVisible(false);
		}
		if (_glock != NULL)
		{
			_glock->runAction(Sequence::create(FadeOut::create(0.2f), Hide::create(), NULL));
			//_glock->setVisible(false);
		}
		if (_slock != NULL)
		{
			_slock->runAction(Sequence::create(FadeOut::create(0.2f), Hide::create(), NULL));
			//_slock->setVisible(false);
		}
		_state = NORMALBLOCK;
	}
}
void Block::unCoverCloud()
{
	if (_cloud != NULL)
	{
		_cloud->runAction(Sequence::create(FadeOut::create(0.5f), CCDelayTime::create(2), FadeIn::create(0.5f), NULL));
	}
}
void Block::showBack()
{
	_backSprite->runAction(Sequence::create(Show::create(), FadeIn::create(0), OrbitCamera::create(0.2f, 1, 0, -90, 90, 0, 0), NULL));
	_block->runAction(Sequence::create(DelayTime::create(0.1f), OrbitCamera::create(0.1f, 1, 0, -90, 90, 0, 0), Hide::create(), NULL));
}

void Block::hideBack()
{
	if (_backSprite != NULL && _backSprite->isVisible())
	{
		_block->runAction(Sequence::create(Show::create(), FadeIn::create(0), OrbitCamera::create(0.2f, 1, 0, -90, 90, 0, 0), NULL));
		_backSprite->runAction(Sequence::create(/*DelayTime::create(0.1f), OrbitCamera::create(0.1f, 1, 0, -90, 90, 0, 0), */Hide::create(), NULL));
	}
}

bool Block::isBack()
{
	if (_backSprite != NULL && _backSprite->isVisible())
		return true;
	return false;
}

void Block::addProp(BlockProp bp)
{
	if (_prop != NULL)
	{
		_prop->removeFromParentAndCleanup(true);
		_prop = NULL;
	}
	_bp = bp;
	std::string bpstr = StringUtils::format("bp%d.png", bp);
	_prop = Sprite::createWithSpriteFrameName(bpstr);
	_prop->setAnchorPoint(Vec2(1, 0));
	_prop->setPosition(Vec2(this->getContentSize().width, 10));
	_block->addChild(_prop);

}

void Block::removeProp()
{
	_bp = BP_NONE;
	if (_prop != NULL)
	{
		_prop->removeFromParentAndCleanup(true);
		_prop = NULL;
	}
}

bool Block::hasProp()
{
	return _prop == NULL ? false : true;
}

BlockProp Block::prop()
{
	return _bp;
}

void Block::addGray()
{
	GLProgram * p = new GLProgram();
	p->initWithFilenames("gray.vsh", "gray.fsh");
	p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_POSITION, GLProgram::VERTEX_ATTRIB_POSITION);
	p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_COLOR, GLProgram::VERTEX_ATTRIB_COLOR);
	p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_TEX_COORD, GLProgram::VERTEX_ATTRIB_TEX_COORDS);
	p->link();
	p->updateUniforms();
	if (_stype == B_TURN)
		_turn->setGLProgram(p);
	_block->setGLProgram(p);
}

void Block::removeGray()
{
	std::string str = GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP;//ShaderPositionTextureColor_noMVP;
	GLProgram * pProgram = ShaderCache::getInstance()->getGLProgram(str);
	if (_stype == B_TURN)
		_turn->setGLProgram(pProgram);
	_block->setGLProgram(pProgram);

	CHECK_GL_ERROR_DEBUG();

}

void Block::runFadeIn(float dt)
{
	if (_stype == B_TURN)
	{
		if (_turn != NULL)
		{
			_turn->setOpacity(0);
			_turn->runAction(Sequence::create(DelayTime::create(0.2f), Show::create(), Spawn::create(FadeIn::create(dt), TintTo::create(dt, Color3B(200, 200, 200)), NULL), TintTo::create(0.05f, Color3B(255, 255, 255)), NULL));
		}
		if (_block != NULL)
		{
			_block->setOpacity(0);
			_block->runAction(Sequence::create(DelayTime::create(0.2f), Show::create(), Spawn::create(FadeIn::create(dt), TintTo::create(dt, Color3B(200, 200, 200)), NULL), TintTo::create(0.05f, Color3B(255, 255, 255)), NULL));
			if (_blogo != NULL)
			{
				_blogo->setOpacity(0);
				_blogo->runAction(Sequence::create(DelayTime::create(0.2f), Show::create(), Spawn::create(FadeIn::create(dt), TintTo::create(dt, Color3B(200, 200, 200)), NULL), TintTo::create(0.05f, Color3B(255, 255, 255)), NULL));
			}

			if (_prop != NULL)
			{
				_prop->setOpacity(0);
				_prop->runAction(Sequence::create(DelayTime::create(0.2f), Show::create(), Spawn::create(FadeIn::create(dt), TintTo::create(dt, Color3B(200, 200, 200)), NULL), TintTo::create(0.05f, Color3B(255, 255, 255)), NULL));
			}

		}
	}
	else
	{
		if (_block != NULL)
		{
			_block->setOpacity(0);
			_block->runAction(Sequence::create(DelayTime::create(0.2f), Show::create(), Spawn::create(FadeIn::create(dt), TintTo::create(dt, Color3B(200, 200, 200)), NULL), TintTo::create(0.05f, Color3B(255, 255, 255)), NULL));
			if (_blogo != NULL)
			{
				_blogo->setOpacity(0);
				_blogo->runAction(Sequence::create(DelayTime::create(0.2f), Show::create(), Spawn::create(FadeIn::create(dt), TintTo::create(dt, Color3B(200, 200, 200)), NULL), TintTo::create(0.05f, Color3B(255, 255, 255)), NULL));
			}
			if (_prop != NULL)
			{
				_prop->setOpacity(0);
				_prop->runAction(Sequence::create(DelayTime::create(0.2f), Show::create(), Spawn::create(FadeIn::create(dt), TintTo::create(dt, Color3B(200, 200, 200)), NULL), TintTo::create(0.05f, Color3B(255, 255, 255)), NULL));
			}
		}
	}
}

void Block::setChangeMask(bool val)
{
	_changemask = val;
}

bool Block::getChangeMask()
{
	return _changemask;
}

void Block::reset()
{
	if (_block != NULL)
	{
		this->removeChildByName("promptselect");
		_block->stopAllActions();
		_block->setPosition(this->getContentSize().width / 2, this->getContentSize().height / 2);
	}
}
void Block::runUpAndDown()
{
	if (_block != NULL)
	{
		_block->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.5f, Vec2(0, 5)), MoveBy::create(0.5f, Vec2(0, -5)), NULL)));
		Sprite* promptSelect = Sprite::createWithSpriteFrameName("select.png");
		promptSelect->setPosition(this->getContentSize().width / 2, this->getContentSize().height / 2);
		this->addChild(promptSelect, 1, "promptselect");
	}
}

char Block::converTolowerCase(int val)
{
	if (val >= 10)
		return val - 10 + 'a';
	else
		return val + '0';
}
