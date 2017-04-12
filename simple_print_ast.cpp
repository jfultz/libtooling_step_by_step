#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"

using namespace clang::tooling;


int main(int argc, const char **argv)
{
	llvm::cl::OptionCategory MyToolCategory("simple_print_ast options");
	CommonOptionsParser OptionsParser(argc, argv, MyToolCategory);

	// Use OptionsParser.getCompilations() and OptionsParser.getSourcePathList()
	// to retrieve CompilationDatabase and the list of input file paths.

	ClangTool Tool(OptionsParser.getCompilations(), OptionsParser.getSourcePathList());
	return Tool.run(newFrontendActionFactory<clang::ASTPrintAction>().get());
}

