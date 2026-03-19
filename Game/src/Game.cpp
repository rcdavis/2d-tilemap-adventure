#include "Game.h"

#include "GLFW/glfw3.h"
#include "glad/gl.h"

#include "stb_image.h"

#include "Utils/Log.h"
#include "Utils/GLUtils.h"

static void APIENTRY GLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
	GLsizei length, const GLchar* message, const void* userParam);

Game::Game() :
	mWindow(nullptr),
	mTestImage(0)
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

#ifdef DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

	mWindow = glfwCreateWindow(800, 600, "2D Tilemap Adventure", nullptr, nullptr);
	if (!mWindow) {
		LOG_ERROR("Failed to create GLFW window.");
		return false;
	}

	glfwMakeContextCurrent(mWindow);
	if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
		LOG_ERROR("Failed to initialize GLAD.");
		return false;
	}

#ifdef DEBUG
	if (GLAD_GL_KHR_debug) {
		LOG_INFO("Setting up OpenGL debug callback...");
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(GLDebugCallback, nullptr);
	} else {
		LOG_INFO("Unable to use OpenGL debug callback");
	}
#endif

	QueryRenderSpecs();

	glfwSwapInterval(1);

	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0f, 0.0f, 1.0f, 1.0f);

	mTestImage = GLUtils::LoadTexture("res/textures/overworld.png");

	LOG_INFO("Game initialized successfully.");

	return true;
}

void Game::Shutdown() {
	LOG_INFO("Shutting down game.");

	if (mTestImage) {
		glDeleteTextures(1, &mTestImage);
		mTestImage = 0;
	}

	glfwTerminate();
	mWindow = nullptr;
}

void Game::Update() {
}

void Game::Render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glfwSwapBuffers(mWindow);
}

void Game::QueryRenderSpecs() {
	GLint glMajorVer = 0, glMinorVer = 0;
	glGetIntegerv(GL_MAJOR_VERSION, &glMajorVer);
	glGetIntegerv(GL_MINOR_VERSION, &glMinorVer);
	LOG_INFO("OpenGL Info:");
	LOG_INFO("  Vendor: {0}", (char*)glGetString(GL_VENDOR));
	LOG_INFO("  Renderer: {0}", (char*)glGetString(GL_RENDERER));
	LOG_INFO("  Version: {0}.{1}", glMajorVer, glMinorVer);

	GLint ctxProfileMask = 0;
	glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &ctxProfileMask);
	if (ctxProfileMask & GL_CONTEXT_CORE_PROFILE_BIT)
		LOG_INFO("  Profile: Core");
	else if (ctxProfileMask & GL_CONTEXT_COMPATIBILITY_PROFILE_BIT)
		LOG_INFO("  Profile: Compat");

	GLint flags = 0;
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);

	std::vector<std::string> infoFlags;
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
		infoFlags.emplace_back("Debug");
	if (flags & GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT)
		infoFlags.emplace_back("Forward-Compat");

	if (std::size(infoFlags) > 1) {
		std::string flagText = infoFlags[0];
		for (const auto& t : infoFlags) {
			flagText += " | " + t;
		}

		LOG_INFO("  Context Flags: {0}", flagText.c_str());
	} else if (std::size(infoFlags) == 1) {
		LOG_INFO("  Context Flags: {0}", infoFlags[0].c_str());
	}

	GLint maxTextureSize = 0;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
	LOG_INFO("  Max Texture Size: {0}", maxTextureSize);

	GLint maxRbSize = 0;
	glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE, &maxRbSize);
	LOG_INFO("  Max Renderbuffer Size: {0}", maxRbSize);
}

void Game::GlfwErrorCallback(int error, const char* description) {
	// Ignore Wayland error that complains about not providing the window position.
	if (glfwGetPlatform() == GLFW_PLATFORM_WAYLAND && error == 65548)
		return;

	LOG_ERROR("GLFW Error ({0}): {1}", error, description);
}

void APIENTRY GLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
	GLsizei length, const GLchar* message, const void* userParam)
{
	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH:
		LOG_CRITICAL("OpenGL Debug Message: {0}", message);
		break;

	case GL_DEBUG_SEVERITY_MEDIUM:
		LOG_ERROR("OpenGL Debug Message: {0}", message);
		break;

	case GL_DEBUG_SEVERITY_LOW:
		LOG_WARN("OpenGL Debug Message: {0}", message);
		break;

	case GL_DEBUG_SEVERITY_NOTIFICATION:
		LOG_INFO("OpenGL Debug Message: {0}", message);
		break;

	default:
		LOG_TRACE("OpenGL Debug Message: {0}", message);
		break;
	}
}
