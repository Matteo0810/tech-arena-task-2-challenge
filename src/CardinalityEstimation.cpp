//
// You should modify this file.
//
#include <stdexcept>

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
    this->columnA.erase(this->columnA.begin() + tupleId);
    this->columnB.erase(this->columnB.begin() + tupleId);
}

int CEEngine::query(const std::vector<CompareExpression>& quals)
{
    // maximum of two constraint conditions (one for each column)
    if (quals.size() < 1 || quals.size() > 2)
    {
        throw std::invalid_argument("Received zero or more than two constraints conditions.");
    }

    int estimation = 0;

    // We have to find all tuple that meets both CompareExpression A and B.
    // for instance: A > 0 (s1) and B = 100 (s2)

    for (int i = 0; i < this->columnA.size(); i++) 
    {
        auto qualA = quals[0];
        bool match = false;

        // check critera A
        if (
            (qualA.compareOp == GREATER && qualA.value > this->columnA[i])
            || (qualA.compareOp == EQUAL && qualA.value == this->columnA[i])
        )
        {
            match = true;
        }

        // if critera B exists
        if(quals.size() > 1)
        {
            auto qualB = quals[1];
            if (
                (qualB.compareOp == GREATER && qualB.value > this->columnB[i])
                || (qualB.compareOp == EQUAL && qualB.value == this->columnB[i])
            )
            {
                match = true;
            }
            else
            {
                // it's not a match if exists because we have to match both criteria A and B
                match = false;
            }
        }

        if(match)
        {
            estimation++;
        }
    }
    
    // The cardinality estimation result is the size of | s1 ∩ s2 |
    return estimation;
}

void CEEngine::prepare()
{
    // TODO: prepare procedure
}

CEEngine::CEEngine(int num, DataExecuter *dataExecuter)
{
    this->dataExecuter = dataExecuter;
    this->size = num;

    // initialize columnA & columnB
    this->columnA = std::vector<int>();
    this->columnB = std::vector<int>();

    // allocate a maximum of num to the column
    this->columnA.reserve(this->size);
    this->columnB.reserve(this->size);
}
