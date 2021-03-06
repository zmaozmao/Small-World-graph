#pragma once
#include <queue>
#include <set>
#include <map>
#include <iostream>
#include "GraphRepresentation.h"
#include "priorityFunction.h"

template< typename T > unordered_set <T> oneFilp(unordered_set<T> currentSolution, GraphRepresentation<T> *g,double alpha,double lambda,int priorityFunction, bool bestImprovement)
{
	unordered_set<T> best = currentSolution;

	for (T component : currentSolution)
	{
		unordered_set<T> adjacentSolution = currentSolution;
		adjacentSolution.erase(component);
		adjacentSolution = solutionReconstruction(adjacentSolution, g, alpha, lambda, priorityFunction,component);

		if (adjacentSolution.size() > best.size())
		{
			best = adjacentSolution;
			adjacentSolution.clear();
			if (!bestImprovement)
				return best;


		}
		 adjacentSolution.clear();
	}
	
	return best;

}




template< typename T > unordered_set <T> solutionReconstruction(unordered_set<T> S1, GraphRepresentation<T> *g, double alpha, double lambda, int priorityFunction, T toEsxclude)
{

	bool addable = false;
	unordered_set <T> S;
	while (S.size() < S1.size())
	{
		S = S1;
		
		GraphRepresentation<T> *induced = g->induction(S);
		
		//GraphRepresentation<T> *induced;
		unordered_set<T> ver = g->getNeighborsFromVertices(S);
		vector<T> sorted;

		
		//sorted.insert(sorted.end(), ver.begin(), ver.end());
		
		if (!addable)
			ver.erase(toEsxclude);
		
		if (priorityFunction == 2)

			sorted =  sortedVerteces<T>(ver, induced, g).getSorted();
		
		else
			sorted =  sortedVerteces<T>(ver, g).getSorted();
		
		unordered_set<T>().swap(ver);
		ver.clear();
		delete induced;
		
		for (T v : sorted)
		{
			induced = g->inductionV(S, v);
			double mLAD = maxLocalAverageDistance(induced);
			double mLC = minimumLocalCluster(induced);
			delete induced;
			

			if ((mLAD <= lambda) && (mLC >= alpha))
			{
				S1.insert(v);
				addable = true;
				break;
			}
		}

	}

	S.clear();
	return S1;
}



template< typename T > unordered_set <T> randNeighbor(unordered_set<T> currentSolution, GraphRepresentation<T> *g, double alpha, double lambda, int priorityFunction, int seed)
{
	

	//Generating random number
	std::mt19937 rng(std::random_device{}());
	std::uniform_int_distribution<> dist(0, RAND_MAX);
	int random = dist(rng);

	// random element from solution
	auto r = random % currentSolution.size(); 
	auto it = std::begin(currentSolution);
	advance(it, r);

	
	unordered_set<T> adjacentSolution = currentSolution;
	adjacentSolution.erase(*it);
	
	
	
	adjacentSolution = solutionReconstruction(adjacentSolution, g, alpha, lambda, priorityFunction, *it);

	return adjacentSolution;

}

//Solution Reconstruction ILS
template< typename T > unordered_set <T> solutionReconstruction2(unordered_set<T> S1, GraphRepresentation<T> *g, double alpha, double lambda, int priorityFunction, unordered_set<T> toEsxclude)
{

	bool addable = false;
	unordered_set <T> S;
	while (S.size() < S1.size())
	{
		S = S1;
		GraphRepresentation<T> *induced = g->induction(S);
		// da togliere
		//GraphRepresentation<T> *induced;
		unordered_set<T> ver = g->getNeighborsFromVertices(S);
		vector <T> sorted;
		sorted.insert(sorted.end(), ver.begin(), ver.end());
		if (!addable)
		{
			for(T toEsxcludeA : toEsxclude)
				ver.erase(toEsxcludeA);

		}
		
		if (priorityFunction == 2)

			sorted = sortedVerteces<T>(ver, induced, g).getSorted();

		else
			sorted = sortedVerteces<T>(ver, g).getSorted();

		unordered_set<T>().swap(ver);
		ver.clear();
		delete induced;

		
		for (T v : sorted)
		{
			induced = g->inductionV(S, v);
			double mLAD = maxLocalAverageDistance(induced);
			double mLC = minimumLocalCluster(induced);
			delete induced;


			if ((mLAD <= lambda) && (mLC >= alpha))
			{
				S1.insert(v);
				addable = true;
				break;
			}
		}

	}

	S.clear();
	return S1;
}

//ILS OneFlip Version
template< typename T > unordered_set <T> oneFilp(unordered_set<T> currentSolution, GraphRepresentation<T> *g, double alpha, double lambda, int priorityFunction, bool bestImprovement, unordered_set<T> toExclude)
{
	unordered_set<T> best = currentSolution;

	for (T component : currentSolution)
	{
		unordered_set<T> adjacentSolution = currentSolution;
		adjacentSolution.erase(component);
		toExclude.insert(component);
		adjacentSolution = solutionReconstruction2(adjacentSolution, g, alpha, lambda, priorityFunction, toExclude);
		toExclude.erase(component);
		if (adjacentSolution.size() > best.size())
		{
			best = adjacentSolution;
			adjacentSolution.clear();
			if (!bestImprovement)
				return best;


		}
		adjacentSolution.clear();
	}

	return best;

}