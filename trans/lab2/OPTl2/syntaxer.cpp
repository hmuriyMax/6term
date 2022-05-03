#include "syntaxer.h"
#include "tokenizer.h"

#include <iostream>
#include <algorithm>
#include <sstream>
#include <vector>
#include <stack>

using namespace std;

bool IsProgramCorrect(const std::string& progtext)
{
    std::stringstream stream(progtext);
    try {
        auto tokens = Tokenize(stream);
        reverse(tokens.begin(), tokens.end());
        stack<eToken, vector<eToken>> tokens_stack( move(tokens) );

        SyntaxAnalizer syntaxer( move(tokens_stack) );
        return syntaxer.AnalizeProgram();
    }
    catch (const exception& e) {
        cerr << e.what();
        return false;
    }
    return true;
}



SyntaxAnalizer::SyntaxAnalizer(std::stack<eToken, vector<eToken>> tokens) : m_tokens( move(tokens) )
{
}

bool SyntaxAnalizer::AnalizeProgram()
{
    if (m_tokens.empty()) return false;

    if (!CheckProgname())
        return false;

    if (!CheckDeclaration())
        return false;

    if (!CheckBlock())
        return false;

    if (! (m_tokens.size() == 1 && m_tokens.top() == eToken::tDot) )
        return false;

    return true;
}

bool SyntaxAnalizer::CheckProgname()
{
    if (m_tokens.empty()) return true;
    if (m_tokens.top() != eToken::tProgram) return true;

    m_tokens.pop();
    if (m_tokens.empty()) return false;
    if (m_tokens.top() != eToken::tID) return false;

    m_tokens.pop();
    if (m_tokens.empty()) return false;
    if (m_tokens.top() != eToken::tSemicolon) return false;

    m_tokens.pop();
    return true;
}

bool SyntaxAnalizer::CheckDeclaration()
{
    if (m_tokens.empty()) return true;
    if (m_tokens.top() != eToken::tVar && m_tokens.top() != eToken::tConst) return true;

    if (m_tokens.top() == eToken::tVar) {
        m_tokens.pop();
        if (!CheckVarsAndTypes()) return false;
    }
    else if (m_tokens.top() == eToken::tConst) {
        m_tokens.pop();
        if (!CheckConstAssignments()) return false;
    }

    if (!m_tokens.empty() && (m_tokens.top() == eToken::tVar || m_tokens.top() == eToken::tConst))
        if (!CheckDeclaration())
            return false;

    return true;
}

bool SyntaxAnalizer::CheckVarsAndTypes()
{
    if (m_tokens.empty()) return false;
    if (!CheckVarList()) return false;

    if (m_tokens.top() != eToken::tColon) return false;
    m_tokens.pop();
    if (m_tokens.empty()) return false;

    if (m_tokens.top() != eToken::tInteger && m_tokens.top() != eToken::tBoolean) return false;
    m_tokens.pop();
    if (m_tokens.empty()) return false;

    if (m_tokens.top() != eToken::tSemicolon) return false;
    m_tokens.pop();

    if (!m_tokens.empty() && m_tokens.top() == eToken::tID)
        if (!CheckVarsAndTypes())
            return false;

    return true;
}

bool SyntaxAnalizer::CheckVarList()
{
    if(m_tokens.empty()) return false;
    if (m_tokens.top() != eToken::tID) return false;
    m_tokens.pop();

    if (!m_tokens.empty() && m_tokens.top() == eToken::tComma) {
        m_tokens.pop();
        if (!CheckVarList())
            return false;
    }

    return true;
}

bool SyntaxAnalizer::CheckConstAssignments()
{
    if (m_tokens.empty()) return false;
    if (m_tokens.top() != eToken::tID) return false;
    m_tokens.pop();
    if (m_tokens.empty()) return false;

    if (m_tokens.top() != eToken::tEq) return false;
    m_tokens.pop();
    if (m_tokens.empty()) return false;

    if (!CheckExpression()) return false;

    if (m_tokens.top() != eToken::tSemicolon) return false;
    m_tokens.pop();

    if (!m_tokens.empty() && m_tokens.top() == eToken::tID)
        if (!CheckConstAssignments())
            return false;

    return true;
}

bool SyntaxAnalizer::CheckBlock()
{
    if (m_tokens.size() < 2) return false;
    if (m_tokens.top() != eToken::tBegin) return false;
    m_tokens.pop();

    while (m_tokens.top() != eToken::tEnd) {
        if (!CheckOperator()) return false;
        if (m_tokens.empty()) return false;
    }

    m_tokens.pop();
    return true;
}

bool SyntaxAnalizer::CheckOperator()
{
    if (m_tokens.size() < 2) return false;
    eToken top = m_tokens.top();
    m_tokens.pop();

    if (top == eToken::tID) {
        if (m_tokens.top() != eToken::tAssign) return false;
        m_tokens.pop();
        if (m_tokens.empty() || !CheckExpression()) return false;

        if (m_tokens.top() != eToken::tSemicolon) return false;
        m_tokens.pop();
    }
    else if (top == eToken::tIf) {
        if (!CheckExpression()) return false;
        if (m_tokens.size() < 2) return false;
        if (m_tokens.top() != eToken::tThen) return false;
        m_tokens.pop();
        if (m_tokens.top() == eToken::tBegin) {
            if (!CheckBlock())
                return false;
            if (m_tokens.empty()) return false;
            if (m_tokens.top() != eToken::tSemicolon) return false;
            m_tokens.pop();
        }
        else if (!CheckOperator()) return false;

    }
    else if (top == eToken::tFor) {
        if (m_tokens.size() < 3) return false;
        if (m_tokens.top() != eToken::tID) return false;
        m_tokens.pop();

        if (m_tokens.top() != eToken::tAssign) return false;
        m_tokens.pop();
        if (!CheckExpression()) return false;
        if (m_tokens.empty()) return false;

        if (m_tokens.top() != eToken::tTo && m_tokens.top() != eToken::tDownto) return false;
        m_tokens.pop();
        if (m_tokens.empty()) return false;

        if (!CheckExpression()) return false;
        if (m_tokens.size() < 2) return false;

        if (m_tokens.top() != eToken::tDo) return false;
        m_tokens.pop();
        if (m_tokens.top() == eToken::tBegin) {
            if (!CheckBlock())
                return false;
            if (m_tokens.empty()) return false;
            if (m_tokens.top() != eToken::tSemicolon) return false;
            m_tokens.pop();
        }
        else if (!CheckOperator()) return false;
    }
    else if (top == eToken::tRead) {
        if (m_tokens.top() != eToken::tLP) return false;
        m_tokens.pop();
        if (m_tokens.empty()) return false;

        if (m_tokens.top() != eToken::tRP)
            if (!CheckVarList())
                return false;

        if (m_tokens.size() < 2) return false;
        if (m_tokens.top() != eToken::tRP) return false;
        m_tokens.pop();
        if (m_tokens.top() != eToken::tSemicolon) return false;
        m_tokens.pop();
    }
    else if (top == eToken::tWrite) {
        if (m_tokens.top() != eToken::tLP) return false;
        m_tokens.pop();
        if (m_tokens.empty()) return false;

        if (m_tokens.top() != eToken::tRP)
            if (!CheckExpressions())
                return false;

        if (m_tokens.size() < 2) return false;
        if (m_tokens.top() != eToken::tRP) return false;
        m_tokens.pop();
        if (m_tokens.top() != eToken::tSemicolon) return false;
        m_tokens.pop();
    }
    else return false;

    return true;
}

bool IsBinaryOperator(eToken e) {
    for (const auto t : {
        eToken::tMinus, eToken::tPlus, eToken::tMul, eToken::tDiv,
        eToken::tOr, eToken::tAnd, eToken::tXor,
        eToken::tLess, eToken::tGreater, eToken::tEq, eToken::tNoteq,  eToken::tLeq, eToken::tGeq}) {
        if (e == t) return true;
    }
    return false;
}

bool IsUnaryOperator(eToken e) {
    return e == eToken::tMinus || e == eToken::tPlus || e == eToken::tNot;
}

bool SyntaxAnalizer::CheckExpression()
{
    if (m_tokens.empty()) return false;
    auto top = m_tokens.top();
    m_tokens.pop();

    if (top == eToken::tLP) {
        if (!CheckExpression()) return false;
        if (m_tokens.empty()) return false;
        if (m_tokens.top() != eToken::tRP) return false;
        m_tokens.pop();

        if (!m_tokens.empty() && IsBinaryOperator(m_tokens.top())) {
            m_tokens.pop();
            if (m_tokens.empty()) return false;
            if (!CheckExpression()) return false;
        }
    }
    else if (IsUnaryOperator(top)) {
        if (!CheckExpression()) return false;
    }
    else if (top == eToken::tTrue || top == eToken::tFalse || top == eToken::tID || top == eToken::tNum) {
        if (!m_tokens.empty() && IsBinaryOperator(m_tokens.top())) {
            m_tokens.pop();
            if (m_tokens.empty()) return false;
            if (!CheckExpression()) return false;
        }
    }
    else return false;

    return true;
}

bool SyntaxAnalizer::CheckExpressions()
{
    if (m_tokens.empty()) return false;
    if (!CheckExpression()) return false;
    if (!m_tokens.empty() && m_tokens.top() == eToken::tComma) {
        m_tokens.pop();
        if (!CheckExpressions()) return false;
    }
    return true;
}
