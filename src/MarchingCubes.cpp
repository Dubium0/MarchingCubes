// MarchingCubes.cpp : Defines the entry point for the application.
//

#include "MarchingCubes.h"

using namespace std;

std::string projectDir = "../../../";
// global vars
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
int WIDTH = SCR_WIDTH;
int HEIGHT = SCR_HEIGHT;
bool firstMouse = true;
double lastX = SCR_WIDTH / 2.0f, lastY = SCR_HEIGHT / 2.0f;
Camera camera = Camera();
double lastFrame = 0, deltaTime = 0;


bool onCameraMode = false;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);


int main(int argc, char* argv[])
{


    // glfw initialization------------------------------------------------------------
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Marching Cubes", NULL, NULL);
    if (window == NULL)
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK)
        cout << "GlEW IS NOT OKEY (HELP)" << endl;

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    

    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glEnable(GL_DEPTH_TEST);
    //--------------------------------------------------------------------------------------------------------
    //--init imgui---------------
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    const char* glsl_version = "#version 430";
    ImGui_ImplOpenGL3_Init(glsl_version);

    //-------------------------------

   
    // for 3D grid
    std::vector<float> grid;
    int gridResolution_3D =256;
    float unitSize = 1;
    //3D grid loop
    for (int x = 0; x < gridResolution_3D; x++) {
        for (int y = 0; y < gridResolution_3D; y++) {
            for (int z = 0; z < gridResolution_3D; z++) {
                grid.push_back(((float)x - gridResolution_3D / 2.0f) * unitSize);
                grid.push_back(((float)y - gridResolution_3D / 2.0f) * unitSize);
                grid.push_back(((float)z - gridResolution_3D / 2.0f) * unitSize);
            }
        }
    }
  
    int triangleTable_flat[256 * 16];

    for (int i = 0; i < 256; i++) {
        for (int k = 0; k < 16; k++) {
           
            triangleTable_flat[i * 16 + k] = triangleTable[i][k];
        }
    }

    
    

    VertexArray VAO = VertexArray();
    VertexBuffer VBO = VertexBuffer(&grid[0], grid.size() * sizeof(float));
    VertexArrayAttribute attribute = VertexArrayAttribute();
    attribute.PushAttributef(3);
    VAO.AddVertexArrayAttributef(VBO, attribute);
    Shader shader = Shader(projectDir + "resources/shaders/marchingCubes.vert",
        projectDir + "resources/shaders/marchingCubes.frag",
        projectDir + "resources/shaders/marchingCubes.geom");

 

    //glPointSize(10.0f);
    glm::mat4 model(1.0f);
    glm::mat4 view(1.0f);
    glm::mat4 projection(1.0f);
   
    float scaleValue =1;
   
    
   
    unsigned int edgeMaskSSBO, triangleTableSSBO;
    glGenBuffers(1, &edgeMaskSSBO);
    glCheckError();
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, edgeMaskSSBO);
    glCheckError();
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(edgeMask), edgeMask, GL_STATIC_DRAW);
    glCheckError();
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, edgeMaskSSBO);
    glCheckError();
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    glCheckError();
    glGenBuffers(1, &triangleTableSSBO);
    glCheckError();
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, triangleTableSSBO);
    glCheckError();
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(triangleTable_flat), triangleTable_flat, GL_STATIC_DRAW);
    glCheckError();
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, triangleTableSSBO);
    glCheckError();
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    glCheckError();

    camera.Position = glm::vec3(-0.5f, 6.0f, 14.0f);
    camera.Pitch = -25.0f;
    bool wireFrameMod = false;
    while (!glfwWindowShouldClose(window))
    {
        double currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        //handle input
        processInput(window);

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        // handle rendering

        //imgui stuff
        ImGui::Begin("Camera Settings");
        ImGui::SliderFloat("Movement Speed", &camera.MovementSpeed, 0.0f, 50.0f);
        ImGui::SliderFloat("Sensitivity", &camera.MouseSensitivity, 0.0f, 5.0f);
        ImGui::SliderFloat3("Camera Position", glm::value_ptr(camera.Position), -100.f, 100.0f);
        ImGui::SliderFloat("Camera Pitch", &camera.Pitch, -100.f, 100.0f);
        ImGui::SliderFloat("Camera Yaw", &camera.Yaw, -180.f, 180.0f);
        camera.updateCameraVectors();
        ImGui::End();

        ImGui::Begin("Application Stats");
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::Text("Number of Vertices : %.3f", grid.size() / 3.0f);
      
        ImGui::End();



        ImGui::Begin("Drawing Settins");
        ImGui::SliderFloat("Cube Sizes", &scaleValue, 0.0f, 2.0f);
        ImGui::Checkbox("WireFrameMode", &wireFrameMod);
        if(wireFrameMod){
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        }
        ImGui::End();



        
        



        //opengls stuff
        glClearColor(0.28f, 0.28f, 0.28f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(1.0f) * scaleValue);
        view = camera.GetViewMatrix();
        projection = glm::perspective(glm::radians(camera.Zoom), (float)WIDTH / (float)HEIGHT, 0.01f, 1000.0f);

        shader.SetMatrix4("model", model);
        shader.SetMatrix4("view", view);
        shader.SetMatrix4("projection", projection);
       
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, edgeMaskSSBO);
        glCheckError();
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, triangleTableSSBO);
        glCheckError();
        shader.Activate();
        VAO.Bind();
        glDrawArrays(GL_POINTS, 0, grid.size()/3);
        glCheckError();
        

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        


        // check-call events and swap buffer---------glfw stuff----------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
   

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    WIDTH = width;
    HEIGHT = height;
}
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
       
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {

        if(onCameraMode)
            camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {

        if (onCameraMode)
            camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {

        if (onCameraMode)
            camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {

        if (onCameraMode)
            camera.ProcessKeyboard(RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        if (onCameraMode)
            camera.ProcessKeyboard(UPWARD, deltaTime);

    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {

        if (onCameraMode)
            camera.ProcessKeyboard(DOWNWARD, deltaTime);

    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {

        onCameraMode = true;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE) {

        onCameraMode = false;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}




// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;
    if(onCameraMode)
        camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (onCameraMode)
        camera.ProcessMouseScroll(yoffset);
}