#pragma once
#include <istream>
#include <vector>
#include <string>

enum class eToken {
    tProgram, tVar, tConst, tInteger, tBoolean, tBegin, tEnd, tIf, tThen, tFor, tDo, tTo, tDownto, tRead, tWrite, tOr, tAnd, tXor, tNot, tTrue, tFalse,
    tID, tNum,
    tMinus, tPlus, tMul, tDiv, tEq, tNoteq, tLess, tGreater, tLeq, tGeq, tAssign, tSemicolon, tColon, tDot, tComma, tLP, tRP
};

// throws std::exception in case of error
std::vector<eToken> Tokenize(std::istream& in);