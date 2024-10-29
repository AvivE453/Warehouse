#include <iostream>
#include <fstream>
#include "../include/Parse.h"
#include "../include/WareHouse.h"
#include "../include/Volunteer.h"
#include "../include/Customer.h"
using namespace std;

void Parse::ParseFile(const string &configorationFile, WareHouse &wareHouse)
{
    ifstream myFile(configorationFile);
    string myline;

    while (getline(myFile, myline))
    {
        if (myline.rfind("customer", 0) == 0)
        {
            vector<string> words = tokenize(myline, " ");
            Customer *customer = nullptr;
            if (words[2] == "soldier")
                customer = new SoldierCustomer(wareHouse.assignCustomerId(), (words[1]), stoi(words[3]), stoi(words[4]));
            else
                customer = new CivilianCustomer(wareHouse.assignCustomerId(), words[1], stoi(words[3]), stoi(words[4]));
            wareHouse.addCustomer(customer);
        }
        else if (myline.rfind("volunteer", 0) == 0)
        {
            vector<string> words = tokenize(myline, " ");
            Volunteer *volunteer = nullptr;
            if (words[2] == "collector")
                volunteer = new CollectorVolunteer(wareHouse.assignVolunteerId(), words[1], stoi(words[3]));
            else if (words[2] == "limited_collector")
            {
                volunteer = new LimitedCollectorVolunteer(wareHouse.assignVolunteerId(), words[1], stoi(words[3]), stoi(words[4]));
            }
            else if (words[2] == "driver")
            {
                volunteer = new DriverVolunteer(wareHouse.assignVolunteerId(), words[1], stoi(words[3]), stoi(words[4]));
            }
            else if (words[2] == "limited_driver")
            {
                volunteer = new LimitedDriverVolunteer(wareHouse.assignVolunteerId(), words[1], stoi(words[3]), stoi(words[4]), stoi(words[5]));
            }
            wareHouse.addVolunteer(volunteer);
        }
        myFile >> ws;
        if (!myFile)
            break;
    }
    myFile.close();
}
vector<string> Parse::tokenize(const string &s, const string &del)
{
    vector<string> words;
    int start, end = -1 * del.size();
    do
    {
        start = end + del.size();
        end = s.find(del, start);
        if (s[start] == ' ' || s[start] == '#')
            break;
        words.push_back(s.substr(start, end - start));
    } while (end != -1);
    return words;
}
