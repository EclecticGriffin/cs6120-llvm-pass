#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
using namespace llvm;



namespace {
  struct SkeletonPass : public FunctionPass {
    static char ID;
    SkeletonPass() : FunctionPass(ID) {}


    virtual bool runOnFunction(Function &F) {
      LLVMContext& Ctx = F.getContext();

      FunctionCallee mulZero = F.getParent()->getOrInsertFunction(
        "mul_by_zero", Type::getVoidTy(Ctx), Type::getVoidTy(Ctx)
      );

      FunctionCallee mulOne = F.getParent()->getOrInsertFunction(
        "mul_by_one", Type::getVoidTy(Ctx), Type::getVoidTy(Ctx)
      );

      FunctionCallee divOne = F.getParent()->getOrInsertFunction(
        "div_by_one", Type::getVoidTy(Ctx), Type::getVoidTy(Ctx)
      );

      FunctionCallee addZero = F.getParent()->getOrInsertFunction(
        "add_zero", Type::getVoidTy(Ctx), Type::getVoidTy(Ctx)
      );

      FunctionCallee andZero = F.getParent()->getOrInsertFunction(
        "and_zero", Type::getVoidTy(Ctx), Type::getVoidTy(Ctx)
      );

      FunctionCallee orNegOne = F.getParent()->getOrInsertFunction(
        "or_neg_one", Type::getVoidTy(Ctx), Type::getVoidTy(Ctx)
      );

      FunctionCallee xorZero = F.getParent()->getOrInsertFunction(
        "xor_zero", Type::getVoidTy(Ctx), Type::getVoidTy(Ctx)
      );

      FunctionCallee subZero = F.getParent()->getOrInsertFunction(
        "sub_zero", Type::getVoidTy(Ctx), Type::getVoidTy(Ctx)
      );

      FunctionCallee xorSelf = F.getParent()->getOrInsertFunction(
        "xor_self", Type::getVoidTy(Ctx), Type::getVoidTy(Ctx)
      );

      FunctionCallee andSelf = F.getParent()->getOrInsertFunction(
        "and_self", Type::getVoidTy(Ctx), Type::getVoidTy(Ctx)
      );

      FunctionCallee orSelf = F.getParent()->getOrInsertFunction(
        "or_self", Type::getVoidTy(Ctx), Type::getVoidTy(Ctx)
      );

      for (auto& B:F) {
        for (auto& I:B) {
          if (auto* op = dyn_cast<BinaryOperator>(&I)) {
            // errs() << op->getOpcode() << "\n";
            IRBuilder<> builder(op);
            builder.SetInsertPoint(&B, ++builder.GetInsertPoint());
            Value* args = {};

            Value* lhs = op->getOperand(0);
            Value* rhs = op->getOperand(1);


            switch (op->getOpcode()) {
              case BinaryOperator::Add:
                errs() << "ADD!\n";

                if (auto* val = (dyn_cast<Constant>(lhs))) {
                  if (val->isZeroValue()) {
                    builder.CreateCall(addZero);
                  }
                } else if (auto* val = dyn_cast<Constant>(rhs)) {
                  if (val->isZeroValue()) {
                    builder.CreateCall(addZero);
                  }
                }
                break;

              case BinaryOperator::Sub:
                errs() << "SUB!\n";

                if (auto* val = dyn_cast<Constant>(rhs)) {
                  if (val->isZeroValue()) {
                    builder.CreateCall(subZero);
                  }
                }
                break;

              case BinaryOperator::Mul:
                errs() << "MUL!\n";

                if (auto* val = (dyn_cast<Constant>(lhs))) {
                  if (val->isZeroValue()) {
                    builder.CreateCall(mulZero);
                  } else if (val->isOneValue()) {
                    builder.CreateCall(mulOne);
                  }
                } else if (auto* val = dyn_cast<Constant>(rhs)) {
                  if (val->isZeroValue()) {
                    builder.CreateCall(mulZero);
                  } else if (val->isOneValue()) {
                    builder.CreateCall(mulOne);
                  }
                }
                break;

              case BinaryOperator::UDiv:
              case BinaryOperator::SDiv:
                errs() << "SDIV!\n";
                if (auto* val = dyn_cast<Constant>(rhs)) {
                    if (val->isOneValue()) {
                      builder.CreateCall(divOne);
                    }
                  }
                break;

              case BinaryOperator::Or:
                errs() << "OR!\n";

                if (auto* val = (dyn_cast<Constant>(lhs))) {
                  if (val->isAllOnesValue()) {
                    builder.CreateCall(orNegOne);
                  }
                } else if (auto* val = dyn_cast<Constant>(rhs)) {
                  if (val->isAllOnesValue()) {
                    builder.CreateCall(orNegOne);
                  }
                } else {
                  auto* val_lhs = dyn_cast<LoadInst>(lhs);
                  auto* val_rhs = dyn_cast<LoadInst>(rhs);

                  if (val_lhs && val_rhs) {
                    if (val_lhs->getPointerOperand() == val_rhs->getPointerOperand()) {
                      builder.CreateCall(orSelf);
                    }
                  }
                }

                break;

              case BinaryOperator::And:
                errs() << "AND!\n";

                if (auto* val = (dyn_cast<Constant>(lhs))) {
                  if (val->isZeroValue()) {
                    builder.CreateCall(andZero);
                  }
                } else if (auto* val = dyn_cast<Constant>(rhs)) {
                  if (val->isZeroValue()) {
                    builder.CreateCall(andZero);
                  }
                } else {
                  auto* val_lhs = dyn_cast<LoadInst>(lhs);
                  auto* val_rhs = dyn_cast<LoadInst>(rhs);

                  if (val_lhs && val_rhs) {
                    if (val_lhs->getPointerOperand() == val_rhs->getPointerOperand()) {
                      builder.CreateCall(andSelf);
                    }
                  }

                }

                break;

              case BinaryOperator::Xor:
                errs() << "XOR!\n";

                if (auto* val = (dyn_cast<Constant>(lhs))) {
                  if (val->isZeroValue()) {
                    builder.CreateCall(xorZero);
                  }
                } else if (auto* val = dyn_cast<Constant>(rhs)) {
                  if (val->isZeroValue()) {
                    builder.CreateCall(xorZero);
                  }
                } else {
                  auto* val_lhs = dyn_cast<LoadInst>(lhs);
                  auto* val_rhs = dyn_cast<LoadInst>(rhs);

                  if (val_lhs && val_rhs) {
                    if (val_lhs->getPointerOperand() == val_rhs->getPointerOperand()) {
                      builder.CreateCall(xorSelf);
                    }
                  }
                }
                break;

            }

          }
        }
      }
      return true;
    }
  };
}

char SkeletonPass::ID = 0;

// Automatically enable the pass.
// http://adriansampson.net/blog/clangpass.html
static void registerSkeletonPass(const PassManagerBuilder &,
                         legacy::PassManagerBase &PM) {
  PM.add(new SkeletonPass());
}
static RegisterStandardPasses
  RegisterMyPass(PassManagerBuilder::EP_EarlyAsPossible,
                 registerSkeletonPass);
