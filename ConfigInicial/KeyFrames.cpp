// Practica12
// Juarez Osorio Josue Alexis
// Fecha de entrega: 08 de noviembre de 2026
// No. cuenta: 320083125

#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdio>
#include <string>

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

// Window dimensions
const GLuint WIDTH = 1600, HEIGHT = 900;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(0.0f, 0.10f, 2.2f));
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
//Anim
float rotBall = 0.0f;
bool mostrarPelota = false; // Ocultar pelota para que no distraiga de la animacion
float rotDog = 0.0f;
int dogAnim = 0;
float FLegs = 0.0f;
float FRightLegs = 0.0f;
float RLegs = 0.0f;
float head = 0.0f;
float tail = 0.0f;
float rotDogX = 0.0f;
float rotDogZ = 0.0f;
float headX = 0.0f;


// KeyFrames
float dogPosX = 0.0f, dogPosY = 0.0f, dogPosZ = 0.0f;

#define MAX_FRAMES 9
const float FRAME_DURATION = 1.0f; // Segundos entre fotogramas

typedef struct _frame {
	float rotDog, rotDogInc;
	float dogPosX, dogPosY, dogPosZ;
	float incX, incY, incZ;
	float head, headInc;
	float tail, tailInc;
	float FLegs, FLegsInc;
	float FRightLegs, FRightLegsInc;
	float RLegs, RLegsInc;
	float rotDogX, rotDogXInc;
	float rotDogZ, rotDogZInc;
	float headX, headXInc;
} FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;
bool play = false;
int playIndex = 0;
double frameStartTime = 0.0;

void saveFrame(void)
{
	if (FrameIndex >= MAX_FRAMES) {
		printf("No se pueden guardar mas de %d KeyFrames.\n", MAX_FRAMES);
		return;
	}

	FRAME& f = KeyFrame[FrameIndex];
	f.dogPosX = dogPosX;
	f.dogPosY = dogPosY;
	f.dogPosZ = dogPosZ;
	f.rotDog = rotDog;
	f.rotDogX = rotDogX;
	f.rotDogZ = rotDogZ;
	f.head = head;
	f.headX = headX;
	f.tail = tail;
	f.FLegs = FLegs;
	f.FRightLegs = FRightLegs;
	f.RLegs = RLegs;

	FrameIndex++;
	printf("KeyFrame guardado: %d/%d\n", FrameIndex, MAX_FRAMES);
}

void resetElements(void)
{
	if (FrameIndex == 0) return;

	dogPosX = KeyFrame[0].dogPosX;
	dogPosY = KeyFrame[0].dogPosY;
	dogPosZ = KeyFrame[0].dogPosZ;
	rotDog = KeyFrame[0].rotDog;
	rotDogX = KeyFrame[0].rotDogX;
	rotDogZ = KeyFrame[0].rotDogZ;
	head = KeyFrame[0].head;
	headX = KeyFrame[0].headX;
	tail = KeyFrame[0].tail;
	FLegs = KeyFrame[0].FLegs;
	FRightLegs = KeyFrame[0].FRightLegs;
	RLegs = KeyFrame[0].RLegs;
}

void interpolation(void)
{
	if (playIndex + 1 >= FrameIndex) return;

	FRAME& a = KeyFrame[playIndex];
	FRAME& b = KeyFrame[playIndex + 1];
	a.incX = b.dogPosX - a.dogPosX;
	a.incY = b.dogPosY - a.dogPosY;
	a.incZ = b.dogPosZ - a.dogPosZ;
	a.rotDogInc = b.rotDog - a.rotDog;
	a.rotDogXInc = b.rotDogX - a.rotDogX;
	a.rotDogZInc = b.rotDogZ - a.rotDogZ;
	a.headInc = b.head - a.head;
	a.headXInc = b.headX - a.headX;
	a.tailInc = b.tail - a.tail;
	a.FLegsInc = b.FLegs - a.FLegs;
	a.FRightLegsInc = b.FRightLegs - a.FRightLegs;
	a.RLegsInc = b.RLegs - a.RLegs;
}

// Agregar una postura a la animacion de ejemplo.
void addPose(float py, float rz, float h, float hx, float t,
	float fl, float fr, float rl)
{
	if (FrameIndex >= MAX_FRAMES) return;
	FRAME& f = KeyFrame[FrameIndex++];
	f.dogPosX = 0.0f;
	f.dogPosY = py;
	f.dogPosZ = 0.0f;
	f.rotDog = 0.0f;
	f.rotDogX = 0.0f;
	f.rotDogZ = rz;
	f.head = h;
	f.headX = hx;
	f.tail = t;
	f.FLegs = fl;
	f.FRightLegs = fr;
	f.RLegs = rl;
}

void defaultAnimation(void)
{
	play = false;
	playIndex = 0;
	FrameIndex = 0;

	// El perro comienza de pie, se acuesta y saluda con una pata.
	// La altura disminuye gradualmente durante la caida.
	// addPose(alturaY, rotacionZ, cabeza, cabezaX, cola,
	//         pataDelanteraIzq, pataDelanteraDer, patasTraseras)
	addPose(0.000f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	addPose(-0.030f, 20.0f, -5.0f, 0.0f, -5.0f, 15.0f, 15.0f, -5.0f);
	addPose(-0.090f, 55.0f, -10.0f, 5.0f, -15.0f, 25.0f, 25.0f, -15.0f);
	addPose(-0.160f, 95.0f, -15.0f, 8.0f, -10.0f, 10.0f, 10.0f, -25.0f);
	addPose(-0.160f, 95.0f, -15.0f, 8.0f, -10.0f, 10.0f, 10.0f, -25.0f);
	addPose(-0.160f, 95.0f, -12.0f, 8.0f, -10.0f, 10.0f, 95.0f, -25.0f);
	addPose(-0.160f, 95.0f, -15.0f, 8.0f, -10.0f, 10.0f, 30.0f, -25.0f);
	addPose(-0.160f, 95.0f, -12.0f, 8.0f, -10.0f, 10.0f, 95.0f, -25.0f);
	addPose(-0.160f, 95.0f, -15.0f, 8.0f, -10.0f, 10.0f, 10.0f, -25.0f);

	resetElements();
	printf("Animacion preparada: el perro se acuesta y saluda.\n");
}


// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

// Guardar los KeyFrames en un archivo de texto
void saveAnimationToFile(const char* filename)
{
	if (FrameIndex < 2) {
		printf("Se necesitan al menos dos KeyFrames para guardar.\n");
		return;
	}

	std::ofstream outFile(filename);
	if (!outFile.is_open()) {
		printf("Error: no se pudo guardar %s\n", filename);
		return;
	}

	outFile.precision(9);
	outFile << "REDDOG_V2" << "\n";
	outFile << FrameIndex << "\n";
	for (int i = 0; i < FrameIndex; i++) {
		FRAME& f = KeyFrame[i];
		outFile << f.dogPosX << " " << f.dogPosY << " " << f.dogPosZ << " "
			<< f.rotDog << " " << f.rotDogX << " " << f.rotDogZ << " "
			<< f.head << " " << f.headX << " " << f.tail << " "
			<< f.FLegs << " " << f.FRightLegs << " " << f.RLegs << "\n";
	}

	outFile.close();
	if (outFile.fail()) {
		printf("Error al terminar de escribir %s\n", filename);
	}
	else {
		printf("Animacion guardada en %s (%d KeyFrames).\n", filename, FrameIndex);
	}
}

// Recuperar una animacion guardada en el archivo de texto
void loadAnimationFromFile(const char* filename)
{
	std::ifstream inFile(filename);
	if (!inFile.is_open()) {
		printf("Error: no se encontro el archivo %s\n", filename);
		return;
	}

	std::string version;
	int count = 0;
	if (!(inFile >> version >> count) || version != "REDDOG_V2" ||
		count < 2 || count > MAX_FRAMES) {
		printf("Error: formato o cantidad de KeyFrames no valida.\n");
		return;
	}

	FRAME loaded[MAX_FRAMES] = {};
	for (int i = 0; i < count; i++) {
		FRAME& f = loaded[i];
		if (!(inFile >> f.dogPosX >> f.dogPosY >> f.dogPosZ
			>> f.rotDog >> f.rotDogX >> f.rotDogZ
			>> f.head >> f.headX >> f.tail
			>> f.FLegs >> f.FRightLegs >> f.RLegs)) {
			printf("Error: archivo de animacion incompleto.\n");
			return;
		}
	}

	play = false;
	playIndex = 0;
	FrameIndex = count;
	for (int i = 0; i < count; i++) KeyFrame[i] = loaded[i];
	resetElements();
	printf("Animacion cargada desde %s (%d KeyFrames).\n", filename, FrameIndex);
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
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica12 - Juarez Josue", nullptr, nullptr);

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
		KeyFrame[i].rotDogZ = 0;
		KeyFrame[i].rotDogInc = 0;
		KeyFrame[i].head = 0;
		KeyFrame[i].headInc = 0;
		KeyFrame[i].headX = 0;
		KeyFrame[i].headXInc = 0;
		KeyFrame[i].rotDogX = 0;
		KeyFrame[i].rotDogXInc = 0;
		KeyFrame[i].tail = 0;
		KeyFrame[i].tailInc = 0;
		KeyFrame[i].FLegs = 0;
		KeyFrame[i].FRightLegs = 0;
		KeyFrame[i].FRightLegsInc = 0;
		KeyFrame[i].FLegsInc = 0;
		KeyFrame[i].RLegs = 0;
		KeyFrame[i].RLegsInc = 0;
	}

	defaultAnimation();


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
		modelTemp = model = glm::rotate(model, glm::radians(rotDogX), glm::vec3(1.0f, 0.0f, 0.0f));
		modelTemp = model = glm::rotate(model, glm::radians(rotDogZ), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		DogBody.Draw(lightingShader);
		//Head
		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.0f, 0.093f, 0.208f));
		model = glm::rotate(model, glm::radians(head), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(headX), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		HeadDog.Draw(lightingShader);
		//Tail 
		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.0f, 0.026f, -0.288f));
		model = glm::rotate(model, glm::radians(tail), glm::vec3(0.0f, 0.0f, -1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		DogTail.Draw(lightingShader);
		//Front Left Leg
		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.112f, -0.044f, 0.074f));
		model = glm::rotate(model, glm::radians(FLegs), glm::vec3(-1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		F_LeftLeg.Draw(lightingShader);
		//Front Right Leg
		model = modelTemp;
		model = glm::translate(model, glm::vec3(-0.111f, -0.055f, 0.074f));
		model = glm::rotate(model, glm::radians(FRightLegs), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		F_RightLeg.Draw(lightingShader);
		//Back Left Leg
		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.082f, -0.046, -0.218));
		model = glm::rotate(model, glm::radians(RLegs), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		B_LeftLeg.Draw(lightingShader);
		//Back Right Leg
		model = modelTemp;
		model = glm::translate(model, glm::vec3(-0.083f, -0.057f, -0.231f));
		model = glm::rotate(model, glm::radians(RLegs), glm::vec3(-1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		B_RightLeg.Draw(lightingShader);


		model = glm::mat4(1);
		glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 1);
		model = glm::rotate(model, glm::radians(rotBall), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		if (mostrarPelota) Ball.Draw(lightingShader);
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
	// Controles manuales del perro (desactivados durante la animacion).
	if (!play)
	{
		if (keys[GLFW_KEY_Z]) { rotDogZ += 0.5f; dogPosY += 0.0005f; headX -= 0.2f; }
		if (keys[GLFW_KEY_X]) { rotDogZ -= 0.5f; dogPosY -= 0.0005f; headX += 0.2f; }
		if (keys[GLFW_KEY_E]) { rotDogX += 0.5f; }
		if (keys[GLFW_KEY_R]) { rotDogX -= 0.5f; }
		if (keys[GLFW_KEY_6]) { tail += 0.5f; }
		if (keys[GLFW_KEY_7]) { tail -= 0.5f; }
		if (keys[GLFW_KEY_8]) { FLegs += 0.5f; }
		if (keys[GLFW_KEY_9]) { FLegs -= 0.5f; }
		if (keys[GLFW_KEY_I]) { FRightLegs += 0.5f; }
		if (keys[GLFW_KEY_M]) { FRightLegs -= 0.5f; }
		if (keys[GLFW_KEY_O]) { RLegs += 0.5f; }
		if (keys[GLFW_KEY_P]) { RLegs -= 0.5f; }
		if (keys[GLFW_KEY_4]) { head += 0.5f; }
		if (keys[GLFW_KEY_5]) { head -= 0.5f; }
		if (keys[GLFW_KEY_2]) { rotDog += 0.5f; }
		if (keys[GLFW_KEY_3]) { rotDog -= 0.5f; }
		if (keys[GLFW_KEY_H]) { dogPosZ += 0.002f; }
		if (keys[GLFW_KEY_Y]) { dogPosZ -= 0.002f; }
		if (keys[GLFW_KEY_G]) { dogPosX -= 0.002f; }
		if (keys[GLFW_KEY_J]) { dogPosX += 0.002f; }
	}

	// Camara
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP]) camera.ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN]) camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT]) camera.ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]) camera.ProcessKeyboard(RIGHT, deltaTime);

	// Luces: teclas distintas a las del perro
	if (keys[GLFW_KEY_F5])  pointLightPositions[0].x += 0.01f;
	if (keys[GLFW_KEY_F6])  pointLightPositions[0].x -= 0.01f;
	if (keys[GLFW_KEY_F7])  pointLightPositions[0].y += 0.01f;
	if (keys[GLFW_KEY_F8])  pointLightPositions[0].y -= 0.01f;
	if (keys[GLFW_KEY_F9])  pointLightPositions[0].z += 0.01f;
	if (keys[GLFW_KEY_F10]) pointLightPositions[0].z -= 0.01f;
}

// Manejo de teclas: una accion por pulsacion
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key >= 0 && key < 1024) {
		if (action == GLFW_PRESS) keys[key] = true;
		if (action == GLFW_RELEASE) keys[key] = false;
	}

	if (action != GLFW_PRESS) return;

	if (key == GLFW_KEY_ESCAPE) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	else if (key == GLFW_KEY_L) {
		if (play) {
			play = false;
		}
		else if (FrameIndex >= 2) {
			resetElements();
			playIndex = 0;
			interpolation();
			frameStartTime = glfwGetTime();
			play = true;
			printf("Reproduciendo animacion...\n");
		}
		else {
			printf("Se necesitan al menos dos KeyFrames.\n");
		}
	}
	else if (key == GLFW_KEY_K) {
		if (!play) saveFrame();
	}
	else if (key == GLFW_KEY_C) {
		saveAnimationToFile("animacion_reddog.txt");
	}
	else if (key == GLFW_KEY_V) {
		loadAnimationFromFile("animacion_reddog.txt");
	}
	else if (key == GLFW_KEY_B) {
		defaultAnimation();
	}
	else if (key == GLFW_KEY_N) {
		play = false;
		FrameIndex = 0;
		printf("KeyFrames borrados. Usa K para guardar nuevas posturas.\n");
	}
	else if (key == GLFW_KEY_F4) {
		mostrarPelota = !mostrarPelota;
	}
	else if (key == GLFW_KEY_SPACE) {
		active = !active;
		Light1 = active ? glm::vec3(0.2f, 0.8f, 1.0f) : glm::vec3(0.0f);
	}
}

void Animation()
{
	if (!play || playIndex + 1 >= FrameIndex) return;

	float t = float((glfwGetTime() - frameStartTime) / FRAME_DURATION);
	if (t > 1.0f) t = 1.0f;

	FRAME& f = KeyFrame[playIndex];
	dogPosX = f.dogPosX + f.incX * t;
	dogPosY = f.dogPosY + f.incY * t;
	dogPosZ = f.dogPosZ + f.incZ * t;
	rotDog = f.rotDog + f.rotDogInc * t;
	rotDogX = f.rotDogX + f.rotDogXInc * t;
	rotDogZ = f.rotDogZ + f.rotDogZInc * t;
	head = f.head + f.headInc * t;
	headX = f.headX + f.headXInc * t;
	tail = f.tail + f.tailInc * t;
	FLegs = f.FLegs + f.FLegsInc * t;
	FRightLegs = f.FRightLegs + f.FRightLegsInc * t;
	RLegs = f.RLegs + f.RLegsInc * t;

	if (t >= 1.0f) {
		playIndex++;
		if (playIndex >= FrameIndex - 1) {
			play = false;
			printf("Animacion terminada.\n");
		}
		else {
			frameStartTime = glfwGetTime();
			interpolation();
		}
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