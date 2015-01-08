//
//  BucketList.h
//  SortingCPP
//
//  Created by Raz Friman on 3/1/13.
//  Copyright (c) 2013 Raz Friman. All rights reserved.
//

#ifndef __SortingCPP__SORTINGCOMPETITION__
#define __SortingCPP__SORTINGCOMPETITION__

#include <iostream>
#include <cstring>
#include <string>
#include <fstream>
#include <cmath>
#include <vector>

using namespace std;

class SortingCompetition
{
public:
    // basic constructor that accepts an input
    // file name
    SortingCompetition(const string& inputFileName);
    
    // change the input file name
    void setFileName(const string& inputFileName);
    
    // read the data from the file and store it in
    // a linear data structure
    // no sorting actions can be done in this method
    bool readData();
    
    // copy the data from the original data structure
    // into a second location that will be used for sorting
    // no sorting actions can be done in this method
    bool prepareData();
    
    // sort the data based on the criteria set forth in the hand out
    // THIS IS THE FUNCTION THAT WILL BE TIME
    void sortData();
    
    // using outputFileName, write the "sorted" data structure to the file.
    // This will be used to test the validity of your sorting algorithm
    void outputData(const string& outputFileName);
    
    // Destructor to delete all dynamic memory
    ~SortingCompetition();
    
private:
    // Name of the file to read
    string inputFileName_;
    
    // Array for words and their lengths
    vector<char*> wordArray_;
    vector<int> lengthArray_;
    
    // The final array of the sorted words
    char** finalWordArray_;
    bool isSorted_;
    
    // Position and Capacity of the word array
    int position_;
    
    // Constant values used to sort
    static const int LENGTH_INCREASE = 100;
    static const int INSERTION_LIMIT = 16;
    
    // Array of buckets
    // Each bucket contains an array of words
    char*** buckets_;
    
    // The length of the longest word
    int maxWordLength_;
    
    // Information for bucket position/counts
    int* bucketLengths_;
    int* bucketPositions_;
        int bucketCount_;
    
    // Current array of words to sort
    char** wordsToSort_;
    char** tempWords_;
    
    unsigned char* tempDigits_;
    int tempDigitsLength_;
    
    // Adds a word to the array
    void addWord(char* word);
    
    // Sorts a single bucket
    void sortBucket(int length);
    
    // Loads the word into their corresponding buckets
    void loadBuckets();
    
    // Resizes the word and length arrays
    void resizeArrays();
    
    // Resizes the bucket count arrays
    void resizeBucketCounts();
    
    // Swaps two indices
    void swap(int index1, int index2);
    
    // Radix Sort
    // Referenced: http://z1.invisionfree.com/CPPlearningcommunity/ar/t2392.htm
    void RadixSort (char** elems, int numElems, int bucket);
    void Radix (int digit, int numElems, int bucket, char** src, char** dest);
    
    // Intro-Sort created by David Musser
    // Implemented by Raz Friman (me)
    void introSort(int left, int right, int iterationsLeft);
    
    // Quick Sort: http://rosettacode.org/wiki/Quick_Sort
    // Referenced from: http://www.cplusplus.com/forum/beginner/9388/
    // Optimized by Raz Friman (me)
    int medianOfThreeSplit(int left, int right);
    int partition(int left, int right, int pivot);
    
    // Insertion Sort: http://rosettacode.org/wiki/Insertion_sort
    void insertionSort(int left, int right);
    
    // Referenced from wikibooks
    // http://en.wikibooks.org/wiki/Algorithm_implementation/Sorting/Heapsort
    void heapSort(int left, int right);
    
    // Checks that the sort was correct
    bool checkSort(int left, int right);

};

#endif