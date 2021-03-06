/*
numba - a NUMBer Analyzer
See LICENSE file for license information
*/

#include <iostream>
#include <string>
#include <map>
#include <getopt.h>
#include <stdlib.h>

//the current version of the program
const std::string version = "1.0.0";

/*
Prints information on how to use this program
*/
void printUsageInfo()
{
	using namespace std;

	cout << "Usage: numba <options> num1 num2 ... numN" << endl << endl;
	cout << "Options:" << endl;
	cout << "\t-a\tDisplay the mean/average of the given numbers" << endl << endl;
	cout << "\t-A\tPerform all operations except sorting on the given numbers" << endl << endl;
	cout << "\t-c\tDisplay the number of occurances of each unique number" << endl << endl;
	cout << "\t-g\tDisplay the amount of numbers that are greater than and the amount that are less than the mean/average" << endl << endl;
	cout << "\t-G\tDisplay the amount of numbers that are greater than and the amount that are less than the median" << endl << endl;
	cout << "\t-h\tPrint this information then exit" << endl << endl;
	cout << "\t-m\tDisplay the median (middle value) of the given numbers" << endl << endl;
	cout << "\t-M\tDisplay the mode (most common value) of the given numbers" << endl << endl;
	cout << "\t-o\tDisplay the number of odd and even numbers" << endl << endl;
	cout << "\t-r\tDisplay the range of the given numbers" << endl << endl;
	cout << "\t-R\tDisplay mean, median, mode, and range of the given numbers" << endl << endl;
	cout << "\t-s\tSort the given numbers from smallest to largest" << endl << endl;
	cout << "\t-S\tSort the given numbers from largest to smallest" << endl << endl;
}

/*
Prints name, description, and version number of this program, as well as usage information
*/
void printFullInfo()
{
	using namespace std;

	cout << endl << "numba, a NUMBer Analyzer - version " << version << endl << endl;
	cout << "numba sorts numbers, finds the median, mean, mode, range, and more." << endl << endl;
	printUsageInfo();
}

/*
Swaps two array elements using XOR
*/
void swap(long &x, long &y)
{
	x ^= y;
	y ^= x;
	x ^= y;
}

/*
Splits the given array around the pivot, with smaller numbers to the left
*/
int splitArraySmallToLarge(long* numbers, long pivot, int start, int end)
{
	int left = start;
	int right = end;

	while(left < right)
	{
		//look for lesser element or boundary of array
		while(pivot < numbers[right] && right > left)
		{
			right--;
		}

		swap(numbers[left], numbers[right]);

		//look for greater element or boundary of array
		while(pivot >= numbers[left] && left < right)
		{
			left++;
		}

		swap(numbers[right], numbers[left]);
	}

	return left;
}

/*
Splits the given array around the pivot, with larger numbers to the left
*/
int splitArrayLargeToSmall(long* numbers, long pivot, int start, int end)
{
	int left = start;
	int right = end;

	while(left < right)
	{
		//look for greater element or boundary of array
		while(pivot >= numbers[right] && right > left)
		{
			right--;
		}

		swap(numbers[left], numbers[right]);

		//look for lesser element or boundary of array
		while(pivot < numbers[left] && left < right)
		{
			left++;
		}

		swap(numbers[right], numbers[left]);
	}

	return left;
}

/*
Sorts the given array
*/
void quicksort(long* numbers, int start, int end, bool smallToLarge)
{
	//pivot is currently the leftmost element, possibly should be changed
	long pivot = numbers[start];
	int splitPoint;

	//if they are equal, there is only one element
	if(end > start)
	{
		if(smallToLarge)
		{
			//sort smallest to largest
			splitPoint = splitArraySmallToLarge(numbers, pivot, start, end);
		}
		else
		{
			//sort largest to smallest
			splitPoint = splitArrayLargeToSmall(numbers, pivot, start, end);
		}

		numbers[splitPoint] = pivot;
		quicksort(numbers, start, splitPoint - 1, smallToLarge); //sort first half
		quicksort(numbers, splitPoint + 1, end, smallToLarge);   //sort second half
	}
}

/*
Calculate the mean/average of the numbers
*/
double getMean(long* numbers, unsigned short numNumbers)
{
	long total = 0;

	for(int i = 0; i < numNumbers; ++i)
	{
		total += numbers[i];
	}

	return (double)total / numNumbers;
}

/*
Find the median (middle value) of the numbers
*/
double getMedian(long* numbers, unsigned short numNumbers)
{
	double median;

	quicksort(numbers, 0, numNumbers - 1, true);

	if(numNumbers % 2 == 0)
	{
		//an even number of numbers, so the median is the average of the middle two
		median = numbers[(numNumbers / 2) - 1] + numbers[numNumbers / 2];
		median /= 2.0;
	}
	else
	{
		//an odd number of numbers, so the median is simply the middle number
		median = numbers[numNumbers / 2];
	}

	return median;
}

/*
Given an array of numbers, removes duplicate numbers
*/
void removeDuplicates(long* numbers, unsigned short numNumbers, long* noDuplicates, unsigned short &numUnique)
{
	//there will be at least one unique number
	numUnique = 1;

	quicksort(numbers, 0, numNumbers - 1, true);

	//begin with the first number
	long lastNum = numbers[0];
	noDuplicates[0] = lastNum;

	for(int i = 1; i < numNumbers; ++i)
	{
		//the array is sorted, so duplicates will be grouped together
		if(numbers[i] != lastNum)
		{
			noDuplicates[numUnique] = numbers[i];
			numUnique++;
			lastNum = numbers[i];
		}
	}
}

/*
Find the number of occurances of each unique number
*/
void getCount(long* numbers, unsigned short numNumbers, long* noDuplicates, long* numOccurances, unsigned short &numUnique)
{
	removeDuplicates(numbers, numNumbers, noDuplicates, numUnique);

	for(int i = 0; i < numUnique; ++i)
	{
		//initialize each index before it is used
		numOccurances[i] = 0;

		for(int j = 0; j < numNumbers; ++j)
		{
			if(noDuplicates[i] == numbers[j])
			{
				numOccurances[i]++;
			}
		}
	}
}

/*
Find the mode (most common value) of the numbers
*/
void getMode(long* numbers, unsigned short numNumbers, long* modes, unsigned short* occurances, unsigned short &numModes)
{
	unsigned short numUnique;
	long* noDuplicates = new long[numNumbers];
	long* numOccurances = new long[numNumbers];

	getCount(numbers, numNumbers, noDuplicates, numOccurances, numUnique);

	unsigned short mostOccurances = 0;
	numModes = 0;

	int i;
	//find the most occurances of a number
	for(i = 0; i < numUnique; ++i)
	{
		if(numOccurances[i] > mostOccurances)
		{
			mostOccurances = numOccurances[i];
		}
	}

	//there are no modes when every number only appears once
	if(mostOccurances == 1)
	{
		numModes = 0;
		return;
	}

	//then find every number with that many occurances
	for(i = 0; i < numUnique; ++i)
	{
		if(numOccurances[i] == mostOccurances)
		{
			modes[numModes] = noDuplicates[i];
			occurances[numModes] = numOccurances[i];
			numModes++;
		}
	}

	delete[] noDuplicates;
	delete[] numOccurances;
}

/*
Find the range (difference between largest and smallest numbers)
*/
long getRange(long numbers[], unsigned short numNumbers)
{
	long largest, smallest;
	largest = numbers[0];
	smallest = numbers[0];

	for(int i = 0; i < numNumbers; ++i)
	{
		if(numbers[i] > largest)
		{
			largest = numbers[i];
		}
		if(numbers[i] < smallest)
		{
			smallest = numbers[i];
		}
	}

	return largest - smallest;
}

/*
Find the amount of numbers that are greater than and the amount that are less than the mean/average
*/
void getLessGreaterThanMean(long* numbers, unsigned short numNumbers, unsigned short &numLess, unsigned short &numGreater)
{
	double mean = getMean(numbers, numNumbers);

	numLess = 0;
	numGreater = 0;

	for(int i = 0; i < numNumbers; ++i)
	{
		if(numbers[i] < mean)
		{
			numLess++;
		}
		else if(numbers[i] > mean)
		{
			numGreater++;
		}
	}
}

/*
Find the amount of numbers that are greater than and the amount that are less than the median
*/
void getLessGreaterThanMedian(long* numbers, unsigned short numNumbers, unsigned short &numLess, unsigned short &numGreater)
{
	double median = getMedian(numbers, numNumbers);

	numLess = 0;
	numGreater = 0;

	for(int i = 0; i < numNumbers; ++i)
	{
		if(numbers[i] < median)
		{
			numLess++;
		}
		else if(numbers[i] > median)
		{
			numGreater++;
		}
	}
}

/*
Finds the number of odd versus even numbers
*/
void getOddEven(long* numbers, unsigned short numNumbers, unsigned short &numOdd, unsigned short &numEven)
{
	numOdd = 0;
	numEven = 0;

	for(int i = 0; i < numNumbers; ++i)
	{
		if(numbers[i] % 2 == 0)
		{
			numEven++;
		}
		else
		{
			numOdd++;
		}
	}
}

int main(int argc, char* argv[])
{
	//holds the actions to be performed
	std::map <std::string, bool> actionsToTake;

	actionsToTake["mean"] = false;
	actionsToTake["median"] = false;
	actionsToTake["range"] = false;
	actionsToTake["mode"] = false;
	actionsToTake["count"] = false;
	actionsToTake["lessGreaterThanMean"] = false;
	actionsToTake["lessGreaterThanMedian"] = false;
	actionsToTake["oddEven"] = false;
	actionsToTake["sortSmallToLarge"] = false;
	actionsToTake["SortLargeToSmall"] = false;

	//the number of command line options that are not numbers to operate on
	int numSeparatedOptions = 0;

	//shortcut to stop parsing options if -A is given
	bool optionAllGiven = false;

	//this will hold the numbers that are passed to the program
	long* numbers;

	if(argc == 1)
	{
		//no arguments given, print info and exit
		printUsageInfo();
		return 0;
	}
	else
	{
		//count the number of command line options that are not numbers
		for(int i = 1; i < argc; ++i)
		{
			//if it begins with '-', it is an option
			if(argv[i][0] == '-')
			{
				numSeparatedOptions++;
			}
		}

		char opt;
		//parse options
		while((opt = getopt(argc, argv, "acghmorsAGMRS")) != -1)
		{
			switch(opt)
			{
				case 'a':
					actionsToTake["mean"] = true;
					break;
				case 'A':
					//fill array with all actions except sorting
					actionsToTake["mean"] = true;
					actionsToTake["median"] = true;
					actionsToTake["range"] = true;
					actionsToTake["mode"] = true;
					actionsToTake["count"] = true;
					actionsToTake["lessGreaterThanMean"] = true;
					actionsToTake["lessGreaterThanMedian"] = true;
					actionsToTake["oddEven"] = true;
					optionAllGiven = true;
					break;
				case 'c':
					actionsToTake["count"] = true;
					break;
				case 'g':
					actionsToTake["lessGreaterThanMean"] = true;
					break;
				case 'G':
					actionsToTake["lessGreaterThanMedian"] = true;
					break;
				case 'h':
					printFullInfo();
					return 0;
				case 'm':
					actionsToTake["median"] = true;
					break;
				case 'M':
					actionsToTake["mode"] = true;
					break;
				case 'o':
					actionsToTake["oddEven"] = true;
					break;
				case 'r':
					actionsToTake["range"] = true;
					break;
				case 'R':
					//set mean, median, range, and mode to be performed
					actionsToTake["mean"] = true;
					actionsToTake["median"] = true;
					actionsToTake["range"] = true;
					actionsToTake["mode"] = true;
					break;
				case 's':
					actionsToTake["sortSmallToLarge"] = true;
					break;
				case 'S':
					actionsToTake["SortLargeToSmall"] = true;
					break;
				default: // '?'
					printUsageInfo();
					return 0;
			}

			//shortcut to stop parsing if the -A option was given
			if(optionAllGiven)
			{
				break;
			}
		}
	}

	unsigned short numbersArrayLength = argc - numSeparatedOptions - 1;
	//minus 1 because the first argument argc counts is the program itself
	if(numbersArrayLength > 0)
	{
		numbers = new long[numbersArrayLength];
		for(unsigned short i = 0; i < numbersArrayLength; ++i)
		{
			//plus one because the first argument argc counts is the program itself
			numbers[i] = atoi(argv[numSeparatedOptions + i + 1]);
		}
	}
	else
	{
		//numbers should have been specified because we got an option that wasn't -h
		printUsageInfo();
		return 0;
	}

	//sorting from smallest to largest is the default action if no options are specified
	if(numSeparatedOptions == 0)
	{
		actionsToTake["sortSmallToLarge"] = true;
	}

	//perform each requested action
	if(actionsToTake["mean"]) //get mean
	{
		std::cout << std::endl;

		double mean = getMean(numbers, numbersArrayLength);
		std::cout << "Mean:\t\t" << mean << std::endl;
	}
	if(actionsToTake["median"]) //get median
	{
		std::cout << std::endl;

		double median = getMedian(numbers, numbersArrayLength);
		std::cout << "Median:\t\t" << median << std::endl;
	}
	if(actionsToTake["range"]) //get range
	{
		std::cout << std::endl;

		long range = getRange(numbers, numbersArrayLength);
		std::cout << "Range:\t\t" << range << std::endl;
	}
	if(actionsToTake["mode"]) //get mode(s)
	{
		std::cout << std::endl;

		long* modes = new long[numbersArrayLength];
		unsigned short* occurances = new unsigned short[numbersArrayLength];
		unsigned short numModes;

		getMode(numbers, numbersArrayLength, modes, occurances, numModes);

		if(numModes == 0)
		{
			std::cout << "No mode(s)" << std::endl;
		}
		else
		{
			if(numModes == 1)
			{
				std::cout << "Mode:" << std::endl;
			}
			else
			{
				std::cout << "Modes:" << std::endl;
			}

			for(int i = 0; i < numModes; ++i)
			{
				std::cout << modes[i] << ":\t\t" << occurances[i];
				if(occurances[i] == 1)
				{
					std::cout << " occurance" << std::endl;
				}
				else
				{
					std::cout << " occurances" << std::endl;
				}
			}
		}

		delete[] modes;
		delete[] occurances;
	}
	if(actionsToTake["count"]) //get count of occurances of unique numbers
	{
		std::cout << std::endl;

		unsigned short numUnique;
		long* noDuplicates = new long[numbersArrayLength];
		long* numOccurances = new long[numbersArrayLength];

		getCount(numbers, numbersArrayLength, noDuplicates, numOccurances, numUnique);

		std::cout << "Occurances of unique numbers:" << std::endl;

		for(int i = 0; i < numUnique; ++i)
		{
			std::cout << noDuplicates[i] << ":\t\t" << numOccurances[i] << " occurances" << std::endl;
		}

		delete[] noDuplicates;
		delete[] numOccurances;
	}
	if(actionsToTake["lessGreaterThanMedian"]) //get less/greater than median
	{
		std::cout << std::endl;

		unsigned short numLessMedian;
		unsigned short numGreaterMedian;

		getLessGreaterThanMedian(numbers, numbersArrayLength, numLessMedian, numGreaterMedian);

		std::cout << "Number of values < median: " << numLessMedian << " (" << ((double)numLessMedian / numbersArrayLength * 100) << "%)" << std::endl;
		std::cout << "Number of values > median: " << numGreaterMedian << " (" << ((double)numGreaterMedian / numbersArrayLength) * 100 << "%)" << std::endl;
	}
	if(actionsToTake["lessGreaterThanMean"]) //get less/greater than mean
	{
		std::cout << std::endl;

		unsigned short numLessMean;
		unsigned short numGreaterMean;

		getLessGreaterThanMean(numbers, numbersArrayLength, numLessMean, numGreaterMean);

		std::cout << "Number of values < mean: " << numLessMean << " (" << ((double)numLessMean / numbersArrayLength * 100) << "%)" << std::endl;
		std::cout << "Number of values > mean: " << numGreaterMean << " (" << ((double)numGreaterMean / numbersArrayLength) * 100 << "%)" << std::endl;
	}
	if(actionsToTake["oddEven"]) //get odd/even
	{
		std::cout << std::endl;

		unsigned short numOdd;
		unsigned short numEven;

		getOddEven(numbers, numbersArrayLength, numOdd, numEven);

		std::cout << "Odd: " << numOdd << "(" << ((double)numOdd / numbersArrayLength) * 100 << "%)" << std::endl;
		std::cout << "Even: " << numEven << "(" << ((double) numEven / numbersArrayLength) * 100 << "%)" << std::endl;
	}
	if(actionsToTake["sortSmallToLarge"]) //sort small to large
	{
		std::cout << std::endl << "Numbers sorted from smallest to largest:" << std::endl;

		quicksort(numbers, 0, numbersArrayLength - 1, true);
		for(int i = 0; i < numbersArrayLength; ++i)
		{
			std::cout << numbers[i] << std::endl;
		}
	}
	if(actionsToTake["SortLargeToSmall"]) //sort large to small
	{
		std::cout << std::endl << "Numbers sorted from largest to smallest:" << std::endl;

		quicksort(numbers, 0, numbersArrayLength - 1, false);
		for(int i = 0; i < numbersArrayLength; ++i)
		{
			std::cout << numbers[i] << std::endl;
		}
	}

	std::cout << std::endl;

	delete[] numbers;

	return 0;
}