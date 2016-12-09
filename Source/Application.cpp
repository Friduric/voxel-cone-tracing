#include "Application.h"

// Standard library.
#include <iostream>
#include <iomanip>
#include <time.h>

// External.
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>

// Internal.
#include "Scene\Scene.h"
#include "Scene\ScenePack.h"
#include "Graphic\Graphics.h"
#include "Graphic\Material\MaterialStore.h"
#include "Graphic\Renderer\MeshRenderer.h"
#include "Time\Time.h"

#define __LOG_INTERVAL 100 /* How often we should log frame rate info to the console. = 0 means don't log. */
#if __LOG_INTERVAL > 0
constexpr float __LOG_INTERVAL_TIME_GUARD = 1.0f;
#endif

using __DEFAULT_LEVEL = DragonScene; // The scene that will be loaded on startup.
// (see ScenePack.h for more scenes)

Application & Application::getInstance() {
	static Application application;
	return application;
}

void Application::init() {
	std::cout << "Initialization started." << std::endl;

	// -------------------------------------
	// Initialize GLFW.
	// -------------------------------------
	if (!glfwInit()) {
		std::cerr << "GLFW failed to initialize." << std::endl;
	}
	double timeElapsed = glfwGetTime();

	// OpenGL version 4.5 with core profile.
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE); // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_SAMPLES, MSAA_SAMPLES);

	// Create a new window and set window mode accordingly (including title, size and mode).
#if DEFAULT_FULLSCREEN == 2 
	// Borderless fullscreen.
	SetBorderlessFullscreenMode();
#else
	SetWindowMode(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, DEFAULT_FULLSCREEN == 1);
#endif
	std::cout << "[0] : GLFW initialized." << std::endl;

	// -------------------------------------
	// Initialize GLEW.
	// -------------------------------------
	glewExperimental = GL_TRUE;
	if (glewInit() == GLEW_OK) {
		std::cout << "[1] : GLEW initialized." << std::endl;
	}
	else {
		std::cerr << "GLEW failed to initialize (glewExperimental might not be supported)." << std::endl;
		std::cerr << "Press any key to exit ... " << std::endl;
		getchar();
		return;
	}

	std::vector<std::pair<GLuint, std::string>> requiredGLEWExtensions = {
		{ GLEW_ARB_shader_image_load_store,		"ARB_shader_image_load_store"},
		{ GLEW_VERSION_4_5,						"GLEW_VERSION_4_5 (OpenGL 4.5)"},
		{ GL_ARB_multisample,					"GLFW MSAA" }
	};

	for (const auto & ext : requiredGLEWExtensions) {
		if (!ext.first) {
			std::cerr << "ERROR: " << ext.second << " not supported! Expect unexpected behaviour." << std::endl;
			std::cerr << "Press any key to continue ... " << std::endl;
			getchar();
		}
	}

	// -------------------------------------
	// Initialize graphics.
	// -------------------------------------
	int w, h;
	glfwGetWindowSize(currentWindow, &w, &h);
	MaterialStore::getInstance(); // Initialize material store.
	graphics.init(w, h);
	glfwSetWindowSizeCallback(currentWindow, Application::OnWindowResize);
	glfwSwapInterval(DEFAULT_VSYNC); // vSync.
	std::cout << "[2] : Graphics initialized." << std::endl;

	// -------------------------------------
	// Initialize scene.
	// -------------------------------------
	scene = new __DEFAULT_LEVEL();
	scene->init(w, h);
	std::cout << "[3] : Scene initialized." << std::endl;

	// -------------------------------------
	// Initialize AntTweakBar.
	// -------------------------------------
	TwInit(TW_OPENGL_CORE, NULL);
	int invTwWindowScale = 1;
	TwWindowSize(invTwWindowScale * w, invTwWindowScale * h);

	// Main bar.
	mainTweakBar = TwNewBar("Rendering settings");
	TwType renderingMode = TwDefineEnum("RenderingMode", NULL, 0);
	for (auto * meshRenderer : scene->renderers) if (meshRenderer->tweakable) tweakableRenderers.push_back(meshRenderer);
	TwAddVarRW(mainTweakBar, "Application state", TW_TYPE_INT32, &state, "label='State' group=Rendering");
	TwAddVarRW(mainTweakBar, "Rendering mode", renderingMode, &currentRenderingMode, "enum='0 {Voxel Visualization}, 1 {Voxel Cone Tracing}' group=Rendering");
	auto temp = "mainsep1";
	TwAddSeparator(mainTweakBar, temp, NULL);
	TwAddVarRW(mainTweakBar, "Shadows", TW_TYPE_BOOL8, &graphics.shadows, "group=Settings");
	TwAddVarRW(mainTweakBar, "Direct light", TW_TYPE_BOOL8, &graphics.directLight, "group=Settings");
	TwAddVarRW(mainTweakBar, "Indirect diffuse light", TW_TYPE_BOOL8, &graphics.indirectDiffuseLight, "group=Settings");
	TwAddVarRW(mainTweakBar, "Indirect specular light", TW_TYPE_BOOL8, &graphics.indirectSpecularLight, "group=Settings");

	temp = "mainsep2";
	TwAddSeparator(mainTweakBar, temp, NULL);
	TwAddVarRW(mainTweakBar, "Autogen voxelization", TW_TYPE_BOOL8, &graphics.automaticallyVoxelize, "group=Voxelization");
	TwAddVarRW(mainTweakBar, "Queue voxelization gen", TW_TYPE_BOOL8, &graphics.voxelizationQueued, "group=Voxelization");
	TwAddVarRW(mainTweakBar, "Voxelization sparsity", TW_TYPE_INT32, &graphics.voxelizationSparsity, "group=Voxelization");
	TwAddVarRW(mainTweakBar, "Autogen mipmap", TW_TYPE_BOOL8, &graphics.automaticallyRegenerateMipmap, "group=Voxelization");
	TwAddVarRW(mainTweakBar, "Queue mipmap gen", TW_TYPE_BOOL8, &graphics.regenerateMipmapQueued, "group=Voxelization");

	// Point lights.
	TwStructMember pointMembers[] = {
		{ "X", TW_TYPE_FLOAT, sizeof(glm::float32) * 0, " Min=-2 Max=2 Step=0.01 " },
		{ "Y", TW_TYPE_FLOAT, sizeof(glm::float32) * 1, " Min=-2 Max=2 Step=0.01 " },
		{ "Z", TW_TYPE_FLOAT, sizeof(glm::float32) * 2, " Min=-2 Max=2 Step=0.01 " } };
	TwType pointType = TwDefineStruct("POINT", pointMembers, 3, sizeof(glm::vec3), NULL, NULL);
	if (scene->pointLights.size() > 0) {
		int pp = 0;
		for (auto & p : scene->pointLights) {
			if (!p.tweakable) continue;
			std::string group = "Point Light " + std::to_string(pp);
			std::string setting = "group='" + group + "'";
			TwAddVarRW(mainTweakBar, "color", TW_TYPE_COLOR3F, &p.color, setting.c_str());
			TwAddVarRW(mainTweakBar, "position", pointType, &p.position, setting.c_str());
			pp++;
		}
	}

	// Objects.
	UpdateObjectTweakbar();

	std::cout << "[4] : AntTweakBar initialized." << std::endl;

	// -------------------------------------
	// Initialize input.
	// -------------------------------------
	glfwSetInputMode(currentWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetMouseButtonCallback(currentWindow, GLFWMouseButtonCallback);
	glfwSetCursorPosCallback(currentWindow, GLFWMousePositionCallback);
	glfwSetKeyCallback(currentWindow, GLFWKeyCallback);
	std::cout << "[5] : Input initialized." << std::endl;

	// -------------------------------------
	// Finalize initialization.
	// -------------------------------------
	srand(0);
	Time::time = 0;
	initialized = Time::initialized = true;

	timeElapsed = glfwGetTime() - timeElapsed;
	std::cout << "Initialization finished (" << timeElapsed << " seconds)!" << std::endl;
	glfwSetTime(0);

	std::cout << "Using OpenGL version " << glGetString(GL_VERSION) << std::endl;
}

void Application::UpdateObjectTweakbar() {
	if (objectTweakBar != nullptr) TwDeleteBar(objectTweakBar);
	int N = tweakableRenderers.size();
	if (N <= 0) return;

	TwStructMember vec3members[] = {
		{ "X", TW_TYPE_FLOAT, sizeof(glm::float32) * 0, " Min=-2 Max=2 Step=0.01 " },
		{ "Y", TW_TYPE_FLOAT, sizeof(glm::float32) * 1, " Min=-2 Max=2 Step=0.01 " },
		{ "Z", TW_TYPE_FLOAT, sizeof(glm::float32) * 2, " Min=-2 Max=2 Step=0.01 " }
	};
	TwType pointType = TwDefineStruct("VEC3", vec3members, 3, sizeof(glm::vec3), NULL, NULL);

	TwBar * t = objectTweakBar = TwNewBar("Renderers");

	int xpos = DEFAULT_WINDOW_WIDTH - 215;
	int ypos = 16;
	std::string move = " Renderers position='" + std::to_string(xpos) + " " + std::to_string(ypos) + std::string("'");
	TwDefine(move.c_str());

	for (int i = 0; i < N; ++i) {
		MeshRenderer * meshRenderer = tweakableRenderers[i];
		MaterialSetting * materialSetting = meshRenderer->materialSetting;
		Transform * transform = &meshRenderer->transform;

		std::string groupName = "group='" + meshRenderer->name + " " + std::to_string(i) + "' ";
		std::string temp;

		// Enabled.
		temp = groupName + "label=Enabled";
		TwAddVarRW(t, "Enabled" + i, TW_TYPE_BOOL8, &meshRenderer->enabled, temp.c_str());

		// Diffuse.
		temp = groupName + "label='Diffuse color'";
		TwAddVarRW(t, "Diffuse color" + i, TW_TYPE_COLOR3F, &materialSetting->diffuseColor, temp.c_str());
		temp = groupName + "label='Diffuse reflectivity'" + " min=0 max=1 step=0.01";
		TwAddVarRW(t, "Diffuse reflectivity" + i, TW_TYPE_FLOAT, &materialSetting->diffuseReflectivity, temp.c_str());

		// Specular.
		temp = groupName + "label='Specular color'";
		TwAddVarRW(t, "Specular color" + i, TW_TYPE_COLOR3F, &materialSetting->specularColor, temp.c_str());
		temp = groupName + "label='Specular reflectivity'" + " min=0 max=1 step=0.01";
		TwAddVarRW(t, "Specular reflectivity" + i, TW_TYPE_FLOAT, &materialSetting->specularReflectivity, temp.c_str());
		temp = groupName + "label='Specular diffusion'" + " min=1.8 max=8 step=0.10";
		TwAddVarRW(t, "Specular diffusion" + i, TW_TYPE_FLOAT, &materialSetting->specularDiffusion, temp.c_str());

		// Transparency.
		temp = groupName + "label='Transparency'" + " min=0 max=1 step = 0.01";
		TwAddVarRW(t, "Transparency" + i, TW_TYPE_FLOAT, &materialSetting->transparency, temp.c_str());
		temp = groupName + "label='Refractive index'" + " min=1 max=3 step=0.01";
		TwAddVarRW(t, "Refractive Index" + i, TW_TYPE_FLOAT, &materialSetting->refractiveIndex, temp.c_str());

		// Other.
		temp = groupName + "label='Emissivity'" + " min=0 max=0.9999 step=0.01";
		TwAddVarRW(t, "Emissivity" + i, TW_TYPE_FLOAT, &materialSetting->emissivity, temp.c_str());

		temp = groupName + "sep1";
		TwAddSeparator(t, temp.c_str(), NULL);

		// Transform.
		temp = groupName + "label='Position'";
		TwAddVarRW(t, "Position" + i, pointType, &transform->position, temp.c_str());
		temp = groupName + "label='Scale'";
		TwAddVarRW(t, "Scale" + i, pointType, &transform->scale, temp.c_str());
		temp = groupName + "label='Rotation'";
		TwAddVarRW(t, "Rotation" + i, TW_TYPE_DIR3F, &transform->rotation, temp.c_str());
	}
}

void Application::run()
{
	std::cout << "Application is now running.\n" << std::endl;
	std::cout << " :: Use R to switch between rendering modes.\n";
	// std::cout << " :: Use T to switch between interaction modes." << std::endl;

	double smoothedDeltaTimeAccumulator = 0;
#if __LOG_INTERVAL > 0
	double updateCost = 0;
	double renderCost = 0;
	double timestampCost = 0;
	double timestampLog = 0;
#endif

	// Start the update loop.
	while (!glfwWindowShouldClose(currentWindow) && !exitQueued)
	{
		// --------------------------------------------------
		// Update input and timers.
		// --------------------------------------------------
		UpdateGlobalInputParameters();

		// Update time vals.
		double currentTime = glfwGetTime();
		Time::deltaTime = currentTime - Time::time;
		Time::time = currentTime;
		Time::framesPerSecond = 1.0 / (Time::deltaTime + 0.0000001);

		// --------------------------------------------------
		// Update world.
		// --------------------------------------------------
#if __LOG_INTERVAL > 0
		{
			timestampCost = glfwGetTime();
		}
#endif
		if (!paused) scene->update();
#if __LOG_INTERVAL > 0 
		{
			updateCost += glfwGetTime() - timestampCost;
		}
#endif

		// --------------------------------------------------
		// Rendering.
		// --------------------------------------------------
#if __LOG_INTERVAL > 0
		{
			timestampCost = glfwGetTime();
		}
#endif
		int viewportWidth, viewportHeight;
		glfwGetWindowSize(currentWindow, &viewportWidth, &viewportHeight);
		if (!paused) {
			graphics.render(*scene, viewportWidth, viewportHeight, currentRenderingMode);

		}

		// --------------------------------------------------
		// Tweakbar.
		// --------------------------------------------------
		TwDraw(); // Draw AntTweakBar.

		// --------------------------------------------------
		// Swap buffers and update timers.
		// --------------------------------------------------
#if __LOG_INTERVAL > 0
		{
			renderCost += glfwGetTime() - timestampCost;
		}
#endif

		// Swap front and back buffers.
		if (!paused) glfwSwapBuffers(currentWindow);

		// Poll for and process events.
		glfwPollEvents();

		// Update frame count.
		Time::frameCount++;
		smoothedDeltaTimeAccumulator += Time::deltaTime;

		// Update smoothed delta time.
		if (Time::smoothedDeltaTimeFrameCount > 0 && Time::frameCount % Time::smoothedDeltaTimeFrameCount == 0) {
			Time::smoothedDeltaTime = smoothedDeltaTimeAccumulator / Time::smoothedDeltaTimeFrameCount;
			smoothedDeltaTimeAccumulator = 0;
		}

#if __LOG_INTERVAL > 0
		{
			using namespace std;
			timestampLog += Time::deltaTime;
			if (Time::frameCount % __LOG_INTERVAL == 0 && timestampLog > __LOG_INTERVAL_TIME_GUARD) {
				cout << endl;
				cout << setprecision(8) << fixed << "State: " << state << ", update average time: " << updateCost / __LOG_INTERVAL << ", render average time: " << renderCost / __LOG_INTERVAL << endl;
				cout << setprecision(1) << fixed << "FPS: " << Time::framesPerSecond << " smoothed " << 1.0f / Time::smoothedDeltaTime << setprecision(4) << ", delta: " << Time::deltaTime << ", frame count: " << Time::frameCount << ", smooth delta: " << Time::smoothedDeltaTime << endl;
				updateCost = 0;
				renderCost = 0;
				cout << flush;
				timestampLog = 0;
			}
		}
#endif
	}

	// Clean up and exit.
	glfwDestroyWindow(currentWindow);
	glfwTerminate();
	// TwTerminate();
	std::cout << "Application has now terminated." << std::endl;
}

void Application::UpdateGlobalInputParameters() {
	if (glfwGetKey(currentWindow, GLFW_KEY_ESCAPE)) {
		exitQueued = true;
		return;
	}

	int button_state_x = glfwGetKey(currentWindow, GLFW_KEY_X);
	int button_state_z = glfwGetKey(currentWindow, GLFW_KEY_Z);

	if (button_state_x == GLFW_PRESS && button_state_x != previous_state_x) {
		std::cout << "Application state: " << ++state << std::endl;
	}
	if (button_state_z == GLFW_PRESS && button_state_z != previous_state_z) {
		std::cout << "Application state: " << --state << std::endl;
	}

	previous_state_x = button_state_x;
	previous_state_z = button_state_z;
}

void Application::SetBorderlessFullscreenMode() {
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
	GLFWwindow* newWindow = glfwCreateWindow(mode->width, mode->height, DEFAULT_TITLE, monitor, NULL);
	if (newWindow) {
		currentWindow = newWindow;
		glfwMakeContextCurrent(currentWindow);
	}
	else {
		glfwTerminate();
		std::cerr << "GLFW failed setting borderless fullscreen window mode." << std::endl;
	}
}

void Application::SetWindowMode(int quadWidth, int quadHeight, bool fullscreen = true) {
	GLFWwindow* newWindow = glfwCreateWindow(quadWidth, quadHeight, DEFAULT_TITLE, fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);
	if (newWindow) {
		currentWindow = newWindow;
		glfwMakeContextCurrent(currentWindow);
	}
	else {
		glfwTerminate();
		std::cerr << "GLFW failed setting new window mode." << std::endl;
	}
}

Application::~Application() {
	delete scene;
	if (mainTweakBar != nullptr) TwDeleteBar(mainTweakBar);
	if (objectTweakBar != nullptr) TwDeleteBar(objectTweakBar);
}

Application::Application() : exitQueued(false) {

}

void Application::OnWindowResize(GLFWwindow* window, int quadWidth, int quadHeight)
{

}

void Application::GLFWMousePositionCallback(GLFWwindow * window, double x, double y)
{
	if (!TwEventMousePosGLFW(int(x), int(y))) {

	}
}

void Application::GLFWMouseButtonCallback(GLFWwindow * window, int button, int action, int mods)
{
	if (!TwEventMouseButtonGLFW(button, action)) {

	}
}

void Application::GLFWKeyCallback(GLFWwindow * window, int key, int scanmode, int action, int mods)
{
	auto & app = Application::getInstance();

	if (!TwEventKeyGLFW(key, action)) {

	}

	// Button was pressed down this frame.
	if (action == 1) {
		// Change rendering mode.
		if (key == GLFW_KEY_R) {
			using GRM = Graphics::RenderingMode;
			if (app.currentRenderingMode == GRM::VOXELIZATION_VISUALIZATION) {
				app.currentRenderingMode = GRM::VOXEL_CONE_TRACING;
			}
			else {
				app.currentRenderingMode = GRM::VOXELIZATION_VISUALIZATION;
			}
		}

		// Change viewing mode.
		if (key == GLFW_KEY_M) {
			if (app.currentInputState == InputState::FREE_LOOK) {
				glfwSetInputMode(app.currentWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				app.currentInputState = InputState::TWEAK_BAR;
			}
			else {
				app.currentInputState = InputState::FREE_LOOK;
				glfwSetInputMode(app.currentWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			}
			// Update camera.
			int xwidth, yheight;
			glfwGetWindowSize(window, &xwidth, &yheight);
			glfwSetCursorPos(window, xwidth / 2, yheight / 2); // Reset mouse position for next update iteration.
		}

		// Pause / unpause.
		if (key == GLFW_KEY_P) {
			app.paused = !app.paused;
		}
	}
}