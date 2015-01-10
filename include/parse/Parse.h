#ifndef PARSE_H
#define	PARSE_H
#include "GPS.h"

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <map>
#include <cstring>
#include "../json_spirit/json_spirit.h"

using namespace std;

// Value-Defintions of the different String values
enum StringValue {
    evNotDefined,
    $GPRMC,
    $ACC,
    $GYRO,
    $HEADING,
    evEnd
};

// Map to associate the strings with the enum values
static std::map<std::string, StringValue> s_mapStringValues;

class Parse {
public:
    Parse();
    Parse(const Parse& orig);
    virtual ~Parse();
    json_spirit::Object on_analyze_(const std::string &data);
    std::vector<std::string> split(const std::string &str);
    // Intialization
    static void Initialize();
private:
    GPS gps;
    json_spirit::Object obj;
    
};
#endif	/* PARSE_H */