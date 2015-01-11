#ifndef PARSE_H
#define	PARSE_H
#include "gps.h"

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <map>
#include <cstring>
#include "../json_spirit/json_spirit.h"

using namespace std;

enum ParseValue {
    $GPRMC,
    $ACC,
    $GYRO,
    $HEADING,
};

static std::map<std::string, ParseValue> mapParseValue;

class parser {
public:
    parser();
    parser(const parser& orig);
    virtual ~parser();
    json_spirit::Object on_analyze_(const std::string &data);
    std::vector<std::string> split_(const std::string &str);
    static void Initialize();
private:
    gps gps_;
    json_spirit::Object obj;
    
};
#endif	/* PARSE_H */