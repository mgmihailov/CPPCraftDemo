#pragma once

#include "stdafx.h"

#include <string>
#include <vector>
#include <map>
#include <algorithm>

struct QBRecordV3
{
	unsigned column0; // unique id column
	std::string column1;
	long column2;
	std::string column3;
};

typedef std::vector<QBRecordV3> QBRecordCollectionV3;

// Index the entries by column0 and column1 for logarithmic
// complexity when searching by those two columns.
// Since column0 is for the unique ID, it's a map as we are
// guaranteed to not have duplicates. However, we don't have
// that restriction for column2 values and there can be duplicates,
// hence the multimap.
std::map<unsigned, QBRecordV3*> column0Index;
std::multimap<long, QBRecordV3*> column2Index;

QBRecordCollectionV3 QBFindMatchingRecordsV3(const QBRecordCollectionV3& records, const std::string& columnName, const std::string& matchString)
{
	QBRecordCollectionV3 result;
	if (columnName == "column0")
	{
		unsigned id = std::stoul(matchString);
		auto it = column0Index.find(id);
		if (it != column0Index.end())
		{
			result.push_back(*(it->second));
		}
	}
	else if (columnName == "column1")
	{
		for (const auto& rec : records)
		{
			if (rec.column1.find(matchString) != std::string::npos)
			{
				result.push_back(rec);
			}
		}
	}
	else if (columnName == "column2")
	{
		long val = std::stol(matchString);
		auto range = column2Index.equal_range(val);
		for (auto it = range.first; it != range.second; ++it)
		{
			result.push_back(*(it->second));
		}
	}
	else if (columnName == "column3")
	{
		for (const auto& rec : records)
		{
			if (rec.column3.find(matchString) != std::string::npos)
			{
				result.push_back(rec);
			}
		}
	}
	else
	{
		assert(false && "Invalid column name!");
	}
	return result;
}

void DeleteRecordByIDV3(QBRecordCollectionV3& records, unsigned id)
{
	auto it = std::find_if(records.begin(), records.end(), [&id](const QBRecordV3& rec)
		{
			return rec.column0 == id;
		});

	if (it != records.end())
	{
		column0Index.erase(it->column0);
		column2Index.erase(it->column2);
		records.erase(it);
	}
}

QBRecordCollectionV3 populateDummyDataV3(const std::string& prefix, int numRecords)
{
	QBRecordCollectionV3 data;
	data.reserve(numRecords);
	for (unsigned i = 0; i < (unsigned)numRecords; i++)
	{
		QBRecordV3 rec = { i, prefix + std::to_string(i), i % 100, std::to_string(i) + prefix };
		data.emplace_back(rec);
		column0Index.insert(std::pair<unsigned, QBRecordV3*>(i, &data.back()));
		column2Index.insert(std::pair<long, QBRecordV3*>(i % 100, &data.back()));
	}
	return data;
};