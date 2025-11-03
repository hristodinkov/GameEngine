#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vcruntime_startup.h>

#include "MyClasses/Camera.h"
#include "core/mesh.h"
#include "core/assimpLoader.h"
#include "core/texture.h"

#include "MyClasses/Rotate.h"
#include "MyClasses/Scene.h"
#include "MyClasses/SceneManager.h"
#include "MyClasses/Style.h"

//#define MAC_CLION
#define VSTUDIO

#ifdef MAC_CLION
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#endif

#ifdef VSTUDIO
// Note: install imgui with:
//     ./vcpkg.exe install imgui[glfw-binding,opengl3-binding]
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#endif

Style style;
int g_width = 800;
int g_height = 600;
Camera camera;
bool IMGuiOpened = true;
bool checkboxTest = false;
float value1 = 0.1f;
int value2 =1;
const char* items[] = {"item1", "item2", "item3"};
int current = 0;
void myStyle() {
    style.SetupImGuiStyle();
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    //if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)

    camera.Move(window);
    camera.Rotate(window);

}

void framebufferSizeCallback(GLFWwindow *window,
                             int width, int height) {
    g_width = width;
    g_height = height;
    glViewport(0, 0, width, height);
    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
}



std::string readFileToString(const std::string &filePath) {
    std::ifstream fileStream(filePath, std::ios::in);
    if (!fileStream.is_open()) {
        printf("Could not open file: %s\n", filePath.c_str());
        return "";
    }
    std::stringstream buffer;
    buffer << fileStream.rdbuf();
    return buffer.str();
}

GLuint generateShader(const std::string &shaderPath, GLuint shaderType) {
    printf("Loading shader: %s\n", shaderPath.c_str());
    const std::string shaderText = readFileToString(shaderPath);
    const GLuint shader = glCreateShader(shaderType);
    const char *s_str = shaderText.c_str();
    glShaderSource(shader, 1, &s_str, nullptr);
    glCompileShader(shader);
    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        printf("Error! Shader issue [%s]: %s\n", shaderPath.c_str(), infoLog);
    }
    return shader;
}

void PrintMatrix(glm::mat4 matrix) {
    for (int r=0;r<4;r++) {
        for (int c=0;c<4;c++) {
            printf("%f ",matrix[c][r]);
        }
        printf("\n");
    }

}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow *window = glfwCreateWindow(g_width, g_height, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        printf("Failed to initialize GLAD\n");
        return -1;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    //Setup platforms
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 400");

    glEnable(GL_DEPTH_TEST);
    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    const GLuint modelVertexShader = generateShader("shaders/modelVertex.vs", GL_VERTEX_SHADER);
    const GLuint fragmentShader = generateShader("shaders/fragment.fs", GL_FRAGMENT_SHADER);
    const GLuint textureShader = generateShader("shaders/texture.fs", GL_FRAGMENT_SHADER);

    int success;
    char infoLog[512];
    const unsigned int modelShaderProgram = glCreateProgram();
    glAttachShader(modelShaderProgram, modelVertexShader);
    glAttachShader(modelShaderProgram, fragmentShader);
    glLinkProgram(modelShaderProgram);
    glGetProgramiv(modelShaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(modelShaderProgram, 512, NULL, infoLog);
        printf("Error! Making Shader Program: %s\n", infoLog);
    }
    const unsigned int textureShaderProgram = glCreateProgram();
    glAttachShader(textureShaderProgram, modelVertexShader);
    glAttachShader(textureShaderProgram, textureShader);
    glLinkProgram(textureShaderProgram);
    glGetProgramiv(textureShaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(textureShaderProgram, 512, NULL, infoLog);
        printf("Error! Making Shader Program: %s\n", infoLog);
    }

    glDeleteShader(modelVertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(textureShader);

    core::Mesh quad = core::Mesh::generateQuad();
    core::Model quadModel({quad});
    quadModel.translate(glm::vec3(0,0,-2.5));
    quadModel.scale(glm::vec3(5, 5, 1));

    // Scene scene;
    // //core::Model suzanne = core::AssimpLoader::loadModel("models/nonormalmonkey.obj");
    // auto suzanne = std::make_shared<GameObject>("Suzanne");
    // suzanne->model = core::AssimpLoader::loadModel("models/nonormalmonkey.obj");
    // suzanne->translate(glm::vec3(-2.0f, 0.0f, 0.0f));
    // scene.addObject(suzanne);
    //
    // suzanne->addBehavior(std::make_shared<Rotate>(glm::vec3(1, 0, 0), glm::radians(60.0f)));
    //
    // auto car = scene.addObject(GameObject("car"));
    // car->model = core::AssimpLoader::loadModel("models/car.fbx");
    // car->translate(glm::vec3(2.0f, 0.0f, 0.0f));
    // car->scale(glm::vec3(0.01f, 0.01f, 0.01f));
    // // auto mirror = scene.addObject(GameObject("mirror"));
    // // mirror->model = core::AssimpLoader::loadModel("models/mirror.fbx");
    //
    //
    // //suzanne->addChild(mirror);
    //
    // // mirror->translate(glm::vec3(2.0f, 0.0f, 0.0f));
    // // mirror->scale(glm::vec3(0.01f, 0.01f, 0.01f));
    //
    // auto ford = scene.addObject(GameObject("ford"));
    // ford->model = core::AssimpLoader::loadModel("models/ford.obj");
    // ford->translate(glm::vec3(30.0f, 0.0f, 0.0f));
    // ford->rotate(glm::vec3(1,1,0), glm::radians(60.0f));
    // ford->scale(glm::vec3(1.0f, 1.0f,  1.0f));
    //mirror->addChild(ford);

    SceneManager sceneManager;

    auto scene1 = sceneManager.createScene("Monkey");
    auto suzanne = scene1->addObject(GameObject("Suzanne"));
    suzanne->model = core::AssimpLoader::loadModel("models/nonormalmonkey.obj");
    suzanne->translate(glm::vec3(-2.0f, 0.0f, 0.0f));
    suzanne->addBehavior(std::make_shared<Rotate>(
        glm::vec3(0, 1, 0), glm::radians(30.0f)
    ));

    auto scene2 = sceneManager.createScene("Car");
    auto car = scene2->addObject(GameObject("Car"));
    car->model = core::AssimpLoader::loadModel("models/car.fbx");
    car->translate(glm::vec3(2.0f, 0.0f, 0.0f));
    car->scale(glm::vec3(0.01f, 0.01f, 0.01f));
    car->addBehavior(std::make_shared<Rotate>(
        glm::vec3(1, 0, 0), glm::radians(60.0f)
    ));


    core::Texture cmgtGatoTexture("textures/CMGaTo_crop.png");

    glm::vec4 clearColor = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
    glClearColor(clearColor.r,
                 clearColor.g, clearColor.b, clearColor.a);



    auto cameraPos = glm::vec3(0.0f, 0.0f, 10.0f);
    camera.cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    camera.cameraDirection = glm::normalize(camera.cameraPos - camera.cameraTarget);
    camera.up = glm::vec3(0.0f, 1.0f, 0.0f);
    camera.cameraRight = glm::normalize(glm::cross(camera.up, camera.cameraDirection));
    camera.cameraUp = glm::cross(camera.cameraDirection, camera.cameraRight);
    //VP
    glm::mat4 view = glm::lookAt(camera.cameraPos, camera.cameraTarget, camera.cameraUp);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(g_width) / static_cast<float>(g_height), 0.1f, 100.0f);

    camera.translate(cameraPos);
    //camera.rotate(glm::vec3(1,0,0), -10.0f * 3.1415f / 180);
    camera.speed = 0.005f;
    GLint mvpMatrixUniform = glGetUniformLocation(modelShaderProgram, "mvpMatrix");
    GLint textureModelUniform = glGetUniformLocation(textureShaderProgram, "mvpMatrix");
    GLint textureUniform = glGetUniformLocation(textureShaderProgram, "text");

    double currentTime = glfwGetTime();
    double finishFrameTime = 0.0;
    float deltaTime = 0.0f;
    float rotationStrength = 100.0f;


    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        myStyle();
       // ImGui::SetNextWindowSize(ImVec2(200,200));
        if (ImGui::Begin("My Engine",&IMGuiOpened)) {
            ImGui::SetCursorPosX(ImGui::GetWindowWidth()-ImGui::CalcTextSize("my button").x-10);
            if (ImGui::Button("My button")) {

            }

            ImGui::Checkbox("My Checkbox",&checkboxTest);

            ImGui::SliderFloat("Test float slider",&value1,-0.5f,4);

            ImGui::SliderInt("test int slider", &value2,-3,9);

            ImGui::Combo("Combo", &current,items,IM_ARRAYSIZE(items));

            ImGui::SetCursorPosX(ImGui::GetWindowWidth()/2-ImGui::CalcTextSize("Hello :)").x/2);
            ImGui::Text("Hello :)");

        }ImGui::End();


        processInput(window);

        // Switch scenes with keys
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
            sceneManager.setActiveScene("Monkey");
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
            sceneManager.setActiveScene("Car");
        sceneManager.update(deltaTime);
        projection = glm::perspective(glm::radians(camera.fov),static_cast<float>(g_width) / g_height,0.1f, 100.0f);

        //suzanne.rotate(glm::vec3(0.0f, 1.0f, 0.0f), glm::radians(rotationStrength) * static_cast<float>(deltaTime));

        view = glm::inverse(
            camera.getModelMatrix()); //glm::lookAt(camera.cameraPos, camera.cameraTarget, camera.cameraUp);
        //PrintMatrix(view);

        glUseProgram(textureShaderProgram);

        glUniformMatrix4fv(textureModelUniform, 1, GL_FALSE, glm::value_ptr(projection * view * quadModel.getModelMatrix()));
        glActiveTexture(GL_TEXTURE0);
        glUniform1i(textureUniform, 0);
        glBindTexture(GL_TEXTURE_2D, cmgtGatoTexture.getId());
        quadModel.render();
        glBindVertexArray(0);
        glActiveTexture(GL_TEXTURE0);

        glUseProgram(modelShaderProgram);
        sceneManager.render(modelShaderProgram, projection, view);
       // glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, glm::value_ptr(projection * view * suzanne->model.getModelMatrix()));


        //suzanne->model.render();
        glBindVertexArray(0);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
        finishFrameTime = glfwGetTime();
        deltaTime = static_cast<float>(finishFrameTime - currentTime);

        currentTime = finishFrameTime;

    }

    glDeleteProgram(modelShaderProgram);
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}