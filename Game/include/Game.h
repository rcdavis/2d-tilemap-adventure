#pragma once

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

	static void GlfwErrorCallback(int error, const char* description);

private:
	GLFWwindow* mWindow;
};
