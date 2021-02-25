#include "Application.h"
#include "Cell.h"

#include <iostream>
#include <timeapi.h>

int main(int argc, char* argv[] )
{
	Application* app;
	app = new Application();

	DWORD preTime, postTime, dt;
	preTime = GetTickCount();
	postTime = 0;
	dt = 0;

	app->Start();

	while (app->IsRunning())
	{	
		preTime = GetTickCount();

		app->PollEvents();
		app->Update();
		app->Draw();
		
		postTime = GetTickCount();
		dt = postTime - preTime;

		//16.6 ms per frame would be 60fps
		//so we cap it at just under that
		if (dt < (DWORD)(17))
			Sleep(17 - dt);
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
		std::string title = "Predator and prey.";
		mWindow = SDL_CreateWindow(&title[0], SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, windowFlags);
		mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_SOFTWARE);
		InitGUI();
		//InitConsole();
	}
}

void Application::InitGUI()
{
	/*
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplSDL2_InitForOpenGL(mWindow);
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.WantTextInput = true;
	io.WantCaptureMouse = true;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	ImGui::StyleColorsDark();
	*/
}

void Application::InitConsole()
{
	///*
	if (AllocConsole())
	{
		FILE *fDummy;
		freopen_s(&fDummy, "CONIN$", "r", stdin);
		freopen_s(&fDummy, "CONOUT$", "w", stderr);
		freopen_s(&fDummy, "CONOUT$", "w", stdout);
	}
	//*/
}

void Application::CreateObjects()
{
	mGenerationCount = 0;

	Vector2 cellsize;
	cellsize.x = CELL_RECT_WIDTH;
	cellsize.y = CELL_RECT_HEIGHT;

	mCells = new Cell**[CELL_COUNT_X];
	for (int x = 0; x < CELL_COUNT_X; x++)
	{
		mCells[x] = new Cell*[CELL_COUNT_Y];
		for (int y = 0; y < CELL_COUNT_Y; y++)
		{
			mCells[x][y] = new Cell(cellsize, Vector2(cellsize.x * x, cellsize.y * y), CELL_TYPE_NONE);
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

void Application::ClearCells()
{
	for (int i = 0; i < CELL_COUNT_X; i++)
		for (int j = 0; j < CELL_COUNT_Y; j++)
			mCells[i][j]->CreateCell(CELL_TYPE_NONE, 0);
}

Application::Application()
{
	bIsRunning = false;
	bIsPaused = true;
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

	int mouseX = 0, mouseY = 0;

	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
		case SDL_MOUSEMOTION:
		case SDL_MOUSEBUTTONDOWN:
			SDL_GetMouseState(&mouseX, &mouseY);
			if (mouseX < WINDOW_WIDTH && mouseY < WINDOW_HEIGHT)
			{
				if (e.button.button == SDL_BUTTON_LEFT) 
				{
					mCells[mouseX / CELL_RECT_WIDTH][mouseY / CELL_RECT_HEIGHT]->CreateCell(CELL_TYPE_PREY, MAX_HEALTH / 5);
				}
				else if (e.button.button == SDL_BUTTON_RIGHT)
				{
					mCells[mouseX / CELL_RECT_WIDTH][mouseY / CELL_RECT_HEIGHT]->CreateCell(CELL_TYPE_PREDATOR, MAX_HEALTH / 5);
				}
				else if (e.button.button == SDL_BUTTON_MIDDLE)
				{
					mCells[mouseX / CELL_RECT_WIDTH][mouseY / CELL_RECT_HEIGHT]->CreateCell(CELL_TYPE_NONE, 0);
				}
			}		
			break;

		case SDL_KEYDOWN:

			if (e.key.keysym.sym == SDLK_c)
			{
				ClearCells();
			}

			if (e.key.keysym.sym == SDLK_SPACE)
			{
				bIsPaused = !bIsPaused;
			}

			if (e.key.keysym.sym == SDLK_ESCAPE)
				Stop();
			break;

		case SDL_QUIT:
			Stop();
			break;

		default:
			break;
		}
	}
}

void Application::Update()
{
	if (!bIsPaused)
	{
		mPredCount = 0;
		mPreyCount = 0;

		for (int x = 1; x < CELL_COUNT_X - 1; x++)
		{
			for (int y = 1; y < CELL_COUNT_Y - 1; y++)
			{
				if (mCells[x][y]->GetType() == CELL_TYPE_PREDATOR)
					mPredCount++;
				else if (mCells[x][y]->GetType() == CELL_TYPE_PREY)
					mPreyCount++;
				else
					continue;

				mCells[x][y]->Update();

				switch (mCells[x][y]->GetDirection())
				{
				case CELL_DIRECTION_UP:
					mCells[x][y]->CompareCells(mCells[x][y - 1]);
					break;

				case CELL_DIRECTION_DOWN:
					mCells[x][y]->CompareCells(mCells[x][y + 1]);
					break;

				case CELL_DIRECTION_LEFT:
					mCells[x][y]->CompareCells(mCells[x - 1][y]);
					break;

				case CELL_DIRECTION_RIGHT:
					mCells[x][y]->CompareCells(mCells[x + 1][y]);
					break;
				}
			}
		}
		mGenerationCount++;
		Update_UI();
	}
}

void Application::Update_UI()
{
	std::cout << "Generation: " << mGenerationCount << "\n";
	std::cout << "Predators: " << mPredCount << "\n";
	std::cout << "Prey: " << mPreyCount << "\n";
}

void Application::Draw()
{
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 0);
	SDL_RenderClear(mRenderer);

	for (int x = 1; x < CELL_COUNT_X - 1; x++)
		for (int y = 1; y < CELL_COUNT_Y - 1; y++)
		{
			if (mCells[x][y]->GetType() == CELL_TYPE::CELL_TYPE_NONE)
				continue;

			mCells[x][y]->Draw(*mRenderer);
		}

	if (bIsPaused)
	{
		//Sim paused icon
		SDL_Rect rect;
		rect.x = 0;
		rect.y = 0;
		rect.w = 32;
		rect.h = 32;
		SDL_SetRenderDrawColor(mRenderer, 200, 200, 200, 255);
		SDL_RenderFillRect(mRenderer, &rect);

		rect.w = 8;
		rect.h = 24;
		rect.x = 4;
		rect.y = 4;
		SDL_SetRenderDrawColor(mRenderer, 255, 0, 0, 255);
		SDL_RenderFillRect(mRenderer, &rect);
		rect.x = 20;
		rect.y = 4;
		SDL_SetRenderDrawColor(mRenderer, 255, 0, 0, 255);
		SDL_RenderFillRect(mRenderer, &rect);
	}

	Draw_UI();
	SDL_RenderFlush(mRenderer);

	SDL_RenderPresent(mRenderer);
}

void Application::Draw_UI()
{

}