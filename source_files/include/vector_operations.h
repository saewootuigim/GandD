#ifndef _VECTOR_OPERATIONS_H_
#define _VECTOR_OPERATIONS_H_

#include <iostream>
#include <vector>
#include <string>

using std::vector;
using std::string;
using std::size_t;

string standardize( string str );
bool check_whether_to_skip( string name );
bool check_inside_vector( vector<string> vec, string name, size_t *pos );

#endif