/*
 * main.cpp
 *
 *  Created on: Mar 10, 2014
 *      Author: olynch
 */
#include "SortingOutSorting.h"
using namespace std;

// globals
vector<string> FILEPATTERN ({ "data/unsorted_arrays/arrays", ".txt" }); // may be changed in main from arguments
const vector<int> FILENUMS ({ 10, 50, 100, 500, 1000, 5000, 10000, 50000, 75000, 100000 });
const string LINE_GRAPH_JSON_OUTPUTFILE ("server/linegraph.json");
const vector<string> COLUMN_GRAPHS_JSON_OUTPUTPATTERN ({"server/columngraph", ".json"});

void testAlgorithms(const int filenum_pos, matrix3D& sort_data) {
	// tests all the algorithms on arrays(length).txt, and records the time it takes into sort_data
	int length = FILENUMS[filenum_pos];
	ifstream unsorted_file;
	stringstream file_name_stream;
	string file_name;
	file_name_stream << FILEPATTERN[0] << length << FILEPATTERN[1];
	file_name = file_name_stream.str();
	unsorted_file.open(file_name.c_str());
	if(!unsorted_file) throw invalid_argument("Could not find file" + file_name);
	// unsorted file is now a read-only stream that is attached to arrays(length).txt

	// declarations for the following loop
	char cur; // the variable that holds the current character in the file
	vector<int> unsorted (length); // the array that has ints added to it to be sorted
	string curIntStr = ""; // a string that cur is added to repeatedly until it is turned into an int
	// and then added to unsorted
	int unsortedPos = 0; // the current position in the vector unsorted
	int lineno = 0; // for use in data entry into sort_data
	string::size_type sz;
	/*
	 * This loop reads through unsorted_file, character by character
	 * it reads this input on the fly into a vector, and then passes that vector to be sorted
	 * by all the different algorithms, placing the result times into sort_data.
	 * The advantage of doing this character by character is that the whole file is never
	 * completely loaded into memory, saving space
	 */
	while (unsorted_file) {
		cur = unsorted_file.get();
		if (cur == ',') { // we have finished reading a number
			unsorted[unsortedPos] = stoi(curIntStr, &sz, 10);
			++unsortedPos;
			curIntStr = "";
		}
		else if (cur == '\n') { // we have finished reading a line
			unsorted[unsortedPos] = stoi(curIntStr, &sz, 10);
			curIntStr = "";
			cout << "line: " << lineno << "\n";
			sort_timer::timeSorts(unsorted, lineno, filenum_pos, sort_data);
			// timeSorts records the data for us
			++lineno;
			unsortedPos = 0;
		}
		else { // just another character
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

void JSONify_ColumnGraphs(const matrix3D& sort_data) {
	/*
	 * This method takes in a matrix3D of the times for each sort,
	 * computes the average time for each length of array and sort combination
	 * and outputs it into CanvasJS-readable format
	 */
	using namespace json;
	for (int i = 0; i < FILENUMS.size(); ++i) {
		/*
		 * The final json should look like:
		 * {
		 *    "data": [
		 *         {
		 *             "dataPoints": [
		 * 			       { "label": "sort_type", "y": average_time },
		 *                 etc.
		 *              ]
		 *         }
		 *     ],
		 *     "length": array_length,
		 * }
		 * CanvasJS attributes, like font size, title, etc. are set by the javascript frontend
		 * so that this program doesn't have to be rerun if anything needs to change
		 */
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
		// collapse everything into root
		data_set1["dataPoints"] = dataPoints;
		data.push_back(data_set1);
		root["data"] = data;
		root["length"] = FILENUMS[i];

		// write the json from root out to the output file
		stringstream filename;
		filename << COLUMN_GRAPHS_JSON_OUTPUTPATTERN[0] << FILENUMS[i] << COLUMN_GRAPHS_JSON_OUTPUTPATTERN[1];
		ofstream outputFile (filename.str());
		outputFile << Serialize(root);
		outputFile.close();
	}
}

void JSONify_BigLineGraph(const matrix3D& sort_data) {
	/*
	 * This method takes in a matrix3D of the times for each sort,
	 * computes the average time for each length of array and sort combination
	 * and outputs it into CanvasJS-readable format
	 */
	using namespace json;
	/*
	 * The final json should look like:
	 * {
	 * 	  "data": [
	 * 	     {
	 * 	         "dataPoints": [
	 * 	             { "length": length, "y": average_time, "x": filenum_pos },
	 * 	             etc.
	 * 	         ]
	 * 	     }
	 * 	  ],
	 * }
	 * CanvasJS attributes, like font size, title, etc. are set by the javascript frontend
	 * so that this program doesn't have to be rerun if anything needs to change
	 */
	Object root;
	Array data;
	for (int i = 0; i < 8; ++i) {
		Object data_set;
		Array dataPoints;
		for (int j = 0; j < FILENUMS.size(); ++j) {
			Object datapoint;
			datapoint["length"] = FILENUMS[j];
			datapoint["label"] = FILENUMS[j];
			datapoint["y"] = getAverageTime(sort_data, i, j);
			dataPoints.push_back(datapoint);
		}
		data_set["dataPoints"] = dataPoints;
		data.push_back(data_set);
	}
	root["data"] = data;

	// write the json from root out to the output file
	ofstream outputFile (LINE_GRAPH_JSON_OUTPUTFILE);
	outputFile << Serialize(root);
	outputFile.close();
}

int main(int argc, char** argv) {
	if (argc > 1) { // may contain file pattern
		if (argc != 3) { // explanation below
			cout << "ERROR: SortingOutSorting requires two arguments:\n"
					"The first one is the path to the data files (ie. data/arrays)\n"
					"The second one is the end of the data files (ie. .txt)\n"
					"These two arguments are cocatenated to form the relative path to each data file\n"
					"(ie. data/arrays100.txt) with a number inserted between\n"
					"The default arguments are \"data/unsorted_arrays/array\" and \".txt\"\n"
					"The data from running the sorts is outputted in a similar manner to server/arrays(n).json\n";
			return 1;
		}
		else { //change the default values for FILEPATTERN, also see above
			FILEPATTERN[0] = argv[1];
			FILEPATTERN[1] = argv[2];
		}
	}
	matrix3D sort_data (boost::extents[8][FILENUMS.size()][100]);
	/*
	 * sort_data's first dimension is sorts,
	 * second dimension is length of arrays
	 * third dimension is individual timed trials
	 * sort_data is passed by reference all the way down to timeSorts, where it is modified
	 */
	for (int i = 0; i < FILENUMS.size(); ++i) {
		cout << "working on arrays" << FILENUMS[i] << ".txt" << "\n";
		testAlgorithms(i, sort_data);
	}
	JSONify_ColumnGraphs(sort_data);
	JSONify_BigLineGraph(sort_data);
}
