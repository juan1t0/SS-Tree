#include "Distance.h"

Distance::Distance()
{
}
Distance::~Distance()
{
}

real Distance::operator()(vec_flot &A, vec_flot &B){
	real sum = 0.0;
	tam Atam = A.size();
	#pragma omp parallel for
	for (tam i = 0; i < Atam; ++i) {
		sum += pow((abs(A[i]) - abs(B[i])), 2);
	}
	return sqrt(sum);
}

inline bool operator ==(vec_flot &A, vec_flot &B) {
	tam Atam = A.size();
	tam Btam = B.size();
	if (Atam != Btam) return false;
	for (tam i = 0; i < Atam; ++i) {
		if (A[i] != B[i]) return false;
	}
	return true;
}
void addknn(vec_flot& A, vec_of_data& res, real d, un_int k, vec_flot& dis) {
	tam sr = res.size();
	vec_flot* aa = &A;
	if (res.empty()) {
		res.push_back(aa);
		return;
	}
	tam i;
	for (i = 0; i < sr; ++i) {
		if (d <= dis[i]) {
			break;
		}
	}
	res.insert(res.begin() + i, aa);
	dis.insert(dis.begin() + i, d);
	tam s = dis.size();
	if (s > k) {
		res.erase(res.begin() + k, res.end());
		dis.erase(dis.begin() + k, dis.end());
	}
}