#include <iostream>

#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"

class FunctionDeclASTVisitor : public clang::RecursiveASTVisitor<FunctionDeclASTVisitor>
{
	clang::SourceManager& sourceManager_;
	clang::Rewriter& rewriter_;

public:
	explicit FunctionDeclASTVisitor(clang::SourceManager& sm, clang::Rewriter& r)
		: sourceManager_(sm), rewriter_(r)
	{ }

	virtual bool VisitFunctionDecl(clang::FunctionDecl* func)
	{
		if (sourceManager_.isWrittenInMainFile(func->getSourceRange().getBegin()))
		{
			std::string oldFuncName = func->getNameInfo().getName().getAsString();
			std::string newFuncName = oldFuncName + "_x";
			std::cout << oldFuncName << " -> " << newFuncName << std::endl;
			rewriter_.ReplaceText(func->getNameInfo().getSourceRange(), newFuncName);
		}
		return true;
	}
};

class FunctionDeclASTConsumer : public clang::ASTConsumer
{
	clang::Rewriter rewriter_;
	FunctionDeclASTVisitor visitor_; // doesn't have to be private

public:
	// override the constructor in order to pass CI
	explicit FunctionDeclASTConsumer(clang::CompilerInstance& ci)
		: visitor_(ci.getSourceManager(), rewriter_)
	{ }
 
	virtual void HandleTranslationUnit(clang::ASTContext& astContext)
	{
		rewriter_.setSourceMgr(astContext.getSourceManager(), astContext.getLangOpts());
		visitor_.TraverseDecl(astContext.getTranslationUnitDecl());
		rewriter_.overwriteChangedFiles();
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
	int result = Tool.run(newFrontendActionFactory<FunctionDeclFrontendAction>().get());
}

