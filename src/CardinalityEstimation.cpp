//
// You should modify this file.
//
#include <common/Root.h>
#include <common/Expression.h>

#include <CardinalityEstimation.h>

#include <omp.h>

void CEEngine::insertTuple(const std::vector<int>& tuple)
{
    this->table.emplace_back(tuple[0], tuple[1]);
    this->num_processed++;

    if (this->reservoir.size() < this->sample_size) 
    {
        reservoir.emplace_back(tuple[0], tuple[1]);
    } 
    else 
    {
        // generate pseudo number
        static std::mt19937 rng(std::random_device{}());
        std::uniform_int_distribution<int> dist(0, this->num_processed - 1);

        int index = dist(rng);
        if (index < sample_size) 
        {
            reservoir[index] = {tuple[0], tuple[1]};
        }
    }

    this->total_size++;

}

void CEEngine::deleteTuple(const std::vector<int>& tuple, int tupleId)
{
    if (tupleId >= 0 && tupleId < this->reservoir.size()) 
    {
        reservoir[tupleId] = {-1, -1};
    }
    this->total_size--;
}

int CEEngine::query(const std::vector<CompareExpression>& quals)
{   
    int sample_count = 0;

    // avoid flotting point
    if(this->sample_size < 1)
    {
        return 1;
    }

    // apply quals and do the estimation
    
    #pragma omp parallel for reduction(+:sample_count)
    for (size_t i = 0; i < reservoir.size(); i++) 
    {
        const auto& [colA, colB] = reservoir[i];

        if (colA == -1 && colB == -1) continue;
        
        if ((quals[0].compareOp == GREATER && colA > quals[0].value) ||
            (quals[0].compareOp == EQUAL && colA == quals[0].value) ||
            (quals.size() > 1 && ((quals[1].compareOp == GREATER && colB > quals[1].value) ||
                                  (quals[1].compareOp == EQUAL && colB == quals[1].value)))) 
        {
            sample_count++;
        }
    }

    return (sample_count * this->total_size) / this->sample_size;
}

void CEEngine::prepare() {
    int newTotalSize = this->table.size();
    if (newTotalSize != this->total_size) 
    {
        this->total_size = newTotalSize;
        this->sample_size = std::round(total_size * (SAMPLE_PERCENTAGE / 100.0));
    }

    if (reservoir.size() < sample_size) 
    {
        reservoir.reserve(sample_size);
    }
}

CEEngine::CEEngine(int num, DataExecuter *dataExecuter)
{
    this->dataExecuter = dataExecuter;

    // initialize columnA & columnB
    this->table = std::vector<std::pair<int, int>>();

    // allocate a maximum to table
    this->table.reserve(num);
    
    this->total_size = 0;
    this->num_processed = 0;
    this->sample_size = 0;
}
