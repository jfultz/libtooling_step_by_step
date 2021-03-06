#include <iostream>

#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"

class FunctionDeclASTVisitor : public clang::RecursiveASTVisitor<FunctionDeclASTVisitor>
{
	clang::SourceManager& sourceManager_;

public:
	explicit FunctionDeclASTVisitor(clang::SourceManager& sm)
		: sourceManager_(sm)
	{ }

	virtual bool VisitFunctionDecl(clang::FunctionDecl* func)
	{
		if (sourceManager_.isWrittenInMainFile(func->getSourceRange().getBegin()))
			std::cout << func->getNameInfo().getName().getAsString() << std::endl;
		return true;
	}
};

class FunctionDeclASTConsumer : public clang::ASTConsumer
{
	FunctionDeclASTVisitor visitor_; // doesn't have to be private

public:
	// override the constructor in order to pass CI
	explicit FunctionDeclASTConsumer(clang::CompilerInstance& ci)
		: visitor_(ci.getSourceManager())
	{ }
 
	virtual void HandleTranslationUnit(clang::ASTContext& astContext)
	{
		visitor_.TraverseDecl(astContext.getTranslationUnitDecl());
	}
};

class FunctionDeclFrontendAction : public clang::ASTFrontendAction
{
public:
    virtual std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance& CI, clang::StringRef file) {
        return std::make_unique<FunctionDeclASTConsumer>(CI); // pass CI pointer to ASTConsumer
    }
};


using namespace clang::tooling;

int main(int argc, const char **argv)
{
	llvm::cl::OptionCategory MyToolCategory("print_function_decls options");
	CommonOptionsParser OptionsParser(argc, argv, MyToolCategory);

	// Use OptionsParser.getCompilations() and OptionsParser.getSourcePathList()
	// to retrieve CompilationDatabase and the list of input file paths.

	ClangTool Tool(OptionsParser.getCompilations(), OptionsParser.getSourcePathList());
	return Tool.run(newFrontendActionFactory<FunctionDeclFrontendAction>().get());
}

