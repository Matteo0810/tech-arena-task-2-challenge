//
// You should modify this file.
//
#include <algorithm>

#include <common/Root.h>
#include <common/Expression.h>

#include <CardinalityEstimation.h>

void CEEngine::insertTuple(const std::vector<int>& tuple)
{
    this->columnA.emplace_back(tuple[0]);
    this->columnB.emplace_back(tuple[1]);
}

void CEEngine::deleteTuple(const std::vector<int>& tuple, int tupleId)
{
    if(tupleId < this->total_size) {
        this->columnA.erase(this->columnA.begin() + tupleId);
        this->columnB.erase(this->columnB.begin() + tupleId);
    }
}

int CEEngine::query(const std::vector<CompareExpression>& quals)
{   
    int sample_count = 0;

    // apply quals and do the estimation
    for (int i = 0; i < this->sample_size; i++)
    {
        auto row = this->sample[i];
        bool match = false;

        for (auto qual : quals)
        {
            match = (qual.compareOp == GREATER && qual.value > row[qual.columnIdx])
                || (qual.compareOp == EQUAL && qual.value == row[qual.columnIdx]);
        }

        if (match)
        {
            sample_count++;
        }
    }

    const int estimation = (sample_count / this->sample_size) * this->total_size;
    return estimation;
}

void CEEngine::prepare()
{   
    // percentage to take in all columns
    const int percentage = 10;

    // real time update of theses variables
    this->total_size = this->columnA.size(); // we can either choose column A or B

    this->sample_size = std::round(this->total_size * (percentage / 100.));
    this->sample.resize(this->sample_size);
    
    std::vector<int> indexes(this->total_size);
    for (int i = 0; i < this->total_size; i++) 
    {
        indexes[i] = i;
    }
    
    // shuffle indexes to put randomely row
    std::random_shuffle(indexes.begin(), indexes.end());
    
    for (int i = 0; i < this->sample_size; i++) 
    {
        const int index = indexes[i];
        this->sample[i] = { this->columnA[index], this->columnB[index] };
    }
}

CEEngine::CEEngine(int num, DataExecuter *dataExecuter)
{
    this->dataExecuter = dataExecuter;

    // initialize columnA & columnB
    this->columnA = std::vector<int>();
    this->columnB = std::vector<int>();

    // allocate a maximum to the column
    this->columnA.reserve(num);
    this->columnB.reserve(num);
}
