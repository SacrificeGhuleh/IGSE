//
// Created by zvone on 28-Nov-19.
//

#include <gui.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

// #define IMGUI_IMPL_OPENGL_LOADER_GL3W 1

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
#include <cstdio>

#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>


static bool show_demo_window = true;
static bool show_another_window = false;
static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

// Simple helper function to load an image into a OpenGL texture with common settings
bool Gui::loadTextureFromFile(const char *filename, GLuint *out_texture, int *out_width, int *out_height) {
  // Load from file
  int image_width = 0;
  int image_height = 0;
  unsigned char *image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
  if (image_data == NULL)
    return false;
  
  // Create a OpenGL texture identifier
  GLuint image_texture;
  glGenTextures(1, &image_texture);
  glBindTexture(GL_TEXTURE_2D, image_texture);
  
  // Setup filtering parameters for display
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  
  // Upload pixels into texture
  glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
  stbi_image_free(image_data);
  
  *out_texture = image_texture;
  *out_width = image_width;
  *out_height = image_height;
  
  return true;
}

void Gui::init() {
  glfwSetErrorCallback(glfw_error_callback);
  if (!glfwInit())
    throw "Unable to init GLFW";
  
  // Decide GL+GLSL versions
#if __APPLE__
  // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
  // GL 3.0 + GLSL 130
  glsl_version = "#version 130";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
  //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif
  
  // Create window with graphics context
  window = glfwCreateWindow(1280, 720, winName_, NULL, NULL);
  if (window == NULL)
    throw "Unable to create window";
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1); // Enable vsync
  
  // Initialize OpenGL loader
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
  bool err = gl3wInit() != 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
  bool err = glewInit() != GLEW_OK;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
  bool err = gladLoadGL() == 0;
#else
  bool err = false; // If you use IMGUI_IMPL_OPENGL_LOADER_CUSTOM, your loader is likely to requires some form of initialization.
#endif
  if (err) {
    fprintf(stderr, "Failed to initialize OpenGL loader!\n");
    throw "Failed to initialize OpenGL loader";
  }
}

void Gui::glfw_error_callback(int error, const char *description) {
  fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

void Gui::ui() {
  // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
  if (show_demo_window)
    ImGui::ShowDemoWindow(&show_demo_window);
  
  // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
  {
    static float f = 0.0f;
    static int counter = 0;
    
    ImGui::Begin(
        "Hello, world!");                          // Create a window called "Hello, world!" and append into it.
    
    ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
    ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
    ImGui::Checkbox("Another Window", &show_another_window);
    
    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
    ImGui::ColorEdit3("clear color", (float *) &clear_color); // Edit 3 floats representing a color
    
    if (ImGui::Button(
        "Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
      counter++;
    ImGui::SameLine();
    ImGui::Text("counter = %d", counter);
    
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
                ImGui::GetIO().Framerate);
    ImGui::End();
  }
  
  // 3. Show another simple window.
  if (show_another_window) {
    ImGui::Begin("Another Window",
                 &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
    ImGui::Text("Hello from another window!");
    if (ImGui::Button("Close Me"))
      show_another_window = false;
    ImGui::End();
  }
}

Gui::Gui(const char *winName) : winName_{winName} {
  init();
}

Gui::~Gui() {
  printf("Gui destructor\n");
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  
  glfwDestroyWindow(window);
  glfwTerminate();
}

void Gui::render() {
  preRenderInit();
  renderLoop();
}

void Gui::preRenderInit() {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void) io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
  //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
  io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
  //io.ConfigViewportsNoAutoMerge = true;
  //io.ConfigViewportsNoTaskBarIcon = true;
  
  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  //ImGui::StyleColorsClassic();
  
  // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
  ImGuiStyle &style = ImGui::GetStyle();
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    style.WindowRounding = 0.0f;
    style.Colors[ImGuiCol_WindowBg].w = 1.0f;
  }
  
  // Setup Platform/Renderer bindings
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);
  
  initTextures();
}

void Gui::renderLoop() {
// Main loop
  while (!glfwWindowShouldClose(window)) {
    // Poll and handle events (inputs, window resize, etc.)
    // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
    // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
    // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
    // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
    glfwPollEvents();
    
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    
    static bool p_open = true;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
    
    {
      ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
      
      ImGuiViewport *viewport = ImGui::GetMainViewport();
      ImGui::SetNextWindowPos(viewport->Pos);
      ImGui::SetNextWindowSize(viewport->Size);
      ImGui::SetNextWindowViewport(viewport->ID);
      ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
      ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
      window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                      ImGuiWindowFlags_NoMove;
      window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
      
      
      if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;
      
      ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
      ImGui::Begin("DockSpace Demo", &p_open, window_flags);
      ImGui::PopStyleVar();
      
      ImGui::PopStyleVar(2);
      
      ImGuiIO &io = ImGui::GetIO();
      if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
      }
      
      if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("Menu")) {
          
          if (ImGui::MenuItem("MenuItem1", "", false)) {
          
          }
          if (ImGui::MenuItem("MenuItem2", "", false)) {
          }
          ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
      }
      ImGui::End();
    }
    //Ui routine to be overriden in child class
    
    ui();
    
    // Rendering
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    
    // Update and Render additional Platform Windows
    // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
    //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
      GLFWwindow *backup_current_context = glfwGetCurrentContext();
      ImGui::UpdatePlatformWindows();
      ImGui::RenderPlatformWindowsDefault();
      glfwMakeContextCurrent(backup_current_context);
    }
    glfwSwapBuffers(window);
  }
}
