#include "dg/analysis/PointsTo/PointerAnalysisFI.h"
#include "dg/llvm/analysis/PointsTo/PointerAnalysis.h"

#include "../lib/llvm/analysis/ControlDependence/GraphBuilder.h"
#include "../lib/llvm/analysis/ControlDependence/NonTerminationSensitiveControlDependencyAnalysis.h"

#include <llvm/IR/Module.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/IRReader/IRReader.h>

#include <fstream>

int main(int argc, const char *argv[]) {
    using namespace std;
    using namespace llvm;


    cl::opt<string> OutputFilename("o",
                                   cl::desc("Specify output filename"),
                                   cl::value_desc("filename"),
                                   cl::init(""));

    cl::opt<std::string> inputFile(cl::Positional,
                                   cl::Required,
                                   cl::desc("<input file>"),
                                   cl::init(""));
    llvm::cl::opt<bool> threads("threads",
                                llvm::cl::desc("Consider threads are in input file (default=false)."),
                                llvm::cl::init(false));
    llvm::LLVMContext context;
    llvm::SMDiagnostic SMD;

    cl::ParseCommandLineOptions(argc, argv);

    string module = inputFile;
    string graphVizFileName = OutputFilename;


    std::unique_ptr<Module> M = llvm::parseIRFile(module.c_str(), SMD, context);

    if (!M) {
        llvm::errs() << "Failed parsing '" << module << "' file:\n";
        SMD.print(argv[0], errs());
        return 1;
    }

    dg::LLVMPointerAnalysis pointsToAnalysis(M.get(), "main", dg::analysis::Offset::UNKNOWN, threads);
    pointsToAnalysis.run<dg::analysis::pta::PointerAnalysisFI>();

    dg::cd::NonTerminationSensitiveControlDependencyAnalysis controlDependencyAnalysis(M.get()->getFunction("main"), &pointsToAnalysis);
    controlDependencyAnalysis.computeDependencies();

    if (graphVizFileName == "") {
        controlDependencyAnalysis.dump(std::cout);
    } else {
        std::ofstream graphvizFile(graphVizFileName);
        controlDependencyAnalysis.dump(graphvizFile);
    }
    return 0;
}
