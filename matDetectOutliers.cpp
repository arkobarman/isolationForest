/*******************************************************************************

This is the Matlab mex interface file

*******************************************************************************/

#include <iostream>
#include "mex.h"
#include "iForest.h"

using namespace std;

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]) {
	mexPrintf("Starting outlier detection...\n");
	
	// Read the data sent from Matlab
	int nDataPoints;
	int nFeatures;
	vector<vector<double>> features;
	double **featuresBuff;
	
	size_t nRows = mxGetM(prhs[0]);
	size_t nCols = mxGetN(prhs[0]);
	
	nDataPoints = static_cast<int>(nRows);
	nFeatures = static_cast<int>(nCols);
	
	featuresBuff = (double **)mxCalloc(nCols, sizeof(double *));
	
	for(int i = 0; i < nRows; i++)
		featuresBuff[i] = (double *)mxCalloc(nCols, sizeof(double));
	
	for(int i = 0; i < nCols; i++) {
		for(int j = 0; j < nRows; j++)
			featuresBuff[j][i] = ((double *)mxGetData(prhs[0]))[i*nRows + j];
	}
/*	
	// Testing matrix input
	for(int i = 0; i < nRows; i++) {
		for(int j = 0; j < nCols; j++)
			mexPrintf("%f\t", featuresBuff[i][j]);
		mexPrintf("\n");
	}
*/	
	// Copy feature data into a vector object
	for(int i = 0; i < nDataPoints; i++) {
		vector<double> row;
		for(int j = 0; j < nFeatures; j++) {
			row.push_back(featuresBuff[i][j]);
		}
		features.push_back(row);
		row.clear();
	}

/*
	// Testing if data is copied correctly
	for(int i = 0; i < nRows; i++) {
		for(int j = 0; j < nCols; j++) 
			mexPrintf("%f\t", features[i][j]);
		mexPrintf("\n");
	}
*/

	mexPrintf("Data read successfully\n");

	// Create iForest object
	mexPrintf("Creating iForest object...\n");
	iForest forest(features, 100, 256, nFeatures, nDataPoints);
	
	// Create trees in iForest object
	mexPrintf("Creating trees in the iForest object...\n");
	forest.createTrees();
	
	// Generate anomaly scores for all data points
	vector<double> anomalyScores;
	for(int i = 0; i < nDataPoints; i++) {
		anomalyScores.push_back(forest.getAnomalyScore(features[i]));
		mexPrintf("%f\n",forest.getAnomalyScore(features[i]));
	}
	
	// Prepare return value
	double *anomalyScoresBuff = (double *)mxCalloc(nDataPoints, sizeof(double));
	for(int i = 0; i < nDataPoints; i++)
		anomalyScoresBuff[i] = anomalyScores[i];
	
	plhs[0] = mxCreateDoubleMatrix(1, nDataPoints, mxREAL);
	memcpy(mxGetPr(plhs[0]), anomalyScoresBuff, nDataPoints*sizeof(double));
	
	// Deallocate
	mxFree(anomalyScoresBuff);
	for(int i = 0; i < nRows; i++)
		mxFree(featuresBuff[i]);
	mxFree(featuresBuff);
}