#pragma once

#include <array>

class Application;

class Editor
{
public:
	Editor(Application* application);

	void Update(float deltaTime);

private:
	void ImGuiStyleSettings();

private:
	float deltaTime;
	std::array<float, 300> deltaTimeLog;
	unsigned int frameCount = 0;

	struct ImFont* baseFont;
	struct ImFont* boldFont;

	Application* application;
};