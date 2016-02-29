#include "config.h"

#include <iostream>
#include <string>
#include <boost/asio.hpp>

#define SERVER_IP    "127.0.0.1"
#define PORT_NUMBER  9999

#define CHAR_BUFFER_SIZE 128

class TcpClient
{
private:
    boost::asio::io_service & m_io_service;
    boost::asio::ip::tcp::socket m_socket;
    int m_seq_number;
    std::array<char, CHAR_BUFFER_SIZE> m_receiveBuffer;
    std::string m_writeMessage;

public:
    TcpClient(boost::asio::io_service & io_service) :
            m_io_service(io_service), m_socket(io_service), m_seq_number(0)
    {
    }

    virtual ~TcpClient()
    {
    }

public:
    void Connect(boost::asio::ip::tcp::endpoint & endpoint)
    {
        m_socket.async_connect(endpoint,
                std::bind(&TcpClient::handle_connect, this, std::placeholders::_1));
    }

    void PostWrite()
    {
        if (m_socket.is_open() == false) {
            return;
        }

        if (m_seq_number > 7) {
            m_socket.close();
            return;
        }

        ++m_seq_number;

        std::cin >> m_writeMessage;

        m_socket.async_write_some(boost::asio::buffer(m_writeMessage),
                std::bind(&TcpClient::handle_write, this, std::placeholders::_1,
                        std::placeholders::_2));

        PostReceive();
    }

    void PostReceive()
    {
        m_receiveBuffer.fill(0);

        m_socket.async_read_some(boost::asio::buffer(m_receiveBuffer),
                std::bind(&TcpClient::handle_receive, this, std::placeholders::_1,
                        std::placeholders::_2));
    }

public:
    void handle_connect(boost::system::error_code const & error)
    {
        using namespace std;
        if (error) {
            cout << "Connection fail." << error.value() << ", message: " << error.message() << endl;
        } else {
            cout << "Connection success.\n";
            PostWrite();
        }
    }

    void handle_write(boost::system::error_code const & error, size_t byte_transferred)
    {
    }

    void handle_receive(boost::system::error_code const & error, size_t byte_transferred)
    {
        using namespace std;

        if (error) {
            if (error == boost::asio::error::eof) {
                cout << "Disconnect server.\n";
            } else {
                cout << "error no: " << error.value() << ", error message: " << error.message()
                        << endl;
            }
        } else {
            string const recvMessage = m_receiveBuffer.data();
            cout << "RecvMessage: " << recvMessage << ", size: " << byte_transferred << endl;
            PostWrite();
        }
    }
};

int main(int argc, char ** argv)
{
    using namespace std;

    cout << "echo-client\n";

    boost::asio::io_service io_service;
    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(SERVER_IP),
    /* ** */PORT_NUMBER);

    TcpClient client(io_service);
    client.Connect(endpoint);
    io_service.run();

    cout << "END...\n";

    return 0;
}
