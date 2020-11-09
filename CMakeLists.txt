cmake_minimum_required(VERSION 3.17)
project(BSYSC C)

set(CMAKE_C_STANDARD 99)

add_executable(BSYSC main.c Chapter5/Question1/ForkVariable.c Chapter5/Question2/ForkOutput.c Chapter5/Question3/ForkWithoutWait.c Chapter5/Question4/ForkExec.c Chapter5/Question5/ForkWait.c Chapter5/Question6/ForkWaitPID.c Chapter5/Question7/ForkCloseSO.c Chapter5/Question8/pipe.c Chapter6/systemCall.c Chapter6/contextSwitch.c Chapter13/memory-user.c Chapter14/null.c Chapter14/mallocFree.c Chapter14/integerArray.c)