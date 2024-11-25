//
// You should modify this file.
//
#include <common/Root.h>
#include <common/Expression.h>

#include <CardinalityEstimation.h>

// percentage to take between all columns
const int SAMPLE_PERCENTAGE = 10; // %

void CEEngine::insertTuple(const std::vector<int>& tuple)
{
    this->columnA.emplace_back(tuple[0]);
    this->columnB.emplace_back(tuple[1]);
}

void CEEngine::deleteTuple(const std::vector<int>& tuple, int tupleId)
{
    if(tupleId >= 0 && tupleId < this->total_size) 
    {
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
        std::vector<int> row = { this->columnA[i], this->columnB[i] };

        if (
            (quals[0].compareOp == GREATER && quals[0].value < row[0])
                    || (quals[0].compareOp == EQUAL && quals[0].value == row[0])
            || (quals.size() > 1 && (quals[1].compareOp == GREATER && quals[1].value < row[1])
                    || (quals[1].compareOp == EQUAL && quals[1].value == row[1]))
        )
        {
            sample_count++;
        }
    }

    const int estimation = (sample_count / this->sample_size) * this->total_size;
    return estimation;
}

void CEEngine::prepare()
{  
    // real time update of theses variables
    this->total_size = this->columnA.size(); // we can either choose column A or B

    this->sample_size = std::round(this->total_size * (SAMPLE_PERCENTAGE / 100.));
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
