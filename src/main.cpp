// OS Specific sleep
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include "../include/serial/serial.h"
#include <boost/asio.hpp> 
using namespace::boost::asio;
using namespace std;

using std::string;
using std::exception;
using std::cout;
using std::cerr;
using std::endl;
using std::vector;

void my_sleep(unsigned long milliseconds) {
#ifdef _WIN32
    Sleep(milliseconds); // 100 ms
#else
    usleep(milliseconds * 1000); // 100 ms
#endif
}

void enumerate_ports() {
    vector<serial::PortInfo> devices_found = serial::list_ports();

    vector<serial::PortInfo>::iterator iter = devices_found.begin();

    while (iter != devices_found.end()) {
        serial::PortInfo device = *iter++;

        printf("(%s, %s, %s)\n", device.port.c_str(), device.description.c_str(),
                device.hardware_id.c_str());
        //printf("%s)\n", device.port.c_str());
    }
}

void print_usage() {
    cerr << "Usage: test_serial {-e|<serial port address>} ";
    cerr << "<baudrate> [test string]" << endl;
}

#include <cstdlib>

#include "../include/websocket/server_ws.h"
#include "../include/websocket/client_ws.h"
#include <iostream>
#include <boost/asio.hpp>

#include "../include/json_spirit/json_spirit.h"
#include "../include/parse/Parse.h"

using namespace::boost::asio;

using namespace std;
using namespace SimpleWeb;

int main() {


    //WebSocket (WS)-server at port 8080 using 4 threads
    SocketServer<WS> server(8080, 4);

    auto& echo = server.endpoint["^/gps/?$"];

    //Example 1: echo WebSocket endpoint
    //  Added debug messages for example use of the callbacks
    //  Test with the following JavaScript:
    //    var ws=new WebSocket("ws://localhost:8080/gps");
    //    ws.onmessage=function(evt){console.log(evt.data);};
    //    ws.send("get gps data");
    echo.onmessage = [&server](shared_ptr<SocketServer<WS>::Connection> connection, shared_ptr<SocketServer<WS>::Message> message) {
        //To receive message from client as string (data_ss.str())
        stringstream data_ss;
        message->data >> data_ss.rdbuf();
        cout << "Server: Message received: \"" << data_ss.str() << "\" from " << (size_t) connection.get() << endl;

        serial::Serial my_serial("/dev/ttyACM0", 9600, serial::Timeout::simpleTimeout(10000));
        cout << "Is the serial port open?";
        if (my_serial.isOpen())
            cout << " Yes." << endl;
        else
            cout << " No." << endl;

        Parse pr;
        while (true) {
            // clear buffer
            data_ss.str(std::string());
            message->data.clear();
            
            // convert string to isstringstream
            std::istringstream s_stream(json_spirit::write(pr.on_analyze_(my_serial.readline(256, "\n"))));
            // push stream to stringstream
            s_stream >> data_ss.rdbuf();
            cout << "Server: Sending message \"" << data_ss.str() << "\" to " << (size_t) connection.get() << endl;
            //message->data >> data_ss.rdbuf();
            

            server.send(connection, data_ss, [](const boost::system::error_code & ec) {
                if (ec) {
                    cout << "Server: Error sending message. " <<
                            //See http://www.boost.org/doc/libs/1_55_0/doc/html/boost_asio/reference.html, Error Codes for error code meanings
                            "Error: " << ec << ", error message: " << ec.message() << endl;
                }
            });
            s_stream.clear();
        }
        my_serial.close();
    };

    echo.onopen = [&server](shared_ptr<SocketServer<WS>::Connection> connection) {
        cout << "Server: Opened connection " << (size_t) connection.get() << endl;
    };

    //See RFC 6455 7.4.1. for status codes
    echo.onclose = [](shared_ptr<SocketServer<WS>::Connection> connection, int status, const string & reason) {
        cout << "Server: Closed connection " << (size_t) connection.get() << " with status code " << status << endl;
    };

    //See http://www.boost.org/doc/libs/1_55_0/doc/html/boost_asio/reference.html, Error Codes for error code meanings
    echo.onerror = [](shared_ptr<SocketServer<WS>::Connection> connection, const boost::system::error_code & ec) {
        cout << "Server: Error in connection " << (size_t) connection.get() << ". " <<
                "Error: " << ec << ", error message: " << ec.message() << endl;
    };

    thread server_thread([&server]() {
        //Start WS-server
        server.start();
    });

    server_thread.join();

    return 0;
}