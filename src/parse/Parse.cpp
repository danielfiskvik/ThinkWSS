#include "../../include/parse/Parse.h"

Parse::Parse() {
    // Init the string map
    Initialize();
}

Parse::Parse(const Parse& orig) {
}

Parse::~Parse() {
}

json_spirit::Object Parse::on_analyze_(const std::string& data) {

    std::vector<std::string> elems = split(data);
    char *cstr = new char[elems.at(0).length() + 1];
    strcpy(cstr, elems.at(0).c_str());

    // Switch on the value
    switch (s_mapStringValues[cstr]) {
        case $GPRMC:
            //cout << "Detected the first valid string." << endl;
            obj = gps.build_location_(elems);
            break;
        case $ACC:
            //cout << "Detected the second valid string." << endl;
            break;
        case $GYRO:
            //cout << "Detected the second valid string." << endl;
            break;
        case $HEADING:
            //cout << "Detected the second valid string." << endl;
            break;
        case evEnd:
            //            cout << "Detected program end command. "
            //                    << "Programm will be stopped." << endl;
        default:
            //            cout << "'" << cstr
            //                    << "' is an invalid string. s_mapStringValues now contains "
            //                    << s_mapStringValues.size()
            //                    << " entries." << endl;
            break;
    }
    delete [] cstr;
    elems.clear();

    return obj;
}

std::vector<std::string> Parse::split(const std::string& str) {
    std::vector<std::string> elems;
    string s = str;
    s.erase(remove(s.begin(), s.end(), ' '), s.end());
    std::istringstream ss(s);
    std::string token;
    while (std::getline(ss, token, ',')) {
        if (token != "") {
            elems.push_back(token);
            //cout << token << '\n';
        }
    }
    //cout << '\n' << s << '\n';
    return elems;
}

void Parse::Initialize() {
    s_mapStringValues["$GPRMC"] = $GPRMC;
    s_mapStringValues["$ACC"] = $ACC;
    s_mapStringValues["$GYRO"] = $GYRO;
    s_mapStringValues["$HEADING"] = $HEADING;
    s_mapStringValues["end"] = evEnd;

    //    cout << "s_mapStringValues contains "
    //            << s_mapStringValues.size()
    //            << " entries." << endl;
}