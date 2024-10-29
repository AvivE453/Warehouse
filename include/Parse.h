#pragma once
#include <string>
#include <vector>
using std::string;
using std::vector;

class WareHouse;

class Parse
{
public:
    void ParseFile(const string &configorationFile, WareHouse &wareHouse);
    vector<string> tokenize(const string &s, const string &del);

private:
};