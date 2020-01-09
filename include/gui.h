//
// Created by zvone on 28-Nov-19.
//

#ifndef NXPCARINTERFACE_GUI_H
#define NXPCARINTERFACE_GUI_H


typedef unsigned int GLuint;

struct GLFWwindow;

class Gui {

public:
  explicit Gui(const char *winName = "Nxp Car Interface");
  
  virtual ~Gui();
  
  virtual void render();
  
  static bool loadTextureFromFile(const char *filename, GLuint *out_texture, int *out_width, int *out_height);

protected:
  
  virtual void ui();
  
  virtual void initTextures() {}
  
  virtual void preRenderInit();
  virtual void renderLoop();

private:
  void init();
  
  const char *winName_;
  
  GLFWwindow *window;
  
  const char *glsl_version;
  
  static void glfw_error_callback(int error, const char *description);
};


#endif //NXPCARINTERFACE_GUI_H
