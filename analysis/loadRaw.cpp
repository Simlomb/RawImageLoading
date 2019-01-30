#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <string.h>
#include <stdio.h>
#include <iomanip>
#include "dirent.h"
#include <sys/time.h>
#include <bitset>
#include <limits>
#include <stdlib.h>
#include <png.h>

using namespace std;

// http://en.cppreference.com/w/cpp/language/type_alias
using byte = unsigned char ;

// http://en.cppreference.com/w/cpp/types/numeric_limits
constexpr size_t BITS_PER_BYTE = numeric_limits<byte>::digits ;

// http://en.cppreference.com/w/cpp/utility/bitset
using bits_in_byte = bitset<BITS_PER_BYTE> ;

long BinaryToDecimal(string binaryNumber)
{
  long num = stol(binaryNumber);
  long base = 1, dec = 0;
  while (num > 0)
    {
    int rem = num % 10;
    dec = dec + rem * base;
    base = base * 2;
    num = num / 10;
    }
  return dec;
}

vector< vector<long> > to_2d( const vector<long>& flat_vec, size_t ncols )
{
  // sanity check
  if( ncols == 0 || flat_vec.size()%ncols != 0 )
    {
    throw domain_error( "bad #cols" ) ;
    }

  const auto nrows = flat_vec.size() / ncols ;
  vector< vector<long> > mtx ;
  const auto begin = std::begin(flat_vec) ;
  for( size_t row = 0 ; row < nrows ; ++row )
    {
    mtx.push_back( { begin + row*ncols, begin + (row+1)*ncols } ) ;
    }
  return mtx;
}

void read_bits( string filename, vector< vector <vector<long>>>* matrix, size_t ncols, size_t nrows )
{
  cout<<"Reading : "<<filename<<endl;
  ifstream f(filename, ios::binary);
  char c;
  vector<long> redVector;
  vector<long> greenVector;
  vector<long> blueVector;
  size_t ncols2 = static_cast<size_t>(ncols * 0.5);

  vector<long> greenVectorTemp;
  for (size_t ii = 0; ii < nrows; ii++)
    {
    for (size_t jj = 0; jj < ncols2; jj++)
      {
      if (!(ii % 2))
        {
        f.get(c);
        string redColor = bits_in_byte( byte(c) ).to_string();
        f.get(c);
        string secondBit = bits_in_byte( byte(c) ).to_string();
        //std::reverse(firstBit.begin(), firstBit.end());
        redColor.append(secondBit.substr(0,4));
        string greenColor = secondBit.substr(4,4);
        f.get(c);
        greenColor.append(bits_in_byte( byte(c) ).to_string());
        //std::reverse(secondBit.begin(), secondBit.end());

        redVector.push_back(BinaryToDecimal(redColor));
        greenVectorTemp.push_back(BinaryToDecimal(greenColor));
        }
      else
        {
        f.get(c);
        string greenColor = bits_in_byte( byte(c) ).to_string();
        f.get(c);
        string secondBit = bits_in_byte( byte(c) ).to_string();
        //std::reverse(firstBit.begin(), firstBit.end());
        greenColor.append(secondBit.substr(0,4));
        string blueColor = secondBit.substr(4,4);
        f.get(c);
        blueColor.append(bits_in_byte( byte(c) ).to_string());
        //std::reverse(secondBit.begin(), secondBit.end());

        greenVector.push_back(0.45 * (BinaryToDecimal(greenColor) + greenVectorTemp[jj]));
        blueVector.push_back(BinaryToDecimal(blueColor));
        }
      }
    if (ii % 2)
      {
      greenVectorTemp.clear();
      }
    }

  matrix->push_back(to_2d(redVector, ncols2));
  matrix->push_back(to_2d(greenVector, ncols2));
  matrix->push_back(to_2d(blueVector, ncols2));
}

int main(int argc, char * argv[]) {

  if (!strcmp(argv[1], "-h"))
    {
    //
    //cout<<"-m : for mulitple files (FITS table with the string _r_ in the name)"<<endl;
    //cout<<"-677721s : for a single file (FITS table)"<<endl<<endl;
    return 0;
    }

  string filename;
  if (!strcmp(argv[1], "-s"))
    {
    filename = argv[2];
    }

  vector<vector<vector<long>>> matrix;
  read_bits(filename, &matrix, 4096, 3072);

  ofstream output_file("RedMatrix.txt");
  if (output_file.is_open())
    {
    for (int ii = 0; ii < matrix[0].size(); ++ii)
      {
      for (int jj = 0; jj < matrix[0][ii].size(); ++jj)
        {
        output_file << std::setprecision(0) << matrix[0][ii][jj]<< "  ";
        }
      output_file << endl;
      }
    output_file.close();
    }

  ofstream output_file1("GreenMatrix.txt");
  if (output_file1.is_open())
    {
    for (int ii = 0; ii < matrix[1].size(); ++ii)
      {
      for (int jj = 0; jj < matrix[1][ii].size(); ++jj)
        {
        output_file1 << std::setprecision(0) << matrix[1][ii][jj]<< "  ";
        }
      output_file1 << endl;
      }
    output_file1.close();
    }

  ofstream output_file2("BlueMatrix.txt");
  if (output_file2.is_open())
    {
    for (int ii = 0; ii < matrix[2].size(); ++ii)
      {
      for (int jj = 0; jj < matrix[2][ii].size(); ++jj)
        {
        output_file2 << std::setprecision(0) << matrix[2][ii][jj]<< "  ";
        }
      output_file2 << endl;
      }
    output_file2.close();
    }

  return 0;
}
