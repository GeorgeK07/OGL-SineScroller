/*
OpenGL Demo
*/

/* Include library headers */
// Audio
#include <mikmod.h> // Include before Synchapi.h
// I/O
#include <iostream>
#include <Synchapi.h>
// OpenGL assistance libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// ImGui
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
// Include local class headers`
#include "camera.h"
#include "shader_s.h"
#include "stb_image.h"

/* Global variables */
// Time between current and last frame
float deltaTime = 0.0f;
// Time of last frame
float lastFrame = 0.0f;

// Create camera object
Camera camera(0.0f, 0.0f, 3.0f, 0.0f, 1.0f, 0.0f, -90.0f, 0.0f);

// Last X and Y values of mouse (start in middle of screen)
float lastX = 400;
float lastY = 300;
// Bool to check if first time mouse has entered window
bool firstMouse = 1;

/* Keyboard/mouseclick input function */
// Check what inputs are pressed and do some action
void processInput(GLFWwindow* window) {
	// If escape is pressed, close window
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

/* Window callback function */
// Callback function to change viewport size when window size is changed
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	// Set OpenGL viewport size to passed width and height of window
	glViewport(0, 0, width, height);
}

// Main function
int main() {
	// Initialise GLFW
	glfwInit();
	// Configure GLFW to use OpenGL 3.3 and core profile
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create window object
	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Demo", NULL, NULL);
	// If it can't be created, exit program
	if (window == NULL) {
		// Print error message, terminate GLFW, and exit program with code -1
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// If created, make window context the main context of the thread
	glfwMakeContextCurrent(window);

	//	Initialise GLAD by getting address of OpenGL functions using passed GLFW
	// function
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		// Print error message and exit program with code -1
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Set OpenGL viewport size and location
	glViewport(0, 0, 800, 600);

	// Set callback function to run when window resized
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	/* ImGui Initialization */
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	// Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	// Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	// IF using Docking Branch
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	// Setup Platform/Renderer backends
	//	Second param install_callback=true will install GLFW callbacks and chain
	// to existing ones.
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	// Set up depth testing
	glEnable(GL_DEPTH_TEST);
	//	Change to wireframe (apply to front and back, draw triangles as lines,
	// to draw full, need GL_FULL)
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	// Enable blending to make textures transparent
	glEnable(GL_BLEND);
	// Set blending mode
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Create Shader object
	Shader ourShader("shader.vs", "shader.fs");

	//	Array with vertices for quad and texture coords on it (font texture is
	// 320x192, 32px letters, 10x6 letters, therefore coords are 0.16666666
	// between each letter on the y axis and 0.1 between on the x axis) (make a
	// converter from ASCII characters to coordinates)
	float quadVertices[] = {
		// positions       // texture coords
		0.5f, 0.5f, -0.5f, 0.1f, 0.16666666f, // top right
		0.5f, -0.5f, -0.5f, 0.1f, 0.0f, // bottom right
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // bottom left
		-0.5f, 0.5f, -0.5f, 0.0f, 0.16666666f // top left
	};
	// Indices that select vertices for the two triangles that make the
	// rectangle, reduces overhead
	unsigned int quadIndices[] = { // note that we start from 0!
		0, 1, 3, // first triangle
		1, 2, 3 // second triangle
	};

	// Quad position
	float xPos[10] = { 0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f };
	//	Texture offset x and y for letters of message (string length without end
	// character)
	float xTexOffset[64] = {0.0f};
	float yTexOffset[64] = {0.0f};
	// String message
	std::string message = "HELLO, THIS IS MY FIRST OPENGL PROGRAM! GREETS TO LEARN OPENGL! ";

	// ASCII to texture offset converter
	for (int i = 0; i < message.length(); i++) {
		if (message[i] >= 32 && message[i] <= 41) {
			xTexOffset[i] = -(32 - message[i]) * 0.1f;
			yTexOffset[i] = 0.83333333f;
		} else if (message[i] >= 42 && message[i] <= 51) {
			xTexOffset[i] = -(42 - message[i]) * 0.1f;
			yTexOffset[i] = 0.66666666f;
		} else if (message[i] >= 52 && message[i] <= 61) {
			xTexOffset[i] = -(52 - message[i]) * 0.1f;
			yTexOffset[i] = 0.5f;
		} else if (message[i] >= 62 && message[i] <= 71) {
			xTexOffset[i] = -(62 - message[i]) * 0.1f;
			yTexOffset[i] = 0.33333333f;
		} else if (message[i] >= 72 && message[i] <= 81) {
			xTexOffset[i] = -(72 - message[i]) * 0.1f;
			yTexOffset[i] = 0.16666666f;
		} else if (message[i] >= 82 && message[i] <= 91) {
			xTexOffset[i] = -(82 - message[i]) * 0.1f;
			yTexOffset[i] = 0.0f;
		} else {
			std::cout << "ERROR: UNSUPPORTED ASCII CHARACTER";
			exit(1);
		}
		// DEBUG PRINT OFFSETS
		//std::cout << "xTexOffset " << i << ": " << xTexOffset[i] << std::endl;
		//std::cout << "yTexOffset " << i << ": " << yTexOffset[i] << std::endl;
	}

	/* TEXTURE CREATION */
	// 1ST TEXTURE
	//	Flip image vertically as OpenGL has y coord 0.0 mean bottom while images
	// have it as top
	stbi_set_flip_vertically_on_load(true);
	//	Load a image using stb_image, width, height, and nrChannels are derived
	// from the image's resolution and color channels
	int width, height, nrChannels;
	unsigned char* data = stbi_load("assets/font.png", &width, &height, &nrChannels, 0);

	//	Create texture object, generate 1 texture and store in unsigned int
	// array (or just an unsigned int for 1 texture)
	unsigned int texture1;
	glGenTextures(1, &texture1);
	// Activate texture unit 0 (is by default activated)
	//	OpenGL supplies a minimum of 16 units, 0 to 15, are defined in order so
	// could do GL_TEXTURE0 + 8 to get GL_TEXTURE8
	glActiveTexture(GL_TEXTURE0);
	// Bind texture object for future configuration
	glBindTexture(GL_TEXTURE_2D, texture1);
	// Set texture scaling filters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// Set texture wrapping, S is x, T is Y
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Before generating texture, check if image exists
	if (data) {
		// Generate texture from image for bound texture object
		//	GL_TEXTURE_2D targets only the current bound 2d texture, 0 is mipmap
		// level, GL_RGB is color format to store texture in, width and height self
		// explanatory, 0 is always 0 due to some legacy stuff, GL_RGB is color
		// format of image, GL_UNSIGNED_BYTE is data type of image, and data is the
		// image's variable itself
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		// Also generate mipmaps for texture
		glGenerateMipmap(GL_TEXTURE_2D);
		// Set texture mipmap filtering (can it be different from texture's filtering?)
		// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	} else {
		// If image does not exist, print error
		std::cout << "Failed to load texture" << std::endl;
	}
	// Free memory that holds image
	stbi_image_free(data);

	/* VERTEX ARRAY OBJECT (stores state and data for drawing certain objects
	instead of having to copy vertices into buffer, set vertex attributes pointers,
	and use certain shader program everytime before drawing a different object) */
	// Create a VAO
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	//	Bind the VAO, then configure the VBO, attribute pointers, and shader
	// program afterwards, then unbind, rebinding when it needs to be used
	glBindVertexArray(VAO);

	// Create a VBO for geometry
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	// Bind VBO to the GL_ARRAY_BUFFER (is now set as the current buffer to use and modify)
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Copy vertex data in vertices array to buffer memory on GPU with GL_STATIC_DRAW mangement mode
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

	/* ELEMENT BUFFER OBJECT (stores indices in buffer like a VBO stores vertices) */
	// Create an EBO
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	// Bind EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	// Copy index data to buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW);

	//	Tell OpenGL how to deal with vertex data from currently bound VBO (each
	// vertex has x,y,z coords represented by 4 bytes each)
	//	0 is location of vertex attribute to configure (as specificied in vertex
	// shader), 3 is num of values inside (for x,y,z coords in vec3), GL_FLOAT as
	// coords are floats, GL_FALSE as not using integer data types, 6*sizeof(float)
	// is space between vertex attributes, (void*)0 is 0 cast as a void pointer
	// where position data begins (represents offset)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	// Enable vertex attribute at location 0 (as seen in vertex shader)
	glEnableVertexAttribArray(0);
	// Tell OpenGL how to deal with vertex data from currently bound VBO for texture coords
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	// Enable vertex attribute at location 1
	glEnableVertexAttribArray(1);

	// Use compiled and linked shader program in created Shader object
	ourShader.use();
	// Set texture unit that each shader sampler belongs to
	ourShader.setInt("texture1", 0);

	/* MikMod player code */
	MODULE* module;
	// Register all the drivers
	MikMod_RegisterAllDrivers();
	// Register all the module loaders
	MikMod_RegisterAllLoaders();
	// Initialize MikMod
	md_mode |= DMODE_SOFT_MUSIC;
	// If MikMod did not initialize sound, print error and exit
	if (MikMod_Init("")) {
		std::cerr << "Could not initialize sound, reason: " << MikMod_strerror(MikMod_errno) << std::endl;
		return 1;
	}
	//	Load module, specifying its name, channel count, and a number that
	// determines searching for hidden parts of a song
	module = Player_Load("assets/music.xm", 64, 0);
	// Make song loop
	module->wrap = true;
	// If it did not load, print error and exit
	if (!module) {
		std::cerr << "Could not load module, reason: " << MikMod_strerror(MikMod_errno) << std::endl;
		return 1;
	}
	// Start module
	Player_Start(module);

	// ImGui window settings
	ImGuiWindowFlags statsFlags = ImGuiWindowFlags_NoDecoration |
		ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoFocusOnAppearing |
		ImGuiWindowFlags_NoNav;
	statsFlags |= ImGuiWindowFlags_NoMove;
	bool active = true;

	// Text Offset counter, quad array, and loop bool
	int textOffsetCount = 0;
	int textOffsetQuadArray[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	bool textOffsetLoop = 0;

	// Frame counter
	int frameCount = 0;
	// Fps for ImGui
	float fpsGUI = 0;
	// Previous time to check if second has passed
	float previousTime = glfwGetTime();

	// Render loop while window isn't set to close
	while (!glfwWindowShouldClose(window)) {
		/* Run delta time calculations here */
		// Increment frame counter
		frameCount++;

		// Get current time
		float currentFrame = glfwGetTime();
		//	Check delta time between frame and update lastFrame to this frame
		// for next time
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Display frame rate each second in terminal
		if (currentFrame - previousTime >= 1.0f) {
			// DEBUG FRAME RATE ON TERMINAL
			//std::cout << 1 / deltaTime << std::endl;
			fpsGUI = 1 / deltaTime; // Update fps for ImGui every second
			frameCount = 0;
			previousTime = currentFrame;
		}

		// Check for events, update window state, and run callbacks
		glfwPollEvents();

		// Update MikMod
		MikMod_Update();

		/* Debug GUI window */
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		// Set position of window
		ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always, ImVec2(0.0f, 0.0f));
		// Start window
		ImGui::Begin("Stats", &active, statsFlags);
		// Add text to it (frame rate)
		ImGui::Text("FPS: %f", fpsGUI);
		// End window
		ImGui::End();

		/* Input commands */
		// Process input
		processInput(window);

		/* Rendering commands */
		// Set color to clear screen with
		glClearColor(0.0f, 0.0f, 0.3f, 1.0f);
		// Clear color buffer and depth buffer bits
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Activate and bind texture 1
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);

		// Use compiled and linked shader program in created Shader object
		ourShader.use();

		/* Matrix multiplication to go through each space(local, global, view, clip, screen) */
		// Create projection matrix (view->clip) (uses perspective transformation)
		// Rarely changes, don't need to do each frame (need if changing fov)
		//	1st is FOV in radians, 2nd is aspect ratio, 3rd and 4th are near and far
		// planes of frustrum (visible space), FOV affects degree of what is seen,
		// aspect ratio makes view wider (but not higher)
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), 800.0f / 600.0f, 0.1f, 100.0f);
		// Create projection matrix uniform for shaders
		ourShader.setMat4("projection", projection);

		//	Create view matrix (world->view), move scene inversed to where we
		// want camera to move (uses translate) (-x goes right, -y goes up, -z goes back)
		// Create lookAt matrix using vectors defined globally at start (are changed via input)
		glm::mat4 view = glm::mat4(1.0f);
		view = glm::lookAt(camera.Position, camera.Position + camera.Front, camera.Up);
		// Create uniform for model/view/projection matrix to be used in shaders
		// Also remember to create uniforms after shader has been set up
		// Send each matrix to shader each frame (usually)
		ourShader.setMat4("view", view);

		// Bind the VAO
		glBindVertexArray(VAO);
		//	For loop that renders 10 quad objects (CHANGE IT TO INSTANCED
		// RENDERING IN FUTURE VERSION, FASTER)
		for (int i = 0; i < 10; i++) {
			// Create model matrix to convert local space to world space
			// Change model matrix for each quad
			glm::mat4 model = glm::mat4(1.0f);
			// Calculate xPos of quads
			if (xPos[i] < -5.0f) {
				// Reset xPos of quad
				xPos[i] = 5.0f;
				// Count character offsets and set them
				textOffsetCount += 1;
				textOffsetQuadArray[i] = 9 + textOffsetCount;
				// If it has looped, start from 0
				if (textOffsetLoop == 1) {
					textOffsetQuadArray[i] = textOffsetCount;
				}
			} else {
				// Move xPos of quad left with deltaTime
				xPos[i] = xPos[i] - 3.0f * deltaTime;
			}
			// DEBUG QUAD POSITION
			//std::cout << "Quad " << i << ": " << xPos[i] << std::endl;
			// Translate quad to some calculated location each frame
			model = glm::translate(model, glm::vec3(xPos[i], sin((float)glfwGetTime() * 4 + (float)i * 0.66666666), -4.6f));
			// Set model uniform for each quad
			ourShader.setMat4("model", model);
			// Upload uniforms for texture offsets
			ourShader.setFloat("xTexOffset", xTexOffset[textOffsetQuadArray[i]]);
			ourShader.setFloat("yTexOffset", yTexOffset[textOffsetQuadArray[i]]);
			// DEBUG TEXTURE OFFSET
			//std::cout << "QUADOFFSET " << i << ": " << textOffsetQuadArray[i] << std::endl;

			// Reset count once reached end of string
			//	(IF STRING IS 25 INCLUDING END CHARACTER, IT'S 24 WITHOUT END,
			// 23 FROM 0, USE 23)
			if (textOffsetQuadArray[i] > 63) {
				textOffsetQuadArray[i] = 0;
				textOffsetCount = 0;
				textOffsetLoop = 1;
			}

			//	Draw triangles using data in vertex and index arrays, drawing 6
			// vertices, saying that the indices are the unsigned int type, and
			// specify offset at 0
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

		// Rendering ImGui
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Swap screen buffers (double buffering)
		glfwSwapBuffers(window);
	}

	/* End of program cleanup */
	// Deallocate VAO/VBO/EBO resources
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	// Terminate GLFW, getting rid of all its allocations
	glfwTerminate();

	// Terminate MikMod
	Player_Stop();
	Player_Free(module);
	MikMod_Exit();

	// Terminate ImGui
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	// Exit with code 0
	return 0;
}