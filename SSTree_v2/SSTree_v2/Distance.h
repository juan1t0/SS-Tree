#include <math.h>
#include <vector>
#include <iostream>

typedef std::vector<double> vec_flot;
typedef std::size_t tam;
typedef float real;
typedef unsigned int un_int;
typedef std::vector<vec_flot*> vec_of_data;
typedef short int sh_int;
typedef std::vector<sh_int> indexes;

#pragma once
class Distance
{
public:
	Distance();
	~Distance();

	real operator()(vec_flot &, vec_flot &);
};

inline bool operator ==(vec_flot &, vec_flot &);
void addknn(vec_flot& A, vec_of_data& res, real d, un_int k,vec_flot& dis);