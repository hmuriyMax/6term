#pragma once
#include <string>
#include <vector>
#include <ostream>

void RunAll();
void CheckProgram(std::ostream& out, const std::string& testname, const std::string& progtext);

const std::vector<std::string>& GetValidPrograms();
const std::vector<std::string>& GetInvalidPrograms();