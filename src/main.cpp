#include <CardinalityEstimation.h>
#include <executer/DataExecuterDemo.h>

// TODO: put main as he was before
int main(int argc, char *argv[])
{
    auto start = std::chrono::high_resolution_clock::now();
    // 100'000
    // 1'0000
    int initSize = 100'000; // Initial data size (max: 20'000'000)
    int opSize = 100'000; // Number of operations (max operations: 50'000'000)
    double score = 0;
    int cnt = 0;
    
    DataExecuterDemo dataExecuter(initSize - 1, opSize);
    CEEngine ceEngine(initSize, &dataExecuter);
    Action action = dataExecuter.getNextAction();

    while (action.actionType != NONE) {
        ceEngine.prepare();
        if (action.actionType == INSERT) {
            ceEngine.insertTuple(action.actionTuple);
        } else if (action.actionType == DELETE) {
            ceEngine.deleteTuple(action.actionTuple, action.tupleId);
        } else if (action.actionType == QUERY) {
            int ans = ceEngine.query(action.quals);
            score += dataExecuter.answer(ans);
            cnt++;
        }
        action = dataExecuter.getNextAction();
    }

    std::cout << score / cnt << std::endl;

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Execution time : " << duration.count() << " seconds" << std::endl;

    return EXIT_SUCCESS;
}