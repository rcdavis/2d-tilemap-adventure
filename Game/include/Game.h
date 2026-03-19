#pragma once

#include <stdint.h>

struct GLFWwindow;

class Game {
public:
	Game();
	~Game();

	void Run();

private:
	bool Init();
	void Shutdown();

	void Update();
	void Render();

	void QueryRenderSpecs();

	uint32_t LoadTexture(const char* path);

	static void GlfwErrorCallback(int error, const char* description);

private:
	GLFWwindow* mWindow;

	uint32_t mTestImage;
};
