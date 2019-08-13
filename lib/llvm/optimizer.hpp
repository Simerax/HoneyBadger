#pragma once
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/Scalar.h" // all the llvm-provided create...Pass()

namespace HoneyBadger
{
using namespace llvm;
class Optimizer : public legacy::PassManager
{
public:
    Optimizer() : legacy::PassManager()
    {
        this->add(createTailCallEliminationPass()); // optimize tail calls to loops
        this->add(createReassociatePass());
        this->add(createCFGSimplificationPass());
    }
};

} // namespace HoneyBadger