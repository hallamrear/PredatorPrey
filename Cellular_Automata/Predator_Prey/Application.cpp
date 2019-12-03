#include "Application.h"

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

		
		SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, windowFlags, &mWindow, &mRenderer);

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

void Application::Cleanup()
{
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
		bIsRunning = true;
	}
}

void Application::Stop()
{
	if (bIsRunning)
	{
		bIsRunning = false;
		Cleanup();
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
	default:
		break;
	}
}

void Application::Update()
{
	

	Update_UI();
}

void Application::Update_UI()
{

}

void Application::Draw()
{
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
	SDL_RenderClear(mRenderer);


	Draw_UI();
	SDL_RenderPresent(mRenderer);
}

void Application::Draw_UI()
{

}