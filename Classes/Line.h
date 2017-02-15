#pragma once
#ifndef __LINE_H__
#define __LINE_H__
#include "cocos2d.h"

USING_NS_CC;
using namespace std;

typedef enum
{
	LINE_LEFT = 0,
	LINE_RIGHT,
	LINE_TOP,
	LINE_BOTTOM
}LineType;

class Line : public Sprite
{
public:
	Line();
	~Line();

	static Line* createWithType(LineType type, float x, float y, int row, int col);
	Line* initWithType(LineType type, float x, float y, int row, int col);

	void update(float dt);
    
	float x(){ return _x; }
	void x(float val){ _x = val; }
	float y(){ return _y; }
	void y(float val){ _y = val; }
	int row(){ return _row; }
	void row(int val){ _row = val; }
	int col(){ return _col; }
	void col(int val){ _col = val; }
	LineType type(){ return _type; }

	void StartRun();

private:
	void HideMe();

private:
	float _x;
	float _y;
	int _row;
	int _col;
	LineType _type;

public:
};



#endif