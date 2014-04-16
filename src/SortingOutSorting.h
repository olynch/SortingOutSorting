/*
 * SortingOutSorting.h
 *
 *  Created on: Mar 12, 2014
 *      Author: olynch
 */
#include <vector>
#include <chrono>
#include <string>
#include <map>
#include <algorithm>
#include <random>
#include <functional>
#include <queue>
#include <sstream>
#include <fstream>
#include <iostream>
#include <ios>
#include <cassert>
#include "boost/multi_array.hpp"
#include "json.h"

#ifndef SORTINGOUTSORTING_H_
#define SORTINGOUTSORTING_H_
//declare useful shortcuts
typedef std::vector<int>::iterator iter;
typedef std::vector<int>::const_iterator const_iter;
typedef boost::multi_array<unsigned long, 3> matrix3D;

//declare important methods
namespace sort_timer {
	void bubbleS(std::vector<int>& A);
	void mergeS(std::vector<int>& A);
	void quickS(std::vector<int>& A);
	void insertionS(std::vector<int>& A);
	void selectionS(std::vector<int>& A);
	void heapS(std::vector<int>& A);
	void countingS(std::vector<int>& A);
	void bucketS(std::vector<int>& A);
	void timeSorts(const std::vector<int>& unsorted, const int lineno, const int filenum_pos, matrix3D& sort_data);
}

int main(int argc, char** argv);

#endif /* SORTINGOUTSORTING_H_ */
