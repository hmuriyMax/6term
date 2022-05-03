#include "tokenizer.h"

#include <string>
#include <unordered_map>
#include <algorithm>

using namespace std;

const unordered_map<string, eToken> ReservedIDs = {
    {"program", eToken::tProgram},
    {"var", eToken::tVar},
    {"const", eToken::tConst},
    {"integer", eToken::tInteger},
    {"boolean", eToken::tBoolean},
    {"begin", eToken::tBegin},
    {"end", eToken::tEnd},
    {"if", eToken::tIf},
    {"then", eToken::tThen},
    {"for", eToken::tFor},
    {"do", eToken::tDo},
    {"to", eToken::tTo},
    {"downto", eToken::tDownto},
    {"read", eToken::tRead},
    {"write", eToken::tWrite},
    {"or", eToken::tOr},
    {"and", eToken::tAnd},
    {"xor", eToken::tXor},
    {"not", eToken::tNot},
    {"true", eToken::tTrue},
    {"false", eToken::tFalse}
};


bool IsIDChar(char ch)
{
    return isalpha(ch) || isdigit(ch) || ch == '_' || ch == '-';
}

bool IsIndentChar(char ch)
{
    return ch == ' ' || ch == '\t' || ch == '\n';
}

std::vector<eToken> Tokenize(std::istream& in)
{
    vector<eToken> result;

    for (char ch; in.get(ch); )
    {
        if (IsIndentChar(ch)) continue;

        else if (isalpha(ch) || ch == '_')
        {
            string id;
            while (IsIDChar(ch)) {
                id += ch;
                if (!in.get(ch)) break;
            }
            if (in) {
                in.putback(ch);
            }

            for_each(id.begin(), id.end(), [](char& c) {c = tolower(c); });

            if (ReservedIDs.count(id)) result.push_back(ReservedIDs.at(id));
            else result.push_back(eToken::tID);
        }

        else if (isdigit(ch))
        {
            in.putback(ch);
            double num;
            in >> num;
            result.push_back(eToken::tNum);
        }

        else if (ch == '-') result.push_back(eToken::tMinus);
        else if (ch == '+') result.push_back(eToken::tPlus);
        else if (ch == '*') result.push_back(eToken::tMul);
        else if (ch == '/') result.push_back(eToken::tDiv);
        else if (ch == '=') result.push_back(eToken::tEq);

        else if (ch == '<')
        {
            char ch2 = in.get();
            if (in) {
                if (ch2 == '>') result.push_back(eToken::tNoteq);
                else if (ch2 == '=') result.push_back(eToken::tLeq);
                else {
                    in.putback(ch2);
                    result.push_back(eToken::tLess);
                }
            }
            else {
                result.push_back(eToken::tLess);
            }
        }

        else if (ch == '>')
        {
            char ch2 = in.get();
            if (in) {
                if (ch2 == '=') result.push_back(eToken::tGeq);
                else {
                    in.putback(ch2);
                    result.push_back(eToken::tGreater);
                }
            }
            else {
                result.push_back(eToken::tGreater);
            }
        }

        else if (ch == ':')
        {
            char ch2 = in.get();
            if (in) {
                if (ch2 == '=') result.push_back(eToken::tAssign);
                else {
                    in.putback(ch2);
                    result.push_back(eToken::tColon);
                }
            }
            else {
                result.push_back(eToken::tColon);
            }
        }

        else if (ch == ';') result.push_back(eToken::tSemicolon);
        else if (ch == '.') result.push_back(eToken::tDot);
        else if (ch == ',') result.push_back(eToken::tComma);
        else if (ch == '(') result.push_back(eToken::tLP);
        else if (ch == ')') result.push_back(eToken::tRP);

        else throw exception("Tokenizing error: unknown symbol found.");
    }

    return result;
}