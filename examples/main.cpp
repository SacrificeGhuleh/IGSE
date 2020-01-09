// dear imgui - standalone example application for DirectX 11
// If you are new to dear imgui, see examples/README.txt and documentation at the top of imgui.cpp.

#include "example.h"


#ifndef NDEBUG
#include <iostream>
#include <mat.h>

#endif //NDEBUG

// Main code
int main(int, char **) {


#ifndef NDEBUG
  testMats();
#endif //NDEBUG
  
  Example gui;
  
  gui.render();
  
  return 0;
}
