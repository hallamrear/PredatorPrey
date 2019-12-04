#pragma once
#include "SDL.h"

class Vector2;

enum CELL_TYPE
{
	CELL_TYPE_NONE = 0,
	CELL_TYPE_PREDATOR,
	CELL_TYPE_PREY
};

class Cell
{
private:
	CELL_TYPE mType;
	SDL_Rect mRect;

public:
	Cell(Vector2 size, Vector2 pos, CELL_TYPE type);
	~Cell();

	void Update();
	void Draw(SDL_Renderer& renderer);
};

