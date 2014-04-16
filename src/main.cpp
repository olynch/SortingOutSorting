/*
 * main.cpp
 *
 *  Created on: Mar 10, 2014
 *      Author: olynch
 */
#include "SortingOutSorting.h"
using namespace std;

vector<string> filepattern ({ "data/unsorted_arrays/arrays", ".txt" });
vector<int> filenums ({ 10, 50, 100, 500, 1000, 5000, 10000, 50000, 75000, 100000 });
vector<string> JSONOutputPattern ({ "server/arrays", ".json"});

void testAlgorithms(const int filenum_pos, matrix3D& sort_data) {
	int length = filenums[filenum_pos];
	ifstream unsorted_file;
	stringstream file_name_stream;
	string file_name;
	file_name_stream << filepattern[0] << length << filepattern[1];
	file_name = file_name_stream.str();
	unsorted_file.open(file_name.c_str());
	if(!unsorted_file) throw invalid_argument("Could not find file" + file_name);
	char cur;
	vector<int> unsorted(length);
	string curIntStr = "";
	int unsortedPos = 0;
	int lineno = 0;
	string::size_type sz;
	while (unsorted_file) {
		cur = unsorted_file.get();
		if (cur == ',') {
			unsorted[unsortedPos] = stoi(curIntStr, &sz, 10);
			++unsortedPos;
			curIntStr = "";
		}
		else if (cur == '\n') {
			unsorted[unsortedPos] = stoi(curIntStr, &sz, 10);
			curIntStr = "";
			cout << "line: " << lineno << "\n";
			sort_timer::timeSorts(unsorted, lineno, filenum_pos, sort_data);
			++lineno;
			unsortedPos = 0;
		}
		else {
			curIntStr += cur;
		}
	}
}

double getAverageTime(const matrix3D& sort_data, const int sort, const int array_length) {
	unsigned long sum = 0;
	for (int i = 0; i < 100; ++i) {
		sum += sort_data[sort][array_length][i];
	}
	return sum/100.0;
}

void JSONify(const matrix3D& sort_data) {
	using namespace json;
	for (int i = 0; i < filenums.size(); ++i) {
		Object root;
		Array data;
		Object data_set1;
		Array dataPoints;
		for (int j = 0; j < 8; ++j) {
			Object datapoint;
			switch (j) {
			case 0:
				datapoint["label"] = "bubble sort";
				break;
			case 1:
				datapoint["label"] = "merge sort";
				break;
			case 2:
				datapoint["label"] = "quick sort";
				break;
			case 3:
				datapoint["label"] = "insertion sort";
				break;
			case 4:
				datapoint["label"] = "selection sort";
				break;
			case 5:
				datapoint["label"] = "heap sort";
				break;
			case 6:
				datapoint["label"] = "counting sort";
				break;
			case 7:
				datapoint["label"] = "bucket sort";
				break;
			}
			datapoint["y"] = getAverageTime(sort_data, j, i);
			dataPoints.push_back(datapoint);
		}
		data_set1["dataPoints"] = dataPoints;
		data.push_back(data_set1);
		root["data"] = data;
		root["length"] = filenums[i];
		stringstream filename;
		filename << JSONOutputPattern[0] << filenums[i] << JSONOutputPattern[1];
		ofstream outputFile (filename.str());
		outputFile << Serialize(root);
		outputFile.close();
	}
}

int main(int argc, char** argv) {
//	cout << "main started\n";
	matrix3D sort_data (boost::extents[8][filenums.size()][100]);
	/*
	 * sort_data's first dimension is sorts,
	 * second dimension is length of arrays
	 * third dimension is individual timed trials
	 */
	for (int i = 0; i < filenums.size(); ++i) {
		cout << "working on arrays" << filenums[i] << ".txt" << "\n";
		testAlgorithms(i, sort_data);
	}
	JSONify(sort_data);
}
