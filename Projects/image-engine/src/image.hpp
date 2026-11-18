#pragma once

#include <cstdint>

struct Image {
  int width;
  int height;
  uint8_t *pixels;
};

int getIndex(int x, int y, int width);
uint8_t getPixel(const Image &img, int x, int y);
void setPixel(Image &img, int x, int y, uint8_t value);
void generateGradient(Image &img);
