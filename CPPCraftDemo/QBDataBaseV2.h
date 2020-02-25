#pragma once

#include "stdafx.h"

#include <vector>
#include <string>

// The collection uses a SoA layout - all values for a given column
// are stored linearly in memory.
struct QBRecordCollectionV2
{
	std::vector<unsigned> column0;
	std::vector<std::string> column1;
	std::vector<long> column2;
	std::vector<std::string> column3;
};

// Instead of iterating through whole records when searching for a value of a column,
// we iterate specifically through the values of the specific column. This approach
// better utilizes the cache, slightly improving performance.
QBRecordCollectionV2 QBFindMatchingRecordsV2(const QBRecordCollectionV2& records, const std::string& columnName, const std::string& matchString)
{
	QBRecordCollectionV2 result;
	if (columnName == "column0")
	{
		unsigned id = std::stoul(matchString);
		for (auto i = 0u; i < records.column0.size(); ++i)
		{
			if (records.column0[i] == id)
			{
				result.column0.push_back(records.column0[i]);
				result.column1.push_back(records.column1[i]);
				result.column2.push_back(records.column2[i]);
				result.column3.push_back(records.column3[i]);
			}
		}
	}
	else if (columnName == "column1")
	{
		for (auto i = 0u; i < records.column1.size(); ++i)
		{
			if (records.column1[i].find(matchString) != std::string::npos)
			{
				result.column0.push_back(records.column0[i]);
				result.column1.push_back(records.column1[i]);
				result.column2.push_back(records.column2[i]);
				result.column3.push_back(records.column3[i]);
			}
		}
	}
	else if (columnName == "column2")
	{
		long val = std::stol(matchString);
		for (auto i = 0u; i < records.column2.size(); ++i)
		{
			if (records.column2[i] == val)
			{
				result.column0.push_back(records.column0[i]);
				result.column1.push_back(records.column1[i]);
				result.column2.push_back(records.column2[i]);
				result.column3.push_back(records.column3[i]);
			}
		}
	}
	else
	{
		for (auto i = 0u; i < records.column3.size(); ++i)
		{
			if (records.column3[i].find(matchString) != std::string::npos)
			{
				result.column0.push_back(records.column0[i]);
				result.column1.push_back(records.column1[i]);
				result.column2.push_back(records.column2[i]);
				result.column3.push_back(records.column3[i]);
			}
		}
	}

	return result;
};

void DeleteRecordByIDV2(QBRecordCollectionV2& records, unsigned id)
{
	int idx = 0;
	for (; idx < records.column0.size(); ++idx)
	{
		if (records.column0[idx] == id)
		{
			break;
		}
	}

	{
		unsigned temp = records.column0[idx];
		records.column0[idx] = records.column0[records.column0.size() - 1];
		records.column0[records.column0.size() - 1] = temp;
		records.column0.pop_back();
	}

	{
		std::string temp = records.column1[idx];
		records.column1[idx] = records.column1[records.column1.size() - 1];
		records.column1[records.column1.size() - 1] = temp;
		records.column1.pop_back();
	}

	{
		long temp = records.column2[idx];
		records.column2[idx] = records.column2[records.column2.size() - 1];
		records.column2[records.column2.size() - 1] = temp;
		records.column2.pop_back();
	}

	{
		std::string temp = records.column3[idx];
		records.column3[idx] = records.column3[records.column3.size() - 1];
		records.column3[records.column3.size() - 1] = temp;
		records.column3.pop_back();
	}
}

// Utility function used to populate the data base with some data.
QBRecordCollectionV2 populateDummyDataV2(const std::string& prefix, int numRecords)
{
	QBRecordCollectionV2 data;
	data.column0.reserve(numRecords);
	data.column1.reserve(numRecords);
	data.column2.reserve(numRecords);
	data.column3.reserve(numRecords);
	for (unsigned i = 0; i < (unsigned)numRecords; i++)
	{
		data.column0.emplace_back(i);
		data.column1.emplace_back(prefix + std::to_string(i));
		data.column2.emplace_back(i % 100);
		data.column3.emplace_back(std::to_string(i) + prefix);
	}
	return data;
};