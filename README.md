# llvm-pass-skeleton (Modified by Griffin Berlstein for CS6120)

A ~~completely~~ mostly useless LLVM pass. Based on the tutorial by Adrian Sampson (@sampsyo).

Build:

    $ cd llvm-pass-skeleton
    $ mkdir build
    $ cd build
    $ cmake ..
    $ make
    $ cd ..

Run:

    $ clang -Xclang -load -Xclang build/skeleton/libSkeletonPass.* something.c helpers.c

## The pass

This modified pass inserts calls to simple functions that print out messages
when certain reducible operations are executed. The flagged operations are:
1. `a + 0` - Could be replaced by `a`
2. `a - 0` - Could be replaced by `a`
3. `a * 0` - Could be replaced by `0`
4. `a / 1` (int div) - Could be replaced by `a`
5. `a * 1` - Could be replaced by `a`
6. `a & 0` - Could be replaced by `0`
7. `a ^ 0` - Could be replaced by `a`
8. `a | -1` - Could be replaced by `-1`
9. `a ^ a` - Could be replaced by `0`
10. `a | a` - Could be replaced by `a`
11. `a & a` - Could be replaced by `a`

The pass doesn't remove any of these actions, it just places a call to the
printing function after the computation. This will in fact make the program
execute slower which we can pretend is a means of discouraging such behavior.
