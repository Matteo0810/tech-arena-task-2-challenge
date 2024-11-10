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
    /*if(tupleId >= 0 && tupleId <= this->columnA.size())
    {
        this->columnA[tupleId] = -1;
    }

    if(tupleId >= 0 && tupleId <= this->columnB.size())
    {
        this->columnB[tupleId] = -1;
    }*/

    // TODO: avoid theses functions because of their memory use
    this->columnA.erase(this->columnA.begin() + tupleId);
    this->columnB.erase(this->columnB.begin() + tupleId);
}

int CEEngine::query(const std::vector<CompareExpression>& quals)
{   
    int sample_count = 0;

    // apply quals and do the estimation
    for (int i = 0; i < this->sample_size; i++)
    {
        auto qualA = quals[0];
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

    const int estimation = ((sample_count) / this->sample_size) * this->columnA.size();
    return estimation;
}

void CEEngine::prepare()
{   
    const int percentage = 10.;

    this->sample_size = std::round(this->columnA.size() * (percentage / 100.));
    std::cout << this->columnA.size() << std::endl;

    std::vector<int> indexes(this->sample_size);
    
    for (int i = 0; i < this->sample_size; i++) 
    {
        indexes[i] = i;
    }
    
    std::random_shuffle(indexes.begin(), indexes.end());
    
    for (int i = 0; i < this->sample_size; i++) 
    {
        const int index = indexes[i];

        this->sample[i] = {
            this->columnA[index],
            this->columnB[index]
        };
    }
}

CEEngine::CEEngine(int num, DataExecuter *dataExecuter)
{
    this->dataExecuter = dataExecuter;

    this->size = num;

    // initialize columnA & columnB
    this->columnA = std::vector<int>();
    this->columnB = std::vector<int>();

    // allocate a maximum of num to the column
    this->columnA.reserve(num);
    this->columnB.reserve(num);
}
