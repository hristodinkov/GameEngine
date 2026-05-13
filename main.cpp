#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <memory>
#include <vcruntime_startup.h>


#include "MyClasses/Camera.h"
#include "core/mesh.h"
#include "core/assimpLoader.h"
#include "core/texture.h"

#include "MyClasses/Rotate.h"
#include "MyClasses/Scene.h"
#include "MyClasses/SceneManager.h"

#include "MyClasses/Style.h"

#include "MyClasses/CollisionSystem/BenschmarkWriter.h"
#include "MyClasses/CollisionSystem/SpatialHashGrid.h"
#include "MyClasses/CollisionSystem/ColliderModels.h"


//#define MAC_CLION
#define VSTUDIO


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
int current = 0;
//VP
glm::mat4 view ;
glm::mat4 projection;

//excel
bool showFileLockedPopup = false;
std::string lockedFilename;
//test collision
bool benchmarkRunning = false;
double benchmarkStartTime = 0.0;
BenchmarkResult benchmarkResult;
BenchmarkConfig benchmarkConfig;
BenchmarkConfig uiConfig;
bool testIsRunning = false;
std::string filename;
bool useGrid = false;
bool frozenUseGrid = false;
bool useTetrahedron = false;
int numberOfCollishionsThisFrame = 0;
int framesCountTesting = 0;
int benchmarkFrameCounter = 0;


int bmObjectCount = 10;
int bmDuration = 200;
bool bmCamera = false;
char bmLabel[64] = "baseline";

SpatialHashGrid grid;
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

bool isFileLocked(const std::string& filename) {
    std::ofstream file(filename, std::ios::app);
    return !file.is_open();
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


float rectangleVertices[]{
    //Coords |   texCoords
    //x , y     , u    , v
    1.0f, -1.0f, 1.0f, 0.0f,
   -1.0f, -1.0f, 0.0f, 0.0f,
   -1.0f,  1.0f, 0.0f, 1.0f,

    1.0f,  1.0f, 1.0f, 1.0f,
    1.0f, -1.0f, 1.0f, 0.0f,
   -1.0f,  1.0f, 0.0f, 1.0f

};



void drawLines(Shader lineShader, SceneManager sceneManager, std::shared_ptr<GameObject> cube1, GLuint debugVAO, GLuint debugVBO) {
    glLineWidth(5.0f);
    glDisable(GL_DEPTH_TEST);
    auto verts = cube1->collider->getLineVertices();

    lineShader.Activate();
    lineShader.SetMat4Uniform("viewMatrix", view);
    lineShader.SetMat4Uniform("projMatrix", projection);


    glBindVertexArray(debugVAO);
    auto activeScene = sceneManager.getActiveScene();
    for (auto& obj : activeScene->objects)
    {
        if (!obj->collider) continue;
        if (obj->isColliding) {
            lineShader.SetVec3Uniform("color",glm::vec3(1,0,0));
        }
        else {
            lineShader.SetVec3Uniform("color",glm::vec3(0,1,0));
        }

        auto verts = obj->collider->getLineVertices();

        glBindBuffer(GL_ARRAY_BUFFER, debugVBO);
        glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(glm::vec3), verts.data(), GL_DYNAMIC_DRAW);

        glDrawArrays(GL_LINES, 0, verts.size());
    }

    glBindVertexArray(0);
    glEnable(GL_DEPTH_TEST);
}

void postProcessing(Shader invertColorsShader, Shader greyShader, Shader edgeDetectionShader, Shader pixelizationShader, float pixels, float kernelCenterValueMatrix, unsigned int fbTexture, unsigned int quadVAO, int currentPostProcessingMode) {
    if (currentPostProcessingMode != 0) {
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);

        glViewport(0, 0, g_width, g_height);
        glClear(GL_COLOR_BUFFER_BIT);

        Shader* activeShader = nullptr;

        if (currentPostProcessingMode == 1) {
            activeShader = &greyShader;
        }
        else if (currentPostProcessingMode == 2) {
            activeShader = &invertColorsShader;
        }
        else if (currentPostProcessingMode == 3) {
            activeShader = &edgeDetectionShader;
        }
        else if (currentPostProcessingMode == 4) {
            activeShader = &pixelizationShader;

        }

        if (activeShader != nullptr) {
            activeShader->Activate();
            activeShader->SetIntUniform("screenTexture", 0);
            activeShader->SetFloatUniform("Pixels",pixels);
            activeShader->SetFloatUniform("kernelCenterValue",kernelCenterValueMatrix);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, fbTexture);

            glBindVertexArray(quadVAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
        }

        glEnable(GL_CULL_FACE);
    }
}

void im_gui(GLFWwindow *window, core::Model cubeModel,core::Model tetraModel, SceneManager& sceneManager, std::shared_ptr<GameObject> cube1, std::shared_ptr<GameObject> cube2, float deltaTime, float fps, double averageTimePerSAT, BenchmarkConfig& uiConfig, BenchmarkConfig& benchmarkConfig)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    myStyle();

    // if (ImGui::Begin("Let it be light",&IMGuiOpened)) {
    //     //ImGui::SliderFloat3("Light Position", glm::value_ptr(guiLightPos), 0.0f, 10.0f);
    //    // ImGui::ColorEdit3("Light Color", glm::value_ptr(guiLightColor));
    //     // ImGui::SliderFloat("Shininess", &guiShininess, 1.0f, 512.0f);
    //     // ImGui::SliderFloat("Specular Strength", &guiSpecular, 0.0f, 256.0f);
    //     // ImGui::SliderFloat("Ambient Strength", &guiAmbient, 0.0f, 1.0f);
    //     // ImGui::SliderFloat("Light Radis", &guiLightRadius, 0.0f, 100.0f);
    //
    //     ImGui::Combo("Post Process", &currentPostProcessingMode, "None\0Grayscale\0Invert\0EdgeDetection\0Pixalization");
    //     if (currentPostProcessingMode==3) {
    //         ImGui::SliderFloat("Kernel Central Value",&kernelCenterValueMatrix,0,-20);
    //     }
    //     if (currentPostProcessingMode == 4) {
    //         ImGui::SliderFloat("Pixels",&pixels,1,1024);
    //     }
    // } ImGui::End();

    // if (ImGui::Begin("MoveCar",&IMGuiOpenedCarWindow)) {
    //     if (ImGui::SliderFloat3("Position",glm::value_ptr(car2->position),-80.0f,100.0f)) {
    //         car2->setPos(car2->position);
    //     }
    // } ImGui::End();

    if (sceneManager.getSatCount()>0) {
        averageTimePerSAT = sceneManager.getSatTime()/sceneManager.getSatCount();
    }
    else {
        averageTimePerSAT = 0;
    }
    if (ImGui::Begin("Cube Controls"))
    {
        ImGui::Text("Move Cube 1");
        if (ImGui::SliderFloat3("Cube1 Pos", glm::value_ptr(cube1->transform.position), -10.0f, 10.0f))
            cube1->setPos(cube1->transform.position);


        ImGui::Separator();

        ImGui::Text("Move Cube 2");
        if (ImGui::SliderFloat3("Cube2 Pos", glm::value_ptr(cube2->transform.position), -10.0f, 10.0f))
            cube2->setPos(cube2->transform.position);
    }
    ImGui::End();



    if (ImGui::Begin("Benchmark")) {
        ImGui::InputInt("Object Count", &bmObjectCount);
        ImGui::InputInt("Duration (frames)", &bmDuration);
        ImGui::Checkbox("Use Grid", &useGrid);
        ImGui::InputText("Label", bmLabel, sizeof(bmLabel));
        ImGui::Checkbox("Use Tetrahedron",&useTetrahedron);
        ImGui::Checkbox("Camera look away",&bmCamera );
        uiConfig.objectCount = bmObjectCount;
        uiConfig.testDurationFrames = bmDuration;
        framesCountTesting = bmDuration;
        uiConfig.useGrid = useGrid;
        uiConfig.label = bmLabel;
        uiConfig.randomSeed = 47;

        if (ImGui::Button("Spawn Objects")) {
            if (useTetrahedron) {
                sceneManager.spawnCubesInScene(uiConfig.objectCount,uiConfig.randomSeed,tetraModel);
            }
            else {
                sceneManager.spawnCubesInScene(uiConfig.objectCount,uiConfig.randomSeed,cubeModel);
            }

        }
        if (ImGui::Button("Run test")) {
            filename = std::string("test_") + bmLabel + ".xls";

            if (bmCamera) {
                camera.xRotation = 0.0f;
                camera.yRotation = 180.0f;
                camera.transform.rotation = glm::vec3(
                    glm::radians(camera.xRotation),
                    glm::radians(camera.yRotation),
                    0.0f
                );
            }

            camera.transform.updateModelMatrix();

            if (isFileLocked(filename)) {
                lockedFilename = filename;
                showFileLockedPopup = true;
            }
            else if (benchmarkRunning) {
                testIsRunning = true;

            }
            else {

                benchmarkConfig = uiConfig;

                benchmarkRunning = true;
                benchmarkStartTime = glfwGetTime();

                frozenUseGrid = useGrid;

                benchmarkResult = BenchmarkResult();
                benchmarkResult.config = benchmarkConfig;

                sceneManager.resetSatStats();
            }

        }
    }
    ImGui::End();


    if (showFileLockedPopup) {
        ImGui::OpenPopup("File Locked");
    }

    if (ImGui::BeginPopupModal("File Locked", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("The file '%s' is currently open in another program.", lockedFilename.c_str());
        ImGui::Text("Please close it before running the test again or set a new name for the test.");

        if (ImGui::Button("OK", ImVec2(120, 0))) {
            showFileLockedPopup = false;
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

    if (testIsRunning) {
        ImGui::OpenPopup("Test is running!");
    }

    if (ImGui::BeginPopupModal("Test is running!", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("A test is currently running.");
        ImGui::Text("Please wait until the test is complete.");

        if (ImGui::Button("OK", ImVec2(120, 0))) {
            testIsRunning = false;
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }


    if (ImGui::Begin("Performance"))
    {
        ImGui::Text("FPS: %.1f", fps);
        ImGui::Text("Frame Time: %.3f ms", deltaTime * 1000.0f);

        ImGui::Separator();
        ImGui::Text("SAT Tests This Frame: %d", sceneManager.getSatCount());
        ImGui::Text("SAT Time: %.6f s", sceneManager.getSatTime());
        ImGui::Text("Avg Time per SAT: %.9f s", averageTimePerSAT);

    }
    ImGui::End();

    if (ImGui::Begin("Grid Settings")) {
        if (ImGui::Button("CellSize = 200")) grid.cellSize = 200.0f;
        if (ImGui::Button("CellSize = 100")) grid.cellSize = 100.0f;
        if (ImGui::Button("CellSize = 50"))  grid.cellSize = 50.0f;
        if (ImGui::Button("CellSize = 25"))  grid.cellSize = 25.0f;
        if (ImGui::Button("CellSize = 10"))  grid.cellSize = 10.0f;
        if (ImGui::Button("CellSize = 5"))   grid.cellSize = 5.0f;
        if (ImGui::Button("CellSize = 1")) grid.cellSize = 1.0f;
        if (ImGui::Button("CellSize = 0.5"))  grid.cellSize = 0.5f;
        if (ImGui::Button("CellSize = 0.1")) grid.cellSize = 0.1f;
        if (ImGui::Button("CellSize = 0.05")) grid.cellSize = 0.05f;
        if (ImGui::Button("CellSize = 0.01"))  grid.cellSize = 0.01f;
        if (ImGui::Button("CellSize = 0.005")) grid.cellSize = 0.005f;
    }
    ImGui::End();
    processInput(window);
}

void set_modelShader(Shader modelShader, core::Texture cmgtGatoTexture, /*glm::vec3 guiLightPos*/ float guiShininess, float guiSpecular, float guiAmbient, float guiLightRadius) {
    modelShader.Activate();
    //modelShader.SetVec3Uniform("lightPos", guiLightPos);
    modelShader.SetVec3Uniform("cameraPos", camera.getPos());
    //modelShader.SetVec3Uniform("lightColor", guiLightColor);
    modelShader.SetFloatUniform("lightRadius", guiLightRadius);
    modelShader.SetFloatUniform("ambientStrength", guiAmbient);
    modelShader.SetFloatUniform("specularStrength", guiSpecular);
    modelShader.SetFloatUniform("shininess", guiShininess);
    modelShader.SetMat4Uniform("viewMatrix", view);
    modelShader.SetMat4Uniform("projMatrix", projection);
    modelShader.BindTexture("textures/CMGaTo_crop.jpg", cmgtGatoTexture.getId(), 0);
}
void try_run_benchmark_test(SceneManager& sceneManager, float deltaTime, float fps, int targetFrames)
{
    if (!benchmarkRunning)
        return;


    benchmarkFrameCounter++;


    if (benchmarkFrameCounter >= targetFrames)
    {
        benchmarkRunning = false;
        if (bmCamera) {
            camera.xRotation = 0.0f;
            camera.yRotation = 0.0f;
            camera.transform.rotation = glm::vec3(
                glm::radians(camera.xRotation),
                glm::radians(camera.yRotation),
                0.0f
            );
        }

        camera.transform.updateModelMatrix();
        //writeCSV(benchmarkResult,filename);
        writeExcel(benchmarkResult, filename);
        benchmarkFrameCounter = 0;
        return;
    }


    BenchmarkSample sample;
    sample.time = glfwGetTime() - benchmarkStartTime;
    sample.fps = fps;
    sample.frame = benchmarkFrameCounter;
    sample.frameTime = deltaTime;
    sample.satTests = sceneManager.getSatCount();
    sample.satTime = sceneManager.getSatTime();
    //sample.collisions =
    benchmarkResult.samples.push_back(sample);
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

    GLFWwindow *window = glfwCreateWindow(g_width, g_height, "SpaceEngine", NULL, NULL);

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

    Shader grayShader("shaders/gray.vs","shaders/gray.fs");

    Shader textureShader("shaders/modelVertex.vs","shaders/texture.fs");

    Shader invertColorsShader("shaders/invertColors.vs","shaders/invertColors.fs");

    Shader greyShader("shaders/invertColors.vs","shaders/greyFramebuffer.fs");

    Shader edgeDetectionShader("shaders/invertColors.vs","shaders/edgeDetection.fs");

    Shader pixelizationShader("shaders/invertColors.vs","shaders/pixelatedFramebuffer.fs");

    Shader lineShader("shaders/line.vs","shaders/line.fs");


    SceneManager sceneManager;


    core::Mesh cubeMesh (cubeVertices, cubeIndices);
    core::Model cubeModel({cubeMesh});
    core::Mesh tetraMesh(tetraVerts,tetraIndices);
    core::Model tetraModel({tetraMesh});

    // auto scene1 = sceneManager.createScene("Cube");
    // auto monkey = std::make_shared<GameObject>("Strange Monkey");
    // auto suzanne = scene1->addObject(monkey);
    // suzanne->model = core::AssimpLoader::loadModel("models/nonormalmonkey.obj");
    // suzanne->translate(glm::vec3(-2.0f, 0.0f, 0.0f));
    //suzanne->addBehavior(std::make_shared<Translate>(1.0f,glm::vec3(0.1f, 0.0f, 0.0f)));
     // suzanne->addBehavior(std::make_shared<Rotate>(
     //     glm::vec3(0, 1, 0), 1
     // ));

    auto scene1 = sceneManager.createScene("Cube");
    // auto cube = scene1->addObject(std::make_shared<GameObject>("Cube"));
    // cube->model = core::AssimpLoader::loadModel("models/cube.obj");
    // cube->translate(glm::vec3(0, 0, 0));


    // cube->collider = std::make_shared<ConvexCollider>(cube->model->getAllVertices(),cube->model->getAllIndices(),cube->getModelMatrix());
    // cube->collider->update(cube->getModelMatrix());


    auto cube1 = scene1->addObject(std::make_shared<GameObject>("Cube1"));
    cube1->model = tetraModel;
    cube1->setPos(glm::vec3(-2.0f, 0.0f, 0.0f));
    //cube1->rotate(glm::vec3(1.0f, 0.0f, 0.0f),90);
    cube1->collider = std::make_shared<ConvexCollider>(cube1->model->getAllVertices(),cube1->model->getAllIndices(),cube1->getWorldTransform());


    auto cube2 = scene1->addObject(std::make_shared<GameObject>("Cube2"));
    cube2->model = tetraModel;
    cube2->setPos(glm::vec3(2.0f, 0.0f, 0.0f));
    cube2->collider = std::make_shared<ConvexCollider>(cube2->model->getAllVertices(),cube2->model->getAllIndices(),cube2->getWorldTransform());

    //std::shared_ptr<LightObj> mainLight = nullptr;
    //auto light = scene1->addObject(GameObject("Light"));
    // auto light = std::make_shared<LightObj>(glm::vec3(0.0f, 10.0f, 0.0f), glm::vec4(184.0f/256.0f, 23.0f/256.0f, 222.0f/256.0f, 0.0f),10);
    // mainLight = std::move(light);
    // auto lightSharedptr = std::make_shared<GameObject>("Point Light");
    // auto pointlight = scene1->addObject(lightSharedptr);
    //pointlight->translate(light->getPos());



    auto scene2 = sceneManager.createScene("Car");
    auto carSharedptr = std::make_shared<GameObject>("Car");
    auto car = scene2->addObject(carSharedptr);
    car->model = core::AssimpLoader::loadModel("models/car.obj");
    //car->translate(glm::vec3(2.0f, 0.0f, 0.0f));
    //car->scale(glm::vec3(0.01f, 0.01f, 0.01f));
    car->addBehavior(std::make_shared<Rotate>(
        glm::vec3(1, 0, 0), 1
    ));

    auto quadSharedPtr = std::make_shared<GameObject>("Quad");
    auto quad2 = scene2->addObject(quadSharedPtr);
    quad2->model = core::AssimpLoader::loadModel("models/plane.obj");
    //quad2->rotate(glm::vec3(1,0,0),glm::radians(90.0f));
    //quad2->translate(glm::vec3(-2.5f, 0.0f, 0.0f));


    // auto carSharedptr2 = std::make_shared<GameObject>("Car");
    // auto car2 = scene1->addObject(carSharedptr2);
    // car2->model = core::AssimpLoader::loadModel("models/car.obj");
    // car2->translate(glm::vec3(0.0f, 0.0f, -50.0f));
    //car->scale(glm::vec3(0.01f, 0.01f, 0.01f));
    // car->addBehavior(std::make_shared<Rotate>(
    //     glm::vec3(1, 0, 0), glm::radians(60.0f)
    // ));
    core::Texture cmgtGatoTexture("textures/CMGaTo_crop.png");
    core::Texture stone("textures/stone.jpg");
    core::Texture paint("textures/paint.png");
    core::Texture chalk ("textures/chalk.jpg");
    core::Texture santa("textures/Santa.jpg");
    core::Texture tiles("textures/tiles.jpg");

    glm::vec4 clearColor = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
    glClearColor(clearColor.r,
                 clearColor.g, clearColor.b, clearColor.a);


    camera.transform.setPos(glm::vec3(0.0f, 0.0f, 150.0f));
    //camera.rotate(glm::vec3(1,0,0), -10.0f * 3.1415f / 180);
    camera.speed = 0.2f;

    double currentTime = glfwGetTime();
    double finishFrameTime = 0.0;
    float deltaTime = 0.0f;
    float fps = 0.0f;
    float fpsTimer = 0.0f;
    int fpsFrames = 0;
    double averageTimePerSAT = 0;
    BenchmarkConfig cfg;
    static int objectCountInput = 10;
    static bool objectsSpawned = false;

    float rotationStrength = 100.0f;

    //glm::vec3 guiLightPos;
    // if (mainLight) {
    //     guiLightPos = mainLight->getPos();
    // } else {
    //     guiLightPos = glm::vec3(0.0f, 3.0f, 0.0f);
    // }
    //
    // glm::vec3 guiLightColor;
    //
    // if (mainLight) {
    //     guiLightColor = mainLight->getColor();
    // } else {
    //     guiLightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    // }

    float guiShininess = 75.0f;
    float guiSpecular = 42.0;
    float guiAmbient = 0.25f;
    float guiLightRadius = 35.0f;

    float pixels = 512.0;

    float kernelCenterValueMatrix= -8;


    // --- FRAMEBUFFER SETUP ---
    unsigned int framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    // Create texture attachment
    unsigned int fbTexture;
    glGenTextures(1, &fbTexture);
    glBindTexture(GL_TEXTURE_2D, fbTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, g_width, g_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbTexture, 0);


    unsigned int quadVAO, quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);

    glBindVertexArray(quadVAO);

    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), rectangleVertices, GL_STATIC_DRAW);

    // position attribute (vec2) ndc
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) 0);

    // texcoord attribute (vec2)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) (2 * sizeof(float)));

    glBindVertexArray(0);

    //depth + stencil renderbuffer
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, g_width, g_height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    // Check completeness
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        printf("ERROR: Framebuffer is not complete!\n");

    //unbind
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    GLuint debugVAO, debugVBO;
    glGenVertexArrays(1, &debugVAO);
    glGenBuffers(1, &debugVBO);

    glBindVertexArray(debugVAO);
    glBindBuffer(GL_ARRAY_BUFFER, debugVBO);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

    glBindVertexArray(0);

    float aspect = static_cast<float>(g_width) / g_height;

    while (!glfwWindowShouldClose(window))
    {
        int currentPostProcessingMode = 0;
        im_gui(window, cubeModel,tetraModel, sceneManager, cube1, cube2, deltaTime, fps, averageTimePerSAT,uiConfig,benchmarkConfig);

        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
            sceneManager.setActiveScene("Monkey");
        // if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        //     sceneManager.setActiveScene("Car");

        sceneManager.update(deltaTime);

        auto activeScene = sceneManager.getActiveScene();

        std::vector<std::pair<GameObject*, GameObject*>> pairs;

        if (bool useGridNow = benchmarkRunning ? frozenUseGrid : useGrid) {
            grid.buildGrid(activeScene->objects);
            pairs = grid.computePairs();
        } else {
            pairs = sceneManager.computeBruteForcePairs();
        }

        sceneManager.resetSatStats();

        for (auto& [A, B] : pairs) {
            sceneManager.runSAT(A, B);
        }

        try_run_benchmark_test(sceneManager, deltaTime, fps,framesCountTesting);


        projection = glm::perspective(glm::radians(camera.fov), aspect, 0.1f, 600.0f);
        view = glm::lookAt(camera.getPos(),camera.getPos() + camera.getForward(),camera.getUp());

        if (currentPostProcessingMode == 0) {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        } else {
            glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        }

        glViewport(0, 0, g_width, g_height);
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        set_modelShader(modelShader, cmgtGatoTexture, guiShininess, guiSpecular, guiAmbient, guiLightRadius);

        sceneManager.render(modelShader, projection, view);

        drawLines(lineShader, sceneManager, cube1, debugVAO, debugVBO);

        postProcessing(invertColorsShader, greyShader, edgeDetectionShader, pixelizationShader,
            pixels, kernelCenterValueMatrix,fbTexture, quadVAO, currentPostProcessingMode);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
        glfwPollEvents();

        finishFrameTime = glfwGetTime();
        deltaTime = static_cast<float>(finishFrameTime - currentTime);
        currentTime = finishFrameTime;

        fpsFrames++;
        fpsTimer += deltaTime;

        fps = fpsFrames / fpsTimer;
        fpsFrames = 0;
        fpsTimer = 0.0f;

    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}
