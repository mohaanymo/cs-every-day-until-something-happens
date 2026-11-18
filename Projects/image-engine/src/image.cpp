#include "image.hpp"

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
