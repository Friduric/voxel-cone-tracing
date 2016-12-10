// This is the main entry for the voxel cone tracing demo. 
// See 'Graphics.h' and 'voxel_cone_tracing.frag' for the code relevant to voxel cone tracing.
#pragma once

#include "Graphic\Graphics.h"
#include <AntTweakBar\AntTweakBar.h>

class Scene;
struct GLFWwindow;

/// <summary>
/// Singleton implementation of an application and the main entry for the whole application.
/// </summary>
class Application {
public:
	enum InputState { FREE_LOOK = 0, TWEAK_BAR = 1 };
	const unsigned int MSAA_SAMPLES = 0;
	const unsigned int DEFAULT_WINDOW_WIDTH = 1280;
	const unsigned int DEFAULT_WINDOW_HEIGHT = 720;
	const char * DEFAULT_TITLE = "Voxel Cone Tracing by Fredrik Prantare <prantare@gmail.com>";
	const unsigned int DEFAULT_FULLSCREEN = 0; // 0 is window, 1 is fullscreen, 2 is borderless fullscreen.
	const int DEFAULT_VSYNC = 0; // 0 is no vSync, can also use negative vSync (check GLFW docs).

	int state = 0; // Used to simplify debugging. Sent to all shaders continuously.
	Graphics::RenderingMode currentRenderingMode = Graphics::RenderingMode::VOXEL_CONE_TRACING;
	InputState currentInputState = InputState::FREE_LOOK;

	~Application();

	/// <summary> Exits the application if set to true. </summary>
	bool exitQueued = false;

	/// <summary> The currently opened window. </summary>
	GLFWwindow * currentWindow;

	/// <summary> The scene to update and render. </summary>
	Scene * scene;

	/// <summary> The graphical context that is used for rendering the current scene. </summary>
	Graphics graphics;

	/// <summary> Returns the application instance (which is a singleton). </summary>
	static Application & getInstance();

	/// <summary> Initializes the application. </summary>
	void init();

	/// <summary> Runs the application. </summary>
	void run();

	/// <summary> Sets the window mode to be borderless fullscreen. </summary>
	void SetBorderlessFullscreenMode();

	/// <summary> Sets the window mode of the application </summary>
	void SetWindowMode(int quadWidth, int quadHeight, bool fullscreen);

	// Delete copy constructors.
	Application(Application const &) = delete;
	void operator=(Application const &) = delete;

	/// <summary> Pauses/unpauses the game and stops rendering. </summary>
	bool paused = false;

	// --- Callbacks. ---
	// http://www.glfw.org/docs/3.0/group__input.html
	/// <summary> Is called whenever the mouse position has changed. </summary>
	static void GLFWMousePositionCallback(GLFWwindow * window, double x, double y);
	/// <summary> Is called whenever a mouse button has been pressed. </summary>
	static void GLFWMouseButtonCallback(GLFWwindow * window, int button, int action, int mods);
	/// <summary> Is called whenever a key has been pressed. </summary>
	static void GLFWKeyCallback(GLFWwindow * window, int key, int scanmode, int action, int mods);
private:
	// --- Tweakbar ---
	void Application::UpdateObjectTweakbar();
	TwBar * mainTweakBar = nullptr;
	TwBar * objectTweakBar = nullptr;
	std::vector<MeshRenderer*> tweakableRenderers;
	PointLight * tweakablePointLight = nullptr;

	// --- Other ---
	int previous_state_x, previous_state_z; // For testing.
	void UpdateGlobalInputParameters();
	bool initialized = false;
	Application(); // Make sure constructor is private to prevent instantiating outside of singleton pattern.
	static void OnWindowResize(GLFWwindow * window, int quadWidth, int quadHeight);
};
