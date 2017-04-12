#include <iostream>
#include <sstream>

#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Lex/Lexer.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Refactoring.h"
#include "clang/Tooling/RefactoringCallbacks.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"


using namespace clang::ast_matchers;
using namespace clang::tooling;

class IfStatementRewriter : public MatchFinder::MatchCallback
{
	clang::Rewriter rewriter_;

public :
	virtual void run(const MatchFinder::MatchResult &result)
	{
		clang::SourceManager* sourceManager = result.SourceManager;
		const clang::LangOptions& langOptions = result.Context->getLangOpts();
		rewriter_.setSourceMgr(*sourceManager, langOptions);
		const clang::Stmt* ifStmt = result.Nodes.getNodeAs<clang::Stmt>("ifStatement");
		const clang::Stmt* rhs = result.Nodes.getNodeAs<clang::Stmt>("rhs");
		const clang::Stmt* lhs = result.Nodes.getNodeAs<clang::Stmt>("lhs");

		if (ifStmt && rhs && lhs)
		{
			clang::CharSourceRange lhsRange = clang::CharSourceRange::getTokenRange(lhs->getSourceRange());
			clang::CharSourceRange rhsRange = clang::CharSourceRange::getTokenRange(rhs->getSourceRange());
			
			std::string lhsString = clang::Lexer::getSourceText(lhsRange, *sourceManager, langOptions).str();
			std::string rhsString = clang::Lexer::getSourceText(rhsRange, *sourceManager, langOptions).str();

			Replacements replacements;
			replacements.add(Replacement(*sourceManager, lhsRange, rhsString));
			replacements.add(Replacement(*sourceManager, rhsRange, lhsString));
			applyAllReplacements(replacements, rewriter_);
		}
	}
};



int main(int argc, const char **argv)
{
	llvm::cl::OptionCategory MyToolCategory("replace_for_pattern options");
	CommonOptionsParser OptionsParser(argc, argv, MyToolCategory);
	ClangTool Tool(OptionsParser.getCompilations(), OptionsParser.getSourcePathList());

	// Matches 'if (nonliteral == literal)'
	StatementMatcher ifStatementMatcher =
		ifStmt(binaryOperator(
			hasOperatorName("=="),
			hasLHS(expr().bind("lhs")),
			hasRHS(userDefinedLiteral().bind("rhs")))).bind("ifStatement");

	IfStatementRewriter rewriter;
	MatchFinder finder;
	finder.addMatcher(ifStatementMatcher, &rewriter);

	return Tool.run(newFrontendActionFactory(&finder).get());
}

