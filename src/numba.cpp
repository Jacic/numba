/*
numba - a NUMBer Analyzer
See LICENSE file for license information
*/

#include <iostream>
#include <string>
#include <getopt.h>
#include <stdlib.h>

//the current version of the program
const std::string version = "0.11.2";

/*
Prints information on how to use this program
*/
void printUsageInfo()
{
	using namespace std;

	cout << "Usage:" << endl;
	cout << "numba <options> num1 num2 ... numN" << "\n----" << endl;
	cout << "Options:" << endl;
	cout << "-a\t\tDisplay the mean/average of the given numbers" << endl << endl;
	cout << "-A\t\tPerform all operations except sorting on the given numbers" << endl << endl;
	cout << "-c\t\tDisplay the number of occurances of each unique number" << endl << endl;
	cout << "-g\t\tDisplay the amount of numbers that are greater than and the amount that are less than the mean/average" << endl << endl;
	cout << "-G\t\tDisplay the amount of numbers that are greater than and the amount that are less than the median" << endl << endl;
	cout << "-h\t\tPrint this information then exit" << endl << endl;
	cout << "-m\t\tDisplay the median (middle value) of the given numbers" << endl << endl;
	cout << "-M\t\tDisplay the mode (most common value) of the given numbers" << endl << endl;
	cout << "-r\t\tDisplay the range of the given numbers" << endl << endl;
	cout << "-R\t\tDisplay mean, median, mode, and range of the given numbers" << endl << endl;
	cout << "-s\t\tSort the given numbers from smallest to largest" << endl << endl;
	cout << "-S\t\tSort the given numbers from largest to smallest" << endl << endl;
}

/*
Prints name, description, and version number of this program, as well as usage information
*/
void printFullInfo()
{
	using namespace std;

	cout << "----\nnumba, a NUMBer Analyzer version " << version << "\n----" << endl;
	cout << "numba sorts numbers, finds the median, mean, mode, range, and more.\n----" << endl;
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

	long lastNum = numbers[0];
	noDuplicates[0] = lastNum;

	for(int i = 1; i < numNumbers; ++i)
	{
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

int main(int argc, char* argv[])
{
	//default action is to sort from smallest to largest
	std::string actionsToTake[7];
	int numActionsToTake = 0;
	int numOptions = 0;

	//whether we sort from small to large or large to small
	bool smallToLarge;

	//shortcut to stop parsing options if -A is given
	bool optionAllGiven;

	long* numbers;

	if(argc == 1)
	{
		//no arguments given, print info and exit
		printUsageInfo();
		return 0;
	}
	else
	{
		char opt;
		//parse options
		//bug(?) when two options given with one - (ex: -gG doesn't work correctly, but -g -G does)
		while((opt = getopt(argc, argv, "acghmrsAGMRS")) != -1)
		{
			switch(opt)
			{
				case 'a':
					actionsToTake[numActionsToTake] = "getMean";
					numActionsToTake++;
					break;
				case 'A':
					//fill array with all actions except sorting
					actionsToTake[0] = "getMean";
					actionsToTake[1] = "getMedian";
					actionsToTake[2] = "getRange";
					actionsToTake[3] = "getCount";
					actionsToTake[4] = "getMode";
					actionsToTake[5] = "getLessGreaterThanMean";
					actionsToTake[6] = "getLessGreaterThanMedian";
					numActionsToTake = 7;
					optionAllGiven = true;
					break;
				case 'c':
					actionsToTake[numActionsToTake] = "getCount";
					numActionsToTake++;
					break;
				case 'g':
					actionsToTake[numActionsToTake] = "getLessGreaterThanMean";
					numActionsToTake++;
					break;
				case 'G':
					actionsToTake[numActionsToTake] = "getLessGreaterThanMedian";
					numActionsToTake++;
					break;
				case 'h':
					printFullInfo();
					return 0;
				case 'm':
					actionsToTake[numActionsToTake] = "getMedian";
					numActionsToTake++;
					break;
				case 'M':
					actionsToTake[numActionsToTake] = "getMode";
					numActionsToTake++;
					break;
				case 'r':
					actionsToTake[numActionsToTake] = "getRange";
					numActionsToTake++;
					break;
				case 'R':
					//fill array with mean, median, mode, and range
					actionsToTake[numActionsToTake] = "getMean";
					actionsToTake[numActionsToTake + 1] = "getMedian";
					actionsToTake[numActionsToTake + 2] = "getRange";
					actionsToTake[numActionsToTake + 3] = "getMode";
					numActionsToTake += 4;
					break;
				case 's':
					smallToLarge = true;
					actionsToTake[numActionsToTake] = "sortSmallToLarge";
					numActionsToTake++;
					break;
				case 'S':
					smallToLarge = false;
					actionsToTake[numActionsToTake] = "sortLargeToSmall";
					numActionsToTake++;
					break;
				default: // '?'
					printUsageInfo();
					return 0;
			}
			numOptions += 1;

			if(optionAllGiven)
			{
				break;
			}
		}
	}

	unsigned short numbersArrayLength = argc - numOptions - 1;
	//minus 1 because the first argument argc counts is the program itself
	if(numbersArrayLength > 0)
	{
		numbers = new long[numbersArrayLength];
		for(unsigned short i = 0; i < numbersArrayLength; ++i)
		{
			//plus one because the first argument argc counts is the program itself
			numbers[i] = atoi(argv[numOptions + i + 1]);
		}
	}
	else
	{
		//numbers should have been specified because we got an option that wasn't -h
		printUsageInfo();
		return 0;
	}

	//sorting from smallest to largest is the default action if no options are specified
	if(numOptions == 0)
	{
		smallToLarge = true;
		actionsToTake[0] = "sortSmallToLarge";
		numActionsToTake = 1;
	}

	std::cout << std::endl;

	//loop through the requested actions and perform them
	for(int i = 0; i < numActionsToTake; ++i)
	{
		//both small to large and large to small use the same function
		if(actionsToTake[i] == "sortSmallToLarge" || actionsToTake[i] == "sortLargeToSmall")
		{
			quicksort(numbers, 0, numbersArrayLength - 1, smallToLarge);
			for(int i = 0; i < numbersArrayLength; ++i)
			{
				std::cout << numbers[i] << std::endl;
			}
		}
		else if(actionsToTake[i] == "getCount")
		{
			unsigned short numUnique;
			long* noDuplicates = new long[numbersArrayLength];
			long* numOccurances = new long[numbersArrayLength];

			getCount(numbers, numbersArrayLength, noDuplicates, numOccurances, numUnique);

			std::cout << "Occurances:" << std::endl;

			for(int i = 0; i < numUnique; ++i)
			{
				std::cout << noDuplicates[i] << ":\t\t" << numOccurances[i] << " occurances" << std::endl;
			}

			delete[] noDuplicates;
			delete[] numOccurances;
		}
		else if(actionsToTake[i] == "getMean")
		{
			double mean = getMean(numbers, numbersArrayLength);
			std::cout << "Mean:\t\t" << mean << std::endl;
		}
		else if(actionsToTake[i] == "getMedian")
		{
			double median = getMedian(numbers, numbersArrayLength);
			std::cout << "Median:\t\t" << median << std::endl;
		}
		else if(actionsToTake[i] == "getMode")
		{
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
				std::cout << "Mode(s):" << std::endl;

				for(int i = 0; i < numModes; ++i)
				{
					std::cout << modes[i] << ":\t\t" << occurances[i] << " occurances" << std::endl;
				}
			}

			delete[] modes;
			delete[] occurances;
		}
		else if(actionsToTake[i] == "getRange")
		{
			long range = getRange(numbers, numbersArrayLength);
			std::cout << "Range:\t\t" << range << std::endl;
		}
		else if(actionsToTake[i] == "getLessGreaterThanMean")
		{
			unsigned short numLessMean;
			unsigned short numGreaterMean;

			getLessGreaterThanMean(numbers, numbersArrayLength, numLessMean, numGreaterMean);

			std::cout << "Number of values < mean: " << numLessMean << "\tNumber of values > mean: " << numGreaterMean << std::endl;
		}
		else if(actionsToTake[i] == "getLessGreaterThanMedian")
		{
			unsigned short numLessMedian;
			unsigned short numGreaterMedian;

			getLessGreaterThanMedian(numbers, numbersArrayLength, numLessMedian, numGreaterMedian);

			std::cout << "Number of values < median: " << numLessMedian << "\tNumber of values > median: " << numGreaterMedian << std::endl;
		}
	}

	std::cout << std::endl;

	delete[] numbers;

	return 0;
}