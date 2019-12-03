#include "Application.h"

int main()
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

void Application::CreateWindow()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		Uint32 windowFlags = SDL_WINDOW_SHOWN;

		SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, windowFlags, &mWindow, &mRenderer);

		CreateGUI();
	}
}

void Application::CreateGUI()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	//ImGui_ImplSDL2_InitForOpenGL(mWindow);

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.WantTextInput = true;
	io.WantCaptureMouse = true;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

	ImGui::StyleColorsDark();
}


Application::Application()
{
	bIsRunning = false;

}

Application::~Application()
{

}

void Application::Start()
{
	bIsRunning = true;
}

void Application::Stop()
{
	bIsRunning = false;
}

bool const Application::IsRunning()
{
	return bIsRunning;
}


void Application::PollEvents()
{
	SDL_Event e;
	SDL_PollEvent(&e);

	switch (e.type)
	{

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

	Draw_UI();
}

void Application::Draw_UI()
{

}