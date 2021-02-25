#pragma once
#include "SDL.h"

#define MAX_HEALTH 10

class Vector2;

enum CELL_DIRECTION
{
	CELL_DIRECTION_UP = 0,
	CELL_DIRECTION_DOWN = 1,
	CELL_DIRECTION_LEFT = 2,
	CELL_DIRECTION_RIGHT = 3,
};

enum CELL_TYPE
{
	CELL_TYPE_NONE = 0,
	CELL_TYPE_PREDATOR = 1,
	CELL_TYPE_PREY = 2
};

class Cell
{
private:
	CELL_DIRECTION		mDirection;
	CELL_TYPE			mType;
	SDL_Rect			mRect;
	int					mHealth;
	bool				mIsReproducing;

public:
	Cell(Vector2 size, Vector2 pos, CELL_TYPE type);
	Cell(const Cell & cell);
	~Cell();

	void				CreateCell(CELL_TYPE type, int health);
	void				SetRandomDirection();
	void				CompareCells(Cell* victimCell);

	SDL_Rect			GetRect();
	void				SetHealth(int health);
	int					GetHealth();
	bool				IsReproducing();
	void				SetIsReproducing(bool state);
	CELL_TYPE			GetType();
	void				SetType(CELL_TYPE);
	CELL_DIRECTION		GetDirection();
	void				SetDirection(CELL_DIRECTION);

	void				Heal(int amount);
	void				Update();
	void				Draw(SDL_Renderer& renderer);
};

