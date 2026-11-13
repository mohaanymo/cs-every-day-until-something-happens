#include <GLFW/glfw3.h>
#include <cstdint>
#include <glad/gl.h>
#include <iostream>

struct Image {
  int width;
  int height;
  uint8_t *pixels;
};

int getIndex(int x, int y, int width) { return y * width + x; }

uint8_t getPixel(const Image &img, int x, int y) {
  int index = getIndex(x, y, img.width);
  return img.pixels[index];
}

void setPixel(Image &img, int x, int y, uint8_t value) {
  int index = getIndex(x, y, img.width);
  img.pixels[index] = value;
}

void generateGradient(Image &img) {
  for (int x = 0; x < img.width; x++) {
    for (int y = 0; y < img.height; y++) {
      float brightness = ((float)x / (img.width - 1)) * 255;
      setPixel(img, x, y, (uint8_t)brightness);
    }
  }
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(800, 600, "Image Engine", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed creating the window." << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }
  glViewport(0, 0, 800, 600);

  // render loop
  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwTerminate();
  return 0;
}
