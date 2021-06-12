#include "common.h"
#include "Player.h"
#include "Tiles.h"
#include "Maze.h"

#define GLFW_DLL
#include <GLFW/glfw3.h>

constexpr GLsizei WINDOW_WIDTH = 1104;
constexpr GLsizei WINDOW_HEIGHT = 760;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

struct InputState {
    bool keys[1024]{}; //массив состояний кнопок - нажата/не нажата
    GLfloat lastX = 400, lastY = 300; //исходное положение мыши
    bool firstMouse = true;
    bool captureMouse         = true;  // Мышка захвачена нашим приложением или нет?
    bool capturedMouseJustNow = false;
} static Input;




void OnKeyboardPressed(GLFWwindow* window, int key, int scancode, int action, int mode) {
    switch (key) {
    case GLFW_KEY_ESCAPE:
        if (action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);
        break;
    case GLFW_KEY_1:
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        break;
    case GLFW_KEY_2:
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        break;
    default:
        if (action == GLFW_PRESS)
            Input.keys[key] = true;
        else if (action == GLFW_RELEASE)
            Input.keys[key] = false;
    }
}

void processPlayerMovement(Player &player) {
    if (Input.keys[GLFW_KEY_W])
        player.ProcessInput(MovementDir::DOWN);
    else if (Input.keys[GLFW_KEY_S])
        player.ProcessInput(MovementDir::UP);
    if (Input.keys[GLFW_KEY_A])
        player.ProcessInput(MovementDir::LEFT);
    else if (Input.keys[GLFW_KEY_D])
        player.ProcessInput(MovementDir::RIGHT);
}

void OnMouseButtonClicked(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
        Input.captureMouse = !Input.captureMouse;
    }

    if (Input.captureMouse) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        Input.capturedMouseJustNow = true;
    } else {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

}

void OnMouseMove(GLFWwindow* window, double xpos, double ypos) {

    if (Input.firstMouse) {
        Input.lastX = float(xpos);
        Input.lastY = float(ypos);
        Input.firstMouse = false;
    }

    GLfloat xoffset = float(xpos) - Input.lastX;
    GLfloat yoffset = Input.lastY - float(ypos);

    Input.lastX = float(xpos);
    Input.lastY = float(ypos);
}


int initGL() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        return -1;
    }

    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    std::cout << "Controls: "<< std::endl;
    std::cout << "press right mouse button to capture/release mouse cursor  "<< std::endl;
    std::cout << "W, A, S, D - movement  "<< std::endl;
    std::cout << "Space - attack  "<< std::endl;
    std::cout << "press ESC to exit" << std::endl;

    return 0;
}

int main(int argc, char** argv) {

    if(!glfwInit())
        return -1;

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow*  window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "task1 base project", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glfwSetKeyCallback        (window, OnKeyboardPressed);
    glfwSetCursorPosCallback  (window, OnMouseMove);
    glfwSetMouseButtonCallback(window, OnMouseButtonClicked);

    if(initGL() != 0)
        return -1;

    GLenum gl_error = glGetError();
    while (gl_error != GL_NO_ERROR) {
        gl_error = glGetError();
    }

    Image screenBuffer(WINDOW_WIDTH, WINDOW_HEIGHT, 3);

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);  GL_CHECK_ERRORS;
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); GL_CHECK_ERRORS;

    ////////////////////////////////////////////////////

    TMaze maze("../resources/maze.txt");
    maze.SetScreen(screenBuffer);

    lastFrame = glfwGetTime();
    GLfloat interval = 0.07;

    bool spaceReleased = true;
    
    while (!glfwWindowShouldClose(window)) {

        GLfloat currentFrame = glfwGetTime();
        if (currentFrame - lastFrame > interval) {
            TTile::NextFrame();
            lastFrame = currentFrame;
        }

        glfwPollEvents();

        if (!maze.CheckFading() && !maze.CheckWin() && !maze.CheckLose()) {
            processPlayerMovement(maze.GetPlayer());
            if (Input.keys[GLFW_KEY_SPACE] && spaceReleased) {
                maze.MeleeAttack();
                spaceReleased = false;
            }
            if (!Input.keys[GLFW_KEY_SPACE]) {
                spaceReleased = true;
            }
        }
        maze.ProcessGame();

        glRasterPos2f(-1, 1);
        glPixelZoom(3, -3);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
        glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}
