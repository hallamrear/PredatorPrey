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

#include "SDL.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

class Application
{
private:
	bool					bIsRunning;
	SDL_Window*				mWindow;
	SDL_Renderer*			mRenderer;

	void CreateWindow();
	void CreateGUI();

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

	const bool const IsRunning();
};

