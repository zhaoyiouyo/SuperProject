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
    queue1.push_front(TapeDrivesOperation(TypeOperation::READ_AGGR));
    auto ele1 = queue1.pop_back();
    std::cout << "Module A: pop_back " << static_cast<int>(ele1.getTypeOperation()) << std::endl;
    auto ele2 = queue1.pop_front();
    std::cout << "Module A: pop_front " << static_cast<int>(ele2.getTypeOperation()) << std::endl;

    std::cout << "Module A: Size = " << queue1.size() << std::endl;
}

int main() {
    moduleA();

    TapeDrivesQueue& queue1 = TapeDrivesQueue::getInstance();
    for (auto it = queue1.begin(); it != queue1.end(); ++it) {
        std::cout << static_cast<int>(it->getTypeOperation()) << " ";
    }
    std::cout << std::endl;

    auto front = queue1.front();
    auto back = queue1.back();
    std::cout << "Module A: front " << static_cast<int>(front.getTypeOperation()) << std::endl;
    std::cout << "Module A: back " << static_cast<int>(back.getTypeOperation()) << std::endl;

    return 0;
}