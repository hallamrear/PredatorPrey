#include "Cell.h"
#include "Vector2.h"
#include <random>

//PREDATOR
//PREY
//NOTHING
//move randomly
//certain health
//prey health increases over time
//reproduces at max and sets to 0
//predator health decreases
//dies at 0
//if predator goes onto prey they reproduce
//heals using prey health


Cell::Cell(Vector2 size, Vector2 pos, CELL_TYPE type = CELL_TYPE_NONE)
{
	mRect.w = (int)size.x - 1;
	mRect.h = (int)size.y - 1;
	mRect.x = (int)pos.x + 1;
	mRect.y = (int)pos.y + 1;
	CreateCell(type, MAX_HEALTH / 2);
}

Cell::Cell(const Cell &cell)
{
	mDirection = cell.mDirection;
	mType = cell.mType;
	mHealth = cell.mHealth;
	mIsReproducing = cell.mIsReproducing;
}

Cell::~Cell()
{
	mType = CELL_TYPE_NONE;
}

void Cell::CreateCell(CELL_TYPE type, int health)
{
	mIsReproducing = false;
	mType = type;
	mHealth = health;
	SetRandomDirection();
}

void Cell::SetRandomDirection()
{
	int random = (rand() % 4);
	if (random == 0)
		mDirection = CELL_DIRECTION_UP;
	else if (random == 1)
		mDirection = CELL_DIRECTION_DOWN;
	else if (random == 2)
		mDirection = CELL_DIRECTION_LEFT;
	else if (random == 3)
		mDirection = CELL_DIRECTION_RIGHT;
}

void Cell::CompareCells(Cell * victimCell)
{
	if (victimCell->GetType() == mType)
		return;

	switch (mType)
	{
	case CELL_TYPE_NONE:
		return;
		break;
	case CELL_TYPE_PREDATOR:
		if (victimCell->GetType() == CELL_TYPE_PREY)
		{
			victimCell->SetType(CELL_TYPE_NONE);
			victimCell->CreateCell(CELL_TYPE_PREDATOR, mHealth);
			Heal(victimCell->GetHealth());
		}
		else if (victimCell->GetType() == CELL_TYPE_NONE)
		{
			victimCell->SetType(mType);
			victimCell->SetDirection(mDirection);
			victimCell->SetHealth(mHealth);
			victimCell->SetIsReproducing(mIsReproducing);

			mType = CELL_TYPE_NONE;
			mHealth = 0;
			mIsReproducing = false;
		}
		else
		{
			SetRandomDirection();
			return;

		}
		break;
	case CELL_TYPE_PREY:
		if (victimCell->GetType() == CELL_TYPE_PREDATOR)
		{
			victimCell->Heal(mHealth);
			mHealth = 0;
			mType = CELL_TYPE_NONE;
		}
		else if (victimCell->GetType() == CELL_TYPE_NONE)
		{
			if (mIsReproducing)
			{
				victimCell->CreateCell(CELL_TYPE_PREY, 10);
				mHealth = 1;
				mIsReproducing = false;
			}
			else
			{
				victimCell->SetType(mType);
				victimCell->SetDirection(mDirection);
				victimCell->SetHealth(mHealth);
				victimCell->SetIsReproducing(mIsReproducing);

				mType = CELL_TYPE_NONE;
				mHealth = 0;
				mIsReproducing = false;
			}
		}
		else
			return;
		break;
	default:
		break;
	}
}

SDL_Rect Cell::GetRect()
{
	return mRect;
}

void Cell::SetHealth(int health)
{
	mHealth = health;
}

int Cell::GetHealth()
{
	return mHealth;
}

bool Cell::IsReproducing()
{
	return mIsReproducing;
}

void Cell::SetIsReproducing(bool state)
{
	mIsReproducing = state;
}

CELL_TYPE Cell::GetType()
{
	return mType;
}

void Cell::SetType(CELL_TYPE type)
{
	mType = type;
}

CELL_DIRECTION Cell::GetDirection()
{
	return mDirection;
}

void Cell::SetDirection(CELL_DIRECTION direction)
{
	mDirection = direction;
}

void Cell::Heal(int amount)
{
	mHealth += amount;
	mHealth = std::min(mHealth, MAX_HEALTH);
}

void Cell::Update()
{
	if (mType == CELL_TYPE_NONE)
		mHealth = 0;

	switch (mType)
	{
	case CELL_TYPE::CELL_TYPE_PREDATOR:
		Heal(-1);

		if (mHealth == 0)
		{
			mType = CELL_TYPE_NONE;
		}

		break;
	case CELL_TYPE::CELL_TYPE_PREY:
		Heal(1);	

		if (mHealth >= MAX_HEALTH)
		{
			mIsReproducing = true;
		}

		break;
	}

	SetRandomDirection();
}

void Cell::Draw(SDL_Renderer& renderer)
{
	float normalisedHealth = (float)mHealth / (float)MAX_HEALTH; 
	uint8_t col = uint8_t((normalisedHealth * 255));
	col = 255;

	switch (mType)
	{
	case CELL_TYPE_NONE:
		SDL_SetRenderDrawColor(&renderer, 0, 0, 0, 255);
		SDL_RenderFillRect(&renderer, &mRect);
		break;
	case CELL_TYPE_PREDATOR:
		SDL_SetRenderDrawColor(&renderer, col, 0, 0, 255);
		SDL_RenderFillRect(&renderer, &mRect);
		break;

	case CELL_TYPE_PREY:
		SDL_SetRenderDrawColor(&renderer, 0, col, 0, 255);
		SDL_RenderFillRect(&renderer, &mRect);
		break;
	}

	//Think putting this in the application render is faster?
	//SDL_RenderFlush(&renderer);
}
