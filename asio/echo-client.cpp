#include "config.h"

#include <iostream>
#include <string>
#include <boost/asio.hpp>

#define SERVER_IP    "127.0.0.1"
#define PORT_NUMBER  9999

int main(int argc, char ** argv)
{
    using namespace std;

    cout << "echo-client\n";

    boost::asio::io_service io_service;
    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(SERVER_IP),
    /* ** */PORT_NUMBER);

    boost::system::error_code error;
    boost::asio::ip::tcp::socket socket(io_service);
    socket.connect(endpoint, error);

    if (error) {
        cout << "Connection fail." << error.value() << ", message: " << error.message() << endl;
        return 0;
    } else {
        cout << "Connection success.\n";
    }

    for (int i = 0; i < 7; ++i) {
        string message;
        cin >> message;

        boost::system::error_code ignored_error;
        socket.write_some(boost::asio::buffer(message, message.length()), ignored_error);

        cout << "Send: " << message << "!" << endl;

        array<char, 128> buf;
        buf.fill(0);

        boost::system::error_code read_error;
        size_t len = socket.read_some(boost::asio::buffer(buf), read_error);

        if (error) {
            if (error == boost::asio::error::eof) {
                cout << "Disconnect server.\n";
            } else {
                cout << "error no: " << error.value() << ", error message: " << error.message()
                        << endl;
            }
            break;
        }

        string read_message;
        read_message.assign(buf.begin(), buf.begin() + len);
        cout << "Read: " << read_message << "!" << endl;
    }

    if (socket.is_open() == true) {
        socket.close();
    }

    return 0;
}
