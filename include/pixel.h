//
// Created by zvone on 28-Nov-19.
//

#ifndef NXPCARINTERFACE_PIXEL_H
#define NXPCARINTERFACE_PIXEL_H


#include <cstdint>

#include <cstdint>

template<int T_CHANNELS>
union Pixel {
};

template<>
union Pixel<3> {
  Pixel() : r(0), g(0), b(0) {}
  
  Pixel(uint8_t _r, uint8_t _g, uint8_t _b) : r(_r), g(_g), b(_b) {}
  
  struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
  };
  uint8_t data[3];
};

template<>
union Pixel<4> {
  Pixel() : r(0), g(0), b(0), a(255) {}
  
  Pixel(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a) : r(_r), g(_g), b(_b), a(_a) {}
  
  struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
  };
  uint8_t data[4];
};


#endif //NXPCARINTERFACE_PIXEL_H
