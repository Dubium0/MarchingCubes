// MarchingCubes.cpp : Defines the entry point for the application.
//

#include "MarchingCubes.h"

using namespace std;

std::string projectDir = "../../../";
// global vars
const unsigned int SCR_WIDTH = 1320;
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
    glfwSwapInterval(0);
    

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

    
    
   
    const int gridResolution_3D =128; //multiply of 2
    const int gridResolutionCubed = gridResolution_3D * gridResolution_3D * gridResolution_3D;
   
    int triangleTable_flat[256 * 16];

    for (int i = 0; i < 256; i++) {
        for (int k = 0; k < 16; k++) {
           
            triangleTable_flat[i * 16 + k] = triangleTable[i][k];
        }
    }

    //glPointSize(10.0f);
    glm::mat4 model(1.0f);
    glm::mat4 view(1.0f);
    glm::mat4 projection(1.0f);
   
   
    ComputeShader marchingCubesCompute = ComputeShader(projectDir + "resources/shaders/marchingCubes.comp");
   
    unsigned int lookUps, vertexBuffer;
    

    glGenBuffers(1, &lookUps);
    glCheckError();
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, lookUps);
    glCheckError();
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(triangleTable_flat) , triangleTable_flat, GL_DYNAMIC_READ);
    glCheckError();
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, lookUps);
    glCheckError();
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    glGenBuffers(1, &vertexBuffer);
    glCheckError();
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, vertexBuffer);
    glCheckError();
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float)*45* gridResolutionCubed, nullptr, GL_DYNAMIC_COPY);
    glCheckError();
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, vertexBuffer);
    glCheckError();
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    marchingCubesCompute.SetInt("resolution", gridResolution_3D / 2);
    marchingCubesCompute.Activate();
    glDispatchCompute((unsigned int)gridResolution_3D, (unsigned int)gridResolution_3D, (unsigned int)gridResolution_3D);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
    marchingCubesCompute.Deactivate();
    

    // to show the data
    std::vector<float>vertexBuffer_Clean;
    
    {
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, vertexBuffer);

        float* resultData = static_cast<float*>(glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY));
        for (int i = 0; i < 45 * gridResolutionCubed; i += 3) {
            //calcDensities.push_back(resultData[i]);
            if (resultData[i] > -998.0f) {
                vertexBuffer_Clean.push_back(resultData[i + 0]);
                vertexBuffer_Clean.push_back(resultData[i + 1]);
                vertexBuffer_Clean.push_back(resultData[i + 2]);
            }
        }
    
        glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    }
    
    glDeleteBuffers(1, &vertexBuffer);

    unsigned int toBeCalculated, calculation;
    ComputeShader calcNormals_comp = ComputeShader(projectDir + "resources/shaders/calculateNormals.comp");
    glGenBuffers(1, &toBeCalculated);
    glCheckError();
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, toBeCalculated);
    glCheckError();
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float)*vertexBuffer_Clean.size(), &vertexBuffer_Clean[0], GL_DYNAMIC_READ);
    glCheckError();
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER,2, toBeCalculated);
    glCheckError();
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    glGenBuffers(1, &calculation);
    glCheckError();
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, calculation);
    glCheckError();
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float) * vertexBuffer_Clean.size(), nullptr, GL_DYNAMIC_COPY);
    glCheckError();
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, calculation);
    glCheckError();
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    calcNormals_comp.SetInt("resolution", gridResolution_3D / 2);
    calcNormals_comp.Activate();
    glDispatchCompute(vertexBuffer_Clean.size()/9,1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
    calcNormals_comp.Deactivate();

    std::vector<float> surfaceNormals;

    {
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, calculation);

        float* resultData = static_cast<float*>(glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY));
        for (int i = 0; i < vertexBuffer_Clean.size(); i += 3) {
            //calcDensities.push_back(resultData[i]);
           
            surfaceNormals.push_back(resultData[i + 0]);
            surfaceNormals.push_back(resultData[i + 1]);
            surfaceNormals.push_back(resultData[i + 2]);
            //std::cout << resultData[i + 0];
        }

        glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    }

    glDeleteBuffers(1, &calculation);
    glDeleteBuffers(1, &toBeCalculated);

    std::vector<float> verticesWithNormals;

    for (int i = 0; i < surfaceNormals.size(); i += 3) {
        // insert positions
        verticesWithNormals.push_back(vertexBuffer_Clean[i]);
        verticesWithNormals.push_back(vertexBuffer_Clean[i+1]);
        verticesWithNormals.push_back(vertexBuffer_Clean[i+2]);
        // insert normals
        verticesWithNormals.push_back(surfaceNormals[i]);
        verticesWithNormals.push_back(surfaceNormals[i + 1]);
        verticesWithNormals.push_back(surfaceNormals[i + 2]);


    }

    vertexBuffer_Clean.~vector();
    surfaceNormals.~vector();


    VertexArray VAO = VertexArray();
    VertexBuffer VBO = VertexBuffer(&verticesWithNormals[0], sizeof(float) * verticesWithNormals.size());
    VertexArrayAttribute attribute = VertexArrayAttribute();
    attribute.PushAttributef(3);
    attribute.PushAttributef(3);
    VAO.AddVertexArrayAttributef(VBO, attribute);
    Shader shader = Shader(projectDir + "resources/shaders/lit.vert", projectDir + "resources/shaders/lit.frag");
    //grid.~vector();
   
   



    float scaleValue = 1;
    camera.Position = glm::vec3(-0.5f, 6.0f, 14.0f);
    camera.Pitch = -25.0f;
    bool wireFrameMod = false;
    glm::vec3 lightdir(1.0f,0.0f,0.0f);

    std::cout << sizeof(float) * vertexBuffer_Clean.size();
    int vertexCount = 0;
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
        ImGui::Text("Number of Vertices : %.3f", vertexBuffer_Clean.size() / 3.0f);
      
        ImGui::End();



        ImGui::Begin("Drawing Settins");
        ImGui::SliderFloat("Cube Sizes", &scaleValue, 0.0f, 2.0f);
        std::string res = "Resoulution : " +  to_string(gridResolution_3D) ;
        ImGui::Text(res.c_str());
        ImGui::SliderFloat3("Light Direction", glm::value_ptr(lightdir), -1, 1);
        ImGui::Checkbox("WireFrameMode", &wireFrameMod);
        if(wireFrameMod){
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        }
        ImGui::End();
        model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(1.0f) * scaleValue);
        view = camera.GetViewMatrix();
        projection = glm::perspective(glm::radians(camera.Zoom), (float)WIDTH / (float)HEIGHT, 0.01f, 1000.0f);

        shader.SetMatrix4("model", model);
        shader.SetMatrix4("view", view);
        shader.SetMatrix4("projection", projection);
       
        shader.SetVec3("lightDir", lightdir);
       

        //opengls stuff
        glClearColor(0.28f, 0.28f, 0.28f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        shader.Activate();
        VAO.Bind();
        
        
        //if (vertexCount < (unsigned int)(verticesWithNormals.size() / 6)) {
        //    vertexCount+=1;
        //}
        
        glDrawArrays(GL_TRIANGLES, 0, (unsigned int)(verticesWithNormals.size() / 6));
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(128.0f,0.0f,0.0f));
        shader.SetMatrix4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, (unsigned int)(verticesWithNormals.size() / 6));
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