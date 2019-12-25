// dear imgui - standalone example application for DirectX 11
// If you are new to dear imgui, see examples/README.txt and documentation at the top of imgui.cpp.

#include "nxpcarinterface.h"


#ifndef NDEBUG
#include <iostream>
#include "includes/mat.h"

#endif //NDEBUG

// Main code
int main(int, char **) {


#ifndef NDEBUG
  testMats();
#endif //NDEBUG
  
  NxpCarInterface gui;
  
  gui.render();
  
  return 0;
}
