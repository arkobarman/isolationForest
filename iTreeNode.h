/*******************************************************************************

This code defines a class used as a node of a tree for an isolation tree 
implementation. Each node is either an internal node (with no data) with 2 
children or an external node (with data) but no children i.e. leaf nodes.

Arko Barman
08/01/2017

*******************************************************************************/

#ifndef iTreeNode_H
#define iTreeNode_H

#include <set>

using namespace std;

class iTreeNode {
	friend class iForest; // Declaring iForest class as a friend

	private:
		set<double> data; // Stores data for external node, to be used as a 
							// linked list
		int level; // Level of the node in a tree
		iTreeNode* left; // Pointer to left child
		iTreeNode* right; // Pointer to right child
		iTreeNode* parent; // Extra functionality to store parent 
		bool isExternal; // A flag to indicate if node is external i.e. leaf node
		double splitPos; // Position of split at this node, only relevant if 
						// isExternal == false

	public:
		// Constructors and destructor
		iTreeNode();
		iTreeNode(set<double> _data);
		~iTreeNode();
		
		// Other member functions
		
		// Get and set data
		set<double> getData();
		void setData(set<double> _data);
		
		// Add children
		void addLeftChild(iTreeNode* child);
		void addRightChild(iTreeNode* child);
		
		// Get and set parent
		iTreeNode* getParent();
		void setParent(iTreeNode* _parent);
		
		// Get and set split position
		double getSplitPos();
		void setSplitPos(double _split);
		
		// Get and set external flag
		bool getExternalFlag();
		void setExternalFlag(bool _status);
		
		//clear data
		void clearData();
		
		// Get level of node in the tree
		int getLevel();
};

#endif
