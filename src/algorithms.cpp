/*
 * algorithms.cpp
 *
 *  Created on: Mar 5, 2014
 *      Author: olynch
 */
#include "SortingOutSorting.h"
using namespace std;
using namespace std::chrono;

/*
 * The following are methods that are not in sort_timer namespace,
 * They are helpers for the ones that are
 */

int max(const vector<int>& A) {
	//returns the largest element in A
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
	//swap the contents of the two iterators
	int save = *a;
	*a = *b;
	*b = save;
}

inline void swap(vector<int>& A, int a, int b) {
	//swap A[a] and A[b]
	int save = A[a];
	A[a] = A[b];
	A[b] = save;
}

/*
 * End of sort-independent functions
 */

// bubble sort

void sort_timer::bubbleS(vector<int>& A) {
	//sorts A using bubble sort, 'nuff said
	for(iter end = A.end(); end != A.begin() + 1; end--) {
		for(iter j = A.begin() + 1; j != end; ++j)
			if (*(j - 1) > *j) swap(j - 1, j);
	}
}

/*
 * Merge sort and related methods
 */

void mergeS_helper(iter begin, iter end, int size) {
	// sorts the first half, then the second half recursively, then does an inplace merge
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

void sort_timer::mergeS(vector<int>& A) {
	mergeS_helper(A.begin(), A.end(), A.size());
}

/*
 * Quick sort and related methods
 */

int partition(vector<int>& A, int begin, int end) {
	// pre-condition begin < end
	// post-condition return value must be in range [begin, end]
	if (end - begin <= 1) { //optimization for size <= 2
		if (A[begin] > A[end])
			swap(A, begin, end);
		return begin;
	}
	int pivot = begin + ((end - begin + 1) / 2 - 1); // begin + A[begin:end].size()/2 - 1,
	// basically, the middle of A[begin:end], guaranteed to be in (begin, end)
	int pivotval = A[pivot];
	int left = begin;
	int right = end;
	while (left < right) {
		while (A[left] < pivotval) {
			// find a pos less than pivotval
			++left;
		}
		while (A[right] > pivotval) {
			// find a pos greater than pivotval
			--right;
		}
		if (left < right) {
			// make sure we haven't crossed over, then swap
			swap(A, left, right);
			if (A[left] == A[right]) right--;
			//this stops an infinite loop from happening
		}
	}
	//right is pivot
	return right;
}

void quickS_helper(vector<int>& A, int begin, int end) { //sorts the range [begin, end] in the vector A
	// precondition: begin and end must be valid indices in A and begin <= end
	if (begin == end) return;
	// run partition on A recursively
	int pivot = partition(A, begin, end);
	if (pivot > begin) //slice(begin, pivot - 1).size() > 1
        quickS_helper(A, begin, pivot - 1);
	if (pivot < end) //slice(pivot + 1, end).size() > 1
        quickS_helper(A, pivot + 1, end);
}

void sort_timer::quickS(vector<int>& A) {
	quickS_helper(A, 0, A.size() - 1);
}

// Insertion Sort

void sort_timer::insertionS(vector<int>& A) {
	if (A.size() <= 1) return;
	for (iter i = A.begin() + 1; i != A.end(); ++i) {
		// start at A[1], so that A[-1] isn't accessed in the next thing
		for (iter j = i; j != A.begin(); --j) {
			// go down to A[0]
			if (*j >= *(j - 1)) break; //this part is sorted, stop the inner for loop
			swap(j - 1, j); //if not, swap down
		}
	}
}

// Selection Sort

void sort_timer::selectionS(vector<int>& A) {
	iter min;
	for (iter i = A.begin(); i != A.end(); ++i) {
		min = i;
		//min begins as just i
		for (iter j = i; j != A.end(); ++j)
			//find the smallest element
			if (*j < *min) min = j;
		swap(min, i);
		//swap i with the smalles element
	}
}

/*
 * Heap sort and related methods
 */

inline int heap_left(int pos) {
	return 2 * pos + 1;
}
inline int heap_parent(int pos) {
	return (pos - 1) / 2; //integer division automatically gives floor
}

void reheap(vector<int>& A, int pos, int size) {
	//precondition: A[0:size) is a heap with pos not in the right place, but everything else is
	//postcondition: A[0:size) is a heap
	//runtime = O(logn)
	int leftchild = heap_left(pos);
	while (leftchild < size) {
		// find if either of pos's children is larger than it, if so, swap
		if ((A[leftchild] > A[pos]) && (A[leftchild] >= A[leftchild + 1])) {
			swap(A, pos, leftchild);
			pos = leftchild;
		}
		else if ((A[leftchild + 1] > A[pos]) && (A[leftchild + 1] > A[leftchild])) {
			swap(A, pos, leftchild + 1);
			pos = leftchild + 1;
		}
		else return; // no more swapping to be done, pos in the right place
		leftchild = heap_left(pos);
	}
}

void heapify(vector<int>& A) {
	//precondition: pos in A
	//postcondition: A is a maxheap
	//original node is the last node with children
	for (int node = heap_parent(A.size() - 1); node >= 0; --node) {
		reheap(A, node, A.size());
	}
}

void sort_timer::heapS(vector<int>& A) {
	heapify(A);
	for (int i = A.size() - 1; i > 1;) {
		// i is the "size" of the heap, elements in A that are past i are already sorted
		swap(A, 0, i);
		--i;
		reheap(A, 0, i);
	}
	if (A[0] > A[1])
		swap(A, 0, 1); // no point in calling reheap on a 2 element heap
}

// Counting Sort

void sort_timer::countingS(vector<int>& A) {
	vector<int> counts (max(A) + 1, 0); // initial size of vector is one bigger than max(A), and it's filled with 0's
	for (iter i = A.begin(); i != A.end(); ++i) ++counts[*i];
	// iterate through A, counting how many of each element there are
	iter pos = A.begin();
	for (int i = 0; i < counts.size(); ++i) {
		while (counts[i] > 0) {
			// if this position in counts is non-zero, put i in pos, and then decrement counts[i], and repeat until it is zero
			*pos = i;
			++pos;
			--(counts[i]);
		}
	}
}

// Bucket Sort

void sort_timer::bucketS(vector<int>& A) {
	int factor; //factor is the average elements per bucket
	if (A.size() < 20) factor = 3;
	else if (A.size() < 500) factor = 5;
	else if (A.size() < 5000) factor = 10;
	else factor = 20;
	//choose different factors based on size of A
	int num_buckets = A.size() / factor;
	int bucket_range = (max(A) / num_buckets) + 1; // the hash function for the buckets is n / bucket_range
	vector<vector<int>> buckets (num_buckets + 1);
	for (iter i = A.begin(); i != A.end(); ++i)
		// add *i onto the priority queue at bucket[hash(*i)]
		buckets[*i / bucket_range].push_back(*i);
	iter pos = A.begin(); //this iter is used to reference all of
	for (vector<vector<int>>::iterator i = buckets.begin(); i != buckets.end(); ++i) {
		insertionS(*i);
		for (iter j = i->begin(); j != i->end(); ++j) {
			*pos = *j;
			++pos;
		}
	}
}

void sort_timer::timeSorts(const vector<int>& unsorted, const int lineno, const int filenum_pos, matrix3D& sort_data) {
	for (int i = 0; i < 8; i++) { // do all sorts
		vector<int> copyOfA (unsorted); // copy constructor
		steady_clock::time_point startTime = steady_clock::now(); // time before sort called
		switch (i) { // different sort for each i
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
		sort_data[i][filenum_pos][lineno] = duration_cast<nanoseconds>(steady_clock::now() - startTime).count(); // time after sort called
		assert(checkSorted(copyOfA)); // make sure the sort actually worked, but not on the timer
	}
}
