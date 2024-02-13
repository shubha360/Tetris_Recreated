#include "ErrorReporter.h"

void printError(const std::string& error, const char* functionName, const char* file, const int lineNumber) {
	printf(
		"[ERROR] - %s\n"
		"[FUNCTION] - %s\n"
		"[FILE] - %s\n"
		"[LINE] - %d\n\n",
		error.c_str(), functionName, file, lineNumber
	);
}