// Práctica 4
// Juárez Osorio Josué Alexis 
// Fecha de entrega: 13 de septiembre de 2026
// No. cuenta: 320083125

#include<iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

void Inputs(GLFWwindow* window);

void DibujarCubo(
    GLint modelLoc,
    float x, float y, float z,
    float sx, float sy, float sz,
    float r, float g, float b
);

const GLint WIDTH = 900, HEIGHT = 700;

float movX = 0.0f;
float movY = -0.2f;
float movZ = -8.0f;
float rot = -35.0f;

int main() {

    glfwInit();
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(
        WIDTH,
        HEIGHT,
        "Practica 4 - Josue Juarez",
        nullptr,
        nullptr
    );

    int screenWidth, screenHeight;

    glfwGetFramebufferSize(
        window,
        &screenWidth,
        &screenHeight
    );

    if (nullptr == window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;

    if (GLEW_OK != glewInit()) {
        std::cout << "Failed to initialise GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    glViewport(
        0,
        0,
        screenWidth,
        screenHeight
    );

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);

    glBlendFunc(
        GL_SRC_ALPHA,
        GL_ONE_MINUS_SRC_ALPHA
    );

    Shader ourShader(
        "Shader/core.vs",
        "Shader/core.frag"
    );

    // Vertices del cubo
    float vertices[] = {

        // Front
        -0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,

        // Back
        -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,

         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,

        // Right
         0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,

         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,

         // Left
         -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,
         -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,
         -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,

         -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,
         -0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,
         -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,

         // Bottom
         -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,
          0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,
          0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,

          0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,
         -0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,
         -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,

         // Top
         -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,
          0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,
          0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,

          0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,
         -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,
         -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f
    };

    GLuint VBO, VAO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(
        GL_ARRAY_BUFFER,
        VBO
    );

    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(vertices),
        vertices,
        GL_STATIC_DRAW
    );

    // Posicion
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        6 * sizeof(GLfloat),
        (GLvoid*)0
    );

    glEnableVertexAttribArray(0);

    // Color
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        6 * sizeof(GLfloat),
        (GLvoid*)(3 * sizeof(GLfloat))
    );

    glDisableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Proyeccion
    glm::mat4 projection = glm::perspective(
        glm::radians(45.0f),
        (GLfloat)screenWidth / (GLfloat)screenHeight,
        0.1f,
        100.0f
    );

    while (!glfwWindowShouldClose(window))
    {
        Inputs(window);

        glfwPollEvents();

        // Fondo blanco
        glClearColor(
            1.0f,
            1.0f,
            1.0f,
            1.0f
        );

        glClear(
            GL_COLOR_BUFFER_BIT |
            GL_DEPTH_BUFFER_BIT
        );

        ourShader.Use();

        glm::mat4 view = glm::mat4(1.0f);

        view = glm::translate(
            view,
            glm::vec3(
                movX,
                movY,
                movZ
            )
        );

        // Vista desde arriba
        view = glm::rotate(
            view,
            glm::radians(22.0f),
            glm::vec3(
                1.0f,
                0.0f,
                0.0f
            )
        );

        // Rotacion
        view = glm::rotate(
            view,
            glm::radians(rot),
            glm::vec3(
                0.0f,
                1.0f,
                0.0f
            )
        );

        GLint modelLoc =
            glGetUniformLocation(
                ourShader.Program,
                "model"
            );

        GLint viewLoc =
            glGetUniformLocation(
                ourShader.Program,
                "view"
            );

        GLint projecLoc =
            glGetUniformLocation(
                ourShader.Program,
                "projection"
            );

        glUniformMatrix4fv(
            projecLoc,
            1,
            GL_FALSE,
            glm::value_ptr(projection)
        );

        glUniformMatrix4fv(
            viewLoc,
            1,
            GL_FALSE,
            glm::value_ptr(view)
        );

        glBindVertexArray(VAO);

        // Colores
        glm::vec3 verde(
            0.45f,
            0.78f,
            0.05f
        );

        glm::vec3 verdeClaro(
            0.65f,
            0.95f,
            0.08f
        );

        glm::vec3 verdeOscuro(
            0.00f,
            0.25f,
            0.15f
        );

        glm::vec3 amarillo(
            1.00f,
            0.62f,
            0.05f
        );

        glm::vec3 rojo(
            1.00f,
            0.03f,
            0.01f
        );

        glm::vec3 grisOscuro(
            0.12f,
            0.12f,
            0.15f
        );

        glm::vec3 blanco(
            1.00f,
            1.00f,
            1.00f
        );

        // Cuerpo delantero
        DibujarCubo(
            modelLoc,
            0.0f, 0.10f, 0.85f,
            1.90f, 1.00f, 1.35f,
            verde.r, verde.g, verde.b
        );

        DibujarCubo(
            modelLoc,
            0.0f, 0.62f, 0.82f,
            1.75f, 0.18f, 1.20f,
            verdeClaro.r,
            verdeClaro.g,
            verdeClaro.b
        );

        // Abdomen
        DibujarCubo(
            modelLoc,
            0.0f, 0.18f, -0.65f,
            2.05f, 1.00f, 1.55f,
            verde.r,
            verde.g,
            verde.b
        );

        DibujarCubo(
            modelLoc,
            0.0f, 0.70f, -0.68f,
            1.95f, 0.18f, 1.45f,
            verdeClaro.r,
            verdeClaro.g,
            verdeClaro.b
        );

        DibujarCubo(
            modelLoc,
            0.0f, 0.87f, -0.74f,
            1.65f, 0.17f, 1.20f,
            verdeClaro.r,
            verdeClaro.g,
            verdeClaro.b
        );

        DibujarCubo(
            modelLoc,
            0.0f, 1.04f, -0.80f,
            1.30f, 0.17f, 0.92f,
            verdeClaro.r,
            verdeClaro.g,
            verdeClaro.b
        );

        // Detalles superiores
        DibujarCubo(
            modelLoc,
            -0.37f, 1.15f, -0.94f,
            0.27f, 0.08f, 0.27f,
            verdeOscuro.r,
            verdeOscuro.g,
            verdeOscuro.b
        );

        DibujarCubo(
            modelLoc,
            0.37f, 1.15f, -0.94f,
            0.27f, 0.08f, 0.27f,
            verdeOscuro.r,
            verdeOscuro.g,
            verdeOscuro.b
        );

        DibujarCubo(
            modelLoc,
            0.0f, 1.15f, -0.57f,
            0.22f, 0.08f, 0.30f,
            verdeOscuro.r,
            verdeOscuro.g,
            verdeOscuro.b
        );

        // Ojos
        DibujarCubo(
            modelLoc,
            -0.92f, 0.28f, 1.48f,
            0.36f, 0.46f, 0.32f,
            grisOscuro.r,
            grisOscuro.g,
            grisOscuro.b
        );

        DibujarCubo(
            modelLoc,
            -0.92f, 0.28f, 1.67f,
            0.14f, 0.16f, 0.08f,
            blanco.r,
            blanco.g,
            blanco.b
        );

        DibujarCubo(
            modelLoc,
            0.92f, 0.28f, 1.48f,
            0.36f, 0.46f, 0.32f,
            grisOscuro.r,
            grisOscuro.g,
            grisOscuro.b
        );

        DibujarCubo(
            modelLoc,
            0.92f, 0.28f, 1.67f,
            0.14f, 0.16f, 0.08f,
            blanco.r,
            blanco.g,
            blanco.b
        );

        // Bloque blanco superior
        DibujarCubo(
            modelLoc,
            0.0f, 1.02f, 0.82f,
            0.34f, 0.55f, 0.34f,
            blanco.r,
            blanco.g,
            blanco.b
        );

        // Colmillos
        DibujarCubo(
            modelLoc,
            -0.53f, -0.50f, 1.45f,
            0.42f, 0.42f, 0.48f,
            rojo.r,
            rojo.g,
            rojo.b
        );

        DibujarCubo(
            modelLoc,
            0.53f, -0.50f, 1.45f,
            0.42f, 0.42f, 0.48f,
            rojo.r,
            rojo.g,
            rojo.b
        );

        // Patas izquierdas
        DibujarCubo(
            modelLoc,
            -1.05f, -0.08f, 0.65f,
            0.32f, 0.32f, 0.32f,
            amarillo.r,
            amarillo.g,
            amarillo.b
        );

        DibujarCubo(
            modelLoc,
            -1.38f, -0.25f, 0.78f,
            0.45f, 0.38f, 0.45f,
            verdeOscuro.r,
            verdeOscuro.g,
            verdeOscuro.b
        );

        DibujarCubo(
            modelLoc,
            -1.62f, -0.57f, 0.85f,
            0.34f, 0.48f, 0.36f,
            amarillo.r,
            amarillo.g,
            amarillo.b
        );

        DibujarCubo(
            modelLoc,
            -1.10f, -0.05f, 0.02f,
            0.32f, 0.32f, 0.32f,
            amarillo.r,
            amarillo.g,
            amarillo.b
        );

        DibujarCubo(
            modelLoc,
            -1.50f, -0.22f, 0.02f,
            0.48f, 0.38f, 0.43f,
            verdeOscuro.r,
            verdeOscuro.g,
            verdeOscuro.b
        );

        DibujarCubo(
            modelLoc,
            -1.76f, -0.57f, 0.02f,
            0.34f, 0.48f, 0.36f,
            amarillo.r,
            amarillo.g,
            amarillo.b
        );

        DibujarCubo(
            modelLoc,
            -1.05f, -0.02f, -0.58f,
            0.32f, 0.32f, 0.32f,
            amarillo.r,
            amarillo.g,
            amarillo.b
        );

        DibujarCubo(
            modelLoc,
            -1.40f, -0.20f, -0.78f,
            0.45f, 0.38f, 0.45f,
            verdeOscuro.r,
            verdeOscuro.g,
            verdeOscuro.b
        );

        DibujarCubo(
            modelLoc,
            -1.64f, -0.55f, -0.90f,
            0.34f, 0.48f, 0.36f,
            amarillo.r,
            amarillo.g,
            amarillo.b
        );

        // Patas derechas
        DibujarCubo(
            modelLoc,
            1.05f, -0.08f, 0.65f,
            0.32f, 0.32f, 0.32f,
            amarillo.r,
            amarillo.g,
            amarillo.b
        );

        DibujarCubo(
            modelLoc,
            1.38f, -0.25f, 0.78f,
            0.45f, 0.38f, 0.45f,
            verdeOscuro.r,
            verdeOscuro.g,
            verdeOscuro.b
        );

        DibujarCubo(
            modelLoc,
            1.62f, -0.57f, 0.85f,
            0.34f, 0.48f, 0.36f,
            amarillo.r,
            amarillo.g,
            amarillo.b
        );

        DibujarCubo(
            modelLoc,
            1.10f, -0.05f, 0.02f,
            0.32f, 0.32f, 0.32f,
            amarillo.r,
            amarillo.g,
            amarillo.b
        );

        DibujarCubo(
            modelLoc,
            1.50f, -0.22f, 0.02f,
            0.48f, 0.38f, 0.43f,
            verdeOscuro.r,
            verdeOscuro.g,
            verdeOscuro.b
        );

        DibujarCubo(
            modelLoc,
            1.76f, -0.57f, 0.02f,
            0.34f, 0.48f, 0.36f,
            amarillo.r,
            amarillo.g,
            amarillo.b
        );

        DibujarCubo(
            modelLoc,
            1.05f, -0.02f, -0.58f,
            0.32f, 0.32f, 0.32f,
            amarillo.r,
            amarillo.g,
            amarillo.b
        );

        DibujarCubo(
            modelLoc,
            1.40f, -0.20f, -0.78f,
            0.45f, 0.38f, 0.45f,
            verdeOscuro.r,
            verdeOscuro.g,
            verdeOscuro.b
        );

        DibujarCubo(
            modelLoc,
            1.64f, -0.55f, -0.90f,
            0.34f, 0.48f, 0.36f,
            amarillo.r,
            amarillo.g,
            amarillo.b
        );

        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();

    return EXIT_SUCCESS;
}


// Dibuja un cubo con posicion, tamaño y color
void DibujarCubo(
    GLint modelLoc,
    float x, float y, float z,
    float sx, float sy, float sz,
    float r, float g, float b
) {

    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(
        model,
        glm::vec3(x, y, z)
    );

    model = glm::scale(
        model,
        glm::vec3(sx, sy, sz)
    );

    glUniformMatrix4fv(
        modelLoc,
        1,
        GL_FALSE,
        glm::value_ptr(model)
    );

    glVertexAttrib3f(
        1,
        r,
        g,
        b
    );

    glDrawArrays(
        GL_TRIANGLES,
        0,
        36
    );
}


// Controles del modelo
void Inputs(GLFWwindow* window) {

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        movX += 0.08f;

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        movX -= 0.08f;

    if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
        movY += 0.08f;

    if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
        movY -= 0.08f;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        movZ -= 0.08f;

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        movZ += 0.08f;

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        rot += 0.4f;

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        rot -= 0.4f;
}