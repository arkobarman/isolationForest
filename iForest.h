/*******************************************************************************

This code defines a class for storing and manipulating isolation forests.

Arko Barman
08/01/2017

*******************************************************************************/

#ifndef iForest_H
#define iForest_H

#include <iostream>
#include <set>
#include <vector>

#include "iTreeNode.h"

using namespace std;

class iForest {
	private:
		vector<set<double>> data; // input data
		int nTrees; // # trees in the iForest
		int psi; // sub-sampling size
		vector<iTreeNode*> iTrees; // vector storing iTrees
		vector<int> featureIDs; // feature IDs selected for creating trees
		int maxDepth; // maximum depth of created trees
		int nFeatures; // # features
		int nDataPoints; // # data points used in building model
		
		double H(int n); // Harmonic number
		double c(int n); // Avg path length of unsuccessful search in BST
		double getExpectedPathLength(iTreeNode* root, double searchElement); // Get depth of desired searchElement in an iTree
		void divideiTreeNode(iTreeNode* node); // Divide node recursively using pre-order traversal
		
	public:
		iForest();
		iForest(vector<set<double>> _data, int _nTrees, int _psi, int _nFeatures, int _nDataPoints);
		iForest(vector<vector<double>> _features, int _nTrees, int _psi, int _nFeatures, int _nDataPoints);
		~iForest();
		
		void setData(vector<set<double>> _data, int _nFeatures); // Initialize data and # features
		void createTrees(); // Create iTrees from data
		double getAnomalyScore(vector<double> _data);
};

#endif
	