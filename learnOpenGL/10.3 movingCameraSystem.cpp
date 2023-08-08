#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Source.h"
#include <ShaderClass/Shader.h>
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const float CAMERA_SPEED = 5.0f;
const bool NORMALIZED_MOVEMENT = true;

bool mouseEnabled = false;
bool firstMouse = true;

float lastFrame = 0.0f;
float deltaTime = 0.0f;

float mixAmount = 0.5f;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f); //because OpenGl is right-handed (positive z on the middle finger) we use a positive z value to move backwards
glm::vec3 cameraFoward = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

//members required for camera rotation base on mouse inputs
float yaw = -90.0f;
float pitch = 0.0f;
float lastX = 400, lastY = 300;

float zoom = 45.0f;


int main() {
    void framebuffer_size_callback(GLFWwindow * window, int width, int height); //prototype for window resizing
    void processInput(GLFWwindow * window);
    void checkCompileErrors(unsigned int shader);
    void checkSPLinkingErrors(unsigned int shader);
    void mouse_callback(GLFWwindow * window, double xpos, double ypos); //prototype for mouse movement
    void scroll_callback(GLFWwindow * window, double xoffset, double yoffset); //prototype for zooming
    
    //initialize glfw with the major and minor version as well as using the core_profile
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //set up a window//
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL); //create a window with width and height: 800x 600 and with a name
    if (window == NULL) {
        std::cout << "Failed to create a window" << std::endl;
        glfwTerminate();
        return -1;
    }
    //make the window's context the main context on the current thread
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); //registers the function to get called on every window resize
    if (!mouseEnabled) 
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    //initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //set up vertex data(and buffer(s)) and configure vertex attributes

    float vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f
    };

    //translation vectors
    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)
    };


    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f); //target at the origin
    glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget); //the direction is actually the negative of this value
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));


    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f); //Using gram schimdt to find up and right vectors
    glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));

    glm::mat4 view;
    view = glm::lookAt(cameraPos, cameraPos + cameraFoward, cameraUp);



    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    // 1. Bind vertex array object
    glBindVertexArray(VAO);

    // 2. copy our vertices array in a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    // 3. then set our vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // 4. then set our texture cooordinate pointers
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //generating a texture
    unsigned int texture, texture2;
    
    //set up for first texture

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    //set wrapping and filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //loading the image
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("resources/textures/crate.jpg", &width, &height,
        &nrChannels, 0);

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
            GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    else {
        std::cout << "Failed to Load Texture" << std::endl;
    }
    stbi_image_free(data);

    //second texture

    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);

    //set wrapping and filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load("resources/textures/face.png", &width, &height,
        &nrChannels, 0);

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    else {
        std::cout << "Failed to Load Texture" << std::endl;
    }
    stbi_image_free(data);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    Shader ourShader("Shaders/6.CoordSys/6.2.Texture.vs", "Shaders/6.CoordSys/6.2.Texture.fs");


    ourShader.use(); // don’t forget to activate the shader first!
    glUniform1i(glGetUniformLocation(ourShader.ID, "texture"), 0); // manually
    ourShader.setInt("texture2", 1);

    //view matrix
    //view = glm::mat4(1.0f);
    //view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    //projection matrix
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");

    unsigned int modelLoc = glGetUniformLocation(ourShader.ID, "model");
    unsigned int viewLoc = glGetUniformLocation(ourShader.ID, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    unsigned int projectionLoc = glGetUniformLocation(ourShader.ID, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glEnable(GL_DEPTH_TEST);

    ///render loop///
    while (!glfwWindowShouldClose(window)) {
        //handling frame data
        float currentTime = glfwGetTime();
        deltaTime = currentTime - lastFrame;
        lastFrame = currentTime;
        
        
        //input handling
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //set a clear color
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear the color and depth buffers

        //simple camera movement;
        view = glm::lookAt(cameraPos, cameraPos + cameraFoward, cameraUp);
        ourShader.setMat4("view", view);

        //zooming camera
        projection = glm::perspective(glm::radians(zoom), 800.0f / 600.0f, 0.1f, 100.0f);
        ourShader.setMat4("projection", projection);

        ourShader.use();
        //glUseProgram(shaderProgram);
        
        //render triangle 
        /*
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3); */

        ourShader.setFloat("mixPercent", mixAmount);

        for (glm::vec3 pos : cubePositions) {
            //rotate cube 
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, pos);
            model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
            
            
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            //render elements 
            glBindVertexArray(VAO);
            //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            glDrawArrays(GL_TRIANGLES, 0, 36); //draw using arrays since the cube doesn't have indices
        }


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //clean up GLFW's resources that were allocated before ending
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) 
{
    //handles window resizing
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    //adjust initial mouse position when window is first opened
    if (firstMouse) 
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    //calculate offsets
    float xoffset = xpos - lastX;
    lastX = xpos;
    float yoffset = lastY - ypos; //invert because the bottom is the higher coordinate (top of screen is 0)
    lastY = ypos;

    //add offsets to pitch and yaw
    const float sensitivity = 0.1f; //added sensitivity
    yaw += xoffset * sensitivity;
    pitch += yoffset * sensitivity;

    //constrain minimum and maximum pitch
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    //calculate direction vector
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    //adjust camera direction
    cameraFoward = glm::normalize(direction);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) 
{
    zoom -= (float)yoffset;
    if (zoom > 75.0f) {
        zoom = 75.0f;
    }

    if (zoom < 1.0f) {
        zoom = 1.0f;
    }
}

void processInput(GLFWwindow* window) 
{

    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        mixAmount = std::min(mixAmount + 0.01f, 1.0f);
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        mixAmount = std::max(mixAmount - 0.01f, 0.0f);
    }

    //camera controls
    float cameraSpeed = CAMERA_SPEED * deltaTime;
    glm::vec3 movementVector = glm::vec3(0.0f); //used for normalized movement

    //forward movement
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        movementVector += cameraFoward;
    }
    //backwards movement
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        movementVector -= cameraFoward;
    }
    //up movement
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        movementVector += cameraUp;
    }
    //backwards movement
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        movementVector -= cameraUp;
    }
    //strafe right
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        movementVector += glm::normalize(glm::cross(cameraFoward, cameraUp)) ;
    }
    //strafe left
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        movementVector -= glm::normalize(glm::cross(cameraFoward, cameraUp));
    }

    if (movementVector != glm::vec3(0.0f)) 
    {
        cameraPos += (NORMALIZED_MOVEMENT ? glm::normalize(movementVector) : movementVector) * cameraSpeed;
    }

    //enabling/disabling mouse
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
    {
        
        if (mouseEnabled) 
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        else
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }

        mouseEnabled = !mouseEnabled;

    }
}

void checkCompileErrors(unsigned int shader)
{
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" <<
            infoLog << std::endl;
    }
}

void checkSPLinkingErrors(unsigned int program) //check shader program linking 
{
    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (!success)
    {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER_PROGRAM::COMPILATION_FAILED\n" <<
            infoLog << std::endl;
    }
}