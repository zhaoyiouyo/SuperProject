#include <iostream>
#include "shared/BaseQueue.hpp"
#include "shared/TapeDrivesQueue.hpp"
#include "TapeDrivesOperation.hpp"

void moduleA() {
    TapeDrivesQueue& queue1 = TapeDrivesQueue::getInstance();
    queue1.push_back(TapeDrivesOperation(TypeOperation::LOAD_TAPE));
    queue1.push_back(TapeDrivesOperation(TypeOperation::READ_AGGR));
    queue1.push_back(TapeDrivesOperation(TypeOperation::LOAD_TAPE));
    queue1.push_back(TapeDrivesOperation(TypeOperation::READ_AGGR));
    queue1.push_back(TapeDrivesOperation(TypeOperation::LOAD_TAPE));
    queue1.push_back(TapeDrivesOperation(TypeOperation::READ_AGGR));
    queue1.specialOperation();
    std::cout << "Module A: Size = " << queue1.size() << std::endl;
}

int main() {
    moduleA();

    TapeDrivesQueue& queue1 = TapeDrivesQueue::getInstance();
    for (auto it = queue1.begin(); it != queue1.end(); ++it) {
        std::cout << static_cast<int>(it.getNode()->data_.getTypeOperation()) << " ";
    }
    std::cout << std::endl;

    return 0;
}