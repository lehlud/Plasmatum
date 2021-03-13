#!/bin/sh

cd src

flex --outfile=lex.yy.cc lexer.ll
bison parser.yy

#g++ -Wall -Wextra -lfl main.cc lex.yy.cc parser.tab.cc utils.cc -o ../gnu

clang++ -dynamic -Wall -Wextra -lfl -lLLVM \
    main.cc lex.yy.cc parser.tab.cc utils.cc \
    -I/usr/local/include -std=c++14   -fno-exceptions -fno-rtti -D_GNU_SOURCE -D_DEBUG -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS \
    -L/usr/local/lib \
    -lLLVMX86Disassembler -lLLVMX86AsmParser -lLLVMX86CodeGen -lLLVMCFGuard -lLLVMGlobalISel -lLLVMX86Desc -lLLVMX86Info -lLLVMMCDisassembler -lLLVMSelectionDAG -lLLVMAsmPrinter -lLLVMDebugInfoDWARF -lLLVMCodeGen -lLLVMOrcJIT -lLLVMPasses -lLLVMObjCARCOpts -lLLVMCoroutines -lLLVMipo -lLLVMInstrumentation -lLLVMVectorize -lLLVMScalarOpts -lLLVMLinker -lLLVMIRReader -lLLVMAsmParser -lLLVMInstCombine -lLLVMFrontendOpenMP -lLLVMBitWriter -lLLVMAggressiveInstCombine -lLLVMTransformUtils -lLLVMJITLink -lLLVMExecutionEngine -lLLVMTarget -lLLVMAnalysis -lLLVMProfileData -lLLVMRuntimeDyld -lLLVMOrcTargetProcess -lLLVMOrcShared -lLLVMObject -lLLVMTextAPI -lLLVMMCParser -lLLVMBitReader -lLLVMMC -lLLVMDebugInfoCodeView -lLLVMDebugInfoMSF -lLLVMCore -lLLVMRemarks -lLLVMBitstreamReader -lLLVMBinaryFormat -lLLVMSupport -lLLVMDemangle \
    -lrt -ldl -lpthread -lm -ltinfo \
    -O3 -s \
    -o ../plsm #\
    #`llvm-config --cxxflags --ldflags --system-libs --libs core orcjit native` \

#cd ..

#strip plsm -s -R .comment -R .gnu.version --strip-unneeded

