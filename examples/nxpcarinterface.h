//
// Created by zvone on 22-Nov-19.
//
#ifndef NXPCARINTERFACE_NXPCARINTERFACE_H
#define NXPCARINTERFACE_NXPCARINTERFACE_H

#include <memory>
#include <mutex>
#include <atomic>

#include <gui.h>
#include <gpuimage.h>

class NxpCarInterface : public Gui {
public:
  NxpCarInterface() : Gui("Nxp Car Interface") {};
  
  virtual ~NxpCarInterface() = default;
  
  virtual void render() override;

protected:
  
  virtual void ui() override;
  
  virtual void initTextures() override;
  
  virtual void producer();

private:
  std::mutex tex_data_lock_;
  
  std::atomic<bool> finish_request_{false};
};


#endif //NXPCARINTERFACE_NXPCARINTERFACE_H