/*******************************************************************************

Definition of member functions of iForest class

Arko Barman
08/01/2017

*******************************************************************************/

#include <iostream>
#include <set>
#include <vector>
#include <random>
#include <cmath>

#include "iForest.h"

using namespace std;

iForest::iForest() {
	// Initialize with default values according to the paper
	nTrees = 100;
	psi = 256;
	maxDepth = 8; // = ceil(log2(psi))
}

iForest::iForest(vector<set<double>> _data, int _nTrees, int _psi, int _nFeatures, int _nDataPoints) {
	data = _data;
	nTrees = _nTrees;
	psi = _psi;
	maxDepth = ceil(log2(psi)); // Calculate maxDepth according to paper
	nFeatures = _nFeatures;
	nDataPoints = _nDataPoints;
}

iForest::iForest(vector<vector<double>> _features, int _nTrees, int _psi, int _nFeatures, int _nDataPoints) {
	vector<set<double>> _data;
	set<double> dummyData;
	// cout<<"Populating sets...\n";
	for(int j = 0; j < _nFeatures; j++) {
		// cout<<"Feature number: "<<j<<endl;
		for(int i = 0; i < _nDataPoints; i++) {
			dummyData.insert(_features[i][j]);
		}
		_data.push_back(dummyData);
	}
	data = _data;
	nTrees = _nTrees;
	psi = _psi;
	maxDepth = ceil(log2(psi)); // Calculate maxDepth according to paper
	nFeatures = _nFeatures;
	nDataPoints = _nDataPoints;
}

iForest::~iForest() {
}

void iForest::setData(vector<set<double>> _data, int _nFeatures) {
	data = _data;
	nFeatures = _nFeatures;
}

void iForest::divideiTreeNode(iTreeNode* node) { // Uses pre-order traversal
	// Do not split if maxDepth is reached
	if(node->getLevel() == maxDepth) {
		node->setExternalFlag(true);
		// cout<<"No split -- maxDepth reached"<<endl;
		return;
	}
	
	// cout<<"Splitting tree at level: "<<node->getLevel()<<endl;
	
	set<double> _data = node->getData(); // get data from node
	// cout<<"Size of data: "<<_data.size()<<endl;
	
	// Do not split if node has only one element
	if(_data.size() == 1) {
		// cout<<"No split -- size = 1"<<endl;
		node->setExternalFlag(true);
		return;
	}
	
	// Get min and max values
	double minValue = *_data.begin(); // get minimum value
	double maxValue = *_data.rbegin(); // get maximum value
	// cout<<"Min: "<<minValue<<"\t\tMax: "<<maxValue<<endl;
	
	// Randomly generate split position
	random_device rd; // obtain a random number from hardware
	mt19937 eng(rd()); // seed the generator
	uniform_real_distribution<double> distr(minValue,maxValue); // define the range
	double _splitPos = distr(eng); // decide a random split position
	// cout<<"Split position: "<<_splitPos<<endl;
	node->setSplitPos(_splitPos); // set split position in node
	node->clearData(); // clear data in this node
	
	// Make the split
	set<double>::iterator itRightStart = _data.lower_bound(_splitPos); // find iterator to data which appears after _splitPos
	set<double>::iterator itRightEnd = _data.end();
	//itRightEnd--;
	set<double> rightChildData(itRightStart,itRightEnd);
	set<double>::iterator itLeftEnd = itRightStart;
	//itLeftEnd--;
	set<double> leftChildData(_data.begin(),itLeftEnd);
	// cout<<"Left child size: "<<leftChildData.size()<<",\t Right child size: "<<rightChildData.size()<<endl;
	
	// Create left and right children nodes
	iTreeNode* leftChild = new iTreeNode(leftChildData);
	iTreeNode* rightChild = new iTreeNode(rightChildData);
	
	// Insert children nodes
	node->addLeftChild(leftChild);
	node->addRightChild(rightChild);
	
	// Recursive calls for pre-order traversal
	divideiTreeNode(leftChild);
	divideiTreeNode(rightChild);
}

void iForest::createTrees() {
	random_device rd; // obtain a random number from hardware
	mt19937 eng(rd()); // seed the generator
	uniform_int_distribution<int> distr(0, nFeatures - 1); // define the range
	for(int i = 0; i < nTrees; i++) {
		
		// cout<<"Creating trees: "<<i+1<<" out of "<<nTrees<<"..."<<endl;
		// Select an attribute/feature
		int randomFeatureNumber = distr(eng);
		// cout<<"Selected feature number: "<<randomFeatureNumber+1<<endl;
		
		// Create and initialize a tree
		iTreeNode *newTree = new iTreeNode(data[randomFeatureNumber]);
		featureIDs.push_back(randomFeatureNumber);
		iTrees.push_back(newTree);
		
		// Divide the root until desired depth is reached
		divideiTreeNode(newTree);
	}
}

double iForest::H(int n) { // Harmonic number
	return  (log(n) + 0.5772156649); // Euler's constant
}

double iForest::c(int n) { // Avg path length of unsuccessful search in BST
	return (2*H(n - 1) - (2*(n - 1)/n));
}

double iForest::getExpectedPathLength(iTreeNode* root, double searchElement) {
	// Base case of recursion
	if(root->getExternalFlag() == true) {
		double height = root->getLevel();
		// cout<<"External node, level: "<<height<<endl;
		set<double> _data = root->getData();
		int size = _data.size();
		// cout<<"Size of data: "<<size<<endl;
		if(size == 1) { // No adjustment required
			// cout<<"Path length: "<<height-1;
			return static_cast<double>(height - 1); // # edges traversed
		}
		else { // Adjustment required for unbuilt tree beyond maxDepth
			// cout<<"Path length + adjustment: "<<(static_cast<double>(height - 1) + c(size))<<endl;
			return (static_cast<double>(height - 1) + c(size)); // # edges traversed + adjustment term
		}
	}
	// Traverse tree similar to binary search
	if(searchElement < root->getSplitPos())
		getExpectedPathLength(root->left, searchElement);
	else
		getExpectedPathLength(root->right, searchElement);
}

double iForest::getAnomalyScore(vector<double> _data) {
	double totPathLength = 0;
	double avgPathLength = 0;
	for(int i = 0; i < nTrees; i++) {
		int featureNumber = featureIDs[i];
		double expPathLength = getExpectedPathLength(iTrees[i], _data[featureNumber]);
		// cout<<"Feature number: "<<featureNumber<<endl;
		// cout<<"Expected path length: "<<expPathLength<<endl;
		totPathLength += expPathLength;
	}
	avgPathLength = totPathLength/nTrees; // = E(h(x))
	// cout<<"Average path length: "<<avgPathLength<<endl;
	// cout<<"c(nDataPoints): "<<c(nDataPoints)<<endl;
	double normAvgPathLength = avgPathLength/c(nDataPoints);
	// cout<<"Normalized average path length: "<<normAvgPathLength<<endl;
	double anomalyScore = pow(0.5, normAvgPathLength);
	// cout<<anomalyScore<<endl;
	return anomalyScore;
}



