#include "TapeDrivesQueue.hpp"

TapeDrivesQueue& TapeDrivesQueue::getInstance() {
    static TapeDrivesQueue instance;
    return instance;
}

void TapeDrivesQueue::specialOperation() {
    std::cout << "TapeDrivesQueue: Performing special operation." << std::endl;
}