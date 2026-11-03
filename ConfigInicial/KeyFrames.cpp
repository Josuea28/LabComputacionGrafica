//Previo 12
//Juarez Osorio Josue Alexis 
//Fecha de entrega: 03 de noviembre de 2026
//No. cuenta: 320083125

#include <iostream>
#include <cmath>
#include <cstdio>
#include <cstdlib>

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
void Animation();
void startSitAnimation();
void updateSitAnimation();
void resetDogPose();

// Window dimensions
const GLuint WIDTH = 1600, HEIGHT = 900;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.0f,2.0f, 0.0f),
	glm::vec3(0.0f,0.0f, 0.0f),
	glm::vec3(0.0f,0.0f,  0.0f),
	glm::vec3(0.0f,0.0f, 0.0f)
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


glm::vec3 Light1 = glm::vec3(0);
//Animacion del perro
float rotBall = 0.0f;
float rotDog = 0.0f;
float dogPosX = 0.0f;
float dogPosY = 0.0f;
float dogPosZ = 0.0f;

// Angulos independientes de cada parte del perro
float bodyRotX = 0.0f;
float frontLeftLeg = 0.0f;
float frontRightLeg = 0.0f;
float backLeftLeg = 0.0f;
float backRightLeg = 0.0f;
float head = 0.0f;
float tail = 0.0f;
float tailPosY = 0.0f;

// Deltatime
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// Animacion automatica de sentado y saludo
struct DogPose {
	float dogPosY;
	float bodyRotX;
	float frontLeftLeg;
	float frontRightLeg;
	float backLeftLeg;
	float backRightLeg;
	float head;
	float tail;
	float tailPosY;
};

DogPose phaseStart;
DogPose phaseEnd;
bool sitAnimation = false;
bool phaseInitialized = false;
int sitPhase = 0;
float phaseTimer = 0.0f;
float phaseDuration = 0.0f;

//KeyFrames
#define MAX_FRAMES 9
int i_max_steps = 190;
int i_curr_steps = 0;
typedef struct _frame {
	float rotDog;
	float rotDogInc;
	float dogPosX;
	float dogPosY;
	float dogPosZ;
	float incX;
	float incY;
	float incZ;

	float bodyRotX;
	float bodyRotXInc;
	float frontLeftLeg;
	float frontLeftLegInc;
	float frontRightLeg;
	float frontRightLegInc;
	float backLeftLeg;
	float backLeftLegInc;
	float backRightLeg;
	float backRightLegInc;
	float head;
	float headInc;
	float tail;
	float tailInc;
	float tailPosY;
	float tailPosYInc;
}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;			//introducir datos
bool play = false;
int playIndex = 0;

DogPose getDogPose()
{
	DogPose pose;
	pose.dogPosY = dogPosY;
	pose.bodyRotX = bodyRotX;
	pose.frontLeftLeg = frontLeftLeg;
	pose.frontRightLeg = frontRightLeg;
	pose.backLeftLeg = backLeftLeg;
	pose.backRightLeg = backRightLeg;
	pose.head = head;
	pose.tail = tail;
	pose.tailPosY = tailPosY;
	return pose;
}

void setDogPose(DogPose pose)
{
	dogPosY = pose.dogPosY;
	bodyRotX = pose.bodyRotX;
	frontLeftLeg = pose.frontLeftLeg;
	frontRightLeg = pose.frontRightLeg;
	backLeftLeg = pose.backLeftLeg;
	backRightLeg = pose.backRightLeg;
	head = pose.head;
	tail = pose.tail;
	tailPosY = pose.tailPosY;
}

DogPose interpolatePose(DogPose a, DogPose b, float t)
{
	DogPose pose;
	pose.dogPosY = a.dogPosY + (b.dogPosY - a.dogPosY) * t;
	pose.bodyRotX = a.bodyRotX + (b.bodyRotX - a.bodyRotX) * t;
	pose.frontLeftLeg = a.frontLeftLeg + (b.frontLeftLeg - a.frontLeftLeg) * t;
	pose.frontRightLeg = a.frontRightLeg + (b.frontRightLeg - a.frontRightLeg) * t;
	pose.backLeftLeg = a.backLeftLeg + (b.backLeftLeg - a.backLeftLeg) * t;
	pose.backRightLeg = a.backRightLeg + (b.backRightLeg - a.backRightLeg) * t;
	pose.head = a.head + (b.head - a.head) * t;
	pose.tail = a.tail + (b.tail - a.tail) * t;
	pose.tailPosY = a.tailPosY + (b.tailPosY - a.tailPosY) * t;
	return pose;
}

void saveFrame(void)
{
	if (FrameIndex >= MAX_FRAMES) return;

	printf("frameindex %d\n", FrameIndex);

	KeyFrame[FrameIndex].dogPosX = dogPosX;
	KeyFrame[FrameIndex].dogPosY = dogPosY;
	KeyFrame[FrameIndex].dogPosZ = dogPosZ;
	KeyFrame[FrameIndex].rotDog = rotDog;
	KeyFrame[FrameIndex].bodyRotX = bodyRotX;
	KeyFrame[FrameIndex].frontLeftLeg = frontLeftLeg;
	KeyFrame[FrameIndex].frontRightLeg = frontRightLeg;
	KeyFrame[FrameIndex].backLeftLeg = backLeftLeg;
	KeyFrame[FrameIndex].backRightLeg = backRightLeg;
	KeyFrame[FrameIndex].head = head;
	KeyFrame[FrameIndex].tail = tail;
	KeyFrame[FrameIndex].tailPosY = tailPosY;

	FrameIndex++;
}

void resetElements(void)
{
	dogPosX = KeyFrame[0].dogPosX;
	dogPosY = KeyFrame[0].dogPosY;
	dogPosZ = KeyFrame[0].dogPosZ;
	rotDog = KeyFrame[0].rotDog;
	bodyRotX = KeyFrame[0].bodyRotX;
	frontLeftLeg = KeyFrame[0].frontLeftLeg;
	frontRightLeg = KeyFrame[0].frontRightLeg;
	backLeftLeg = KeyFrame[0].backLeftLeg;
	backRightLeg = KeyFrame[0].backRightLeg;
	head = KeyFrame[0].head;
	tail = KeyFrame[0].tail;
	tailPosY = KeyFrame[0].tailPosY;
}

void interpolation(void)
{
	KeyFrame[playIndex].incX = (KeyFrame[playIndex + 1].dogPosX - KeyFrame[playIndex].dogPosX) / i_max_steps;
	KeyFrame[playIndex].incY = (KeyFrame[playIndex + 1].dogPosY - KeyFrame[playIndex].dogPosY) / i_max_steps;
	KeyFrame[playIndex].incZ = (KeyFrame[playIndex + 1].dogPosZ - KeyFrame[playIndex].dogPosZ) / i_max_steps;
	KeyFrame[playIndex].rotDogInc = (KeyFrame[playIndex + 1].rotDog - KeyFrame[playIndex].rotDog) / i_max_steps;

	KeyFrame[playIndex].bodyRotXInc = (KeyFrame[playIndex + 1].bodyRotX - KeyFrame[playIndex].bodyRotX) / i_max_steps;
	KeyFrame[playIndex].frontLeftLegInc = (KeyFrame[playIndex + 1].frontLeftLeg - KeyFrame[playIndex].frontLeftLeg) / i_max_steps;
	KeyFrame[playIndex].frontRightLegInc = (KeyFrame[playIndex + 1].frontRightLeg - KeyFrame[playIndex].frontRightLeg) / i_max_steps;
	KeyFrame[playIndex].backLeftLegInc = (KeyFrame[playIndex + 1].backLeftLeg - KeyFrame[playIndex].backLeftLeg) / i_max_steps;
	KeyFrame[playIndex].backRightLegInc = (KeyFrame[playIndex + 1].backRightLeg - KeyFrame[playIndex].backRightLeg) / i_max_steps;
	KeyFrame[playIndex].headInc = (KeyFrame[playIndex + 1].head - KeyFrame[playIndex].head) / i_max_steps;
	KeyFrame[playIndex].tailInc = (KeyFrame[playIndex + 1].tail - KeyFrame[playIndex].tail) / i_max_steps;
	KeyFrame[playIndex].tailPosYInc = (KeyFrame[playIndex + 1].tailPosY - KeyFrame[playIndex].tailPosY) / i_max_steps;
}

void resetDogPose()
{
	dogPosX = 0.0f;
	dogPosY = 0.0f;
	dogPosZ = 0.0f;
	rotDog = 0.0f;
	bodyRotX = 0.0f;
	frontLeftLeg = 0.0f;
	frontRightLeg = 0.0f;
	backLeftLeg = 0.0f;
	backRightLeg = 0.0f;
	head = 0.0f;
	tail = 0.0f;
	tailPosY = 0.0f;
	sitAnimation = false;
	phaseInitialized = false;
	play = false;
}

void startSitAnimation()
{
	// Comienza desde la posicion de pie para que la transicion sea clara.
	bodyRotX = 0.0f;
	dogPosY = 0.0f;
	frontLeftLeg = 0.0f;
	frontRightLeg = 0.0f;
	backLeftLeg = 0.0f;
	backRightLeg = 0.0f;
	head = 0.0f;
	tail = 0.0f;
	tailPosY = 0.0f;

	play = false;
	sitAnimation = true;
	sitPhase = 0;
	phaseInitialized = false;
	phaseTimer = 0.0f;
}

void updateSitAnimation()
{
	if (sitPhase == 2)
	{
		// Pequenos movimientos de la pata derecha para simular el saludo.
		phaseTimer += deltaTime;
		frontRightLeg = -55.0f + 6.0f * sin(phaseTimer * 10.0f);
		tail = 155.0f + 2.0f * sin(phaseTimer * 5.0f);

		if (phaseTimer >= 1.4f)
		{
			frontRightLeg = -55.0f;
			tail = 155.0f;
			sitAnimation = false;
			sitPhase = 0;
			phaseInitialized = false;
		}
		return;
	}

	if (!phaseInitialized)
	{
		phaseStart = getDogPose();
		phaseEnd = phaseStart;
		phaseTimer = 0.0f;

		if (sitPhase == 0)
		{
			// Baja la parte trasera y recoge las patas posteriores.
			phaseEnd.dogPosY = -0.04f;
			phaseEnd.bodyRotX = -18.0f;
			phaseEnd.frontLeftLeg = -20.0f;
			phaseEnd.frontRightLeg = 20.0f;
			phaseEnd.backLeftLeg = -40.0f;
			phaseEnd.backRightLeg = 40.0f;
			phaseEnd.tail = 155.0f;
			phaseEnd.tailPosY = -0.08f;
			phaseDuration = 1.25f;
		}
		else if (sitPhase == 1)
		{
			// Extiende la pata delantera derecha hacia el frente.
			phaseEnd.frontRightLeg = -55.0f;
			phaseDuration = 0.65f;
		}
		phaseInitialized = true;
	}

	phaseTimer += deltaTime;
	float t = phaseTimer / phaseDuration;
	if (t > 1.0f) t = 1.0f;
	float smooth = t * t * (3.0f - 2.0f * t);
	setDogPose(interpolatePose(phaseStart, phaseEnd, smooth));

	if (phaseTimer >= phaseDuration)
	{
		setDogPose(phaseEnd);
		sitPhase++;
		phaseTimer = 0.0f;
		phaseInitialized = false;
	}
}



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
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Previo12 - Juarez Josue", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

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


	//models
	Model DogBody((char*)"Models/DogBody.obj");
	Model HeadDog((char*)"Models/HeadDog.obj");
	Model DogTail((char*)"Models/TailDog.obj");
	Model F_RightLeg((char*)"Models/F_RightLegDog.obj");
	Model F_LeftLeg((char*)"Models/F_LeftLegDog.obj");
	Model B_RightLeg((char*)"Models/B_RightLegDog.obj");
	Model B_LeftLeg((char*)"Models/B_LeftLegDog.obj");
	Model Piso((char*)"Models/piso.obj");
	Model Ball((char*)"Models/ball.obj");


	//KeyFrames
	for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[i].dogPosX = 0;
		KeyFrame[i].dogPosY = 0;
		KeyFrame[i].dogPosZ = 0;
		KeyFrame[i].incX = 0;
		KeyFrame[i].incY = 0;
		KeyFrame[i].incZ = 0;
		KeyFrame[i].rotDog = 0;
		KeyFrame[i].rotDogInc = 0;
		KeyFrame[i].bodyRotX = 0;
		KeyFrame[i].bodyRotXInc = 0;
		KeyFrame[i].frontLeftLeg = 0;
		KeyFrame[i].frontLeftLegInc = 0;
		KeyFrame[i].frontRightLeg = 0;
		KeyFrame[i].frontRightLegInc = 0;
		KeyFrame[i].backLeftLeg = 0;
		KeyFrame[i].backLeftLegInc = 0;
		KeyFrame[i].backRightLeg = 0;
		KeyFrame[i].backRightLegInc = 0;
		KeyFrame[i].head = 0;
		KeyFrame[i].headInc = 0;
		KeyFrame[i].tail = 0;
		KeyFrame[i].tailInc = 0;
		KeyFrame[i].tailPosY = 0;
		KeyFrame[i].tailPosYInc = 0;
	}


	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO, EBO;
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

	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);


	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();
		Animation();

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// OpenGL options
		glEnable(GL_DEPTH_TEST);


		glm::mat4 modelTemp = glm::mat4(1.0f); //Temp



		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();

		glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "specular"),1);

		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.6f, 0.6f, 0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.6f, 0.6f, 0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.3f, 0.3f, 0.3f);


		// Point light 1
		glm::vec3 lightColor;
		lightColor.x = abs(sin(glfwGetTime() * Light1.x));
		lightColor.y = abs(sin(glfwGetTime() * Light1.y));
		lightColor.z = sin(glfwGetTime() * Light1.z);


		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 0.2f, 0.2f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.045f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.075f);


		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.2f, 0.2f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.2f, 0.2f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.3f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.0f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(18.0f)));


		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 5.0f);

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
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Piso.Draw(lightingShader);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		//Body
		modelTemp = model = glm::translate(model, glm::vec3(dogPosX, dogPosY, dogPosZ));
		modelTemp = model = glm::rotate(model, glm::radians(rotDog), glm::vec3(0.0f, 1.0f, 0.0f));
		modelTemp = model = glm::rotate(model, glm::radians(bodyRotX), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		DogBody.Draw(lightingShader);
		//Head
		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.0f, 0.093f, 0.208f));
		model = glm::rotate(model, glm::radians(head), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		HeadDog.Draw(lightingShader);
		//Tail 
		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.0f, 0.026f + tailPosY, -0.288f));
		model = glm::rotate(model, glm::radians(tail), glm::vec3(0.0f, 0.0f, -1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		DogTail.Draw(lightingShader);
		//Front Left Leg
		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.112f, -0.044f, 0.074f));
		model = glm::rotate(model, glm::radians(frontLeftLeg), glm::vec3(-1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		F_LeftLeg.Draw(lightingShader);
		//Front Right Leg
		model = modelTemp;
		model = glm::translate(model, glm::vec3(-0.111f, -0.055f, 0.074f));
		model = glm::rotate(model, glm::radians(frontRightLeg), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		F_RightLeg.Draw(lightingShader);
		//Back Left Leg
		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.082f, -0.046, -0.218));
		model = glm::rotate(model, glm::radians(backLeftLeg), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		B_LeftLeg.Draw(lightingShader);
		//Back Right Leg
		model = modelTemp;
		model = glm::translate(model, glm::vec3(-0.083f, -0.057f, -0.231f));
		model = glm::rotate(model, glm::radians(backRightLeg), glm::vec3(-1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		B_RightLeg.Draw(lightingShader);


		model = glm::mat4(1);
		glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 1);
		model = glm::rotate(model, glm::radians(rotBall), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Ball.Draw(lightingShader);
		glDisable(GL_BLEND);  //Desactiva el canal alfa 
		glBindVertexArray(0);


		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Draw the light object (using light's vertex attributes)

		model = glm::mat4(1);
		model = glm::translate(model, pointLightPositions[0]);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);


		// Swap the screen buffers
		glfwSwapBuffers(window);
	}




	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();



	return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{
	// Movimiento manual del perro
	if (keys[GLFW_KEY_2])
		rotDog += 60.0f * deltaTime;

	if (keys[GLFW_KEY_3])
		rotDog -= 60.0f * deltaTime;

	if (keys[GLFW_KEY_H])
		dogPosZ += 0.5f * deltaTime;

	if (keys[GLFW_KEY_Y])
		dogPosZ -= 0.5f * deltaTime;

	if (keys[GLFW_KEY_G])
		dogPosX -= 0.5f * deltaTime;

	if (keys[GLFW_KEY_J])
		dogPosX += 0.5f * deltaTime;

	// Movimiento de la camara
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
		camera.ProcessKeyboard(FORWARD, deltaTime);

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
		camera.ProcessKeyboard(BACKWARD, deltaTime);

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
		camera.ProcessKeyboard(LEFT, deltaTime);

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
		camera.ProcessKeyboard(RIGHT, deltaTime);

	// Movimiento de la luz con teclas numericas
	if (keys[GLFW_KEY_7]) pointLightPositions[0].x -= 0.5f * deltaTime;
	if (keys[GLFW_KEY_8]) pointLightPositions[0].x += 0.5f * deltaTime;
	if (keys[GLFW_KEY_9]) pointLightPositions[0].y += 0.5f * deltaTime;
	if (keys[GLFW_KEY_0]) pointLightPositions[0].y -= 0.5f * deltaTime;
	if (keys[GLFW_KEY_U]) pointLightPositions[0].z -= 0.5f * deltaTime;
	if (keys[GLFW_KEY_I]) pointLightPositions[0].z += 0.5f * deltaTime;
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (action == GLFW_PRESS)
	{
		if (key == GLFW_KEY_ESCAPE)
			glfwSetWindowShouldClose(window, GL_TRUE);

		// Guarda la pose actual como un nuevo KeyFrame.
		if (key == GLFW_KEY_K)
		{
			if (FrameIndex < MAX_FRAMES)
				saveFrame();
		}

		// Reproduce los KeyFrames guardados.
		if (key == GLFW_KEY_L)
		{
			if (!play && FrameIndex > 1)
			{
				sitAnimation = false;
				resetElements();
				playIndex = 0;
				i_curr_steps = 0;
				interpolation();
				play = true;
			}
			else if (play)
				play = false;
		}

		// Inicia la animacion automatica: sentarse y saludar con la pata derecha.
		if (key == GLFW_KEY_P)
			startSitAnimation();

		// Regresa al perro a la posicion de pie.
		if (key == GLFW_KEY_R)
			resetDogPose();

		// Borra los KeyFrames guardados.
		if (key == GLFW_KEY_C)
		{
			FrameIndex = 0;
			play = false;
		}

		// Enciende o apaga la luz.
		if (key == GLFW_KEY_SPACE)
		{
			active = !active;
			if (active)
				Light1 = glm::vec3(0.2f, 0.8f, 1.0f);
			else
				Light1 = glm::vec3(0.0f);
		}
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

void Animation()
{
	if (play)
	{
		if (i_curr_steps >= i_max_steps)
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)
			{
				printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else
			{
				i_curr_steps = 0;
				interpolation();
			}
		}
		else
		{
			dogPosX += KeyFrame[playIndex].incX;
			dogPosY += KeyFrame[playIndex].incY;
			dogPosZ += KeyFrame[playIndex].incZ;
			rotDog += KeyFrame[playIndex].rotDogInc;
			bodyRotX += KeyFrame[playIndex].bodyRotXInc;
			frontLeftLeg += KeyFrame[playIndex].frontLeftLegInc;
			frontRightLeg += KeyFrame[playIndex].frontRightLegInc;
			backLeftLeg += KeyFrame[playIndex].backLeftLegInc;
			backRightLeg += KeyFrame[playIndex].backRightLegInc;
			head += KeyFrame[playIndex].headInc;
			tail += KeyFrame[playIndex].tailInc;
			tailPosY += KeyFrame[playIndex].tailPosYInc;
			i_curr_steps++;
		}
	}
	else if (sitAnimation)
	{
		updateSitAnimation();
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