// COMPILATION: gcc page_replace_random_pages.cpp -o page_replace_random_pages -lstdc++
//         RUN: ./page_replace_random_pages
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#define INT_MAX 1000
using namespace std;

// const int mrow = 9, mcol = 25; // use only for test case
const int mrow = 9, mcol = 101;
int matrix[mrow][mcol];
int fill_mat = 0;
deque<int> temp_q;
double time_arr[mcol-1];

int fifopageFaults(int pages[], int page_num, int frame_size)
{
	for (size_t i = 0; i < mrow; i++)
	{
		for (size_t j = 0; j < mcol; j++)
		{
			matrix[i][j] = fill_mat;
		}
	}
	// set data structure to find the existence of a page
	unordered_set<int> page_set;
	// keeping track of the pages
	deque<int> indices;
	int page_faults = 0;

	for (int i=0; i<page_num; i++)
	{
		chrono::steady_clock::time_point start1 = chrono::steady_clock::now();
		// check for the set empty place
		if (page_set.size() < frame_size)
		{
			// if not in the set then add it to it
			if (page_set.find(pages[i]) == page_set.end())
			{
				page_set.insert(pages[i]);
				matrix[mrow-1][i+1] = -1;
				// page fault tracker
				page_faults++;
				// adding the page to the trackers
				indices.push_back(pages[i]);
				temp_q.push_front(pages[i]);
			}
		} //end if

		else
		{
			// if not in the set then add it to it
			if (page_set.find(pages[i]) == page_set.end())
			{
				// getting the front
				int val = indices.front();
				// poping from the deque to empty space for the new page
				indices.pop_front();
				// erasing the poped page from the set too
				page_set.erase(val);
				// adding the new page to the set
				page_set.insert(pages[i]);
				// adding the page to the trackers
				indices.push_back(pages[i]);
				temp_q.push_front(pages[i]);
				// adding the page fault to the matrix to show the occurance
				matrix[mrow-1][i+1] = -1;
				// page fault tracker
				page_faults++;
			}
		} //end else

		// updating the matrix with the deque trackers
		for (int row=mrow-2;row>=0;row--){
			matrix[row][i+1] = temp_q[row];
		}
		chrono::steady_clock::time_point end1 = chrono::steady_clock::now();
		time_arr[i] = chrono::duration<double> (end1 - start1).count();

	} //end for
	return page_faults;
}

int lrupageFaults(int pages[], int page_num, int frame_size)
{
	for (size_t i = 0; i < mrow; i++)
	{
		for (size_t j = 0; j < mcol; j++)
		{
			matrix[i][j] = fill_mat;
		}
	}
	unordered_set<int> page_set;
	// keeping track of the lru pages
	unordered_map<int, int> indices;

	int page_faults = 0;
	for (int i=0; i<page_num; i++)
	{
		chrono::steady_clock::time_point start1 = chrono::steady_clock::now();
		// check for the set empty place
		if (page_set.size() < frame_size)
		{
			// if not in the set then add it to it
			if (page_set.find(pages[i]) == page_set.end())
			{
				page_set.insert(pages[i]);
				temp_q.push_front(pages[i]);
				matrix[mrow-1][i+1] = -1;
				// page fault tracker
				page_faults++;
			}
			// add new page to the deque
			indices[pages[i]] = i;
		} // end if
		else
		{
			// if not in the set then add it to it
			if (page_set.find(pages[i]) == page_set.end())
			{
				// finding lru pages in the set
				int lru = INT_MAX, val;
				unordered_set<int>::iterator iter;
				for (iter = page_set.begin(); iter != page_set.end(); iter++)
				{
					if (indices[*iter] < lru)
					{
						lru = indices[*iter];
						val = *iter;
					}
				}
				// erasing indices page
				page_set.erase(val);
				// adding the new page
				page_set.insert(pages[i]);
				temp_q.push_front(pages[i]);
				matrix[mrow-1][i+1] = -1;
				// Increment page faults
				page_faults++;
			}
			// updating the new page to the indices
			indices[pages[i]] = i;
		} // end else
		// updating the matrix with the deque trackers
		for (int row=mrow-2;row>=0;row--){
			matrix[row][i+1] = temp_q[row];
		}
		chrono::steady_clock::time_point end1 = chrono::steady_clock::now();
		time_arr[i] = chrono::duration<double> (end1 - start1).count();
	} //end for
	return page_faults;
}

int secondChancePageFaults(int pages[], int page_num, int frame_size)
{
    for (size_t i = 0; i < mrow; i++) {
        for (size_t j = 0; j < mcol; j++) {
            matrix[i][j] = fill_mat;
        }
    }
    int p = 0, page_faults = 0;
    int inputFrames[frame_size][2];
    for (int j = 0; j < frame_size; j++) {
		// initializing as empty
        inputFrames[j][0] = -1;
		// initializing as zero
        inputFrames[j][1] = 0;
    }
    for (int i = 0; i < page_num; i++)
	{
		chrono::steady_clock::time_point start1 = chrono::steady_clock::now();
        bool check = false;
        for (int j = 0; j < frame_size; j++) {
            if (inputFrames[j][0] == pages[i]) {
                check = true;
                inputFrames[j][1] = 1;
                break;
            }
        }
        if (check == false) {

            while (inputFrames[p][1] == 1) {
                inputFrames[p][1] = 0;
                p++;
                if (p == frame_size)
                    p = 0;
            }
            inputFrames[p][0] = pages[i];
            inputFrames[p][1] = 1;
            temp_q.push_front(pages[i]);
            // adding the page fault to the matrix to show the occurance
            matrix[mrow-1][i+1] = -1;
            page_faults++;
            p++;
            if (p == frame_size)
                p = 0;
        }
        for (int row=mrow-2;row>=0;row--){
			matrix[row][i+1] = temp_q[row];
		}
		chrono::steady_clock::time_point end1 = chrono::steady_clock::now();
		time_arr[i] = chrono::duration<double> (end1 - start1).count();
    } // end for
    return page_faults;
}

int main()
{
	string algo[3] = {"fifo", "lru", "second_chance"};
	int number, options, input_options, frame_size, page_num = mcol-1, rand_range = 9;
	cout << "\nChoose an option [1] FIFO [2] LRU [3] SECOND-CHANCE (eg. 1): ";
	cin >> options;
    cout << "\nEnter frame size [4 or 8]: ";
    cin >> frame_size;

	int pages[page_num];
	string file_name;
		file_name = "gorjianm_proj2_input.txt";
		ofstream output_file0(file_name);
		for(int i = 0;i < page_num;i++){
			output_file0 << (rand() % rand_range) + 1 << " ";
		}
		output_file0.close();
		ifstream input_file(file_name);
		for (int i = 0; i < page_num; i++) {
			input_file >> number;
			pages[i] = number;
		}
		output_file0.close();
		cout << "[INFO] 100 random pages were added to: " + file_name << endl;
	// }
	string file_name1 = "gorjianm_proj2_output_"+ to_string(frame_size) + "_frames_" + algo[options-1] + ".txt";
	ofstream output_file(file_name1);

	if (options == 1)
	{
		output_file << "--------------------------------------------------" << endl;
		output_file << "FIFO Page Replacement Algorithm" << endl;
		output_file << "--------------------------------------------------" << endl;
		output_file << "Page Faults: " << fifopageFaults(pages, page_num, frame_size) << endl;
	}
	if (options == 2)
	{
		output_file << "--------------------------------------------------" << endl;
		output_file << "LRU Page Replacement Algorithm" << endl;
		output_file << "--------------------------------------------------" << endl;
		output_file << "Page Faults: " << lrupageFaults(pages, page_num, frame_size) << endl;
	}
	if (options == 3)
	{
		output_file << "--------------------------------------------------" << endl;
		output_file << "SECOND-CHANCE Page Replacement Algorithm" << endl;
		output_file << "--------------------------------------------------" << endl;
		output_file << "Page Faults: " << secondChancePageFaults(pages, page_num, frame_size) << endl;
	}
	output_file << "--------------------------------------------------" << endl;
	output_file << "Time:" << endl;
	output_file << "--------------------------------------------------" << endl;
	for (size_t i = 0; i < mcol-1; i++)
	{
		output_file << time_arr[i] << " ";
	}
	output_file << "\n";
	output_file << "--------------------------------------------------" << endl;

	for (size_t i = 0; i < frame_size; i++)
	{
		for (size_t j = 0; j < mcol; j++)
			{
				if(matrix[i][j] == 0)
                	output_file <<"  ";
				else if (matrix[i][j] == -1)
					output_file << "F ";
            	else
					output_file << matrix[i][j] << " ";
			}
		output_file << "\n";
	}
	for (size_t j = 0; j < mcol; j++)
	{
		if(matrix[mrow-1][j] == 0)
            output_file <<"  ";
		if (matrix[mrow-1][j] == -1)
			output_file << "F ";
	}

	output_file << "\n";
	output_file << "--------------------------------------------------" << endl;
	output_file.close();
	cout << "[INFO] The results were added to: " << file_name1 << endl;
	return 0;
}