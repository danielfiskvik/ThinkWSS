#ifndef GPS_H
#define	GPS_H

#include <string>
#include <vector>
#include <map>
#include <cstring>
#include <iostream>
#include <fstream>
#include "../../include/json_spirit/json_spirit.h"
#include "../../include/json_spirit/writer_template.h"
#include "../../include/json_spirit/value.h"
using namespace std;

enum FixValue {
    A,
    V
};

static std::map<std::string, FixValue> mapFixValues;

struct Coordinates {
    float lon;
    float lat;
};

struct Location {
    float speed;
    bool fix;
    Coordinates coords;
    float bearing;
};

class gps {
public:
    gps();
    gps(const gps& orig);
    virtual ~gps();
    json_spirit::Object build_location_(std::vector<std::string> &data);
    bool check_fix_(const std::string fix);
    static void Initialize();
    float convert_speed_(const std::string &fix);
    float convert_bearing_(const std::string &rawBearing);
    float parse_float_(const std::string &str);
    double convert_deg_(const std::string &heading, const std::string &rawLon);
        
private:
    
};

#endif	/* GPS_H */

