#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <memory>
#include <vcruntime_startup.h>


#include "MyClasses/Utilities/Camera.h"
#include "core/mesh.h"
#include "core/assimpLoader.h"
#include "core/texture.h"

#include "MyClasses/Transform/Rotate.h"
#include "MyClasses/SceneManagement/Scene.h"
#include "MyClasses/SceneManagement/SceneManager.h"

#include "MyClasses/UI/Style.h"

#include "MyClasses/CollisionSystem/BenschmarkWriter.h"
#include "MyClasses/CollisionSystem/SpatialHashGrid.h"
#include "MyClasses/CollisionSystem/ColliderModels.h"

#include "MyClasses/UI/UIPanels.h"
#include "MyClasses/GlobalVariables.h"


//#define MAC_CLION
#define VSTUDIO


#ifdef VSTUDIO
// Note: install imgui with:
//     ./vcpkg.exe install imgui[glfw-binding,opengl3-binding]

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "MyClasses/Utilities/LightObj.h"
#endif

    //VP
    glm::mat4 view ;
    glm::mat4 projection;

    BenchmarkConfig benchmarkConfig;
    BenchmarkConfig uiConfig;

void framebufferSizeCallback(GLFWwindow *window,
                             int width, int height) {
    g_width = width;
    g_height = height;
    glViewport(0, 0, width, height);
    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
}

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
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
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

void set_modelShader(Shader modelShader, float guiShininess, float guiSpecular, float guiAmbient) {
    modelShader.Activate();
    modelShader.SetVec3Uniform("cameraPos", camera.getPos());
    modelShader.SetVec3Uniform("lightPos", guiLightPos);
    modelShader.SetVec3Uniform("lightColor", guiLightColor);
    modelShader.SetFloatUniform("lightRadius", guiLightRadius);
    modelShader.SetFloatUniform("ambientStrength", guiAmbient);
    modelShader.SetFloatUniform("specularStrength", guiSpecular);
    modelShader.SetFloatUniform("shininess", guiShininess);
    modelShader.SetMat4Uniform("viewMatrix", view);
    modelShader.SetMat4Uniform("projMatrix", projection);
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
        if (autoRun) {
            exit(0);
        }
        return;
    }


    BenchmarkSample sample;
    sample.time = glfwGetTime() - benchmarkStartTime;
    sample.fps = fps;
    sample.frame = benchmarkFrameCounter;
    sample.frameTime = deltaTime;
    sample.satTests = sceneManager.getSatCount();
    sample.satTime = sceneManager.getSatTime();
    sample.collisions = sceneManager.getCollisionsThisFrame();
    benchmarkResult.samples.push_back(sample);
}



int main(int argc,char** argv) {

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];

        if (arg == "--objects") bmObjectCount = std::stoi(argv[++i]);
        else if (arg == "--frames") bmDuration = std::stoi(argv[++i]);
        else if (arg == "--grid") useGrid = std::stoi(argv[++i]) != 0;
        else if (arg == "--cell") grid.cellSize = std::stof(argv[++i]);
        else if (arg == "--label") {
            strncpy(bmLabel, argv[++i], sizeof(bmLabel) - 1);
            bmLabel[sizeof(bmLabel) - 1] = '\0';
        }
        else if (arg == "--tetra") useTetrahedron = std::stoi(argv[++i]) != 0;
        else if (arg == "--seed") uiConfig.randomSeed = std::stoi(argv[++i]);
        else if (arg == "--autorun") autoRun = true;
    }

    if (autoRun&&bmCamera) {
        if (bmCamera) {
            camera.xRotation = 0.0f;
            camera.yRotation = 180.0f;
            camera.transform.rotation = glm::vec3(
                glm::radians(camera.xRotation),
                glm::radians(camera.yRotation),
                0.0f
            );
        }
    }
    uiConfig.objectCount = bmObjectCount;
    uiConfig.testDurationFrames = bmDuration;
    uiConfig.useGrid = useGrid;
    uiConfig.label = bmLabel;
    uiConfig.randomSeed = uiConfig.randomSeed;


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

    // ---------------- Scene 1: Playground  ----------------
    auto playgroundScene = sceneManager.createScene("Playground");

    auto monkey = std::make_shared<GameObject>("Strange Monkey");
    auto suzanne = playgroundScene->addObject(monkey);
    suzanne->model = core::AssimpLoader::loadModel("models/nonormalmonkey.obj");
    suzanne->transform.translate(glm::vec3(-2.0f, 0.0f, 0.0f));
    suzanne->addBehavior(std::make_shared<Rotate>(glm::vec3(0, 1, 0), 40));

    auto orbiter = std::make_shared<GameObject>("Orbiter");
    orbiter->model = core::AssimpLoader::loadModel("models/sphere.obj");;
    orbiter->setPos(glm::vec3(3.0f, 0.0f, 0.0f));
    orbiter->addBehavior(std::make_shared<Rotate>(glm::vec3(0, 1, 1), 300));
    orbiter->transform.scale(glm::vec3(0.5f, 0.5f, 0.5f));
    suzanne->addChild(orbiter);

    auto carSharedptr = std::make_shared<GameObject>("Car");
    auto car = playgroundScene->addObject(carSharedptr);
    car->setPos(glm::vec3(-7.0f, 0.0f, 0.0f));
    car->model = core::AssimpLoader::loadModel("models/car.obj");
    car->addBehavior(std::make_shared<Rotate>(glm::vec3(1, 0, 0), 20));

    auto quadSharedPtr = std::make_shared<GameObject>("Quad");
    auto quad2 = playgroundScene->addObject(quadSharedPtr);
    quad2->setPos(glm::vec3(5.0f, 0.0f, 0.0f));
    quad2->model = core::AssimpLoader::loadModel("models/plane.obj");
    quad2->transform.setRotation(glm::vec3(90,0,0));

    // ---------------- Scene 2: Collision Detection ----------------
    auto collisionScene = sceneManager.createScene("Collision");

    auto cube1 = collisionScene->addObject(std::make_shared<GameObject>("Cube1"));
    cube1->model = tetraModel;
    cube1->setPos(glm::vec3(-2.0f, 0.0f, 0.0f));
    cube1->collider = std::make_shared<ConvexCollider>(cube1->model->getAllVertices(), cube1->model->getAllIndices(), cube1->getWorldTransform());

    auto cube2 = collisionScene->addObject(std::make_shared<GameObject>("Cube2"));
    cube2->model = tetraModel;
    cube2->setPos(glm::vec3(2.0f, 0.0f, 0.0f));
    cube2->collider = std::make_shared<ConvexCollider>(cube2->model->getAllVertices(), cube2->model->getAllIndices(), cube2->getWorldTransform());

    core::Texture cmgtGatoTexture("textures/CMGaTo_crop.png");
    core::Texture stone("textures/stone.jpg");
    core::Texture paint("textures/paint.png");
    core::Texture chalk ("textures/chalk.jpg");
    core::Texture santa("textures/Santa.jpg");
    core::Texture tiles("textures/tiles.jpg");
    core::Texture black("textures/black.png");

    suzanne->texture = chalk;
    orbiter->texture = santa;
    car->texture = cmgtGatoTexture;
    quad2->texture = tiles;

    cube1->texture = black;
    cube2->texture = black;

    glm::vec4 clearColor = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
    glClearColor(clearColor.r,
                 clearColor.g, clearColor.b, clearColor.a);


    camera.transform.setPos(glm::vec3(0.0f, 0.0f, 20.0f));
    //camera.rotate(glm::vec3(1,0,0), -10.0f * 3.1415f / 180);
    camera.speed = 0.02f;

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

    //LightObj light_obj (glm::vec3(0,10,0),glm::vec4(184.0f/256.0f, 23.0f/256.0f, 222.0f/256.0f, 1.0f),35);

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
    int currentPostProcessingMode = 0;
    while (!glfwWindowShouldClose(window))
    {

        UIContext ctx{ sceneManager, cube1, cube2, deltaTime, fps, averageTimePerSAT,
               uiConfig, benchmarkConfig, currentPostProcessingMode,
               pixels, kernelCenterValueMatrix, cubeModel, tetraModel,
             guiShininess, guiSpecular, guiAmbient };
        im_gui(window, ctx);

        auto sceneNames = sceneManager.getSceneNames();
        for (int i = 0; i < (int)sceneNames.size() && i < 5; i++) {
            if (glfwGetKey(window, GLFW_KEY_1 + i) == GLFW_PRESS) {
                sceneManager.setActiveScene(sceneNames[i]);
            }
        }

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

        for (int i = 0; i < pairs.size(); i++) {
            GameObject* A = pairs[i].first;
            GameObject* B = pairs[i].second;
            sceneManager.runSAT(A, B);
        }

        try_run_benchmark_test(sceneManager, deltaTime, fps,bmDuration);

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

        set_modelShader(modelShader, guiShininess, guiSpecular, guiAmbient);

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
