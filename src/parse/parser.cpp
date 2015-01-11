#include "../../include/parse/parser.h"

parser::parser() {
    Initialize();
}

parser::parser(const parser& orig) {
}

parser::~parser() {
}

json_spirit::Object parser::on_analyze_(const std::string& data) {
    
    std::vector<std::string> elems = split_(data);
    char *cstr = new char[elems.at(0).length() + 1];
    strcpy(cstr, elems.at(0).c_str());

    switch (mapParseValue[cstr]) {
        case $GPRMC:
            //send data to gps class to structure data
            obj = gps_.build_location_(elems);
            break;
        case $ACC:
            //send data to accelerometer class to structure data
            break;
        case $GYRO:
            //send data to gyro class to stucture data
            break;
        case $HEADING:
            //send data to heading class to stucture data
            break;
        default:
            cout << "'" << cstr
                    << "' is an invalid string. mapParseValue now contains "
                    << mapParseValue.size()
                    << " entries." << endl;
            break;
    }
    delete [] cstr;
    elems.clear();

    return obj;
}

std::vector<std::string> parser::split_(const std::string& str) {
    std::vector<std::string> elems;
    string s = str;
    s.erase(remove(s.begin(), s.end(), ' '), s.end());
    std::istringstream ss(s);
    std::string token;
    while (std::getline(ss, token, ',')) {
        if (token != "") {
            elems.push_back(token);
        }
    }
    return elems;
}

void parser::Initialize() {
    mapParseValue["$GPRMC"] = $GPRMC;
    mapParseValue["$ACC"] = $ACC;
    mapParseValue["$GYRO"] = $GYRO;
    mapParseValue["$HEADING"] = $HEADING;
}