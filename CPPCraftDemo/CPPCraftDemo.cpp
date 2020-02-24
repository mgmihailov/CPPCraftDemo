#include "stdafx.h"
#include <assert.h>
#include <chrono>
#include <iostream>
#include <ratio>

#include "QBDataBaseV1.h"
#include "QBDataBaseV2.h"

int main(int argc, _TCHAR* argv[])
{
	const int DB_SIZE = 100000;
    using namespace std::chrono;
	std::cout.precision(20);
	std::cout.setf(std::ios_base::floatfield, std::ios_base::fixed);
	{
		// populate a bunch of data
		auto data = populateDummyData("testdata", DB_SIZE);
		// Find a record that contains and measure the perf
		auto startTimer = steady_clock::now();
		auto filteredSet = QBFindMatchingRecords(data, "column1", "testdata5");
		auto filteredSet2 = QBFindMatchingRecords(data, "column2", "24");
		std::cout << "profiler (original): " << double((steady_clock::now() - startTimer).count()) * steady_clock::period::num / steady_clock::period::den << std::endl;

		// make sure that the function is correct
		assert(filteredSet.size() == 11111);
		assert(filteredSet2.size() == DB_SIZE / 100);
	}

	// SoA approach with vectors
	{
		auto dataV2 = populateDummyDataV2("testdata", DB_SIZE);
		auto startTimer = steady_clock::now();
		auto filteredSetV2 = QBFindMatchingRecordsV2(dataV2, "column1", "testdata5");
		auto filteredSetV22 = QBFindMatchingRecordsV2(dataV2, "column2", "24");
		std::cout << "profiler (SoA approach): " << double((steady_clock::now() - startTimer).count()) * steady_clock::period::num / steady_clock::period::den << std::endl;
		assert(filteredSetV2.column0.size() == 11111);
		assert(filteredSetV2.column1.size() == 11111);
		assert(filteredSetV2.column2.size() == 11111);
		assert(filteredSetV2.column3.size() == 11111);
		assert(filteredSetV22.column0.size() == DB_SIZE / 100);
		assert(filteredSetV22.column1.size() == DB_SIZE / 100);
		assert(filteredSetV22.column2.size() == DB_SIZE / 100);
		assert(filteredSetV22.column3.size() == DB_SIZE / 100);
	}

	return 0;
}

