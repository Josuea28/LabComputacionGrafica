//Practica: 9
//Juarez Osorio Josue Alexis
//No. cuenta: 320083125
//Fecha de entrega: 18 de octubre de 2026
#include <iostream>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"


// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();

// Window dimensions
const GLuint WIDTH = 1600, HEIGHT = 900;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(0.0f, 3.0f, 9.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;

// Light attributes
glm::vec3 lightPos(-2.4f, 4.8f, -2.0f);
bool active = true;

// Transicion dia / noche
GLfloat dayTransition = 1.0f;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(-1.8f,2.10f, -1.3f),
	glm::vec3(0.9f,2.10f, -0.8f),
	glm::vec3(2.1f,2.20f, -1.4f),
	glm::vec3(0.0f,2.30f, -1.5f),
	glm::vec3(-3.0f,2.30f, -2.2f)
};

float vertices[] = {
	 -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	  0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	  0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	  0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	 -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	  0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	  0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	  0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	 -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	 -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

	 -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	 -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	 -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	 -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	 -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	 -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	  0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	  0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	  0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	  0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	  0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	  0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	 -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	  0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	  0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	  0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	 -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	 -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	 -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	  0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	 -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	 -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};



// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

int main()
{
	// Init GLFW
	glfwInit();

	// Set all the required options for GLFW
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Fuentes de luz", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwMaximizeWindow(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	// GLFW Options
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;

	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);



	Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");

	Model Dog((char*)"Models/RedDog.obj");
	Model Stadium((char*)"Models/Stadium.obj");

	Model Sun((char*)"Models/Sun.obj");
	Model Moon((char*)"Models/Moon.obj");
	Model Ball((char*)"Models/SoccerBall.obj");
	Model Cone((char*)"Models/Cone.obj");
	Model Trophy((char*)"Models/Trophy.obj");
	Model Scoreboard((char*)"Models/Scoreboard.obj");
	Model Flag((char*)"Models/CornerFlag.obj");



	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);



	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();

		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;


		// Check if any events have been activiated
		glfwPollEvents();

		DoMovement();



		// Posicion del Sol
		glm::vec3 sunPosition(-3.5f, -1.5f + (6.5f * dayTransition), -5.5f);

		// Posicion de la Luna
		glm::vec3 moonPosition(3.5f, 5.0f - (6.5f * dayTransition), -5.5f);



		// Posicion del marcador
		glm::vec3 scoreboardPosition(-2.4f, 1.72f, -4.6f);



		// Color del cielo
		glm::vec3 nightSky(0.02f, 0.03f, 0.10f);
		glm::vec3 daySky(0.25f, 0.42f, 0.65f);
		glm::vec3 skyColor = glm::mix(nightSky, daySky, dayTransition);



		// Clear the colorbuffer
		glClearColor(skyColor.x, skyColor.y, skyColor.z, 1.0f);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// OpenGL options
		glEnable(GL_DEPTH_TEST);



		//Load Model


		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();


		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");

		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);



		// Directional light
		// Sol y Luna

		glm::vec3 currentLightPosition = glm::mix(moonPosition, sunPosition, dayTransition);

		glm::vec3 direction = glm::normalize(glm::vec3(0.0f, 1.7f, 0.0f) - currentLightPosition);


		glm::vec3 nightAmbient(0.12f, 0.14f, 0.24f);
		glm::vec3 dayAmbient(0.50f, 0.50f, 0.45f);

		glm::vec3 nightDiffuse(0.18f, 0.23f, 0.40f);
		glm::vec3 dayDiffuse(1.0f, 0.90f, 0.70f);

		glm::vec3 nightSpecular(0.15f, 0.20f, 0.40f);
		glm::vec3 daySpecular(0.50f, 0.50f, 0.45f);


		glm::vec3 ambientColor = glm::mix(nightAmbient, dayAmbient, dayTransition);
		glm::vec3 diffuseColor = glm::mix(nightDiffuse, dayDiffuse, dayTransition);
		glm::vec3 specularColor = glm::mix(nightSpecular, daySpecular, dayTransition);


		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), direction.x, direction.y, direction.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), ambientColor.x, ambientColor.y, ambientColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), diffuseColor.x, diffuseColor.y, diffuseColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), specularColor.x, specularColor.y, specularColor.z);



		// Point light 1
		// Balon

		glm::vec3 lightColor;

		if (active)
		{
			lightColor = glm::vec3(0.85f, 0.90f, 1.0f);
		}
		else
		{
			lightColor = glm::vec3(0.0f);
		}

		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x * 0.10f, lightColor.y * 0.10f, lightColor.z * 0.10f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.70f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 1.80f);



		// Point light 2
		// Cono

		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), 0.05f, 0.02f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), 1.0f, 0.30f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), 1.0f, 0.45f, 0.10f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.70f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 1.80f);



		// Point light 3
		// Trofeo

		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].ambient"), 0.05f, 0.04f, 0.01f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].diffuse"), 1.0f, 0.70f, 0.15f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].specular"), 1.0f, 0.85f, 0.30f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].linear"), 0.70f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].quadratic"), 1.80f);



		// Point light 4
		// Perro

		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].ambient"), 0.05f, 0.04f, 0.03f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].diffuse"), 0.80f, 0.65f, 0.45f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].specular"), 0.80f, 0.70f, 0.50f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].linear"), 0.70f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].quadratic"), 1.80f);



		// Point light 5
		// Banderin

		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[4].position"), pointLightPositions[4].x, pointLightPositions[4].y, pointLightPositions[4].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[4].ambient"), 0.00f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[4].diffuse"), 0.05f, 0.80f, 0.75f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[4].specular"), 0.20f, 1.00f, 1.00f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[4].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[4].linear"), 0.70f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[4].quadratic"), 1.80f);



		// SpotLight
		// Marcador

		glm::vec3 scoreboardTarget = scoreboardPosition + glm::vec3(0.0f, 0.75f, 0.0f);

		glm::vec3 spotDirection = glm::normalize(scoreboardTarget - lightPos);

		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), spotDirection.x, spotDirection.y, spotDirection.z);

		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.08f, 0.10f, 0.15f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.90f, 0.95f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 1.0f, 1.0f, 1.0f);

		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.032f);

		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(10.0f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(18.0f)));



		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 16.0f);



		// Create camera transformations
		glm::mat4 view;

		view = camera.GetViewMatrix();



		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");

		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");

		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");



		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


		glm::mat4 model(1);



		//Carga de modelo 
		view = camera.GetViewMatrix();



		//Estadio
		model = glm::mat4(1);

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);

		Stadium.Draw(lightingShader);



		//Sol
		model = glm::mat4(1);

		model = glm::translate(model, sunPosition);

		model = glm::scale(model, glm::vec3(0.30f, 0.30f, 0.30f));

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);

		Sun.Draw(lightingShader);



		//Luna
		model = glm::mat4(1);

		model = glm::translate(model, moonPosition);

		model = glm::scale(model, glm::vec3(0.28f, 0.28f, 0.28f));

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);

		Moon.Draw(lightingShader);



		//Perro
		model = glm::mat4(1);

		model = glm::translate(model, glm::vec3(0.0f, 1.83f, -1.5f));

		model = glm::scale(model, glm::vec3(0.30f, 0.30f, 0.30f));

		//glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);

		Dog.Draw(lightingShader);

		//glDisable(GL_BLEND);  //Desactiva el canal alfa 



		//Balon
		model = glm::mat4(1);

		model = glm::translate(model, glm::vec3(-1.8f, 1.79f, -1.3f));

		model = glm::scale(model, glm::vec3(0.07f, 0.07f, 0.07f));

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);

		Ball.Draw(lightingShader);



		//Cono
		model = glm::mat4(1);

		model = glm::translate(model, glm::vec3(0.9f, 1.72f, -0.8f));

		model = glm::scale(model, glm::vec3(0.10f, 0.10f, 0.10f));

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);

		Cone.Draw(lightingShader);



		//Trofeo
		model = glm::mat4(1);

		model = glm::translate(model, glm::vec3(2.1f, 1.72f, -1.4f));

		model = glm::scale(model, glm::vec3(0.065f, 0.065f, 0.065f));

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);

		Trophy.Draw(lightingShader);



		//Banderin
		model = glm::mat4(1);

		model = glm::translate(model, glm::vec3(-3.0f, 1.72f, -2.2f));

		model = glm::scale(model, glm::vec3(0.22f, 0.22f, 0.22f));

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);

		Flag.Draw(lightingShader);



		//Marcador
		model = glm::mat4(1);

		model = glm::translate(model, scoreboardPosition);

		model = glm::scale(model, glm::vec3(0.28f, 0.28f, 0.28f));

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);

		Scoreboard.Draw(lightingShader);



		glBindVertexArray(0);



		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();


		// Get location objects for the matrices on the lamp shader
		modelLoc = glGetUniformLocation(lampShader.Program, "model");

		viewLoc = glGetUniformLocation(lampShader.Program, "view");

		projLoc = glGetUniformLocation(lampShader.Program, "projection");



		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));



		// Draw the SpotLight object
		model = glm::mat4(1);

		model = glm::translate(model, lightPos);

		model = glm::scale(model, glm::vec3(0.015f));

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(VAO);

		glDrawArrays(GL_TRIANGLES, 0, 36);



		// Draw the Point Light objects
		for (GLuint i = 0; i < 5; i++)
		{
			model = glm::mat4(1);

			model = glm::translate(model, pointLightPositions[i]);

			model = glm::scale(model, glm::vec3(0.012f)); // Make it a smaller cube

			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			glBindVertexArray(VAO);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}


		glBindVertexArray(0);



		// Swap the screen buffers
		glfwSwapBuffers(window);
	}



	// Terminate GLFW, clearing any resources allocated by GLFW.
	glDeleteVertexArrays(1, &VAO);

	glDeleteBuffers(1, &VBO);

	glfwTerminate();



	return 0;
}


// Moves/alters the camera positions based on user input
void DoMovement()
{

	// Camera controls
	if (keys[GLFW_KEY_W])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);

	}

	if (keys[GLFW_KEY_S])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);


	}

	if (keys[GLFW_KEY_A])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);


	}

	if (keys[GLFW_KEY_D])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);


	}



	// Dia
	if (keys[GLFW_KEY_UP])
	{
		dayTransition += 0.35f * deltaTime;

		if (dayTransition > 1.0f)
		{
			dayTransition = 1.0f;
		}
	}



	// Noche
	if (keys[GLFW_KEY_DOWN])
	{
		dayTransition -= 0.35f * deltaTime;

		if (dayTransition < 0.0f)
		{
			dayTransition = 0.0f;
		}
	}

}


// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{

	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}


	if (key >= 0 && key < 1024)
	{

		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}

		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}

	}



	// Enciende y apaga la luz del balon
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		active = !active;
	}

}


void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{

	if (firstMouse)
	{

		lastX = xPos;

		lastY = yPos;

		firstMouse = false;

	}


	GLfloat xOffset = xPos - lastX;

	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left


	lastX = xPos;

	lastY = yPos;


	camera.ProcessMouseMovement(xOffset, yOffset);
}