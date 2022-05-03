#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <iostream>
#include <exception>
#include <cctype> 
#include <queue>
#include <stdarg.h>
#define ctok current_token.tok

using std::vector;
using std::string;
using std::unordered_map;
using std::exception;
using std::queue;

enum class trm {
	tProgram, tVar, tBegin, tEnd, tRead, tWrite, tIf, tThen, tElse, tRepeat, tUntil, tInt, tType, tRecord, tBool, tId, tNum, 
	tPlus, tMinus, tMult, tDiv, tOr, tAnd, tNot, tTrue, tFalse, tGeq, tEq, tLeq, tLss, tGrt, tAsgn, tDot, tCom, tColon, tSColon, 
	tLBr, tRBr
};

const unordered_map<string, trm> TokenMatcher = {
	{"program", trm::tProgram},
	{"var",		trm::tVar},
	{"begin",	trm::tBegin},
	{"end",		trm::tEnd},
	{"read",	trm::tRead},
	{"write",	trm::tWrite},
	{"if",		trm::tIf},
	{"then",	trm::tThen},
	{"else",	trm::tElse},
	{"repeat",	trm::tRepeat},
	{"until",	trm::tUntil},
	{"int",		trm::tInt},
	{"type",	trm::tType},
	{"record",	trm::tRecord},
	{"bool",	trm::tBool},
	{"or",		trm::tOr},
	{"and",		trm::tAnd},
	{"not",		trm::tNot},
	{"true",	trm::tTrue},
	{"false",	trm::tFalse},
};

struct Token {
	trm tok = trm::tProgram;
	int str_num;
	string code_part = "";
};

bool operator==(Token lhs, trm rhs) {
	return lhs.tok == rhs;
}

class Pas_synt
{
private:
	bool	tokenised = false;
	Token	current_token;
	queue	<Token> tokens;
	vector	<trm> expect;
	vector	<trm> stack;
	

	bool IsId(char ch) {
		return isalpha(ch) || isdigit(ch) || ch == '_';
	}

	bool IsDelmtr(char ch) {
		return ch == ' ' || ch == '\n' || ch == '\t';
	}

	void GetTokens(std::istream& in) {
		queue<Token> res;
		string tmp_token = "";
		bool reading_token = false, reading_num = true;
		int str_num = 1;

		for (char ch; in.get(ch); ) {
			if (reading_token && (!IsId(ch) || reading_num && !isdigit(ch))) {
				res.push({ TokenMatcher.count(tmp_token) ? TokenMatcher.at(tmp_token) :
					reading_num ? trm::tNum : trm::tId, str_num, tmp_token });
				tmp_token = "";
				reading_token = false;
				reading_num = true;
			}

			if (IsId(ch)) {
				reading_token = true;
				reading_num = reading_num && isdigit(ch);
				tmp_token += tolower(ch);
				continue;
			}

			if (ch == '\n')
				str_num++;
			if (IsDelmtr(ch)) continue;
			else if (ch == '-') res.push({trm::tMinus, str_num, "-"});
			else if (ch == '+') res.push({trm::tPlus, str_num, "+" });
			else if (ch == '*') res.push({trm::tMult, str_num, "*" });
			else if (ch == '/') res.push({trm::tDiv, str_num, "/" });
			else if (ch == '=') res.push({trm::tEq, str_num, "=" });
			else if (ch == '<')
			{
				char ch2 = in.get();
				if (in) {
					if (ch2 == '=') res.push({trm::tLeq, str_num, "<=" });
					else {
						in.putback(ch2);
						res.push({trm::tLss, str_num, "<" });
					}
				}
				else {
					res.push({trm::tLss, str_num, "<" });
				}
			}
			else if (ch == '>')
			{
				char ch2 = in.get();
				if (in) {
					if (ch2 == '=') res.push({trm::tGeq, str_num, ">=" });
					else {
						in.putback(ch2);
						res.push({trm::tGrt, str_num, ">" });
					}
				}
				else {
					res.push({trm::tGrt, str_num, ">" });
				}
			}
			else if (ch == ':')
			{
				char ch2 = in.get();
				if (in) {
					if (ch2 == '=') res.push({trm::tAsgn, str_num, ":=" });
					else {
						in.putback(ch2);
						res.push({trm::tColon, str_num, ":" });
					}
				}
				else {
					res.push({trm::tColon, str_num, ":" });
				}
			}
			else if (ch == ';') res.push({trm::tSColon, str_num, ";" });
			else if (ch == '.') res.push({trm::tDot, str_num, "." });
			else if (ch == ',') res.push({trm::tCom, str_num, "," });
			else if (ch == '(') res.push({trm::tLBr, str_num, "(" });
			else if (ch == ')') res.push({trm::tRBr, str_num, ")" });
			else {
				string err = "Tokener error: unexpected symbol \"";
				err += ch;
				err += "\" on line ";
				char k[10];
				_itoa_s(str_num, k, _countof(k), 10);
				err += k;
				throw exception(err.c_str());
			}
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
		if (!in_vector(expect, current_token.tok)) ErrorThrower(current_token);
	}

	void ErrorThrower(const Token& tok) {
		string err = "Syntax error: unexpected token on line ";
		char k[10];
		_itoa_s(tok.str_num, k, _countof(k), 10);
		err += k;
		err += ": \"" + tok.code_part + "\"";
		throw exception(err.c_str());
	}

	void NextToken(trm expectations) {
		expect = {expectations};
		PopToken();
	}

	void NextToken(const vector<trm>& exp1, const vector<trm>& exp2) {
		expect = exp1;
		for (trm el : exp2) expect.push_back(el);
		PopToken();
	}

	void NextToken(const vector<trm>& expectations) {
		expect = expectations;
		PopToken();
	}

	void program() {
		NextToken({ trm::tBegin, trm::tType, trm::tVar, trm::tProgram });
		if (ctok == trm::tProgram) {
			NextToken(trm::tId);
			NextToken(trm::tSColon);
			NextToken({ trm::tBegin, trm::tType, trm::tVar });
		}
		if (ctok == trm::tType) {
			NextToken(trm::tId);
			type_decl({ trm::tVar, trm::tBegin });
		}
		if (ctok == trm::tVar) {
			NextToken(trm::tId);
			var_decl({ trm::tBegin });
		}
		if (ctok == trm::tBegin) {
			block({ trm::tDot });
		}
		else throw exception("Something strange happened in PROGRAM");
	}

	void type_decl(const vector<trm>& finish) {
		vector<trm> fcopy = *new vector<trm>(finish);
		fcopy.push_back(trm::tId);

		NextToken(trm::tEq);
		var_type();
		NextToken(trm::tSColon);
		NextToken({ trm::tId, trm::tVar, trm::tBegin });
		if (ctok == trm::tId)
			type_decl(finish);
	}

	void var_decl(const vector<trm>& finish) {
		vector<trm> fcopy = *new vector<trm>(finish);
		fcopy.push_back(trm::tId);

		NextToken({ trm::tColon, trm::tCom });
		if (ctok == trm::tCom)
			id_list({ trm::tColon });
		var_type();
		NextToken(trm::tSColon);
		NextToken(fcopy);
		if (ctok == trm::tId)
			var_decl(finish);
	}

	void var_type() {
		NextToken({ trm::tRecord, trm::tBool, trm::tInt, trm::tId });
		if (ctok == trm::tRecord){
			NextToken({ trm::tId, trm::tEnd });
			if (ctok == trm::tId)
				var_decl({trm::tEnd});
		}

	}

	void command(const vector<trm>& finish) {
		switch (ctok) {
		case (trm::tWrite):
			write(finish);
			break;
		case (trm::tRead):
			read(finish);
			break;
		case (trm::tIf):
			if_block(finish);
			break;
		case (trm::tRepeat):
			repeat_block(finish);
			break;
		case (trm::tId):
			assignment(finish);
			break;
		case (trm::tBegin):
			block(finish);
			break;
		case (trm::tEnd):
			return;
		default:
			throw exception("Something strange happened in COMMAND");
		}
	}

	void block(const vector<trm>& finish) {
		while (ctok != trm::tEnd) {
			NextToken({ trm::tId, trm::tWrite, trm::tRead, trm::tIf, trm::tRepeat, trm::tBegin, trm::tEnd });
			command({ trm::tSColon, trm::tEnd });
		}
		NextToken(finish);
	}

	void assignment(const vector<trm>& finish) {
		vector<trm> fcopy = *new vector<trm>(finish);
		fcopy.push_back(trm::tCom);

		NextToken(trm::tAsgn);
		if (!expr(fcopy)) ErrorThrower(current_token);
		if (ctok == trm::tCom)
			assignment(finish);
	}

	bool expr(const vector<trm>& finish) {
		NextToken(finish, { trm::tId, trm::tLBr, trm::tMinus, trm::tTrue, trm::tFalse, trm::tNum});
		if (in_vector(finish, ctok)) return false;
		switch (ctok) {
			case(trm::tLBr):
				if (!expr({ trm::tRBr })) ErrorThrower(current_token);
			case(trm::tId):
			case(trm::tTrue):
			case(trm::tFalse):
			case(trm::tNum):
				NextToken(finish, { trm::tDiv, trm::tMult, trm::tPlus, trm::tMinus,
					trm::tGrt, trm::tLss, trm::tGeq, trm::tLeq, trm::tEq });
				if (!in_vector(finish, ctok)) {
					if (!expr(finish)) ErrorThrower(current_token);
				}
				break;
			case(trm::tMinus):
				if (!expr(finish)) ErrorThrower(current_token);
				break;
			default:
				throw exception("Something strange happened in EXPR");
		}
		return true;
	}

	void if_block(const vector<trm>& finish) {
		vector<trm> fcopy = *new vector<trm>(finish);
		fcopy.push_back(trm::tThen);

		if (!expr(fcopy)) ErrorThrower(current_token);
		NextToken({ trm::tId, trm::tWrite, trm::tRead, trm::tIf, trm::tRepeat, trm::tBegin, trm::tEnd });
		fcopy = *new vector<trm>(finish);
		fcopy.push_back(trm::tElse);
		command(fcopy);
		if (ctok == trm::tElse) {
			NextToken({ trm::tId, trm::tWrite, trm::tRead, trm::tIf, trm::tRepeat, trm::tBegin, trm::tEnd });
			command(finish);
		}
	}

	void read(const vector<trm>& finish) {
		NextToken(trm::tLBr);
		id_list({ trm::tRBr });
		NextToken(finish);
	}

	void write(const vector<trm>& finish) {
		NextToken(trm::tLBr);
		expr_list({ trm::tRBr });
		NextToken(finish);
	}

	void repeat_block(const vector<trm>& finish) {
		NextToken({ trm::tId, trm::tWrite, trm::tRead, trm::tIf, trm::tRepeat, trm::tBegin, trm::tEnd });
		command({trm::tUntil});
		if (!expr(finish)) ErrorThrower(current_token);
	}

	void expr_list(const vector<trm>& finish) {
		vector<trm> fcopy = *new vector<trm>(finish);
		fcopy.push_back(trm::tCom);

		expr(fcopy);
		if (ctok == trm::tCom)
			expr_list(finish);
	}

	void id_list(const vector<trm>& finish) {
		vector<trm> fcopy = *new vector<trm>(finish);
		fcopy.push_back(trm::tCom);

		NextToken(finish, { trm::tId });
		if (in_vector(finish, ctok)) return;
		NextToken(fcopy);
		if (ctok == trm::tCom)
			id_list(finish);
	}

public:
	bool TestProgram(std::istream& in) {
		try {
			GetTokens(in);
			SyntaxChecker();
		}
		catch (exception ex) {
			std::cout << ex.what();
			return false;
		}
		return true;
	}
};
