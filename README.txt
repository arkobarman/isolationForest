C++ compilation:
g++ -std=c++11 test.cpp iForest.cpp iTreeNode.cpp -o test.exe

Matlab mex compilation:
mex matDetectOutliers.cpp iForest.cpp iTreeNode.cpp


Matlab function call:
anomalyScores = matDetectOutliers(features);

features - 
each row = data point
each column = feature/attribute
