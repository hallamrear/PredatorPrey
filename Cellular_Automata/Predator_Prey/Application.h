#pragma once

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

#include <Windows.h>
#include "SDL.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "Vector2.h"

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 800
#define CELL_COUNT_X  200
#define CELL_COUNT_Y  200
#define CELL_RECT_WIDTH (WINDOW_WIDTH / CELL_COUNT_X)
#define CELL_RECT_HEIGHT (WINDOW_HEIGHT / CELL_COUNT_Y)

class Application
{
private:
	int								mGenerationCount;
	int								mPredCount;
	int								mPreyCount;
	bool							bIsRunning;
	bool							bIsPaused;
	SDL_Window*						mWindow;
	SDL_Renderer*					mRenderer;
	class Cell***					mCells;

	void InitWindow();
	void InitGUI();
	void InitConsole();
	void CreateObjects();
	void Cleanup();

	void ClearCells();

	void Update_UI();
	void Draw_UI();

public:
	Application();
	~Application();

	void Start();
	void Stop();

	void PollEvents();
	void Update();
	void Draw();

	bool IsRunning();
};

