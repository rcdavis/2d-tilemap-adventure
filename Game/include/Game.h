#pragma once

#include <stdint.h>
#include <memory>

struct GLFWwindow;
class GLTexture;

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

	static void GlfwErrorCallback(int error, const char* description);

private:
	GLFWwindow* mWindow;

	std::shared_ptr<GLTexture> mTestTexture;
};
