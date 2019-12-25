//
// Created by zvone on 22-Nov-19.
//

#include <thread>
#include <chrono>

#include "nxpcarinterface.h"

using namespace std::chrono_literals;

void NxpCarInterface::ui() {
  Gui::ui();
  {
    std::lock_guard<std::mutex> lock(tex_data_lock_);
    //originalImage_->render();
  }
}

void NxpCarInterface::initTextures() {
  Gui::initTextures();
  
}

void NxpCarInterface::render() {
  std::thread producer_thread(&NxpCarInterface::producer, this);
  
  Gui::render();
  
  finish_request_.store(true, std::memory_order_release);
  producer_thread.join();
}

void NxpCarInterface::producer() {
  while (!finish_request_.load(std::memory_order_acquire)) {
  
  }
  std::cout << "Producer thread stop\n";
}
