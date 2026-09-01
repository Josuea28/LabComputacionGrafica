#include<iostream>

//#define GLEW_STATIC

#include <GL/glew.h>

#include <GLFW/glfw3.h>

// Shaders
#include "Shader.h"

void resize(GLFWwindow* window, int width, int height);

const GLint WIDTH = 800, HEIGHT = 600;


int main() {
	glfwInit();
	//Verificaci�n de compatibilidad 
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 2 - Josue Juarez", NULL, NULL);
	glfwSetFramebufferSizeCallback(window, resize);
	
	//Verificaci�n de errores de creacion  ventana
	if (window== NULL) 
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;

	//Verificaci�n de errores de inicializaci�n de glew

	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialise GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Imprimimos informacin de OpenGL del sistema
	std::cout << "> Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "> Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "> Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "> SL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;


	// Define las dimensiones del viewport
	//glViewport(0, 0, screenWidth, screenHeight);

    Shader ourShader("Shader/core.vs", "Shader/core.frag");

	// Set up vertex data (and buffer(s)) and attribute pointers
	float vertices[] = {
		// 1. Punta del Hocico (Rojo)
		 0.40f,  0.20f, 0.0f,    0.9f, 0.1f, 0.2f,
		 0.25f,  0.28f, 0.0f,    0.9f, 0.1f, 0.2f,
		 0.28f,  0.15f, 0.0f,    0.9f, 0.1f, 0.2f,

		 // 2. Base del Hocico (Amarillo)
		  0.25f,  0.28f, 0.0f,    0.9f, 0.9f, 0.1f,
		  0.15f,  0.35f, 0.0f,    0.9f, 0.9f, 0.1f,
		  0.10f,  0.25f, 0.0f,    0.9f, 0.9f, 0.1f,

		  // 3. Conector del Hocico (Verde)
		   0.25f,  0.28f, 0.0f,    0.6f, 0.8f, 0.1f,
		   0.10f,  0.25f, 0.0f,    0.6f, 0.8f, 0.1f,
		   0.28f,  0.15f, 0.0f,    0.6f, 0.8f, 0.1f,

		   // 4. Frente de la Cabeza (Rosa Fuerte)
			0.15f,  0.35f, 0.0f,    1.0f, 0.4f, 0.6f,
			0.00f,  0.60f, 0.0f,    1.0f, 0.4f, 0.6f,
			0.10f,  0.25f, 0.0f,    1.0f, 0.4f, 0.6f,

			// 5. Pico de la Corona (Verde)
			 0.00f,  0.60f, 0.0f,    0.6f, 0.8f, 0.1f,
			-0.10f,  0.80f, 0.0f,    0.6f, 0.8f, 0.1f,
			-0.15f,  0.65f, 0.0f,    0.6f, 0.8f, 0.1f,

			// 6. Parte trasera de la Corona (Amarillo)
			-0.10f,  0.80f, 0.0f,    0.9f, 0.9f, 0.1f,
			-0.25f,  0.70f, 0.0f,    0.9f, 0.9f, 0.1f,
			-0.15f,  0.65f, 0.0f,    0.9f, 0.9f, 0.1f,

			// 7. Centro de la Cabeza (Rosa Claro)
			 0.00f,  0.60f, 0.0f,    1.0f, 0.6f, 0.7f,
			-0.15f,  0.65f, 0.0f,    1.0f, 0.6f, 0.7f,
			 0.10f,  0.25f, 0.0f,    1.0f, 0.6f, 0.7f,

			 // 8. Nuca Alta (Rojo)
			 -0.15f,  0.65f, 0.0f,    0.9f, 0.1f, 0.2f,
			 -0.25f,  0.70f, 0.0f,    0.9f, 0.1f, 0.2f,
			 -0.20f,  0.45f, 0.0f,    0.9f, 0.1f, 0.2f,

			 // 9. Zona del Ojo/Mejilla (Amarillo)
			 -0.15f,  0.65f, 0.0f,    0.9f, 0.9f, 0.1f,
			 -0.20f,  0.45f, 0.0f,    0.9f, 0.9f, 0.1f,
			  0.10f,  0.25f, 0.0f,    0.9f, 0.9f, 0.1f,

			  // 10. Cuello Trasero (Rosa Fuerte)
			  -0.20f,  0.45f, 0.0f,    1.0f, 0.4f, 0.6f,
			  -0.25f,  0.20f, 0.0f,    1.0f, 0.4f, 0.6f,
			   0.10f,  0.25f, 0.0f,    1.0f, 0.4f, 0.6f,

			   // 11. Cuello Frontal (Verde)
				0.10f,  0.25f, 0.0f,    0.6f, 0.8f, 0.1f,
			   -0.25f,  0.20f, 0.0f,    0.6f, 0.8f, 0.1f,
				0.00f,  0.10f, 0.0f,    0.6f, 0.8f, 0.1f,

				// 12. Aleta Dorsal Superior (Amarillo)
				-0.20f,  0.45f, 0.0f,    0.9f, 0.9f, 0.1f,
				-0.45f,  0.35f, 0.0f,    0.9f, 0.9f, 0.1f,
				-0.25f,  0.20f, 0.0f,    0.9f, 0.9f, 0.1f,

				// 13. Aleta Dorsal Inferior (Rosa Fuerte)
				-0.45f,  0.35f, 0.0f,    1.0f, 0.4f, 0.6f,
				-0.40f,  0.05f, 0.0f,    1.0f, 0.4f, 0.6f,
				-0.25f,  0.20f, 0.0f,    1.0f, 0.4f, 0.6f,

				// 14. Pecho (Rosa Claro)
				 0.00f,  0.10f, 0.0f,    1.0f, 0.6f, 0.7f,
				-0.25f,  0.20f, 0.0f,    1.0f, 0.6f, 0.7f,
				 0.10f, -0.10f, 0.0f,    1.0f, 0.6f, 0.7f,

				 // 15. Espalda Media (Rojo)
				 -0.25f,  0.20f, 0.0f,    0.9f, 0.1f, 0.2f,
				 -0.30f, -0.15f, 0.0f,    0.9f, 0.1f, 0.2f,
				  0.10f, -0.10f, 0.0f,    0.9f, 0.1f, 0.2f,

				  // 16. Vientre Alto (Amarillo)
				   0.10f, -0.10f, 0.0f,    0.9f, 0.9f, 0.1f,
				  -0.30f, -0.15f, 0.0f,    0.9f, 0.9f, 0.1f,
				   0.00f, -0.35f, 0.0f,    0.9f, 0.9f, 0.1f,

				   // 17. Espalda Baja (Verde)
				   -0.30f, -0.15f, 0.0f,    0.6f, 0.8f, 0.1f,
				   -0.20f, -0.45f, 0.0f,    0.6f, 0.8f, 0.1f,
					0.00f, -0.35f, 0.0f,    0.6f, 0.8f, 0.1f,

					// 18. Vientre Bajo (Rosa Fuerte)
					 0.00f, -0.35f, 0.0f,    1.0f, 0.4f, 0.6f,
					-0.20f, -0.45f, 0.0f,    1.0f, 0.4f, 0.6f,
					 0.10f, -0.55f, 0.0f,    1.0f, 0.4f, 0.6f,

					 // 19. Base Cola Trasera (Rojo)
					 -0.20f, -0.45f, 0.0f,    0.9f, 0.1f, 0.2f,
					 -0.25f, -0.75f, 0.0f,    0.9f, 0.1f, 0.2f,
					  0.10f, -0.55f, 0.0f,    0.9f, 0.1f, 0.2f,

					  // 20. Cola Curva Inferior (Amarillo)
					  -0.25f, -0.75f, 0.0f,    0.9f, 0.9f, 0.1f,
					  -0.05f, -0.90f, 0.0f,    0.9f, 0.9f, 0.1f,
					   0.10f, -0.55f, 0.0f,    0.9f, 0.9f, 0.1f,

					   // 21. Cola Curva Derecha (Rosa Claro)
					   -0.05f, -0.90f, 0.0f,    1.0f, 0.6f, 0.7f,
						0.25f, -0.75f, 0.0f,    1.0f, 0.6f, 0.7f,
						0.10f, -0.55f, 0.0f,    1.0f, 0.6f, 0.7f,

						// 22. Cola Curva Interna (Verde)
						 0.25f, -0.75f, 0.0f,    0.6f, 0.8f, 0.1f,
						 0.15f, -0.45f, 0.0f,    0.6f, 0.8f, 0.1f,
						 0.10f, -0.55f, 0.0f,    0.6f, 0.8f, 0.1f,

						 // 23. Cola Punta Externa (Rojo)
						  0.25f, -0.75f, 0.0f,    0.9f, 0.1f, 0.2f,
						  0.35f, -0.50f, 0.0f,    0.9f, 0.1f, 0.2f,
						  0.15f, -0.45f, 0.0f,    0.9f, 0.1f, 0.2f,

						  // 24. Espiral Cola Centro (Amarillo)
						   0.35f, -0.50f, 0.0f,    0.9f, 0.9f, 0.1f,
						   0.20f, -0.60f, 0.0f,    0.9f, 0.9f, 0.1f,
						   0.15f, -0.45f, 0.0f,    0.9f, 0.9f, 0.1f,

						   // 25. VÉRTICE EXTRA PARA EL OJO (Índice 72) - Color Negro
							 0.06f, 0.38f, 0.0f, 0.0f, 0.0f, 0.0f
	};

	// Los índices se quedan exactamente en 72 (solo dibujan los triángulos)
	unsigned int indices[] = {
		 0,  1,  2,   3,  4,  5,   6,  7,  8,
		 9, 10, 11,  12, 13, 14,  15, 16, 17,
		18, 19, 20,  21, 22, 23,  24, 25, 26,
		27, 28, 29,  30, 31, 32,  33, 34, 35,
		36, 37, 38,  39, 40, 41,  42, 43, 44,
		45, 46, 47,  48, 49, 50,  51, 52, 53,
		54, 55, 56,  57, 58, 59,  60, 61, 62,
		63, 64, 65,  66, 67, 68,  69, 70, 71
	};

	GLuint VBO, VAO,EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Enlazar  Vertex Array Object
	glBindVertexArray(VAO);

	//2.- Copiamos nuestros arreglo de vertices en un buffer de vertices para que OpenGL lo use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3.Copiamos nuestro arreglo de indices en  un elemento del buffer para que OpenGL lo use
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 4. Despues colocamos las caracteristicas de los vertices

	//Posicion
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	//Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		// Draw our first triangle
		ourShader.Use();
		glBindVertexArray(VAO);


        //glPointSize(10);
        //glDrawArrays(GL_POINTS,0,4);
        
        //glDrawArrays(GL_LINES,0,4);
        //glDrawArrays(GL_LINE_LOOP,0,4);
        
		glDrawElements(GL_TRIANGLES, 72, GL_UNSIGNED_INT, 0);

		glPointSize(25.0f);
		glDrawArrays(GL_POINTS, 72, 1);

        glBindVertexArray(0);
    
		// Swap the screen buffers
		glfwSwapBuffers(window);
	}


	glfwTerminate();
	return EXIT_SUCCESS;
}

void resize(GLFWwindow* window, int width, int height)
{
	// Set the Viewport to the size of the created window
	glViewport(0, 0, width, height);
	//glViewport(0, 0, screenWidth, screenHeight);
}