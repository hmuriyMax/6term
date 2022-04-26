#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <iostream>
#include <exception>
#include <queue>
#include <stdarg.h>
#define ctok current_token.tok

using std::vector;
using std::string;
using std::unordered_map;
using std::exception;
using std::queue;

enum class tmls {
	tProgram, tVar, tBegin, tEnd, tRead, tWrite, tIf, tThen, tElse, tRepeat, tUntil, tInt, tType, tRecord, tBool, tId, tNum, 
	tPlus, tMinus, tMult, tDiv, tOr, tAnd, tNot, tTrue, tFalse, tGeq, tEq, tLeq, tLss, tGrt, tAsgn, tDot, tCom, tColon, tSColon, 
	tLBr, tRBr
};

const unordered_map<string, tmls> TokenMatcher = {
	{"program", tmls::tProgram},
	{"var",		tmls::tVar},
	{"begin",	tmls::tBegin},
	{"end",		tmls::tEnd},
	{"read",	tmls::tRead},
	{"write",	tmls::tWrite},
	{"if",		tmls::tIf},
	{"then",	tmls::tThen},
	{"else",	tmls::tElse},
	{"repeat",	tmls::tRepeat},
	{"until",	tmls::tUntil},
	{"int",		tmls::tInt},
	{"type",	tmls::tType},
	{"record",	tmls::tRecord},
	{"boolean", tmls::tBool},
	{"or",		tmls::tOr},
	{"and",		tmls::tAnd},
	{"not",		tmls::tNot},
	{"true",	tmls::tTrue},
	{"false",	tmls::tFalse},
};

struct Token {
	tmls tok;
	int str_num;
};

bool operator==(Token lhs, tmls rhs) {
	return lhs.tok == rhs;
}

class Pas_synt
{
private:
	bool	tokenised = false;
	Token	current_token;
	queue	<Token> tokens;
	vector	<tmls> expect;
	vector	<tmls> stack;
	

	bool IsId(char ch) {
		return isalpha(ch) || isdigit(ch) || ch == '_';
	}

	bool IsDelmtr(char ch) {
		return ch == ' ' || ch == '\n';
	}

	void GetTokens(std::istream& in) {
		queue<Token> res;
		string tmp_token = "";
		bool reading_token = false, reading_num = true;
		int str_num = 1;

		for (char ch; in.get(ch); ) {
			if (IsId(ch)) {
				reading_num = reading_num && isdigit(ch);
				tmp_token += tolower(ch);
				continue;
			}

			else if (reading_token) {
				res.push({ TokenMatcher.count(tmp_token) ? TokenMatcher.at(tmp_token) :
					reading_num ? tmls::tNum : tmls::tId, str_num });
				reading_token = false;
				reading_num = true;
			}

			if (ch == '\n')
				str_num++;
			if (IsDelmtr(ch)) continue;
			else if (ch == '-') res.push({tmls::tMinus, str_num});
			else if (ch == '+') res.push({tmls::tPlus, str_num});
			else if (ch == '*') res.push({tmls::tMult, str_num});
			else if (ch == '/') res.push({tmls::tDiv, str_num});
			else if (ch == '=') res.push({tmls::tEq, str_num});
			else if (ch == '<')
			{
				char ch2 = in.get();
				if (in) {
					if (ch2 == '=') res.push({tmls::tLeq, str_num});
					else {
						in.putback(ch2);
						res.push({tmls::tLss, str_num});
					}
				}
				else {
					res.push({tmls::tLss, str_num});
				}
			}
			else if (ch == '>')
			{
				char ch2 = in.get();
				if (in) {
					if (ch2 == '=') res.push({tmls::tGeq, str_num});
					else {
						in.putback(ch2);
						res.push({tmls::tGrt, str_num});
					}
				}
				else {
					res.push({tmls::tGrt, str_num});
				}
			}
			else if (ch == ':')
			{
				char ch2 = in.get();
				if (in) {
					if (ch2 == '=') res.push({tmls::tAsgn, str_num});
					else {
						in.putback(ch2);
						res.push({tmls::tColon, str_num});
					}
				}
				else {
					res.push({tmls::tColon, str_num });
				}
			}
			else if (ch == ';') res.push({tmls::tSColon, str_num});
			else if (ch == '.') res.push({tmls::tDot, str_num});
			else if (ch == ',') res.push({tmls::tCom, str_num});
			else if (ch == '(') res.push({tmls::tLBr, str_num});
			else if (ch == ')') res.push({tmls::tRBr, str_num});
			else throw exception("Token error: unknown symbol");
		}
		tokenised = true;
		tokens = res;
	}

	void SyntaxChecker() {
		if (!tokenised)
			throw exception("Structure error: Tried to check syntax of untokenised code");
		program();
	}

	template <typename T>
	bool in_vector(const vector<T>& vec, const T& what) {
		for (T el : vec)
			if (el == what)
				return true;
		return false;
	}

	void PopToken() {
		if (tokens.empty())
			throw exception("Syntax error: unexpected program end");
		current_token = tokens.front();
		tokens.pop();
		CheckToken();
	}

	void CheckToken() {
		if (!in_vector(expect, current_token.tok)) {
			const char* err = new char[50];
			err = "Syntax error: unexpected token on line " + current_token.str_num;
			throw exception(err);
		}
	}

	void NextToken(tmls expectations, ...) {
		expect = {};
		va_list pnt;
		va_start(pnt, expectations);
		for (tmls* i = &expectations; i; i++) {
			expect.push_back(*i);
		}
		PopToken();
	}

	void NextToken(const vector<tmls>& expectations) {
		expect = expectations;
		PopToken();
	}

	void program() {
		NextToken(tmls::tProgram);
		NextToken(tmls::tId);
		NextToken(tmls::tType, tmls::tVar, tmls::tBegin);
		switch (ctok) {
			case(tmls::tType):
				NextToken(tmls::tId);
				type_decl();
				break;
			case (tmls::tVar):
				NextToken(tmls::tId);
				var_decl();
				break;
			case (tmls::tBegin):
				command();
				break;
			default:
				throw exception("Something strange happened...");
		}
	}

	void type_decl(const vector<tmls>& finish) {
		vector<tmls> fcopy = *new vector<tmls>(finish);
		fcopy.push_back(tmls::tId);

		NextToken(tmls::tEq);
		var_type();
		NextToken(tmls::tSColon);
		NextToken(tmls::tId, tmls::tVar, tmls::tBegin);
		if (ctok == tmls::tId)
				type_decl();
				break;
			case (tmls::tVar):
				var_decl({tmls::tBegin});
			case (tmls::tBegin):
				command();
				break;
			default:
				throw exception("Something strange happened...");
		}
	}

	void var_decl(const vector<tmls>& finish) {
		vector<tmls> fcopy = *new vector<tmls>(finish);
		fcopy.push_back(tmls::tId);

		NextToken(tmls::tColon);
		var_type();
		NextToken(tmls::tColon);
		NextToken(fcopy);
		if (ctok == tmls::tId)
			var_decl(finish);
	}
	void var_type();
	void command();
	void block();
	void id_list();
	void assignment();
	void expr();
	void if_block();
	void read();
	void write();
	void repeat_block();
	void expr_list();

public:
	Pas_synt();
};
