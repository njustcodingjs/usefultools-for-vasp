#pragma once

#ifndef BINIO_H
#define BINIO_H

#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <Eigen/Eigen>

namespace ionizing {
  // using namespace std;
  // using namespace Eigen;

  class BinIO {
  public:
    BinIO(const char* FileName);
    ~BinIO();
    BinIO(const BinIO&) = delete;             // Prevent 'copying' operations.
    BinIO& operator=(const BinIO&) = delete;

  template <typename T>
    T readElement();

  template <typename T>
    Eigen::Matrix<T, Eigen::Dynamic, 1> readVectorCol(const long size);

  template <typename T>
    Eigen::Matrix<T, 1, Eigen::Dynamic> readVectorRow(const long size);

  template <typename T>
    Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> readMatrix(const long nRow, const long nCol);

  private:
    std::ifstream ifs;
    std::ofstream ofs;

  private:
    void _openFile(const char* FileName);

  }; // end of class BinIO

/********************************************/
/********  start of implementations  ********/
/********************************************/

  BinIO::BinIO(const char* FileName) {
    _openFile(FileName);
    std::cout << "BinIO created successful" << std::endl;
  }

  BinIO::~BinIO() {
    ifs.close();
    std::cout << "BinIO destructed" << std::endl;
  }

  void BinIO::_openFile(const char* FileName) {
    ifs.open(FileName, std::ios::binary | std::ios::in);
    std::string str_stars(10, '*');
    if (ifs.fail()) {
      std::cerr << str_stars << " Binary File \"" << FileName << "\" open failed " << str_stars << std::endl;
      std::abort();
    }
  }

  template <typename T>
    T BinIO::readElement() {
      long len = sizeof(T);
      T x;
      ifs.read((char*) &x, len);
      return x;
    }

  template <typename T>
    Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> BinIO::readMatrix(const long nRow, const long nCol) {
      Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> matrix;
      matrix.resize(nRow, nCol);
      ifs.read((char*) matrix.data(), nRow * nCol * sizeof(T));
      return matrix;
    }

  template <typename T> 
    Eigen::Matrix<T, Eigen::Dynamic, 1> BinIO::readVectorCol(const long size) {
      return readMatrix<T>(size, 1);
    }
 
  template <typename T>
    Eigen::Matrix<T, 1, Eigen::Dynamic> BinIO::readVectorRow(const long size) {
      return readMatrix<T>(1, size);
    } 

};
#endif  // BINIO_H
