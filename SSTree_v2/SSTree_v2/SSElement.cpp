#include "SSElement.h"

SSElement::SSElement()
{
	this->height = 0;
	this->radius = 0.0;
	this->M = 0;
	this->m = 0;
	this->parent = 0;
	this->isLeaf = true;
	this->wasReinsert = false;

	this->children = new std::vector<SSElement*>;
	this->quant_child = 0;
	this->total_quant_subtree = 0;

	this->data = new vec_of_data;
	this->quant_data = 0;
}

SSElement::SSElement(un_int MM, un_int d)
{
	this->height = 0;
	this->radius = 0.0;
	this->margin.assign(d, 0.0);
	this->M = MM;
	this->m = MM / 2;
	this->parent = 0;
	this->isLeaf = true;
	this->wasReinsert = false;
	
	this->children = new std::vector<SSElement*>;
	this->quant_child = 0;
	this->total_quant_subtree = 0;
	this->min.assign(d, 0);
	this->max.assign(d, 0);

	this->data = new vec_of_data;
	this->quant_data = 0;
}

SSElement::~SSElement()
{
}

bool SSElement::addDato(vec_flot &A) {
	this->data->push_back(&A);
	this->quant_data += 1;
	if (quant_data == 1) {
		this->centroid = A;
		this->radius = 0;
		return true;
	}
	real d = dist(A, this->centroid);
	if (this->radius < d) {
		this->radius = (this->radius + d) / 2;
		recalculateCentroid(A);
	}
	return true;
}
bool SSElement::addChild(SSElement*& S) {
	S->parent = this;
	this->quant_child += 1;
	this->children->push_back(S);
	if (quant_child == 1) {
		this->centroid = S->centroid;
		tam sc = centroid.size();
		this->radius = S->radius;
		#pragma omp parallel for
		for (un_int i = 0; i < sc; ++i)
			this->margin[i] = centroid[i] + radius;
		return true;
	}
	tam sc = centroid.size();
	real d = dist(this->centroid, S->centroid) + S->radius;
	if (this->radius < d) {
		this->radius = (this->radius + d) / 2;
		recalculateCentroid(S);
		#pragma omp parallel for
		for (un_int i = 0; i < sc; ++i)
			this->margin[i] = centroid[i] + radius;
	}
	return true;
}
void SSElement::recalculateCentroid(vec_flot &A){
	tam aT = A.size();
	un_int id = this->quant_data - 1;
	for (tam i = 0; i < aT; ++i) {
		if ((data->at(min[i]))->at(i) > A[i]) {
			min[i] = id;
			centroid[i] = (A[i] + data->at(max[i])->at(i)) / 2;
		}
		if ((data->at(max[i]))->at(i) < A[i]) {
			max[i] = id;
			centroid[i] = (A[i] + data->at(min[i])->at(i)) / 2;
		}
	}
}
void SSElement::recalculateCentroid(SSElement *&SS) {
	tam aT = SS->centroid.size();
	un_int id = this->quant_child - 1;
	for (tam i = 0; i < aT; ++i) {
		if (margin[min[i]] > SS->margin[i]) {
			min[i] = id;
			centroid[i] = (SS->margin[i] + margin[max[i]]) / 2;
		}
		if (margin[max[i]] < SS->margin[i]) {
			max[i] = id;
			centroid[i] = (SS->margin[i] + margin[min[i]]) / 2;
		}
	}
}
void SSElement::free(){
	this->children = new std::vector<SSElement*>;
	this->data= new vec_of_data;
	quant_child = 0;
	quant_data = 0;
	centroid.clear();
	radius = 0.0;
}