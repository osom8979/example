/**
 * @file   NetStreamClient.cpp
 * @brief  NetStreamClient class implementation.
 * @author zer0
 * @date   2018-12-09
 */

#include <libtbag/net/socket/NetStreamClient.hpp>
#include <libtbag/debug/Assert.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/mq/node/MqStreamClient.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace net    {
namespace socket {

using MqType     = NetStreamClient::MqType;
using MqEvent    = NetStreamClient::MqEvent;
using MqParams   = NetStreamClient::MqParams;
using MqInternal = NetStreamClient::MqInternal;

void onConnectCb(void * parent)
{
    auto * sock = (NetStreamClient*)parent;
    assert(sock != nullptr);
    if (sock->_callbacks.begin_cb) {
        sock->_callbacks.begin_cb();
    }
}

void onCloseCb(void * parent)
{
    auto * sock = (NetStreamClient*)parent;
    assert(sock != nullptr);
    if (sock->_callbacks.end_cb) {
        sock->_callbacks.end_cb();
    }
}

std::size_t onWriteCb(void * node, MqEvent event, char const * buffer, std::size_t size, void * parent)
{
    using MqStreamClient = libtbag::mq::node::MqStreamClient;

    auto * sock = (NetStreamClient*)parent;
    assert(sock != nullptr);

    Err code;
    if (sock->type() == MqType::MT_PIPE) {
        auto * tcp = (MqStreamClient::TcpClient*)node;
        code = tcp->write(tcp->write_req, buffer, size);
    } else if (sock->type() == MqType::MT_TCP) {
        auto * pipe = (MqStreamClient::PipeClient*)node;
        code = pipe->write(pipe->write_req, buffer, size);
    } else {
        TBAG_INACCESSIBLE_BLOCK_ASSERT();
        return 0;
    }

    if (isFailure(code)) {
        tDLogE("onWriteCb() Write error: {}", code);
    }
    return isSuccess(code) ? 1U : 0U;
}

void onReadCb(void * node, char const * buffer, std::size_t size, void * parent)
{
    auto * sock = (NetStreamClient*)parent;
    assert(sock != nullptr);
    if (sock->_callbacks.recv_cb) {
        sock->_callbacks.recv_cb(buffer, size);
    }
}

// ------------------------------
// NetStreamClient implementation
// ------------------------------

NetStreamClient::NetStreamClient(MqParams const & params, Callbacks const & cbs)
        : libtbag::mq::MqConn(params, no_init), _callbacks(cbs)
{
    MqInternal internal;
    internal.active_cb     = &onConnectCb;
    internal.close_cb      = &onCloseCb;
    internal.default_write = &onWriteCb;
    internal.default_read  = &onReadCb;
    internal.parent        = this;

    if (!create(internal)) {
        throw std::bad_alloc();
    }
}

NetStreamClient::NetStreamClient(std::string const & uri, Callbacks const & cbs)
        : NetStreamClient(getParams(uri), cbs)
{
    // EMPTY.
}

NetStreamClient::~NetStreamClient()
{
    // [IMPORTANT]
    // The callback member of the derived class is called after the destructor call of the base class.
    // At this time, you must call destroy() to prevent callback members from being destroyed before the Base class.
    destroy();
}

} // namespace socket
} // namespace net

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

