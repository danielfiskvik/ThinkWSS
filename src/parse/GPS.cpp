#include "../../include/parse/GPS.h"

GPS::GPS() {
    Initialize();
}

GPS::GPS(const GPS& orig) {
}

GPS::~GPS() {
}

json_spirit::Object GPS::build_location_(std::vector<std::string>& data) {
    Location loc;
    loc.fix = check_fix_(data.at(2));
    loc.speed = convert_speed_(data.at(7));
    loc.bearing = convert_bearing_(data.at(8));
    loc.coords.lon = convert_deg_(data.at(4), "0"+data.at(3));
    loc.coords.lat = convert_deg_(data.at(6), data.at(5));
//    cout << "fix: " << std::boolalpha << loc.fix << "\nspeed: " << loc.speed << "\nbearing: " <<
//            loc.bearing << "\nlon: " << loc.coords.lon << "\nlat: " <<
//            loc.coords.lat << endl;
    
    json_spirit::Object location;
    location["speed"] = loc.speed;
    location["fix"] = loc.fix;
    location["bearing"] = loc.bearing;
    
    json_spirit::Object coords;
    coords["lon"] = loc.coords.lon;
    coords["lat"] = loc.coords.lat;
     
    location["coordinates"] = coords;
    json_spirit::Value value(location);
    
//    string str = json_spirit::write(value);
//    cout << "str: " << str << endl;

    //ofstream os;
    //os.open("json_example.json");
    //json_spirit::write_stream(json_spirit::Value(location), os, json_spirit::pretty_print);    
    
    return location;
}

bool GPS::check_fix_(const std::string fix) {
    char *cstr = new char[fix.length() + 1];
    strcpy(cstr, fix.c_str());
    // Switch on the value
    switch (s_mapFixValues[cstr]) {
        case A:
            //cout << "Detected the first valid string." << endl;
            return true;
            break;
        case V:
            //cout << "Detected the second valid string." << endl;
            return false;
            break;
        default:
            return false;
            //            cout << "'" << cstr
            //                    << "' is an invalid string. s_mapStringValues now contains "
            //                    << s_mapStringValues.size()
            //                    << " entries." << endl;
            break;
    }
    delete [] cstr;
}

void GPS::Initialize() {
    s_mapFixValues["A"] = A;
    s_mapFixValues["V"] = V;

    //    cout << "s_mapStringValues contains "
    //            << s_mapStringValues.size()
    //            << " entries." << endl;
}

// convertSpeed convert knot to m/s.
// Returns float32 of m/s speed

float GPS::convert_speed_(const std::string &fix) {
    const float convertion = (0.514444444);
    return convertion * parse_float_(fix);
}

float GPS::convert_bearing_(const std::string &rawBearing) {
    return parse_float_(rawBearing);
}

float GPS::parse_float_(const std::string &str) {
    std::string::size_type sz; // alias of size_t
    return std::stof(str, &sz);
}

float GPS::parse_float_(const std::string& str, int sz) {
    return 0;
}

double GPS::convert_deg_(const std::string& heading, const std::string& rawLon) {
    long deg = parse_float_(rawLon.substr(0,3));
    unsigned found = rawLon.find_last_of("/\\");
    long min = parse_float_(rawLon.substr(3,found));
    //string deg = parse_float_(rawLon);
    float decimal = deg + (min / 60);
    if (heading == "S" || heading == "W") {
        decimal = -decimal;
    }
    return decimal;
}   