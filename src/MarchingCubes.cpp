// MarchingCubes.cpp : Defines the entry point for the application.
//

#include "MarchingCubes.h"

using namespace std;

int main()
{
    const unsigned int SCR_WIDTH = 800;
    const unsigned int SCR_HEIGHT = 800;

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CMAKE BUILDED OPENGL APP", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK)
        std::cout << "GlEW IS NOT OKEY (HELP)" << std::endl;

    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);


    while (!glfwWindowShouldClose(window))
    {
       
        // check-call events and swap buffer
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
}
