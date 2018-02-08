#include "iTreeNode.h"
#include "iForest.h"

#include <fstream>
#include <string>
#include <sstream>

using namespace std;

int main() {
	cout<<"Beginning test..."<<endl<<endl;
	
	cout<<"Testing iTreeNode class..."<<endl;
	
	cout<<"Initializing data..."<<endl;
	double testData[] = {5.4, 1.2, 7.8, 9.2, 0.5, -2.5};
	
	cout<<"Creating a set..."<<endl;
	set<double> _data(testData, testData + 6);
	
	cout<<"Creating an iTreeNode object with the set..."<<endl;
	iTreeNode *root = new iTreeNode(_data);
	
	cout<<"Testing getData()..."<<endl;
	set<double> newData = root->getData();
	if(newData != _data)
		cout<<"Failed!!!"<<endl;
	
	cout<<"Testing setData()..."<<endl;
	newData.insert(10.6);
	root->setData(newData);

	cout<<"Testing addLeftChild()..."<<endl;
	iTreeNode *leftChild = new iTreeNode();
	root->addLeftChild(leftChild);

	cout<<"Testing addRightChild()..."<<endl;
	iTreeNode *rightChild = new iTreeNode();
	root->addRightChild(rightChild);

	cout<<"Testing getParent()..."<<endl;
	if(leftChild->getParent() != root || rightChild->getParent() != root)
		cout<<"Failed!!!"<<endl;

	cout<<"Testing setParent()..."<<endl;
	iTreeNode *dummy = new iTreeNode();
	root->setParent(dummy);
	if(root->getParent() != dummy)
		cout<<"Failed!!!"<<endl;

	cout<<"Testing setSplitPos()..."<<endl;
	root->setSplitPos(5);

	cout<<"Testing getSplitPos()..."<<endl;
	if(root->getSplitPos() != 5)
		cout<<"Failed!!!"<<endl;

	cout<<"Testing getExternalFlag()..."<<endl;
	if(dummy->getExternalFlag() != true || root->getExternalFlag() != false)
		cout<<"Failed!!!"<<endl;

	cout<<"Testing setExternalFlag()..."<<endl;
	dummy->setExternalFlag(false);
	if(dummy->getExternalFlag())
		cout<<"Failed!!!"<<endl;

	cout<<"Testing clearData()..."<<endl;
	root->clearData();
	newData = root->getData();
	if(newData.size())
		cout<<"Failed!!!"<<endl;

	cout<<"Testing getLevel()..."<<endl;
	if(root->getLevel() != 1 || leftChild->getLevel() !=2 || rightChild->getLevel() != 2) {
		cout<<"Root level = "<<root->getLevel()<<endl;
		cout<<"Children level = "<<leftChild->getLevel()<<" and "<<rightChild->getLevel()<<endl;
		cout<<"Failed!!!"<<endl;
	}
	cout<<"Success"<<endl<<endl<<endl;


	cout<<"Testing iForest class..."<<endl;

	cout<<"Reading data from file..."<<endl;
	ifstream inf;
	ofstream outf;
	string line;
	vector<vector<double>> inputData;
	vector<double> featureVector;
	string currentLine;
/*
	for(int i = 0; i < 4096; i++)
		featureData.push_back(temp);
*/
	inf.open("features_32_LBP.txt");
	outf.open("iForest_anomalyScores_32_LBP.txt");
	int iDataPoints = 0, iFeatures = 0; 
	while(getline(inf, currentLine)) {
		stringstream line(currentLine);
		iFeatures = 0;
		while(line) {
			string s;
			if(!getline(line, s, ','))
				break;
			featureVector.push_back(stof(s));
		}
		inputData.push_back(featureVector);
		featureVector.clear();
		iDataPoints++;
	}
	cout<<"# features: "<<inputData[1].size()<<"\t# Data points: "<<inputData.size()<<endl;
	
	cout<<"Creating an iForest object..."<<endl;
	iForest forest(inputData, 100, 256, inputData[0].size(), inputData.size());
/*
	cout<<"Testing setData()..."<<endl;
	forest.setData(featureData, 7);
*/

	cout<<"Testing createTrees()..."<<endl;
	forest.createTrees();
	
	cout<<"Testing getAnomalyScore()..."<<endl;
	cout<<"Anomaly scores: "<<endl;
	for(int i = 0; i < inputData.size(); i++) {
		double anomalyScore = forest.getAnomalyScore(inputData[i]);
		outf<<anomalyScore<<endl;
		cout<<anomalyScore<<endl;
	}
	inf.close();
	outf.close();

	cout<<"Success"<<endl<<endl;
	
	return 0;
}
