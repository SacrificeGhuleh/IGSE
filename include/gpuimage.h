//
// Created by zvone on 29-Nov-19.
//

#ifndef NXPCARINTERFACE_GPUIMAGE_H
#define NXPCARINTERFACE_GPUIMAGE_H

#include <mat.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

// About Desktop OpenGL function loaders:
//  Modern desktop OpenGL doesn't have a standard portable header file to load OpenGL function pointers.
//  Helper libraries are often used for this purpose! Here we are supporting a few common ones (gl3w, glew, glad).
//  You may use another loader/header of your choice (glext, glLoadGen, etc.), or chose to manually implement your own.
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
  
  #include <GL/gl3w.h>    // Initialize with gl3wInit()

#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
  #include <GL/glew.h>    // Initialize with glewInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
  #include <glad/glad.h>  // Initialize with gladLoadGL()
#else

  #include IMGUI_IMPL_OPENGL_LOADER_CUSTOM

#endif

// Include glfw3.h after our OpenGL definitions

#include <GLFW/glfw3.h>


template<int T_ROWS, int T_COLS, int T_CHANNELS>
class GpuImage : public Mat<T_ROWS, T_COLS, T_CHANNELS> {
public: //methods
  
  GpuImage(const char *imgName) : name(imgName) {
    assign();
  }
  
  explicit GpuImage(const uint8_t (&data)[T_ROWS * T_COLS * T_CHANNELS], const char *imgName) : name(imgName) {
    assign();
  }
  
  explicit GpuImage(const uint8_t *data, const char *imgName) : name(imgName) {
    assign();
  }
  
  virtual ~GpuImage() {
  
  }
  
  void render(float scale = 1.f) {
    if (ImGui::Begin(name)) {
      assert(scale > 0.f);
      //refresh and render only if not collapsed
      refresh();
      ImGui::Text("pointer = %p", getGpuTexturePtrId());
      ImGui::Text("size = %d x %d", this->width, this->height);
      ImGui::Image((void *) (intptr_t) getGpuTexturePtrId(), ImVec2(this->width * scale, this->height * scale));
    }
    ImGui::End();
  }
  
  unsigned int getGpuTexturePtrId() const {
    return gpuTexturePtrId;
  }

public: //fields
  const char *name;
private: //methods
  void refresh() {
    glBindTexture(GL_TEXTURE_2D, gpuTexturePtrId);
    
    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // Upload pixels into texture
    
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    
    if (T_CHANNELS == 3) {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->width, this->height, 0, GL_RGB, GL_UNSIGNED_BYTE, this->data_);
    } else if (T_CHANNELS == 4) {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, this->data_);
    } else {
      throw "Invalid number of channels";
    }
  }
  
  void assign() {
    // Create a OpenGL texture identifier
    glGenTextures(1, &gpuTexturePtrId);
    glBindTexture(GL_TEXTURE_2D, gpuTexturePtrId);
    
    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // Upload pixels into texture
    
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    
    if (T_CHANNELS == 3) {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->width, this->height, 0, GL_RGB, GL_UNSIGNED_BYTE, this->data_);
    } else if (T_CHANNELS == 4) {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, this->data_);
    } else {
      throw "Invalid number of channels";
    }
  }

private: //fields
  unsigned int gpuTexturePtrId;
};


#endif //NXPCARINTERFACE_GPUIMAGE_H
