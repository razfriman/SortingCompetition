//
//  BucketList.cpp
//  SortingCPP
//
//  Created by Raz Friman on 3/1/13.
//  Copyright (c) 2013 Raz Friman. All rights reserved.
//

#include "BucketList.h"

BucketList::BucketList(const string& inputFileName)
{
    // Initialize all varaibles to their default values
    wordArray_ = new char*[INCREASE];
    lengthArray_ = new int[INCREASE];
    position_ = 0;
    capacity_ = INCREASE;
    
    wordsToSort_ = 0;
    maxWordLength_ = 0;
    wordsToSort_ = 0;
    buckets_ = 0;
    
    inputFileName_ = inputFileName;
    
    isSorted_ = false;
    
    // Initialize the buckets
    bucketPositions_ = new int[LENGTH_INCREASE];
    bucketLengths_ = new int[LENGTH_INCREASE];
    bucketCount_ = 0;
    
    for (int i = 0; i < LENGTH_INCREASE; i++)
    {
        // Fill the buckets with 0's
        bucketPositions_[i] = 0;
        bucketLengths_[i] = 0;
    }
}

BucketList::~BucketList()
{
    // Delete all allocated memory
    
    for (int i = 0; i < position_; i++)
    {
        // Delete all the words
        delete [] wordArray_[i];
    }
    
    for (int i = 0; i <= maxWordLength_; i++)
    {
        // Empty the buckets
        delete [] buckets_[i];
    }
    
    // Delete all the arrays
    delete [] buckets_;
    delete [] wordArray_;
    delete [] lengthArray_;
    delete [] finalWordArray_;
    delete [] bucketPositions_;
    delete [] bucketLengths_;
}

void BucketList::setFileName(const string &inputFileName)
{
    // Set the file name to read from
    inputFileName_ = inputFileName;
}

bool BucketList::readData()
{
    // Read the file and parse the words
    ifstream inputFile(inputFileName_.c_str());
    
    if (!inputFile)
    {
        // Error reading file
        cout << "Error: Cannot open file for reading: " << inputFileName_ << endl;
        return false;
    }
    
    // Allocate memory for the current word
    char* currentWord = new char[80];
    
    // Read all words till the end of the file
    while(inputFile >> currentWord)
    {
        // Add the word to the arrays
        addWord(currentWord);
    }
    
    // Close the file
    inputFile.close();
    
    // De-allocate the memory for the current word
    delete [] currentWord;
    
    return  true;
}

bool BucketList::prepareData()
{
    // If we already sorted, then delete the old data
    if (isSorted_)
    {
        // Delete the sorted data
        delete [] finalWordArray_;
        
        wordsToSort_ = 0;
        
        for(int i = 0; i <= maxWordLength_; i++)
        {
            // Reset the bucket positions
            bucketPositions_[i] = 0;
            delete [] buckets_[i];
        }
        
        delete [] buckets_;
    }
    
    finalWordArray_ = new char*[position_];
    
    // Determine the maximum length of the words
    for(int i = 0; i < position_; i++)
    {
        if (lengthArray_[i] > maxWordLength_)
        {
            maxWordLength_ = lengthArray_[i];
        }
    }
    
    // Initialize the buckets
    buckets_ = new char**[maxWordLength_ + 1];
    
    for(int i = 0; i <= maxWordLength_; i++)
    {
        buckets_[i] = new char*[bucketLengths_[i]];
    }
    
    return true;
}

void BucketList::sortData()
{
    // Loads all the data into its corresonding "bucket"
    // These buckets are determined by the primary key - word length
    loadBuckets();
    
    int currentPosition = 0;
    
    for(int i = maxWordLength_; i > 0; i--)
    {
        // Sort each bucket, then set its current position
        sortBucket(i);
        
        for(int j = 0; j < bucketPositions_[i]; j++)
        {
            finalWordArray_[currentPosition] = buckets_[i][j];
            currentPosition++;
        }
        
    }
    
    // Finished sorting
    isSorted_ = true;
}

void BucketList::outputData(const string &outputFileName)
{
    // Outputs the sorted data to the file
    
    // Open the file
    ofstream outFile(outputFileName.c_str());
    
    if (!isSorted_)
    {
        // Error, sort method not called
        cout << "Error: Wordw were not sorted. Please call the sortData method" << endl;
        return;
    }
    
    if (!outFile)
    {
        // Error writing to file
        cout << "Error: Cannot write to file: " << outputFileName << endl;
        return;
    }
    
    if (!checkSort(0, position_ - 1))
    {
        // Check to make sure the sort was correct
        cout << "Error: Words were not sorted correctly" << endl;
        return;
    }
    
    for(int i = 0; i < position_; i++)
    {
        // Output each word to the file
        
        outFile << finalWordArray_[i] << " ";
        //cout << wordArray_[i] << endl;
    }
    
    // Close the fiel
    outFile.close();
}

void BucketList::addWord(char *word)
{
    int length = (int) strlen(word);
    
    if (position_ == capacity_)
    {
        // If we reached the maximum capacity, then resize
        resizeArrays();
    }
    
    if (length > bucketCount_)
    {
        // If the word does not have a bucket, create it
        resizeBucketCounts();
    }
    
    // Create memory for the word
    wordArray_[position_] = new char[length + 1];
    
    // Copy the word
    strcpy(wordArray_[position_], word);
    
    // Update the length array
    lengthArray_[position_] = length;
    
    // Update the items in the bucket
    bucketLengths_[length]++;
    
    // Update the word count
    position_++;
    
}

void BucketList::loadBuckets()
{
    for(int i = 0; i < position_; i++)
    {
        int length = lengthArray_[i];
        
        // Assign each word to its corresponding bucket
        buckets_[length][bucketPositions_[length]] = wordArray_[i];
        
        // Update the bucket position
        bucketPositions_[length]++;
    }
}

void BucketList::resizeArrays()
{
    // Create new temp arrays with bigger capacities (10 bigger)
    int* tempLengthArray = new int[capacity_ + INCREASE];
    char** tempWordArray = new char*[capacity_ + INCREASE];
    
    // Copy the old values to the tempArrays
    for(int i = 0; i < capacity_; i++)
    {
        tempWordArray[i] = wordArray_[i];
        tempLengthArray[i] = lengthArray_[i];
    }
    
    // Increase the capacity and delete the old array pointers
    capacity_ += INCREASE;
    delete [] lengthArray_;
    delete [] wordArray_;
    
    // Set the array pointers equal to the temp arrays
    wordArray_ = tempWordArray;
    lengthArray_ = tempLengthArray;
}

void BucketList::resizeBucketCounts()
{
    int newLength = bucketCount_ + LENGTH_INCREASE;
    
    // Create bigger arrays for Length and Position
    int* tempBucketLengths = new int[newLength];
    int* tempBucketPositions = new int[newLength];
    
    for (int i = 0; i < newLength; i++)
    {
        // Set all values to 0
        tempBucketLengths[i] = 0;
        tempBucketPositions[i] = 0;
    }
    
    for (int i = 0; i < bucketCount_; i++)
    {
        // Transfer the old bucket lengths
        tempBucketLengths[i] = bucketLengths_[i];
    }
    
    // Set the new length
    bucketCount_ = newLength;
    
    // Delete the old arrays
    delete [] bucketLengths_;
    delete [] bucketPositions_;
    
    // Set the array pointers to the temp arrays
    bucketLengths_ = tempBucketLengths;
    bucketPositions_ = tempBucketPositions;
}

void BucketList::swap(int index1, int index2)
{

    if (index1 == index2)
    {
        return;
        //sameI++;
    }
    
    /*
    if (strcmp(wordsToSort_[index1], wordsToSort_[index2]) == 0)
    {
        sameV++;
    }
    */
    
    // Swaps two indices in the array
    char* tempWord = wordsToSort_[index1];
    wordsToSort_[index1] = wordsToSort_[index2];
    wordsToSort_[index2] = tempWord;
}

void BucketList::sortBucket(int length, bool intro)
{
    // Sorts a single bucket
    wordsToSort_ = buckets_[length];
    
    int count = bucketPositions_[length];
    
    if (count == 0)
    {
        // Skip empty buckets
        return;
    }
    
    if (intro)
    {
    // Optimize the sort by setting the maximum iterations
    // This ensures O(n*log(n)) performance
        int bucketIterations =  floor(log(count) / log(2));
    // Sort each bucket using IntroSort
        introSort(0, count - 1, bucketIterations);
    }
    else
    {
        RadixSort(wordsToSort_, count, length);
    }
}



// LSD RADIX SORT
void BucketList::Radix (int digit, int numElems, int bucket, char** src, char** dest)
{
    int count[256] = { 0 };
    int indexTable[256];
     
    for (int i = 0; i < numElems; i++)
    {
        if(digit < bucket)
        {
            count[src[i][digit]]++;
        }
    }
    
    // Create the Radix Lookup Table
    indexTable[0]=0;
    for (int i = 1; i < 256; i++)
    {
        indexTable[i] = indexTable[i - 1] + count[i - 1];
    }
    
    for (int i = 0; i < numElems; i++)
    {
        dest[indexTable[src[i][digit]]++] = src[i];
    }
}

void BucketList::RadixSort (char** elems, int numElems, int bucket)
{    
    if (numElems < 5)
    {
        insertionSort(0, numElems - 1);
        return;
    }
    
    char** temp = new char*[numElems];
    
    // if the number of maximum digits is odd, sort the first digit to make it even
    if(bucket & 1)
    {
        Radix (bucket - 1, numElems, bucket, elems, temp);
        memcpy(elems, temp, sizeof(char*) * numElems);
        --bucket;
    }
    
    // sort the remaining digits
    for(int spot = bucket - 1; spot > 0; spot -= 2)
    {
        Radix (spot, numElems, bucket,  elems, temp);
        Radix (spot - 1, numElems, bucket, temp, elems);
    }
    
    delete [] temp;
}

void BucketList::introSort(int left, int right, int iterationLeft)
{
    if (right - left < INSERTION_LIMIT)
    {
        // If the count is less than the INSERTION_LIMIT
        // Then switch to insertion sort
        insertionSort(left, right);
        return;
    }
    
    if (iterationLeft == 0)
    {
        // After we reach the maximum number of iterations
        // Then switch to heap sort
        heapSort(left, right);
        return;
    }
    
    // Otherwise we use quick sort
    int pivotIndex = medianOfThreeSplit(left, right);
        
    char* pivotValue = wordsToSort_[pivotIndex];
    
    // Swap the pivot to the end so we can keep track of it
    swap(pivotIndex, right);
    
    int storeIndex = left;
    
    for (int i = left; i < right; i++)
    {
        if (strcmp(wordsToSort_[i], pivotValue) <= 0)
        {
            
            if (i == storeIndex)
            {
                // If the indices are the same, do not swap
                // Increase the storeIndex
                storeIndex++;
                continue;
            }
            swap(i, storeIndex);
            storeIndex++;
        }
    }
    
    swap(storeIndex, right);
    
    int pivotNewIndex = storeIndex;

    //int pivotNewIndex = partition(left, right, pivotIndex);
    
    introSort(left, pivotNewIndex - 1, iterationLeft - 1);
    introSort(pivotNewIndex + 1, right, iterationLeft - 1);
}

// Finds a Pivot for quicksort using Median of Threes
// Referenced from: http://www.cplusplus.com/forum/beginner/9388/
int BucketList::medianOfThreeSplit(int left, int right)
{
    int center = (left + right) / 2;
    
    
    // Move the Median of Threes to the center of the array
    if (strcmp(wordsToSort_[center], wordsToSort_[left]) < 0)
    {
        swap(left, center);
    }
    if (strcmp(wordsToSort_[right], wordsToSort_[left]) < 0)
    {
        swap(left, right);
    }
    if (strcmp(wordsToSort_[right], wordsToSort_[center]) < 0)
    {
        swap(center, right);
    }
     
    //swap(center, right - 1);
    
    // Return the index of the pivot
    return center;
}
// Partitions an array into two parts and sorts it giving a new pivot index
int BucketList::partition(int left, int right, int pivot)
{
    char* pivotValue = wordsToSort_[pivot];
    
    // Swap the pivot to the end so we can keep track of it
    swap(pivot, right);
    
    int storeIndex = left;
    
    for (int i = left; i < right; i++)
    {
        if (strcmp(wordsToSort_[i], pivotValue) <= 0)
        {
           
            if (i == storeIndex)
            {
                // If the indices are the same, do not swap
                // Increase the storeIndex
                storeIndex++;
                continue;
            }
            
            swap(i, storeIndex);
            storeIndex++;
        }
    }
    
    swap(storeIndex, right);
    cout << storeIndex - left << " ";
    return storeIndex;
}

void BucketList::heapSort(int left, int right)
{
    
    // Store the temporary values
    char* temp = 0;
    
    // Count of elements to sort
    unsigned int n = (right - left) + 1;
    
    // Heap Indices
    unsigned int parent = n / 2;
    unsigned int index;
    unsigned int child;
    
    // Loop until array is sorted
    while (1) {
        if (parent > 0)
        {
            // FIRST - Sort the heap
            parent--;
            temp = wordsToSort_[left + parent];
        }
        else
        {
            // SECOND - Extract elements from heap
            n--;
            
            if (n == 0)
            {
                // When the heap is empty, we are done
                return;
            }
            
            // Save the lost heap entry to temporary
            temp = wordsToSort_[left + n];
            
            wordsToSort_[left + n] = wordsToSort_[left];
        }
        
        // INSERT - pushing the temp value down the heap, replacing the parent
        index = parent;
        // Get the LEFT child index
        child = index * 2 + 1;
        
        while (child < n) {
            
            // Choose the largest child
            if (child + 1 < n  &&  strcmp(wordsToSort_[left + child + 1], wordsToSort_[left + child]) > 0)
            {
                // Right child exists and is bigger
                child++;
            }
            
            // Is the largest child larger than the entry?
            if (strcmp(wordsToSort_[left + child], temp) > 0)
            {
                // Overwrite entry with child
                wordsToSort_[left + index] = wordsToSort_[left + child];
                
                // Move index to the child and set LEFT child
                index = child;
                child = index * 2 + 1;
            }
            else
            {
                // Done with the sorting loop
                break;
            }
        }
        
        // Store the temporary value at its new location
        wordsToSort_[left + index] = temp;
    }
}

void BucketList::insertionSort(int left, int right)
{
    for (int i = left + 1; i <= right; i++)
    {
        // Save the temporary word
        char* tempWord = wordsToSort_[i];
        
        int currentIndex = i;
        
        // Move all smaller elements and insert the current element
        while (currentIndex > left &&  strcmp(wordsToSort_[currentIndex - 1], tempWord) > 0)
        {
            swap(currentIndex, currentIndex - 1);
            currentIndex--;
        }
    }
}


bool BucketList::checkSort(int left, int right)
{
    // Checks that the words are sorted correctly
    
    for (int i = left + 1; i <= right; i++)
    {
        int length1 = (int) strlen(finalWordArray_[i - 1]);
        int length2 = (int) strlen(finalWordArray_[i]);
        
        if (length2 > length1)
        {
            // Error in Primary Key - Length
            cout << (i - 1) << " Error with LENGTH: " << finalWordArray_[i - 1] << " Length: " << length1 << endl;
            cout << i << " Error with LENGTH: " << finalWordArray_[i] << " Length: " << length2 << endl;
            return false;
        }
        else if (length1 == length2)
        {
            if (strcmp(finalWordArray_[i], finalWordArray_[i - 1]) < 0)
            {
                // Error in Secondary Key - Alphabetical
                cout << (i - 1) << " Error with WORD: " << finalWordArray_[i - 1] << " Length: " << length1 << endl;
                cout << i << " Error with WORD: " << finalWordArray_[i] << " Length: " << length2 << endl;
                return false;
            }
        }
    }
    
    // Sorted correctly
    return true;
}

