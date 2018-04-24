#include "matrix.h"
#include <iostream>
#include <string>

Matrix::Matrix()
{
  for(int i = 1 ; i< 9; i++){
  	mat[i-1] = i;
  	mat[i+7] = i;
  }
  std::random_shuffle(mat,mat+16);
}

Matrix::~Matrix()
{
}

std::string Matrix::getPosition(int row, int column){
  	return std::to_string(mat[row*8 + column]) + ".png";
  }

