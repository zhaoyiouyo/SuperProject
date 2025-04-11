#include "TapeDrivesQueue.hpp"

TapeDrivesQueue& TapeDrivesQueue::getInstance() {
    static TapeDrivesQueue instance;
    return instance;
}