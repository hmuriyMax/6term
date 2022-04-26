#include "ut.h"
#include "syntaxer.h"

#include <iostream>
#include <string>

using namespace std;

void RunProgramTest(const std::string& testname, const std::string& progtext, bool expected)
{
    if (IsProgramCorrect(progtext) != expected)
        throw exception(
            string(testname + " failed. Terminating.").c_str()
        );
}

void CheckProgram(std::ostream& out, const std::string& testname, const std::string& progtext)
{
    out << testname << ": program is ";
    if (!IsProgramCorrect(progtext))
        out << "not ";
    out << "syntactically correct";
}

void RunAll()
{
    try
    {
        for (int i = 0; i < GetValidPrograms().size(); ++i) {
            string testname = "Test for correctness " + to_string(i+1);
            RunProgramTest(testname, GetValidPrograms()[i], true);
            std::cerr << testname << ": program is correct\n";
        }

        for (int i = GetValidPrograms().size(); i < GetValidPrograms().size() + GetInvalidPrograms().size(); ++i) {
            string testname = "Test for uncorrectness " + to_string(i+1);
            RunProgramTest(testname, GetInvalidPrograms()[i - GetValidPrograms().size()], false);
            std::cerr << testname << ": program isn\'t correct\n";
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}

/////////////////////////////////////////////////////////////////////

const std::vector<std::string>& GetValidPrograms()
{
    static vector<string> result = {
R"(
begin
end.
)",
////////////////////////////////////////////////////
R"(
var ___     :    integer;
var _______:
boolean;
const D = -3 + ___;
var N__00 : boolean;
begin
end.
)",
////////////////////////////////////////////////////
R"(
program start;
var a, b, c: integer;
    de, fg_01: boolean;
const M = 10.1; N = -15.2;

begin
    a := 11;
    b:=-3;
    write(a + b);
end.
)",
////////////////////////////////////////////////////
R"(
begin
if 3 < 4 then
    read(a, x1, y1);
if x1 >= y1 or a = x1 then
begin
    for i := 1 to a do
    begin
        write(x1+i, y1);
        x1 := x1 + y1;
    end;
    write();
    write(x1);
end;
end.
)",
////////////////////////////////////////////////////
R"(
begin
    a := (4 + 3) * +11 * a / (((i))) - -1;
    b:= a;
    for c:= true downto false do
        b := b * a;
end.
)"
    };

    return result;
}


const std::vector<std::string>& GetInvalidPrograms()
{
    static vector<string> result = {
R"(
program abc;
)",
////////////////////////////////////////////////////
R"(
var a: integer;
begin
end
)",
////////////////////////////////////////////////////
R"(
program __444
begin
end.
)",
////////////////////////////////////////////////////
R"(
var N, C, const : boolean;
begin
end.
)",
////////////////////////////////////////////////////
R"(
var C: integer
begin
end.
)",
////////////////////////////////////////////////////
R"(
begin
if 3 < 4 then
    read(a, x1, y1);
if x1 >= y1 or a = x1 then
begin
    for i : = 1 to a do
    begin
        write(x1 + i, y1);
        x1: = x1 + y1;
    end;
    write();
    write(x1);
end
end.
)",
////////////////////////////////////////////////////
R"(
begin
if 3 < 4 then
    read(a, x1, y1);
if x1 >= y1 or a = then
begin
    for i : = 1 to a do
    begin
        write(x1 + i, y1);
        x1: = x1 + y1;
    end;
    write();
    write(x1);
end
end.
)"
    };

    return result;
}