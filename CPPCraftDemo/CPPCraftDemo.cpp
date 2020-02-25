#include "stdafx.h"
#include <chrono>
#include <iostream>
#include <ratio>

#include "QBDataBaseV1.h"
#include "QBDataBaseV2.h"
#include "QBDataBaseV3.h"

int main(int argc, _TCHAR* argv[])
{
	const int DB_SIZE = 100000;
	using namespace std::chrono;
	std::cout.precision(10);
	std::cout.setf(std::ios_base::floatfield, std::ios_base::fixed);

	{
		// populate a bunch of data
		auto data = populateDummyData("testdata", DB_SIZE);
		// Find a record that contains and measure the perf
		auto startTimer = steady_clock::now();
		
		auto filteredSetByCol0 = QBFindMatchingRecords(data, "column0", "50000");
		auto filteredSetByCol1 = QBFindMatchingRecords(data, "column1", "testdata5");
		auto filteredSetByCol2 = QBFindMatchingRecords(data, "column2", "24");
		auto filteredSetByCol3 = QBFindMatchingRecords(data, "column3", "5testdata");
		DeleteRecordByID(data, 3633);
		auto filteredSet2 = QBFindMatchingRecords(data, "column0", "3633");

		std::cout << "profiler (original): " << double((steady_clock::now() - startTimer).count()) * steady_clock::period::num / steady_clock::period::den << std::endl;

		// make sure that the function is correct
		assert(filteredSetByCol0.size() == 1);
		assert(filteredSetByCol1.size() == 11111);
		assert(filteredSetByCol2.size() == DB_SIZE / 100);
		assert(filteredSetByCol3.size() == DB_SIZE / 10);
		assert(filteredSet2.size() == 0);
	}

	// SoA approach with vectors
	{
		auto dataV2 = populateDummyDataV2("testdata", DB_SIZE);
		auto startTimer = steady_clock::now();
		
		auto filteredSetByCol0V2 = QBFindMatchingRecordsV2(dataV2, "column0", "50000");
		auto filteredSetByCol1V2 = QBFindMatchingRecordsV2(dataV2, "column1", "testdata5");
		auto filteredSetByCol2V2 = QBFindMatchingRecordsV2(dataV2, "column2", "24");
		auto filteredSetByCol3V2 = QBFindMatchingRecordsV2(dataV2, "column3", "5testdata");
		DeleteRecordByIDV2(dataV2, 3633);
		auto filteredSetV22 = QBFindMatchingRecordsV2(dataV2, "column0", "3633");

		std::cout << "profiler (SoA approach): " << double((steady_clock::now() - startTimer).count()) * steady_clock::period::num / steady_clock::period::den << std::endl;
		
		assert(filteredSetByCol0V2.column0.size() == 1);
		assert(filteredSetByCol1V2.column0.size() == 11111);
		assert(filteredSetByCol2V2.column0.size() == DB_SIZE / 100);
		assert(filteredSetByCol3V2.column0.size() == DB_SIZE / 10);
		assert(filteredSetV22.column0.size() == 0);
	}

	// Binary search tree indexes approach
	{
		auto dataV3 = populateDummyDataV3("testdata", DB_SIZE);
		auto startTimer = steady_clock::now();

		auto filteredSetByCol0V3 = QBFindMatchingRecordsV3(dataV3, "column0", "50000");
		auto filteredSetByCol1V3 = QBFindMatchingRecordsV3(dataV3, "column1", "testdata5");
		auto filteredSetByCol2V3 = QBFindMatchingRecordsV3(dataV3, "column2", "24");
		auto filteredSetByCol3V3 = QBFindMatchingRecordsV3(dataV3, "column3", "5testdata");
		DeleteRecordByIDV3(dataV3, 3633);
		auto filteredSetV32 = QBFindMatchingRecordsV3(dataV3, "column0", "3633");

		std::cout << "profiler (Binary search tree indexes approach): " << double((steady_clock::now() - startTimer).count()) * steady_clock::period::num / steady_clock::period::den << std::endl;
		
		assert(filteredSetByCol0V3.size() == 1);
		assert(filteredSetByCol1V3.size() == 11111);
		assert(filteredSetByCol2V3.size() == DB_SIZE / 100);
		assert(filteredSetByCol3V3.size() == DB_SIZE / 10);
		assert(filteredSetV32.size() == 0);
	}

	return 0;
}

