#include "Framework/Application.h"
#include "Framework/Renderer.h"
#include "Framework/Editor.h"
#include "Framework/Input.h"
#include "Utilities/Logger.h"

#define WIN32_LEAN_AND_MEAN 
#include <Windows.h>
#include <cassert>
#include <chrono>

// ImGui //
#include <imgui.h>
#include <imgui_impl_dx12.h>
#include <imgui_impl_win32.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace EngineInternal
{
	bool doResize = false;
}
using namespace EngineInternal;

Application::Application()
{
	RegisterWindowClass();

	renderer = new Renderer(applicationName, windowWidth, windowHeight);
	editor = new Editor(this);

	LOG("Successfully initialized - 'Insert Application Name'");
}

void Application::Run()
{
	std::chrono::high_resolution_clock* clock = new std::chrono::high_resolution_clock();
	auto t0 = std::chrono::time_point_cast<std::chrono::milliseconds>((clock->now())).time_since_epoch();;
	float deltaTime = 1.0f;

	MSG msg = {};
	while(runApplication && msg.message != WM_QUIT)
	{
		// DeltaTime //
		auto t1 = std::chrono::time_point_cast<std::chrono::milliseconds>((clock->now())).time_since_epoch();
		deltaTime = (t1 - t0).count() * .001;
		t0 = t1;

		// Window's Callback //
		if(::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}

		// Engine Loop //
		Start();
		Update(deltaTime);
		Render();
	}
}

void Application::Start()
{
	if(doResize)
	{
		renderer->Resize();
		doResize = false;
	}

	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void Application::Update(float deltaTime)
{
	Input::Update();

	editor->Update(deltaTime);

	if(Input::GetKeyDown(KeyCode::Escape))
	{
		runApplication = false;
	}
}

void Application::Render()
{
	ImGui::Render();
	renderer->Render();
}

void Application::RegisterWindowClass()
{
	WNDCLASSEXW windowClassDescription = {};
	HINSTANCE hInstance = GetModuleHandle(NULL);

	windowClassDescription.cbSize = sizeof(WNDCLASSEX);
	windowClassDescription.style = CS_HREDRAW | CS_VREDRAW;
	windowClassDescription.lpfnWndProc = &WindowsCallback;
	windowClassDescription.cbClsExtra = 0;
	windowClassDescription.cbWndExtra = 0;
	windowClassDescription.hInstance = hInstance;
	windowClassDescription.hIcon = ::LoadIcon(hInstance, NULL);
	windowClassDescription.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	windowClassDescription.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	windowClassDescription.lpszMenuName = NULL;
	windowClassDescription.lpszClassName = applicationName.c_str();
	windowClassDescription.hIconSm = ::LoadIcon(hInstance, NULL);

	static ATOM atom = ::RegisterClassExW(&windowClassDescription);
	assert(atom > 0);
}

LRESULT Application::WindowsCallback(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_DESTROY:
		::PostQuitMessage(0);
		break;

	case WM_SIZE:
		doResize = true;
		break;
	}

	// ImGui Windows callback //
	ImGui_ImplWin32_WndProcHandler(hwnd, message, wParam, lParam);

	return ::DefWindowProcW(hwnd, message, wParam, lParam);
}