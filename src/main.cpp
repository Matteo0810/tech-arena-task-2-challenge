#include <CardinalityEstimation.h>
#include <executer/DataExecuterDemo.h>

void solve(const int initSize, const int opSize)
{
    std::cout << "Execution with " << initSize << " data size and " << opSize << " operations" << std::endl;
    
    auto start = std::chrono::high_resolution_clock::now();
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

    std::cout << "Error marge: " << score / cnt << "%" << std::endl;
    
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Execution time : " << duration.count() << " seconds" << std::endl;
}


int main(int argc, char *argv[])
{
    // Initial data size (max: 30'000'000)
    // Number of operations (max operations: 20'000'000)

    // case 1
    solve(100'000, 10'000);

    // case 2
    solve(1'000'000, 100'000);

    // case 3
    solve(10'000'000, 1'000'000);
    
    return EXIT_SUCCESS;
}