#include "Cell.h"
#include "Vector2.h"


#include <random>

Cell::Cell(Vector2 size, Vector2 pos, CELL_TYPE type = CELL_TYPE_NONE)
{
	mRect.w = size.x;
	mRect.h = size.y;
	mRect.x = pos.x;
	mRect.y = pos.y;

	mType = type;
}

Cell::~Cell()
{
	mType = CELL_TYPE_NONE;
}

void Cell::Update()
{

}

void Cell::Draw(SDL_Renderer& renderer)
{
	switch (mType)
	{
	case CELL_TYPE_NONE:
		SDL_SetRenderDrawColor(&renderer, 0, 0, 0, 255);
		break;

	case CELL_TYPE_PREDATOR:
		SDL_SetRenderDrawColor(&renderer, 255, 0, 0, 255);
		break;

	case CELL_TYPE_PREY:
		SDL_SetRenderDrawColor(&renderer, 0, 255, 0, 255);
		break;
	}
	SDL_RenderFillRect(&renderer, &mRect);
}
