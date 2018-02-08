/*******************************************************************************

Definition of member functions of iTreeNode class

Arko Barman
08/01/2017

*******************************************************************************/

#include <set>
#include <iostream>
#include "iTreeNode.h"

using namespace std;

// Default constructor
iTreeNode::iTreeNode() {
	level = 1;
	left = NULL;
	right = NULL;
	parent = NULL;
	isExternal = true;
}

// Parametrized constructor
iTreeNode::iTreeNode(set<double> _data) {
	setData(_data);
	level = 1;
	left = NULL;
	right = NULL;
	parent = NULL;
	isExternal = true;
}

// Destructor
iTreeNode::~iTreeNode() {
	if(left) {
		left->~iTreeNode();
		delete left;
	}
	if(right) {
		right->~iTreeNode();
		delete right;
	}
}

// Get data
set<double> iTreeNode::getData() {
	return data;
}

// Set data
void iTreeNode::setData(set<double> _data) {
	data = _data;
}

// Adding left child
void iTreeNode::addLeftChild(iTreeNode* child) {
	child->setParent(this);
	child->level = this->level + 1;
	left = child;
	setExternalFlag(false); // node is no longer external
}

// Adding right child
void iTreeNode::addRightChild(iTreeNode* child) {
	child->setParent(this);
	child->level = this->level + 1;
	right = child;
	setExternalFlag(false); // node is no longer external
}

// Get parent
iTreeNode* iTreeNode::getParent() {
	return parent;
}

// Set parent
void iTreeNode::setParent(iTreeNode* _parent) {
	parent = _parent;
}

// Get split position
double iTreeNode::getSplitPos() {
	return splitPos;
}

// Set split position
void iTreeNode::setSplitPos(double _split) {
	splitPos = _split;
}

// Get external flag
bool iTreeNode::getExternalFlag() {
	return isExternal;
}

// Set external flag
void iTreeNode::setExternalFlag(bool _status) {
	isExternal = _status;
}

// Clear data
void iTreeNode::clearData() {
	data.clear();
}

// Get level of the node in the tree
int iTreeNode::getLevel() {
	return level;
}
