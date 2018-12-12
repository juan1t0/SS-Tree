#include "Distance.h"

//element of the SStree
#pragma once
class SSElement
{
public:
	un_int height;
	real radius;
	vec_flot centroid;
	vec_flot margin;
	un_int M, m;
	SSElement* parent;
	bool isLeaf;
	bool wasReinsert;
	Distance dist;
	indexes min;
	indexes max;
	
	std::vector<SSElement*> *children;
	un_int quant_child;
	un_int total_quant_subtree;

	vec_of_data *data;
	un_int quant_data;

	SSElement();
	SSElement(un_int M,un_int d);
	~SSElement();

	bool addDato(vec_flot &);
	bool addChild(SSElement*&);
	void free();
	void recalculateCentroid(vec_flot &);
	void recalculateCentroid(SSElement *&);
};