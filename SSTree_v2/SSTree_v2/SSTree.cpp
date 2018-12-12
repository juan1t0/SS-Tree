#include "SSTree.h"

SSTree::SSTree()
{
	this->root = 0;
}

SSTree::SSTree(un_int MM,un_int d)
{
	this->root = new SSElement(MM,d);
	this->dim = d;
}

SSTree::~SSTree(){}

bool SSTree::insert(vec_flot &A) {
	SSElement* SA = this->root;
	while (!SA->isLeaf) {
		SA = nearSon(SA, A);
	}
	bool in = SA->addDato(A);
	if (!in) return false;
	SSElement* SAA = 0;
	if (SA->quant_data > SA->M) {
		SAA = TreatOverflow(SA);
	}
	un_int mm = root->M;
	SSElement* rot = AdjusTree(SA, SAA);
	if (rot) {
		SSElement* newRoot = new SSElement(mm, this->dim);
		newRoot->addChild(rot);
		newRoot->addChild(this->root);
		this->root = newRoot;
	}
	return true;
}
SSElement* SSTree::nearSon(SSElement* & SS, vec_flot &A) {
	un_int qSons = SS->quant_child, minSon = 0;
	real min_dist = dist((SS->children->at(0))->centroid, A);
	#pragma omp parallel for
	for (un_int i = 1; i < qSons; ++i) {
		real tempD = dist((SS->children->at(i))->centroid, A);
		if (tempD < min_dist) {
			min_dist = tempD;
			minSon = i;
		}
	}
	SSElement* temp = SS->children->at(minSon);
	return temp;
}
SSElement* SSTree::TreatOverflow(SSElement*& S) {
	SSElement* SS = 0;
	if (1 || S->wasReinsert) {
		SS = SplitLeaf(S);
	}
	else {
		Reinsert(S);
	}
	return SS;
}
SSElement* SSTree::AdjusTree(SSElement*& S, SSElement*& SS) {
	while (S) {
		SSElement* dad = S->parent;
		if (!dad)break;
		if (SS) {
			dad->addChild(SS);
			if (dad->quant_child > dad->M){
				SS = ForkBranch(dad);
			}
			else {
				SS = 0;
				break;
			}
		}
		S = dad;
	}
	return SS;
}
void SSTree::Reinsert(SSElement*& S) {
	S->wasReinsert = true;
	un_int porcent = S->M * 0.3 , qDat = S->M-1;
	vec_of_data toRein(S->data->end()-porcent, S->data->end());
	#pragma omp parallel for
	for (un_int i = 0; i < porcent; ++i) {
		remove(S,qDat);
		--qDat;
	}
	#pragma omp parallel for
	for (un_int i = 0; i < porcent; ++i) {
		insert(*(toRein[i]));
	}
}
bool SSTree::remove(SSElement*& S, un_int i){
	vec_flot dato = *(S->data->at(i));
	S->data->erase(S->data->begin() + i);
	for (un_int j = 0; j < dim; ++j) {
		un_int jj=0;
		if (S->min[j] == i) {
			//buscar nuevo min
			S->centroid[j] = (S->data->at(S->max[j])->at(j) + S->data->at(S->min[jj])->at(j)) / 2;
		}
		if (S->max[j] == i) {
			//buscar nuevo max
			S->centroid[j] = (S->data->at(S->max[jj])->at(j) + S->data->at(S->min[j])->at(j)) / 2;
		}
	}
	return true;
}

SSElement* SSTree::SplitLeaf(SSElement*& S) {
	SSElement* SS = new SSElement(S->M, dim);
	vec_of_data *dataTemp = S->data;
	leafChoosePivots(dataTemp, S, SS);
	leafAllocateData(dataTemp, S, SS);
	return SS;
}
SSElement* SSTree::ForkBranch(SSElement*& S) {
	SSElement* SS = new SSElement(S->M, dim);
	vec_child allSons(*(S->children));
	branchChoosePivots(allSons, S, SS);
	branchAllocateNodes(allSons, S, SS);
	return SS;
}

void SSTree::leafChoosePivots(vec_of_data* & D, SSElement* & S, SSElement* & SS) {
	indexes min = S->min;
	indexes max = S->max;
	S->free();
	real maxVariance = abs(D->at(max[0])->at(0)) - abs(D->at(min[0])->at(0));
	un_int p = 0;
	#pragma omp parallel for
	for (un_int i = 1; i < dim; ++i) {
		real varianceAux = abs(D->at(max[i])->at(i)) - abs(D->at(min[i])->at(i));
		if(maxVariance < varianceAux){
			maxVariance = varianceAux;
			p = i;
		}
	}
	sh_int M = max[p], m = min[p];
	S->addDato(*(D->at(m)));
	SS->addDato(*(D->at(M)));
	D->erase(D->begin()+m);
	D->erase(D->begin() + (M-1));
}
void SSTree::leafAllocateData(vec_of_data*& D, SSElement*& S, SSElement*& SS) {
	while (!D->empty()) {
		real ds = dist(*(D->at(0)),S->centroid);
		real dss = dist( * (D->at(0)), S->centroid);
		if (ds <= dss) {
			S->addDato(*(D->at(0)));
		}
		else {
			SS->addDato(*(D->at(0)));
		}
		D->erase(D->begin());
	}
}
void SSTree::branchChoosePivots(vec_child& CH, SSElement*& S, SSElement*& SS) {
	indexes min = S->min;
	indexes max = S->max;
	S->free();
	real maxVariance = abs(CH.at(max[0])->margin[0]) - abs(CH.at(min[0])->margin[0]);
	un_int p = 0;
	#pragma omp parallel for
	for (un_int i = 1; i < dim; ++i) {
		real varianceAux = abs(CH.at(max[i])->margin[i]) - abs(CH.at(min[i])->margin[i]);
		if (maxVariance < varianceAux) {
			maxVariance = varianceAux;
			p = i;
		}
	}
	sh_int M = max[p], m = min[p];
	S->addChild(CH[m]);
	SS->addChild(CH[M]);
	CH.erase(CH.begin() + m);
	CH.erase(CH.begin() + (M - 1));
}
void SSTree::branchAllocateNodes(vec_child& CH, SSElement*& S, SSElement*& SS) {
	while (!CH.empty()) {
		real ds = dist(CH[0]->centroid, S->centroid);
		real dss = dist(CH[0]->centroid, S->centroid);
		if (ds <= dss) {
			S->addChild(CH[0]);
		}
		else {
			SS->addChild(CH[0]);
		}
		CH.erase(CH.begin());
	}
}

vec_of_data SSTree::search(vec_flot &A, un_int kn) {
	vec_of_data res;
	vec_flot distances;
	real d;
	SSElement* SS = this->root;
	while (!SS->isLeaf) {
		real mind = dist(A, SS->children->at(0)->centroid);
		un_int mi = 0;
		for (un_int i = 1; i < SS->quant_child; ++i) {
			d = dist(A, SS->children->at(i)->centroid);
			if (mind > d) {
				mi = i;
				mind = d;
			}
		}
		SS = SS->children->at(mi);
	}
	#pragma omp parallel for
	for (un_int i = 0; i < root->quant_data; ++i) {
		d = dist(A, *(root->data->at(i)));
		distances.push_back(d);
		addknn(A, res, d, kn, distances);
	}
	return res;
}