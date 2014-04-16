/*
 * algorithms.cpp
 *
 *  Created on: Mar 5, 2014
 *      Author: olynch
 */
#include "SortingOutSorting.h"
using namespace std;
using namespace std::chrono;

int max(const vector<int>& A) {
	int max = 0;
	for (const_iter i = A.cbegin(); i != A.cend(); ++i) {
		if (*i > max)
			max = *i;
	}
	return max;
}

void printVector(const vector<int>& A) {
	cout << '{';
	for (const_iter i = A.cbegin(); i != A.cend(); ++i)
		cout << *i << ", ";
	cout << "}\n";
}

bool checkSorted(const vector<int>& A) {
	const_iter prev = A.cbegin();
	for (const_iter i = A.cbegin() + 1; i != A.cend(); prev = i++)
		if (*i < *prev) return false;
	return true;
}

inline void swap(iter a, iter b) {
	int save = *a;
	*a = *b;
	*b = save;
}

inline void swap(vector<int>& A, int a, int b) {
	int save = A[a];
	A[a] = A[b];
	A[b] = save;
}

int partition(vector<int>& A, int begin, int end) {
	// pre-condition begin < end
	// post-condition return value must be in range [begin, end]
	if (end - begin <= 1) { //optimization for size <= 2
		if (A[begin] > A[end])
			swap(A, begin, end);
		return begin;
	}
	int pivot = begin + ((end - begin + 1) / 2 - 1);
	int pivotval = A[pivot];
	int left = begin;
	int right = end;
	while (left < right) {
		while (A[left] < pivotval) {
//			cout << "left: " << left << "\n";
			++left;
		}
		while (A[right] > pivotval) {
			--right;
		}
		if (left < right) {
			swap(A, left, right);
			if (A[left] == A[right]) right--;
		}
	}
	//right is pivot
	return right;
}

void quickS_helper(vector<int>& A, int begin, int end) { //sorts the range [begin, end] in the vector A
	// precondition: begin and end must be valid indices in A and begin <= end
	if (begin == end) return;
	int pivot = partition(A, begin, end);
	if (pivot > begin) //slice(begin, pivot - 1).size() > 1
        quickS_helper(A, begin, pivot - 1);
	if (pivot < end) //slice(pivot + 1, end).size() > 1
        quickS_helper(A, pivot + 1, end);
}

namespace heap {
inline int right(int pos) {
	return 2 * pos + 2;
}
inline int left(int pos) {
	return 2 * pos + 1;
}
inline int parent(int pos) {
	return (pos - 1) / 2; //integer division automatically gives floor
}

void reheap(vector<int>& A, int pos, int size) {
	//precondition: A[0:size) is a heap with pos not in the right place, but everything else is
	//postcondition: A[0:size) is a heap
	//runtime = O(logn)
	int leftchild = left(pos);
	while (leftchild < size) {
		if ((A[leftchild] > A[pos]) && (A[leftchild] >= A[leftchild + 1])) {
			swap(A, pos, leftchild);
			pos = leftchild;
		}
		else if ((A[leftchild + 1] > A[pos]) && (A[leftchild + 1] > A[leftchild])) {
			swap(A, pos, leftchild + 1);
			pos = leftchild + 1;
		}
		else return; // no more swapping to be done
		leftchild = left(pos);
	}
}

void heapify(vector<int>& A) {
	//precondition: pos in A
	//postcondition: A is a maxheap
	int node = parent(A.size() - 1); //original node is the last node with children
	while (node >= 0) {
		reheap(A, node, A.size());
		--node;
	}
}
}

void sort_timer::heapS(vector<int>& A) {
	heap::heapify(A);
//	printVector(A);
	for (int i = A.size() - 1; i > 1;) {
		swap(A, 0, i);
		--i;
		heap::reheap(A, 0, i);
//		printVector(A);
	}
	if (A[0] > A[1])
		swap(A, 0, 1); // no point in calling reheap on a 2 element heap
//	printVector(A);
}


void mergeS_helper(iter begin, iter end, int size) {
	if (size == 1) return;
	mergeS_helper(begin, begin + (size / 2), size / 2);
	if (size % 2 == 0) {
		mergeS_helper(begin + (size / 2), end, size / 2);
	}
	else {
		mergeS_helper(begin + (size / 2), end, (size / 2) + 1);
	}
	inplace_merge(begin, begin + (size / 2), end);
}

void sort_timer::bubbleS(vector<int>& A) {
	//tested, is complete
	for(iter end = A.end(); end != A.begin() + 1; end--) {
		for(iter j = A.begin() + 1; j != end; ++j)
			if (*(j - 1) > *j) swap(j - 1, j);
	}
}

void sort_timer::insertionS(vector<int>& A) {
	for (iter i = A.begin() + 1; i != A.end(); ++i) {
		for (iter j = i; j != A.begin(); --j) {
			if (*j >= *(j - 1)) break; //this part is sorted, stop the inner for loop
			swap(j - 1, j); //if not, swap down
		}
	}
}

void sort_timer::selectionS(vector<int>& A) {
	iter min;
	for (iter i = A.begin(); i != A.end(); ++i) {
		min = i;
		for (iter j = i; j != A.end(); ++j)
			if (*j < *min) min = j;
		swap(min, i);
	}
}

void sort_timer::mergeS(vector<int>& A) {
	mergeS_helper(A.begin(), A.end(), A.size());
}

void sort_timer::quickS(vector<int>& A) {
	quickS_helper(A, 0, A.size() - 1);
}

void sort_timer::countingS(vector<int>& A) {
	vector<int> counts (max(A) + 1, 0);
	for (iter i = A.begin(); i != A.end(); ++i) ++counts[*i];
	iter pos = A.begin();
	for (int i = 0; i < counts.size(); ++i) {
		while (counts[i] > 0) {
			*pos = i;
			++pos;
			--(counts[i]);
		}
	}
}

void sort_timer::bucketS(vector<int>& A) {
	int factor; //factor is the average elements per bucket
	if (A.size() < 20) factor = 3;
	else if (A.size() < 500) factor = 5;
	else if (A.size() < 5000) factor = 10;
	else factor = 20;
	int num_buckets = A.size() / factor;
	int bucket_range = (max(A) / num_buckets) + 1;
	vector<priority_queue<int, vector<int>, greater<int> > > buckets (num_buckets + 1);
	for (iter i = A.begin(); i != A.end(); ++i)
		buckets[*i / bucket_range].push(*i);
	iter pos = A.begin();
	for (vector<priority_queue<int, vector<int>, greater<int> > >::iterator i = buckets.begin(); i != buckets.end(); ++i) {
		while (! i->empty()) {
			*pos = i->top();
			i->pop();
			++pos;
		}
	}
}

void sort_timer::timeSorts(const vector<int>& unsorted, const int lineno, const int filenum_pos, matrix3D& sort_data) {
	bool allSuccess = true;
	for (int i = 0; i < 8; i++) {
		vector<int> copyOfA (unsorted.cbegin(), unsorted.cend());
		steady_clock::time_point startTime = steady_clock::now();
//		printVector(copyOfA);
		switch (i) {
		case 0:
			bubbleS(copyOfA);
			break;
		case 1:
			mergeS(copyOfA);
			break;
		case 2:
			quickS(copyOfA);
			break;
		case 3:
			insertionS(copyOfA);
			break;
		case 4:
			selectionS(copyOfA);
			break;
		case 5:
			heapS(copyOfA);
			break;
		case 6:
			countingS(copyOfA);
			break;
		case 7:
			bucketS(copyOfA);
			break;
		}
		sort_data[i][filenum_pos][lineno] = duration_cast<nanoseconds>(steady_clock::now() - startTime).count();
		assert(checkSorted(copyOfA));
	}
//	if (allSuccess) cout << "all success";
//	else cout << "at least one failed";
}
