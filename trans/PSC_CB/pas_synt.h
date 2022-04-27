#ifndef PAS_SYNT_H_INCLUDED
#define PAS_SYNT_H_INCLUDED

#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <exception>
#include <queue>
#include <stdarg.h>
#define ctok current_token.tok

using std::vector;
using std::string;
using std::map;
using std::exception;
using std::queue;

enum class Trm {
	tProgram, tVar, tBegin, tEnd, tRead, tWrite, tIf, tThen, tElse, tRepeat, tUntil, tInt, tType, tRecord, tBool, tId, tNum,
	tPlus, tMinus, tMult, tDiv, tOr, tAnd, tNot, tTrue, tFalse, tGeq, tEq, tLeq, tLss, tGrt, tAsgn, tDot, tCom, tColon, tSColon,
	tLBr, tRBr
};

const map<string, Trm> TokenMatcher = {
	{"program", Trm::tProgram},
	{"var",		Trm::tVar},
	{"begin",	Trm::tBegin},
	{"end",		Trm::tEnd},
	{"read",	Trm::tRead},
	{"write",	Trm::tWrite},
	{"if",		Trm::tIf},
	{"then",	Trm::tThen},
	{"else",	Trm::tElse},
	{"repeat",	Trm::tRepeat},
	{"until",	Trm::tUntil},
	{"int",		Trm::tInt},
	{"type",	Trm::tType},
	{"record",	Trm::tRecord},
	{"boolean", Trm::tBool},
	{"or",		Trm::tOr},
	{"and",		Trm::tAnd},
	{"not",		Trm::tNot},
	{"true",	Trm::tTrue},
	{"false",	Trm::tFalse},
};

struct Token {
	Trm tok;
	int str_num;
};

bool operator==(Token lhs, Trm rhs) {
	return lhs.tok == rhs;
}

class Pas_synt
{
private:
	bool	tokenised = false;
	Token	current_token;
	queue	<Token> tokens;
	vector	<Trm> expect;
	vector	<Trm> stack;


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
					reading_num ? Trm::tNum : Trm::tId, str_num });
				reading_token = false;
				reading_num = true;
			}

			if (ch == '\n')
				str_num++;
			if (IsDelmtr(ch)) continue;
			else if (ch == '-') res.push({Trm::tMinus, str_num});
			else if (ch == '+') res.push({Trm::tPlus, str_num});
			else if (ch == '*') res.push({Trm::tMult, str_num});
			else if (ch == '/') res.push({Trm::tDiv, str_num});
			else if (ch == '=') res.push({Trm::tEq, str_num});
			else if (ch == '<')
			{
				char ch2 = in.get();
				if (in) {
					if (ch2 == '=') res.push({Trm::tLeq, str_num});
					else {
						in.putback(ch2);
						res.push({Trm::tLss, str_num});
					}
				}
				else {
					res.push({Trm::tLss, str_num});
				}
			}
			else if (ch == '>')
			{
				char ch2 = in.get();
				if (in) {
					if (ch2 == '=') res.push({Trm::tGeq, str_num});
					else {
						in.putback(ch2);
						res.push({Trm::tGrt, str_num});
					}
				}
				else {
					res.push({Trm::tGrt, str_num});
				}
			}
			else if (ch == ':')
			{
				char ch2 = in.get();
				if (in) {
					if (ch2 == '=') res.push({Trm::tAsgn, str_num});
					else {
						in.putback(ch2);
						res.push({Trm::tColon, str_num});
					}
				}
				else {
					res.push({Trm::tColon, str_num });
				}
			}
			else if (ch == ';') res.push({Trm::tSColon, str_num});
			else if (ch == '.') res.push({Trm::tDot, str_num});
			else if (ch == ',') res.push({Trm::tCom, str_num});
			else if (ch == '(') res.push({Trm::tLBr, str_num});
			else if (ch == ')') res.push({Trm::tRBr, str_num});
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

	void NextToken(Trm expectations, ...) {
		expect = {};
		va_list pnt;
		va_start(pnt, expectations);
		for (Trm* i = &expectations; i; i++) {
			expect.push_back(*i);
		}
		PopToken();
	}

	void NextToken(const vector<Trm>& expectations, Trm exp1, ...) {
		expect = expectations;
		va_list pnt;
		va_start(pnt, exp1);
		for (Trm* i = &exp1; i; i++) {
			expect.push_back(*i);
		}
		PopToken();
	}

	void NextToken(const vector<Trm>& expectations) {
		expect = expectations;
		PopToken();
	}

	void program() {
		NextToken(Trm::tProgram);
		NextToken(Trm::tId);
		NextToken(Trm::tType, Trm::tVar, Trm::tBegin);
		switch (ctok) {
			case(Trm::tType):
				NextToken(Trm::tId);
				type_decl({ Trm::tVar, Trm::tBegin });
			case (Trm::tVar):
				NextToken(Trm::tId);
				var_decl({ Trm::tBegin });
			case (Trm::tBegin):
				block({ Trm::tDot });
				break;
			default:
				throw exception("Something strange happened...");
		}
	}

	void type_decl(const vector<Trm>& finish) {
		vector<Trm> fcopy = *new vector<Trm>(finish);
		fcopy.push_back(Trm::tId);

		NextToken(Trm::tEq);
		var_type();
		NextToken(Trm::tSColon);
		NextToken(Trm::tId, Trm::tVar, Trm::tBegin);
		if (ctok == Trm::tId)
			type_decl(finish);
	}

	void var_decl(const vector<Trm>& finish) {
		vector<Trm> fcopy = *new vector<Trm>(finish);
		fcopy.push_back(Trm::tId);

		NextToken(Trm::tColon);
		var_type();
		NextToken(Trm::tSColon);
		NextToken(fcopy);
		if (ctok == Trm::tId)
			var_decl(finish);
	}

	void var_type() {
		NextToken(Trm::tRecord, Trm::tBool, Trm::tInt, Trm::tId);
		if (ctok == Trm::tRecord){
			NextToken(Trm::tId, Trm::tEnd);
			if (ctok == Trm::tId)
				var_decl({Trm::tEnd});
		}

	}

	void command(const vector<Trm>& finish) {
		switch (ctok) {
		case (Trm::tWrite):
			write(finish);
			break;
		case (Trm::tRead):
			read(finish);
			break;
		case (Trm::tIf):
			write(finish);
			break;
		case (Trm::tRepeat):
			write(finish);
			break;
		case (Trm::tId):
			assignment(finish);
			break;
		case (Trm::tBegin):
			block(finish);
			break;
		default:
			throw exception("Something strange happened...");
		}
	}

	void block(const vector<Trm>& finish) {
		NextToken(Trm::tId, Trm::tWrite, Trm::tRead, Trm::tIf, Trm::tRepeat, Trm::tBegin, Trm::tEnd);
		while (ctok != Trm::tEnd) {
			command({ Trm::tSColon, Trm::tEnd });
			NextToken(Trm::tId, Trm::tWrite, Trm::tRead, Trm::tIf, Trm::tRepeat, Trm::tBegin, Trm::tEnd);
		}
		NextToken(finish);
	}

	void assignment(const vector<Trm>& finish) {
		vector<Trm> fcopy = *new vector<Trm>(finish);
		fcopy.push_back(Trm::tCom);

		NextToken(Trm::tAsgn);
		expr(fcopy);
		if (ctok == Trm::tCom)
			assignment(finish);
	}

	void expr(const vector<Trm>& finish) {
		NextToken(Trm::tId, Trm::tLBr, Trm::tMinus);
		switch (ctok) {
			case(Trm::tId):
				NextToken(finish, Trm::tDiv, Trm::tMult, Trm::tPlus, Trm::tMinus,
					Trm::tGrt, Trm::tLss, Trm::tGeq, Trm::tLeq, Trm::tEq);
				if (!in_vector(finish, ctok)) {
					NextToken(Trm::tId);
					NextToken(finish);
				}
				break;
			case(Trm::tLBr):
				expr({ Trm::tRBr });
			case(Trm::tMinus):
				expr(finish);
			default:
				throw exception("Something strange happened...");
		}
	}

	void if_block(const vector<Trm>& finish) {
		vector<Trm> fcopy = *new vector<Trm>(finish);
		fcopy.push_back(Trm::tThen);

		NextToken(Trm::tId, Trm::tWrite, Trm::tRead, Trm::tIf, Trm::tRepeat, Trm::tBegin, Trm::tEnd);
		command(fcopy);
		if (ctok == Trm::tElse) {
			NextToken(Trm::tId, Trm::tWrite, Trm::tRead, Trm::tIf, Trm::tRepeat, Trm::tBegin, Trm::tEnd);
			command(finish);
		}
	}

	void read(const vector<Trm>& finish) {
		NextToken(Trm::tLBr);
		id_list({ Trm::tRBr });
		NextToken(finish);
	}

	void write(const vector<Trm>& finish) {
		NextToken(Trm::tLBr);
		expr_list({ Trm::tRBr });
		NextToken(finish);
	}

	void repeat_block(const vector<Trm>& finish) {
		NextToken(Trm::tId, Trm::tWrite, Trm::tRead, Trm::tIf, Trm::tRepeat, Trm::tBegin, Trm::tEnd);
		command({Trm::tUntil});
		expr(finish);
	}

	void expr_list(const vector<Trm>& finish) {
		vector<Trm> fcopy = *new vector<Trm>(finish);
		fcopy.push_back(Trm::tCom);

		expr(fcopy);
		if (ctok == Trm::tCom)
			expr_list(finish);
	}

	void id_list(const vector<Trm>& finish) {
		vector<Trm> fcopy = *new vector<Trm>(finish);
		fcopy.push_back(Trm::tCom);

		NextToken(Trm::tId);
		NextToken(fcopy);
		if (ctok == Trm::tCom)
			id_list(finish);
	}

public:
	void TestProgram(std::istream& in) {
		GetTokens(in);
		SyntaxChecker();
		std::cout << "correct";
	}
};

#endif // PAS_SYNT_H_INCLUDED
