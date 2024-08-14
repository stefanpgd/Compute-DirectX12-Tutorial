#include "Framework/Input.h"

int Input::frameCount = 0;
std::map<KeyCode, int> Input::keysDown;

void Input::Update()
{
	frameCount++;

	for(auto it = keysDown.begin(); it != keysDown.end();)
	{
		// If key is not held anymore, remove entry
		if(!GetKey(it->first))
		{
			it = keysDown.erase(it);
		}
		else
		{
			++it;
		}
	}
}

bool Input::GetKey(KeyCode key)
{
	return ImGui::IsKeyDown(ImGuiKey(key));
}

bool Input::GetKeyDown(KeyCode key)
{
	// If key is in the 'keysDown' record, check if its still the same frame
	if(keysDown.count(key))
	{
		if(keysDown[key] == frameCount)
		{
			return true;
		}

		return false;
	}

	// If key is pressed, record the current key with the frame
	if(ImGui::IsKeyDown(ImGuiKey(key)))
	{
		keysDown[key] = frameCount;
		return true;
	}

	return false;
}

bool Input::GetMouseButton(MouseCode button)
{
	return ImGui::GetIO().MouseDown[unsigned int(button)];
}

int Input::GetMouseX()
{
	return ImGui::GetIO().MousePos.x;
}

int Input::GetMouseY()
{
	return ImGui::GetIO().MousePos.y;
}

int Input::GetMouseVelocityX()
{
	return ImGui::GetIO().MouseDelta.x;
}

int Input::GetMouseVelocityY()
{
	return ImGui::GetIO().MouseDelta.y;
}