#ifndef TESTS_H
#define TESTS_H

#include <string>
using namespace std;

extern bool TESTING;

string antivirusManagerTest();
string diskTest();
string fileTest();
string folderTest();
string fileManagerTest();
string virusTest();

void runTesting();

#endif
