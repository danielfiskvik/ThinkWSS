#include "../../include/parse/gps.h"

gps::gps() {
    Initialize();
}

gps::gps(const gps& orig) {
}

gps::~gps() {
}

json_spirit::Object gps::build_location_(std::vector<std::string>& data) {
    Location loc;
    loc.fix = check_fix_(data.at(2));
    loc.speed = convert_speed_(data.at(7));
    loc.bearing = convert_bearing_(data.at(8));
    loc.coords.lon = convert_deg_(data.at(4), "0" + data.at(3));
    loc.coords.lat = convert_deg_(data.at(6), data.at(5));

    json_spirit::Object location;
    location["speed"] = loc.speed;
    location["fix"] = loc.fix;
    location["bearing"] = loc.bearing;

    json_spirit::Object coords;
    coords["lon"] = loc.coords.lon;
    coords["lat"] = loc.coords.lat;

    location["coordinates"] = coords;
    json_spirit::Value value(location);

    return location;
}

bool gps::check_fix_(const std::string fix) {
    char *cstr = new char[fix.length() + 1];
    strcpy(cstr, fix.c_str());
    switch (mapFixValues[cstr]) {
        case A:
            return true;
            break;
        case V:
            return false;
            break;
        default:
            return false;
            cout << "'" << cstr
                    << "' is an invalid string. mapFixValues now contains "
                    << mapFixValues.size()
                    << " entries." << endl;
            break;
    }
    delete [] cstr;
}

void gps::Initialize() {
    mapFixValues["A"] = A;
    mapFixValues["V"] = V;
}

// convertSpeed convert knot to m/s.
// Returns float32 of m/s speed
float gps::convert_speed_(const std::string &fix) {
    const float convertion = (0.514444444);
    return convertion * parse_float_(fix);
}

float gps::convert_bearing_(const std::string &rawBearing) {
    return parse_float_(rawBearing);
}

float gps::parse_float_(const std::string &str) {
    std::string::size_type sz; // alias of size_t
    return std::stof(str, &sz);
}

double gps::convert_deg_(const std::string& heading, const std::string& rawLon) {
    long deg = parse_float_(rawLon.substr(0, 3));
    unsigned found = rawLon.find_last_of("/\\");
    long min = parse_float_(rawLon.substr(3, found));

    float decimal = deg + (min / 60);
    if (heading == "S" || heading == "W") {
        decimal = -decimal;
    }
    return decimal;
} 