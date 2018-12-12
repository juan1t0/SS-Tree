#include "SSElement.h"

typedef std::vector<SSElement*> vec_child;

#pragma once
class SSTree
{
public:
	SSElement *root;
	Distance dist;
	un_int dim;

	SSTree();
	SSTree(un_int MM, un_int d);
	~SSTree();

	bool insert(vec_flot &);
	vec_of_data search(vec_flot &, un_int);
	SSElement* nearSon(SSElement* &, vec_flot &);
	SSElement* TreatOverflow(SSElement*&);
	SSElement* AdjusTree(SSElement*&, SSElement*&);
	SSElement* SplitLeaf(SSElement*&);
	SSElement* ForkBranch(SSElement*&);
	void Reinsert(SSElement*&);
	bool remove(SSElement*&, un_int i);
	void leafChoosePivots(vec_of_data*&, SSElement*&, SSElement*&);
	void leafAllocateData(vec_of_data*&, SSElement*&, SSElement*&);
	void branchChoosePivots(vec_child&, SSElement*&, SSElement*&);
	void branchAllocateNodes(vec_child&, SSElement*&, SSElement*&);
};
