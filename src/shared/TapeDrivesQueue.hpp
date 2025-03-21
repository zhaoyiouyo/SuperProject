#pragma once

#include "BaseQueue.hpp"
#include "TapeDrivesOperation.hpp"

class TapeDrivesQueue : public BaseQueue<TapeDrivesOperation> {
public:
    static TapeDrivesQueue& getInstance();

    void specialOperation() override;

private:
    TapeDrivesQueue(const TapeDrivesQueue&) = delete;
    TapeDrivesQueue& operator=(const TapeDrivesQueue&) = delete;

    TapeDrivesQueue() = default;
};