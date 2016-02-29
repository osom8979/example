#include "config.h"

#include <iostream>
#include <array>
#include <string>
#include <boost/asio.hpp>

#define SERVER_IP    "127.0.0.1"
#define PORT_NUMBER  9999

int main(int argc, char ** argv)
{
    using namespace std;

    cout << "echo-server\n";

    boost::asio::io_service io_service;
    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), PORT_NUMBER);
    boost::asio::ip::tcp::acceptor acceptor(io_service, endpoint);

    boost::asio::ip::tcp::socket socket(io_service);
    acceptor.accept(socket);

    cout << "Accept client.\n";

    while (true) {
        array<char, 128> buf;
        buf.fill(0);

        boost::system::error_code error;
        size_t len = socket.read_some(boost::asio::buffer(buf), error);

        if (error) {
            if (error == boost::asio::error::eof) {
                cout << "Disconnect client.\n";
            } else {
                cout << "error no: " << error.value() << ", error message: " << error.message()
                        << endl;
            }
            break;
        }

        cout << "Read: " << &buf[0] << "!" << endl;

        string re_message;
        re_message = "RE: ";
        re_message.append(buf.begin(), buf.begin() + len);

        boost::system::error_code ignored_error;
        socket.write_some(boost::asio::buffer(re_message, re_message.length()), ignored_error);
        cout << "Send: " << re_message << "!" << endl;
    }

    return 0;
}

