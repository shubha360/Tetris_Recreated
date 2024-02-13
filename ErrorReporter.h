#pragma once

#include <iostream>
#include <string>

#define REPORT_ERROR(error, func) printError(error, #func, __FILE__, __LINE__);

extern void printError(const std::string& error, const char* functionName, const char* file, const int lineNumber);