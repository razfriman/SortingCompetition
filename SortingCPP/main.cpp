/*
 * SortingCompetition main driver
 * CSE 2341
 * Spring 2013
 */

#include <iostream>
#include <chrono>
#include "SortingCompetition.h"


int main(int argc, char** argv)
{
    
	const int NUM_REPS = 15; // number of times to run sort
	unsigned int totalMilliseconds = 0;
    
	//SortingCompetition sorter("/Users/razfriman/Documents/XCodeProjects/SortingCPP/Build/Products/Debug/inputSimple.txt");
	SortingCompetition sorter("/Users/razfriman/Documents/XCodeProjects/SortingCPP/Build/Products/Debug/input1.txt");
    //SortingCompetition sorter("/Users/razfriman/Documents/XCodeProjects/SortingCPP/Build/Products/Debug/shakespeare.txt");
    //SortingCompetition sorter("/Users/razfriman/Documents/XCodeProjects/SortingCPP/Build/Products/Debug/tomsawyer.txt");
    //SortingCompetition sorter("shakespeare.txt");
	
    if (!sorter.readData())
	{
		std::cerr << "Error reading data" << std::endl;
		return 1;
	}
	    
	for (int i = 0; i < NUM_REPS; i++)
	{
		if (!sorter.prepareData())
		{
			std::cerr << "Error preparing data" << std::endl;
			return 1;
		}
        
		std::chrono::time_point<std::chrono::system_clock> start, end;
		start = std::chrono::system_clock::now();
        
		sorter.sortData();
		
        end = std::chrono::system_clock::now();
		unsigned int milliseconds = (unsigned int) std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
		
        totalMilliseconds += milliseconds;
		std::cout << "Sort #" << i+1 << " Time: " << milliseconds << " milliseconds" << std::endl;
	}
	
    sorter.outputData("output.txt");
	std::cout << "Average sorting time: " << totalMilliseconds / NUM_REPS << " milliseconds" << std::endl;
    }
