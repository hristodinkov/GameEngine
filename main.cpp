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
#include "MyClasses/Shader.h"
#include "MyClasses/Style.h"
#include "MyClasses/Translate.h"

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
#include "MyClasses/LightObj.h"
#endif

Style style;

int g_width = 1200;
int g_height = 800;
Camera camera;
bool IMGuiOpened = true;
bool IMGuiOpenedCarWindow = true;
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

//RawEngine old methods

// GLuint generateShader(const std::string &shaderPath, GLuint shaderType) {
//     printf("Loading shader: %s\n", shaderPath.c_str());
//     const std::string shaderText = readFileToString(shaderPath);
//     const GLuint shader = glCreateShader(shaderType);
//     const char *s_str = shaderText.c_str();
//     glShaderSource(shader, 1, &s_str, nullptr);
//     glCompileShader(shader);
//     GLint success = 0;
//     glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
//     if (!success) {
//         char infoLog[512];
//         glGetShaderInfoLog(shader, 512, NULL, infoLog);
//         printf("Error! Shader issue [%s]: %s\n", shaderPath.c_str(), infoLog);
//     }
//     return shader;
// }

void PrintMatrix(glm::mat4 matrix) {
    for (int r=0;r<4;r++) {
        for (int c=0;c<4;c++) {
            printf("%f ",matrix[c][r]);
        }
        printf("\n");
    }

}

float rectangleVertices[]{
    //Coords    //texCoords
    1.0f, -1.0f, 1.0f, 0.0f,
   -1.0f, -1.0f, 0.0f, 0.0f,
   -1.0f,  1.0f, 0.0f, 1.0f,

    1.0f,  1.0f, 1.0f, 1.0f,
    1.0f, -1.0f, 1.0f, 0.0f,
   -1.0f,  1.0f, 0.0f, 1.0f

};

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

    int success;
    char infoLog[512];

    Shader modelShader("shaders/modelVertex.vs","shaders/fragment.fs");
    Shader textureShader("shaders/modelVertex.vs","shaders/texture.fs");

    Shader invertColorsShader("shaders/invertColors.vs","shaders/invertColors.fs");

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
    //suzanne->addBehavior(std::make_shared<Translate>(1.0f,glm::vec3(0.1f, 0.0f, 0.0f)));
    // suzanne->addBehavior(std::make_shared<Rotate>(
    //     glm::vec3(0, 1, 0), glm::radians(30.0f)
    // ));

    std::shared_ptr<LightObj> mainLight = nullptr;
    //auto light = scene1->addObject(GameObject("Light"));
    auto light = std::make_shared<LightObj>(glm::vec3(0.0f, 10.0f, 0.0f), glm::vec4(184.0f/256.0f, 23.0f/256.0f, 222.0f/256.0f, 0.0f),10);
    mainLight = std::move(light);
    auto pointlight = scene1->addObject(GameObject("PointLight"));
    //pointlight->translate(light->getPos());


    auto scene2 = sceneManager.createScene("Car");
    auto car = scene2->addObject(GameObject("Car"));
    car->model = core::AssimpLoader::loadModel("models/car.obj");
    car->translate(glm::vec3(2.0f, 0.0f, 0.0f));
    //car->scale(glm::vec3(0.01f, 0.01f, 0.01f));
    car->addBehavior(std::make_shared<Rotate>(
        glm::vec3(1, 0, 0), glm::radians(60.0f)
    ));

    auto car2 = scene1->addObject(GameObject("Car2"));
    car2->model = core::AssimpLoader::loadModel("models/car.obj");
    car2->translate(glm::vec3(0.0f, 0.0f, -50.0f));
    //car->scale(glm::vec3(0.01f, 0.01f, 0.01f));
    // car->addBehavior(std::make_shared<Rotate>(
    //     glm::vec3(1, 0, 0), glm::radians(60.0f)
    // ));
    core::Texture cmgtGatoTexture("textures/CMGaTo_crop.png");
    core::Texture stone("textures/stone.jpg");
    core::Texture paint("textures/paint.png");
    core::Texture chalk ("textures/chalk.jpg");

    glm::vec4 clearColor = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
    glClearColor(clearColor.r,
                 clearColor.g, clearColor.b, clearColor.a);

    unsigned int rectVAO,rectVBO;
    glGenVertexArrays(1, &rectVAO);
    glGenBuffers(1, &rectVBO);
    glBindVertexArray(rectVAO);
    glBindBuffer(GL_ARRAY_BUFFER, rectVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), &rectangleVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(2*sizeof(float)));



    auto cameraPos = glm::vec3(0.0f, 0.0f, 10.0f);
    camera.cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    camera.cameraDirection = glm::normalize(camera.cameraPos - camera.cameraTarget);

    camera.cameraRight = glm::normalize(glm::cross(glm::vec3(0,1,0), camera.cameraDirection));
    camera.cameraUp = glm::cross(camera.cameraDirection, camera.cameraRight);
    //VP
    glm::mat4 view = glm::lookAt(camera.cameraPos, camera.cameraTarget, camera.cameraUp);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(g_width) / static_cast<float>(g_height), 0.1f, 100.0f);

    camera.translate(cameraPos);
    //camera.rotate(glm::vec3(1,0,0), -10.0f * 3.1415f / 180);
    camera.speed = 0.005f;

    double currentTime = glfwGetTime();
    double finishFrameTime = 0.0;
    float deltaTime = 0.0f;
    float rotationStrength = 100.0f;

    glm::vec3 guiLightPos = mainLight ? mainLight->getPos() : glm::vec3(0.0f, 3.0f, 0.0f);
    glm::vec3 guiLightColor = mainLight ? glm::vec3(mainLight->getColor()) : glm::vec3(1.0f);
    float guiShininess = 75.0f;
    float guiSpecular = 42.0;
    float guiAmbient = 0.25f;
    float guiLightRadius = 35.0f;

    glm::vec3 carPos = car2->getPos();

    unsigned int FBO;
    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    unsigned int framebufferTexture;
    glGenTextures(1, &framebufferTexture);
    glBindTexture(GL_TEXTURE_2D, framebufferTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, g_width, g_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebufferTexture, 0);

    unsigned int renderBuffer;
    glGenRenderbuffers(1, &renderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, g_width, g_height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL, GL_RENDERBUFFER, renderBuffer);

    auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
        printf("Framebuffer error: %d\n", fboStatus);




    float aspect = static_cast<float>(g_width) / g_height;
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        myStyle();
       // ImGui::SetNextWindowSize(ImVec2(200,200));
        if (ImGui::Begin("Let it be light",&IMGuiOpened)) {
            //ImGui::SetCursorPosX(ImGui::GetWindowWidth()-ImGui::CalcTextSize("my button").x-10);
            // if (ImGui::Button("My button")) {
            //
            // }

            // ImGui::Checkbox("My Checkbox",&checkboxTest);
            //
            // ImGui::SliderFloat("Test float slider",&value1,-0.5f,4);
            //
            // ImGui::SliderInt("test int slider", &value2,-3,9);
            //
            // ImGui::Combo("Combo", &current,items,IM_ARRAYSIZE(items));
            //
            // ImGui::SetCursorPosX(ImGui::GetWindowWidth()/2-ImGui::CalcTextSize("Hello :)").x/2);
            // ImGui::Text("Hello :)");

            ImGui::SliderFloat3("Light Position", glm::value_ptr(guiLightPos), 0.0f, 10.0f);
            ImGui::ColorEdit3("Light Color", glm::value_ptr(guiLightColor));
            ImGui::SliderFloat("Shininess", &guiShininess, 1.0f, 512.0f);
            ImGui::SliderFloat("Specular Strength", &guiSpecular, 0.0f, 256.0f);
            ImGui::SliderFloat("Ambient Strength", &guiAmbient, 0.0f, 1.0f);
            ImGui::SliderFloat("Light Radis", &guiLightRadius, 0.0f, 100.0f);


        }ImGui::End();

        if (ImGui::Begin("MoveCar",&IMGuiOpenedCarWindow)) {
            if (ImGui::SliderFloat3("Position",glm::value_ptr(car2->position),-80.0f,100.0f)) {
                car2->setPos(car2->position);
            }
        }ImGui::End();

        processInput(window);

        // Switch scenes with keys
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
            sceneManager.setActiveScene("Monkey");
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
            sceneManager.setActiveScene("Car");

        sceneManager.update(deltaTime);


        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        projection = glm::perspective(glm::radians(camera.fov),aspect,0.1f, 100.0f);

        view = glm::inverse(camera.getModelMatrix());

        // textureShader.Activate();
        //
        // textureShader.SetMat4Uniform("mvpMatrix",projection * view * quadModel.getModelMatrix());
        //
        // textureShader.SetIntUniform("texture0", 0);
        //
        // glActiveTexture(GL_TEXTURE0);
        // glBindTexture(GL_TEXTURE_2D, cmgtGatoTexture.getId());
        //
        // quadModel.render();

        modelShader.Activate();

        modelShader.SetVec3Uniform("lightPos", guiLightPos);
        modelShader.SetVec3Uniform("cameraPos", camera.getPos());
        modelShader.SetVec3Uniform("lightColor", guiLightColor);

        modelShader.SetFloatUniform("lightRadius", guiLightRadius);
        modelShader.SetFloatUniform("ambientStrength", guiAmbient);
        modelShader.SetFloatUniform("specularStrength", guiSpecular);
        modelShader.SetFloatUniform("shininess", guiShininess);

        modelShader.SetMat4Uniform("viewMatrix", view);
        modelShader.SetMat4Uniform("projMatrix", projection);

        modelShader.BindTexture("textures/chalk.jpg",chalk.getId(),0);

        sceneManager.render(modelShader, projection, view);




        glBindVertexArray(0);

        // glBindFramebuffer(GL_FRAMEBUFFER, FBO);
        // glEnable(GL_DEPTH_TEST);

        // invertColorsShader.Activate();
        // invertColorsShader.SetIntUniform("screenTexture", 0);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        //invertColorsShader.Activate();
        //glBindVertexArray(rectVAO);
        //glDisable(GL_DEPTH_TEST);
        //glBindTexture(GL_TEXTURE_2D, framebufferTexture);
        //glDrawArrays(GL_TRIANGLES, 0, 6);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
        glfwPollEvents();
        finishFrameTime = glfwGetTime();
        deltaTime = static_cast<float>(finishFrameTime - currentTime);

        currentTime = finishFrameTime;
    }


    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}