#include "config.h"

#include <iostream>
#include <algorithm>
#include <array>
#include <string>
#include <list>
#include <functional>

#include <boost/asio.hpp>

#define SERVER_IP    "127.0.0.1"
#define PORT_NUMBER  9999

#define CHAR_BUFFER_SIZE 128

class Session
{
private:
    boost::asio::ip::tcp::socket m_socket;
    std::string m_writeMessage;
    std::array<char, CHAR_BUFFER_SIZE> m_receiveBuffer;

public:
    Session(boost::asio::io_service & io_service) :
            m_socket(io_service)
    {
    }

    boost::asio::ip::tcp::socket & Socket()
    {
        return m_socket;
    }

    void PostReceive()
    {
        m_receiveBuffer.fill(0);
        m_socket.async_read_some(boost::asio::buffer(m_receiveBuffer),
                std::bind(&Session::handle_receive, this, std::placeholders::_1,
                        std::placeholders::_2));
    }

public:
    void handle_write(boost::system::error_code const & error, size_t bytes_transferred)
    {
    }

    void handle_receive(boost::system::error_code const & error, size_t bytes_transferred)
    {
        using namespace std;

        if (error) {
            if (error == boost::asio::error::eof) {
                cout << "Disconnect client.\n";
            } else {
                cout << "error no: " << error.value() << ", error message: " << error.message()
                        << endl;
            }
        } else {
            string const recv_message = m_receiveBuffer.data();
            cout << "Recv: " << recv_message << ", size: " << bytes_transferred << endl;

            m_writeMessage = "Re: ";
            m_writeMessage += recv_message;

            m_socket.async_write_some(boost::asio::buffer(m_writeMessage),
                    std::bind(&Session::handle_write, this, std::placeholders::_1,
                            std::placeholders::_2));

            PostReceive();
        }
    }
};

class TcpServer
{
private:
    int m_seqNumber;
    boost::asio::ip::tcp::acceptor m_acceptor;
    Session * m_session;

public:
    TcpServer(boost::asio::io_service & io_service) :
            m_acceptor(io_service,
                    boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), PORT_NUMBER))
    {
        m_session = nullptr;
        StartAccept();
    }

    virtual ~TcpServer()
    {
        if (m_session != nullptr) {
            delete m_session;
        }
    }

public:
    void StartAccept()
    {
        using namespace std;

        cout << "Wait for client...\n";
        m_session = new Session(m_acceptor.get_io_service());
        m_acceptor.async_accept(m_session->Socket(),
                std::bind(&TcpServer::handle_accept, this, m_session, std::placeholders::_1));
    }

    void handle_accept(Session * session, boost::system::error_code const & error)
    {
        using namespace std;
        if (!error) {
            cout << "Acept client.\n";
            session->PostReceive();
        }
    }

};

int main(int argc, char ** argv)
{
    using namespace std;

    cout << "echo-server\n";

    boost::asio::io_service io_service;
    TcpServer server(io_service);
    io_service.run();

    cout << "END...\n";

    return 0;
}

