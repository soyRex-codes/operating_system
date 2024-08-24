//********************************************************************
//
// Rajkumar Kushwaha
// Operating Systems,360
// Programming Project #6: Project Simpager
// April 4, 2024
//********************************************************************

#include <iostream>
#include <vector>
#include <deque>
#include <unordered_map>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <string>
#include <climits> // Include for INT_MAX
#include <sstream> // Include for stringstream

using namespace std;

// Function to calculate the number of page faults using FIFO(First in and first out) algorithm
int FIFO(const vector<int>& page_reference, int frames) {
    deque<int> page_frames;// Queue to store the page frames
    int page_faults = 0; // Counter for the number of page faults

    // Iterate through the page reference vector
    for (int page : page_reference) {
        // Check if the page is already in the page frames
        if (find(page_frames.begin(), page_frames.end(), page) == page_frames.end()) {
            page_faults++; // Increment the page fault counter

            // If the page frames are full, remove the oldest page frame
            if (page_frames.size() == frames) {
                page_frames.pop_front();
            }
            page_frames.push_back(page); // Add the new page to the page frames
        }
    }
    return page_faults;
}

// Function to calculate the number of page faults using LRU algorithm
int LRU(const vector<int>& page_reference, int frames) {
    vector<int> page_frames; // Vector to store the page frames
    unordered_map<int, int> last_used; // Map to store the last accessed time of each page
    int page_faults = 0; // Counter for the number of page faults
    int time_stamp = 0; // Variable to store the current time stamp
    // Iterate through the page reference vector
    for (int page : page_reference) {
        // Check if the page is already in the page frames
        if (find(page_frames.begin(), page_frames.end(), page) == page_frames.end()) {
            page_faults++;

             // If the page frames are full, remove the least recently used page
            if (page_frames.size() == frames) {
                int lru_page = min_element(last_used.begin(), last_used.end(), [](const auto& lhs, const auto& rhs) {
                    return lhs.second < rhs.second;
                })->first;
                page_frames.erase(remove(page_frames.begin(), page_frames.end(), lru_page), page_frames.end());
                last_used.erase(lru_page);
            }
            page_frames.push_back(page); // Add the new page to the page frames
        }
        last_used[page] = time_stamp++; //update the acessed time of page
    }
    return page_faults;
}

// Function to calculate the number of page faults using OPT algorithm
int OPT(const vector<int>& page_reference, int frames) {
    int page_faults = 0; // Counter for the number of page faults
    vector<int> page_frames; // Vector to store the page frames
    // Iterate through the page reference vector
    for (int i = 0; i < page_reference.size(); ++i) {
        if (find(page_frames.begin(), page_frames.end(), page_reference[i]) == page_frames.end()) {
            page_faults++; // Increment the page fault counter
            
            // If the page frames are full, replace the page that will be accessed farthest in the future
            if (page_frames.size() == frames) {
                vector<int> distances(page_frames.size(), INT_MAX);
                
                 // Iterate through the page frames
                for (int j = 0; j < page_frames.size(); ++j) {
                    auto it = find(page_reference.begin() + i + 1, page_reference.end(), page_frames[j]);
                    // If the page is found in the future, calculate the distance to its next access
                    if (it != page_reference.end()) {
                        distances[j] = distance(page_reference.begin() + i, it);
                    }
                } // Find the page with the maximum distance to its next access
                auto page_to_replace = max_element(distances.begin(), distances.end());
                page_frames.erase(page_frames.begin() + distance(distances.begin(), page_to_replace));
            }
            page_frames.push_back(page_reference[i]);
        }
    }
    return page_faults;
}
// Function to calculate the number of page faults using RAND algorithm
int RAND(const vector<int>& page_reference, int frames) {
    int page_faults = 0; // Counter for the number of page faults
    vector<int> page_frames; // Vector to store the page frames
    
    srand(time(NULL)); // Seed the random number generator
    
    // Iterate through the page reference vector
    for (int page : page_reference) {
        if (find(page_frames.begin(), page_frames.end(), page) == page_frames.end()) {
            page_faults++;

             // If the page frames are full, randomly choose a page to replace
            if (page_frames.size() == frames) {
                int index = rand() % frames;
                page_frames[index] = page;
            } else {
                // If the page frames are not full, add the new page to the page frames
                page_frames.push_back(page);
            }
        }
    }
    return page_faults;
}

int main() {
    string page_reference_str;
    getline(cin, page_reference_str);
    vector<int> page_reference;
    int page;
    stringstream ss(page_reference_str);
    while (ss >> page) {
        page_reference.push_back(page);
    }
// Initializing the number of page frames and the page reference vector
    int frames;
    string frames_line;
    getline(cin, frames_line);
    stringstream(frames_line) >> frames;


    cout << "Page Reference String:" << endl;
    cout << page_reference_str << endl;
    cout << "Number of Frames: " << frames << endl;

    string algorithm;
    while (getline(cin, algorithm)) {
        if (algorithm == "FIFO") {
            cout << "FIFO:" << FIFO(page_reference, frames) << endl;
        } else if (algorithm == "LRU") {
            cout << "LRU:" << LRU(page_reference, frames) << endl;
        } else if (algorithm == "OPT") {
            cout << "OPT:" << OPT(page_reference, frames) << endl;
        } else if (algorithm == "RAND") {
            cout << "RAND:" << RAND(page_reference, frames) << endl;
        }
    }

    return 0;
}
