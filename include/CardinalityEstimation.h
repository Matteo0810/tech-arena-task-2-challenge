#ifndef CARDINALITYESTIMATION_ENGINE
#define CARDINALITYESTIMATION_ENGINE
//
// You should modify this file.
//
#include <unordered_set>

#include <executer/DataExecuter.h>
#include <common/Expression.h>

const int SAMPLE_PERCENTAGE = 10; // %

class CEEngine {
public:
    /**
     * Insert a tuple, indicating that the tuple is inserted and appended to the end of the disk.
     * @param tuple Inserted tuple.
     */
    void insertTuple(const std::vector<int>& tuple);
    /**
     * Deletion function. Pass a tuple and tupleId, indicating that the tuple at the tupleId position is deleted.
     * @param tuple Deleted tuple.
     * @param tupleId Location of the deleted tuple.
     */
    void deleteTuple(const std::vector<int>& tuple, int tupleId);
    /**
     * Query function, pass in expression, return estimated cardinality result.
     * @param quals expression.
     * @return return estimated cardinality result.
     */
    int query(const std::vector<CompareExpression>& quals);
    
    /**
     * Preprocessing function of the cardinality estimation algorithm. This function is executed before each operation
     * is called.
     */
    void prepare();
    
    /**
     * The constructor function of cardinality estimation.
     * @param num Size of the initial data set.
     * @param dataExecuter Interfaces for datasets.
     */
    CEEngine(int num, DataExecuter *dataExecuter);
    ~CEEngine() = default;

private:
    DataExecuter *dataExecuter;

    int sample_size;
    int total_size;
    int num_processed;

    std::vector<std::pair<int, int>> table;
    std::vector<std::pair<int, int>> reservoir;
};

#endif