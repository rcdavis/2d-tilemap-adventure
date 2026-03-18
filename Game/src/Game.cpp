#include "Game.h"

#include "GLFW/glfw3.h"
#include "glad/glad.h"

#include "Utils/Log.h"

Game::Game() :
	mWindow(nullptr)
{}

Game::~Game() {
	Shutdown();
}

void Game::Run() {
	if (!Init()) {
		return;
	}

	while (!glfwWindowShouldClose(mWindow)) {
		glfwPollEvents();

		Update();

		Render();

		glfwSwapBuffers(mWindow);
	}
}

bool Game::Init() {
	glfwSetErrorCallback(GlfwErrorCallback);

	if (!glfwInit()) {
		LOG_ERROR("Failed to initialize GLFW.");
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	mWindow = glfwCreateWindow(800, 600, "2D Tilemap Adventure", nullptr, nullptr);
	if (!mWindow) {
		LOG_ERROR("Failed to create GLFW window.");
		return false;
	}

	glfwMakeContextCurrent(mWindow);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		LOG_ERROR("Failed to initialize GLAD.");
		return false;
	}

	glfwSwapInterval(1);

	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0f, 0.0f, 1.0f, 1.0f);

	LOG_INFO("Game initialized successfully.");

	return true;
}

void Game::Shutdown() {
	LOG_INFO("Shutting down game.");

	glfwTerminate();
	mWindow = nullptr;
}

void Game::Update() {
}

void Game::Render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Game::GlfwErrorCallback(int error, const char* description) {
	// Ignore Wayland error that complains about not providing the window position.
	if (glfwGetPlatform() == GLFW_PLATFORM_WAYLAND && error == 65548)
		return;

	LOG_ERROR("GLFW Error ({0}): {1}", error, description);
}
