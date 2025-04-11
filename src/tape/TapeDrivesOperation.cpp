#include "TapeDrivesOperation.hpp"

TapeDrivesOperation::TapeDrivesOperation(TypeOperation op)
: op_(op)
{

}

TypeOperation TapeDrivesOperation::getTypeOperation()
{
    return op_;
}

void TapeDrivesOperation::setTypeOperation(TypeOperation op)
{
    op_ = op;
}