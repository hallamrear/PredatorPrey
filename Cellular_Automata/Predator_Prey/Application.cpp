#include "Application.h"
#include "Cell.h"

#include <iostream>

int main(int argc, char* argv[] )
{
	Application* app;
	app = new Application();

	app->Start();

	while (app->IsRunning())
	{	
		app->PollEvents();
		app->Update();
		app->Draw();
	} 

	delete app;
	app = nullptr;

	return 0;
}

void Application::InitWindow()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		Uint32 windowFlags = SDL_WINDOW_SHOWN;
		std::string title = "Cellular Automata - Predator and prey.";
		mWindow = SDL_CreateWindow(&title[0], SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, windowFlags);
		mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_PRESENTVSYNC);
		InitGUI();
	}
}

void Application::InitGUI()
{
	//IMGUI_CHECKVERSION();
	//ImGui::CreateContext();
	//ImGui_ImplSDL2_InitForOpenGL(mWindow);
	//ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.WantTextInput = true;
	//io.WantCaptureMouse = true;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	//ImGui::StyleColorsDark();
}

void Application::CreateObjects()
{
	Vector2 cellsize;
	cellsize.x = WINDOW_WIDTH / CELL_COUNT_X;
	cellsize.y = WINDOW_HEIGHT / CELL_COUNT_Y;

	CELL_TYPE type;

	mCells = new Cell**[CELL_COUNT_X];
	for (int x = 0; x < CELL_COUNT_X; x++)
	{
		mCells[x] = new Cell*[CELL_COUNT_Y];
		for (int y = 0; y < CELL_COUNT_Y; y++)
		{
			int typeID = (rand() % 3) + 1;
			if (typeID == 2)
				type = CELL_TYPE_PREY;
			else
				if(typeID == 1)
				type = CELL_TYPE_PREDATOR;
			else
				type = CELL_TYPE_NONE;

			mCells[x][y] = new Cell(cellsize, Vector2(cellsize.x * x, cellsize.y * y), type);
		}
	}
}

void Application::Cleanup()
{
	for(int x = CELL_COUNT_X - 1; x > 0; x--)
	{
		for (int y = CELL_COUNT_Y - 1; y > 0; y--)
		{
			delete mCells[x][y];
			mCells[x][y] = nullptr;			
		}

		delete mCells[x];
		mCells[x] = nullptr;
	}
	delete mCells;
	mCells = nullptr;

	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();

	bIsRunning = false;
}


Application::Application()
{
	bIsRunning = false;
}

Application::~Application()
{
	Cleanup();
}

void Application::Start()
{
	if (!bIsRunning)
	{
		InitWindow();
		CreateObjects();
		bIsRunning = true;
	}
}

void Application::Stop()
{
	if (bIsRunning)
	{
		bIsRunning = false;
	}
}

bool Application::IsRunning()
{
	return bIsRunning;
}

void Application::PollEvents()
{
	SDL_Event e;
	SDL_PollEvent(&e);

	switch (e.type)
	{
	case SDL_QUIT:
		Stop();
		break;

	default:
		break;
	}
}

void Application::Update()
{
	for (int x = 0; x < CELL_COUNT_X; x++)
		for (int y = 0; y < CELL_COUNT_Y; y++)
			mCells[x][y]->Update();

	Update_UI();
}

void Application::Update_UI()
{

}

void Application::Draw()
{
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
	SDL_RenderClear(mRenderer);

	for (int x = 0; x < CELL_COUNT_X; x++)
		for (int y = 0; y < CELL_COUNT_Y; y++)
			mCells[x][y]->Draw(*mRenderer);

	Draw_UI();
	SDL_RenderPresent(mRenderer);
}

void Application::Draw_UI()
{

}