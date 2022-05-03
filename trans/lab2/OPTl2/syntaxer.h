#pragma once
#include "tokenizer.h"

#include <string>
#include <stack>
#include <vector>

bool IsProgramCorrect(const std::string& progtext);

class SyntaxAnalizer
{
public:
    SyntaxAnalizer(std::stack<eToken, std::vector<eToken>> tokens);

    bool AnalizeProgram();
private:
    std::stack<eToken, std::vector<eToken>> m_tokens;
private:
    bool CheckProgname();
    bool CheckDeclaration();
    bool CheckBlock();
    bool CheckVarsAndTypes();
    bool CheckVarList();
    bool CheckConstAssignments();
    bool CheckOperator();
    bool CheckExpression();
    bool CheckExpressions();
};