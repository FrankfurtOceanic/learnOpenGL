#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Source.h"
#include <CustomOpenGL/Shader.h>
#include <CustomOpenGL/Camera.h>
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCREEN_WIDTH / 2.0f;
float lastY = SCREEN_HEIGHT / 2.0f;
bool mouseEnabled = false;
bool firstMouse = true;

//frame time data
float lastFrame = 0.0f;
float deltaTime = 0.0f; //time between last frame and current frame

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
glm::vec3 lightDir = -glm::normalize(lightPos);
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);


int main() {
    void framebuffer_size_callback(GLFWwindow * window, int width, int height); //prototype for window resizing
    void processInput(GLFWwindow * window);
    void mouse_callback(GLFWwindow * window, double xpos, double ypos); //prototype for mouse movement
    void scroll_callback(GLFWwindow * window, double xoffset, double yoffset); //prototype for zooming
    
    //initialize glfw with the major and minor version as well as using the core_profile---------------------------------------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //set up a window//---------------------------------------------------------------
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


    //initialize GLAD ---------------------------------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }



    //set up vertex data(and buffer(s)) and configure vertex attributes ---------------------------------------------------------------

    float vertices[] = {
        // positions // normals // texture coords
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
    };

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

    glm::vec3 pointLightPositions[] = {
        glm::vec3(0.7f, 0.2f, 2.0f),
        glm::vec3(2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f, 2.0f, -12.0f),
        glm::vec3(0.0f, 0.0f, -3.0f)
    };



    //configuring vertex buffers ---------------------------------------------------------------

    unsigned int VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);
    
    //first we set up the VAO for the cube

    // 1. Bind vertex array object
    glBindVertexArray(cubeVAO);

    // 2. copy our vertices array in a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    // 3. then set our vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); //position
    glEnableVertexAttribArray(0);

    // 3. then set our vertex attributes pointers
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); //normal
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); //texcoord
    glEnableVertexAttribArray(2);

    //then we set up the VAO for the light source
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO); //no need to bufferData since it's already been done

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); //only need position for the light
    glEnableVertexAttribArray(0);



    //enabling depth buffer for the scene ---------------------------------------------------------------
    glEnable(GL_DEPTH_TEST);

    //setting up the shader programs ---------------------------------------------------------------
    Shader objectShader("Shaders/15.MappedMaterials/15.1.MappedMaterial.vs", "Shaders/16.LightCasters/16.5.MultipleLights.fs");
    objectShader.use();
    //material properties
    objectShader.setFloat("material.shininess", 32.0f);
    
    //directional light properties
    objectShader.setVec3("dirLight.direction", glm::normalize(glm::vec3(-1.0f)));
    objectShader.setVec3("dirLight.ambientStrength", 0.1f, 0.1f, 0.1f);
    objectShader.setVec3("dirLight.diffuseStrength", 0.8f, 0.8f, 0.8f);
    objectShader.setVec3("dirLight.specularStrength", 1.0f, 1.0f, 1.0f);

    //point light properties
    for (int i = 0; i < 4; i++) 
    {
        objectShader.setVec3("pointLights[" + std::to_string(i) + "].position", pointLightPositions[i]);

        objectShader.setVec3("pointLights[" + std::to_string(i) + "].ambientStrength", 0.1f, 0.1f, 0.1f);
        objectShader.setVec3("pointLights[" + std::to_string(i) + "].diffuseStrength", 0.5f, 0.5f, 0.5f);
        objectShader.setVec3("pointLights[" + std::to_string(i) + "].specularStrength", 1.0f, 1.0f, 1.0f);

        objectShader.setFloat("pointLights[" + std::to_string(i) + "].constant", 1.0f);
        objectShader.setFloat("pointLights[" + std::to_string(i) + "].linear", 0.09f);
        objectShader.setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0.032f);
    }


    objectShader.setFloat("spot.cutoff", glm::cos(glm::radians(12.5f))); //use the cosine because the dot product returns a cosine value (also save computation by not having to use arccos)
    objectShader.setFloat("spot.outerCutoff", glm::cos(glm::radians(17.5f)));

    objectShader.setVec3("spot.ambientStrength", 0.1f, 0.1f, 0.1f);
    objectShader.setVec3("spot.diffuseStrength", 0.8f, 0.8f, 0.8f);
    objectShader.setVec3("spot.specularStrength", 1.0f, 1.0f, 1.0f);

    objectShader.setFloat("spot.constant", 1.0f);
    objectShader.setFloat("spot.linear", 0.09f);
    objectShader.setFloat("spot.quadratic", 0.032f);






    Shader lightShader("Shaders/12.Lighting/12.1.Colors.vs", "Shaders/12.Lighting/12.1.LightSource.fs");


  

    //generating a texture --------------------------------------------------
    unsigned int diffuse, specular;

    //set up for first texture

    glGenTextures(1, &diffuse);
    glBindTexture(GL_TEXTURE_2D, diffuse);

    //set wrapping and filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //loading the diffuse image
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("resources/textures/container2.png", &width, &height,
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

    //set up for second texture

    glGenTextures(1, &specular);
    glBindTexture(GL_TEXTURE_2D, specular);

    //set wrapping and filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    //loading the diffuse image
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load("resources/textures/container2_specular.png", &width, &height,
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
    glBindTexture(GL_TEXTURE_2D, diffuse);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specular);


    objectShader.use();
    objectShader.setInt("material.diffuseMap", 0);
    objectShader.setInt("material.specularMap", 1);




    ///render loop///
    while (!glfwWindowShouldClose(window)) {
        //handling frame data
        float currentTime = glfwGetTime();
        deltaTime = currentTime - lastFrame;
        lastFrame = currentTime;
        
        //input handling
        processInput(window);

        glClearColor(0.1f, 0.1f, 0.11f, 1.0f); //set a clear color
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear the color and depth buffers


        //rendering cube ----------------------------------------------------------------------------

        objectShader.use();
        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
        objectShader.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        objectShader.setMat4("view", view);
        objectShader.setVec3("viewPos", camera.Position);

        objectShader.setVec3("spot.position", camera.Position);
        objectShader.setVec3("spot.direction", camera.Foward);

        glBindVertexArray(cubeVAO);
        for (unsigned int i = 0; i < sizeof(cubePositions) / sizeof(glm::vec3); i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle),
                glm::vec3(1.0f, 0.3f, 0.5f));
            objectShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }



        //rendering lights ----------------------------------------------------------------------------
        lightShader.use();

        lightShader.setMat4("projection", projection);

        // camera/view transformation
        lightShader.setMat4("view", view);

        lightShader.setVec3("lightColor", lightColor);

        // object transformation

        glBindVertexArray(cubeVAO);
        for (unsigned int i = 0; i < sizeof(pointLightPositions) / sizeof(glm::vec3); i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.3f));
            lightShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
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

    camera.ProcessMouseMovement(xoffset, yoffset, true);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) 
{
    camera.ProcessMouseScroll(yoffset);
}

void processInput(GLFWwindow* window) 
{

    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }



    //forward movement
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    //backwards movement
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    //up movement
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(UP, deltaTime);
    }
    //backwards movement
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(DOWN, deltaTime);
    }
    //strafe right
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
    //strafe left
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(LEFT, deltaTime);
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