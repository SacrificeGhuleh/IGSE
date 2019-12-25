//
// Created by zvone on 28-Nov-19.
//

#ifndef NXPCARINTERFACE_MAT_H
#define NXPCARINTERFACE_MAT_H


#include <cstdio>
#include <cassert>
#include <cstring>

#ifndef NDEBUG

#include <iostream>

#endif //NDEBUG

#include "pixel.h"

template<int T_ROWS, int T_COLS, int T_CHANNELS>
class Mat {
public:
  explicit Mat() :
      cols(T_COLS),
      rows(T_ROWS),
      channels(T_CHANNELS),
      size(T_ROWS * T_COLS * T_CHANNELS),
      pixels(T_ROWS * T_COLS) {
#ifndef NDEBUG
    std::cout << "Allocating data" << std::endl;
#endif //NDEBUG
    data_ = new uint8_t[size];
    
    std::memset(data_, 0, T_ROWS * T_COLS * T_CHANNELS);
  }
  
  explicit Mat(const uint8_t(&data)[T_ROWS * T_COLS * T_CHANNELS]) :
      cols(T_COLS),
      rows(T_ROWS),
      channels(T_CHANNELS),
      size(T_ROWS * T_COLS * T_CHANNELS),
      pixels(T_ROWS * T_COLS) {
#ifndef NDEBUG
    std::cout << "Allocating data" << std::endl;
#endif //NDEBUG
    data_ = new uint8_t[size];
    
    std::memcpy(data_, data, T_ROWS * T_COLS * T_CHANNELS);
  }
  
  explicit Mat(const uint8_t *data) :
      cols(T_COLS),
      rows(T_ROWS),
      channels(T_CHANNELS),
      size(T_ROWS * T_COLS * T_CHANNELS),
      pixels(T_ROWS * T_COLS) {
#ifndef NDEBUG
    std::cout << "Allocating data" << std::endl;
#endif //NDEBUG
    data_ = new uint8_t[size];
    
    std::memcpy(data_, data, T_ROWS * T_COLS * T_CHANNELS);
  }
  
  virtual ~Mat() {
#ifndef NDEBUG
    std::cout << "Deleting data" << std::endl;
#endif //NDEBUG
    delete[] data_;
  }
  
  Pixel<T_CHANNELS> &at(const unsigned int index) {
    assert(((void) "Index out of array", index < pixels));
    return reinterpret_cast<Pixel<T_CHANNELS> &>(data_[index * T_CHANNELS]);
  }
  
  Pixel<T_CHANNELS> &at(const unsigned int row, const unsigned int col) {
    assert(((void) "Row out of bounds", row < T_ROWS));
    assert(((void) "Col out of bounds", col < T_COLS));
    
    const unsigned int offset = (row * T_COLS + col);
    return at(offset);
  }
  
  uint8_t *getData() { return data_; }
  
  uint8_t *getRow(const int row) {
    assert(((void) "Row out of bounds", row < T_ROWS));
    return data_ + (row * T_COLS * T_CHANNELS);
  }
  
  Mat<1, T_COLS, T_CHANNELS> getRowMat(const int row) {
    return Mat<1, 3, 3>(getRow(1));
  }
  
  void oneChannelToMultiChannel(
      const uint8_t(&oneChannelData)[T_COLS],
      uint8_t(&multiChannelData)[T_COLS * T_CHANNELS]) {
    for (int col = 0; col < T_COLS; col++) {
      for (int channel = 0; channel < T_CHANNELS; channel++) {
        int idx = col * T_CHANNELS + channel;
        assert(((void) "Idx out of bounds", idx < T_COLS * T_CHANNELS));
        multiChannelData[idx] = oneChannelData[col];
      }
    }
  }
  
  void pushBack(const uint8_t(&data)[T_COLS * T_CHANNELS]) {
    std::memmove(data_, data_ + T_COLS * T_CHANNELS,
                 size - T_COLS * T_CHANNELS);
    std::memcpy(data_ + (size - T_COLS * T_CHANNELS), data,
                T_COLS * T_CHANNELS);
  }
  
  void pushFront(const uint8_t(&data)[T_COLS * T_CHANNELS]) {
    std::memmove(data_ + T_COLS * T_CHANNELS, data_,
                 size - T_COLS * T_CHANNELS);
    std::memcpy(data_, data, T_COLS * T_CHANNELS);
  }
  
  void pushBack(const uint8_t(&data)[T_COLS]) {
    uint8_t multiData[T_COLS * T_CHANNELS];
    oneChannelToMultiChannel(data, multiData);
    pushBack(multiData);
  }
  
  void pushFront(const uint8_t(&data)[T_COLS]) {
    uint8_t multiData[T_COLS * T_CHANNELS];
    oneChannelToMultiChannel(data, multiData);
    pushFront(multiData);
  }
  
  void print() {
    for (int row = 0; row < T_ROWS; row++) {
      for (int col = 0; col < T_COLS; col++) {
        auto &pix = at(row, col);
        printf("|%03d,%03d,%03d| ", pix.r, pix.g, pix.b);
      }
      printf("\n");
    }
  }

public:
  const unsigned int size;
  const unsigned int pixels;
  const unsigned int channels;
  union {
    const unsigned int width;
    const unsigned int cols;
  };
  
  union {
    const unsigned int height;
    const unsigned int rows;
  };

protected:
  uint8_t *data_;
private:
};

static void testMats() {
  
  Mat<3, 3, 3> mat;
  uint8_t pixels[9] = {67, 68, 69,
                       51, 52, 53,
                       98, 85, 54};
  
  uint8_t pixels3[3] = {55, 88, 22};
  
  mat.print();
  
  mat.pushBack(pixels);
  std::cout << "pushing pixels\n";
  mat.print();
  mat.pushBack(pixels3);
  std::cout << "pushing pixels\n";
  mat.print();
  
  mat.pushFront(pixels);
  std::cout << "pushing pixels\n";
  mat.print();
  
  mat.pushFront(pixels3);
  std::cout << "pushing pixels\n";
  mat.print();
  
  auto row = Mat<1, 3, 3>(mat.getRow(1));
  row.print();
  
  auto rowMat = mat.getRowMat(2);
  rowMat.print();
}

#endif //NXPCARINTERFACE_MAT_H
