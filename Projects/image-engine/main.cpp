#include <GLFW/glfw3.h>
#include <fstream>
#include <glad/gl.h>
#include <iostream>
#include <sstream>

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

void initializeGLFW() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void initializeGLAD() {
  if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return;
  }
  glViewport(0, 0, 800, 600);
}

GLFWwindow *createWindow() {
  GLFWwindow *window = glfwCreateWindow(800, 600, "Image Engine", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed creating the window." << std::endl;
    glfwTerminate();
  }
  glfwMakeContextCurrent(window);
  return window;
}

// helper function
std::string readShaderFile(const char *filepath) {
  std::ifstream file(filepath);
  std::stringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}

int main() {
  // shaders
  std::string vShader = readShaderFile("shaders/vertex.glsl");
  std::string fShader = readShaderFile("shaders/fragment.glsl");

  const char *vertexShaderSource = vShader.c_str();
  const char *fragmentShaderSource = fShader.c_str();

  initializeGLFW();
  GLFWwindow *window = createWindow();
  if (window == NULL) {
    return -1;
  }
  initializeGLAD();

  float vertices[] = {
      // Position             // Color
      // X       Y      Z     R     G     B
      -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // Bottom-left: Red
      0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // Bottom-right: Green
      0.0f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f  // Top: Blue
  };

  unsigned int VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  unsigned int vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  unsigned int fragemntSahder;
  fragemntSahder = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragemntSahder, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragemntSahder);

  unsigned int shaderProgram;
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragemntSahder);
  glLinkProgram(shaderProgram);

  int success;
  char infoLog[512];
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cout << infoLog << std::endl;
    return 0;
  }

  // render loop
  while (!glfwWindowShouldClose(window)) {
    processInput(window);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwTerminate();
  return 0;
}
