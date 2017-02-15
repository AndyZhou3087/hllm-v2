#pragma once
#ifndef __BLOCK_H__
#define __BLOCK_H__
#include "cocos2d.h"

USING_NS_CC;
using namespace std;
typedef enum
{
	NORMALBLOCK,
	DESTROID,
	DISABLE
}BlockState;

typedef enum
{
	BP_NONE = -1,
	BP_BOMB,
	BP_X2,
	BP_ADDTIME,
	BP_CTOGOLD
}BlockProp;

typedef enum
{
	B_NOTHING,
	B_NORMAL,
	B_UNAVAILABLE,
	B_RHOMBICBARRIER,
	B_HEXAGONBARRIER,
	B_RHOMBIC,
	B_HEXAGON,
	B_TURN,
	B_GOLDKEY,
	B_SILVERKEY,
	B_ONE,
	B_TWO,
	B_THREE,
	B_FOUR,
	B_FIVE,
	B_SIX,
	B_SEVEN,
	B_EIGHT,
	B_NINE,
	B_COVER,
	B_COIN,
	B_BOMB
}SpecialType;

typedef enum
{
	SS_NORMAL,
	SS_FROZEN,
	SS_GOLDLOCK,
	SS_SILVERLOCK,
	SS_GOLDLOCKFROZEN,
	SS_SILVERLOCKFROZEN
}SpecialStatus;

typedef enum
{
	M_STOP,
	M_UP,
	M_DOWN,
	M_LEFT,
	M_RIGHT
}MoveRule;

class Block : public Sprite
{
public:
	Block();
	~Block();

	static Block* createWithType(int type, float x, float y, int row, int col, SpecialType stype, SpecialStatus sstatus, MoveRule mr, bool needscale);
	Block* initWithType(int type, float x, float y, int row, int col, SpecialType stype, SpecialStatus sstatus, MoveRule mr, bool needscale);

	void update(float dt);
	float x(){ return _x; }
	void x(float val){ _x = val; }
	float y(){ return _y; }
	void y(float val){ _y = val; }
	int type(){ return _type; }
    void type(int val){ _type = val; }
	int row(){ return _row; }
	void row(int val){ _row = val; }
	int col(){ return _col; }
	void col(int val){ _col = val; }
	BlockState state(){ return _state; }
	void state(BlockState val);
	SpecialType stype(){ return _stype; }
	void stype(SpecialType val){ _stype = val; }
	SpecialStatus sstate(){ return _sstatus; }
	void sstate(SpecialStatus val);
	MoveRule moverule(){ return _mr; }
	void setFocus(bool value);
	void unCoverCloud();
	void changeType(int type);
	void changeSpecialType(SpecialType stype, int type = -1);
	void showBack();
	void hideBack();
	void changeToTurn(int type);
	bool isBack();
	void addGray();
	void removeGray();
	void addProp(BlockProp bp);
	void removeProp();
	bool hasProp();
	void setChangeMask(bool val);
	bool getChangeMask();
	BlockProp prop();
	void runFadeIn(float dt);

	void reset();
	void runUpAndDown();
private:
	char converTolowerCase(int val);
private:
	float _x;
	float _y;
	float _newx;
	float _newy;
	int _type;
	int _row;
	int _col;
	BlockState _state;
	Sprite * _block;
	Sprite * _blogo;
	Sprite * _turn;
	Sprite * _focus;
	bool _focusing;
	Animate *_focusanimate;
	SpecialType _stype;
	SpecialStatus _sstatus;
	MoveRule _mr;
	Sprite* _frozen;
	Sprite* _glock;
	Sprite* _slock;
	Sprite* _cloud;
	Sprite* _backSprite;
	Sprite* _prop;
	BlockProp _bp;
	int _OriZorer;
	bool _changemask;
public:
};



#endif