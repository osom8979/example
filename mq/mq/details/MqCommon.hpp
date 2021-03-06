/**
 * @file   MqCommon.hpp
 * @brief  MqCommon class prototype.
 * @author zer0
 * @date   2018-11-10
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_MQ_DETAILS_MQCOMMON_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_MQ_DETAILS_MQCOMMON_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/util/BufferInfo.hpp>
#include <libtbag/type/TypeTable.hpp>
#include <libtbag/uvpp/Loop.hpp>
#include <libtbag/Unit.hpp>

#include <cassert>
#include <cstdint>
#include <cstdlib>

#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <utility>
#include <type_traits>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace mq      {
namespace details {

/**
 * Message event number.
 *
 * @remarks
 *  Negative integers are used by the system.
 *  Custom events use positive integers.
 */
using MqEvent = int32_t;

TBAG_CONSTEXPR MqEvent const ME_MSG   = 0;
TBAG_CONSTEXPR MqEvent const ME_CLOSE = -1000;

inline char const * const getEventName(MqEvent event) TBAG_NOEXCEPT
{
    switch (event) {
    // clang-format off
    case ME_MSG:   return "MSG";
    case ME_CLOSE: return "ME_CLOSE";
    // clang-format on
    default:
        if (event > ME_MSG) {
            return "USER";
        } else {
            return "UNKNOWN";
        }
    }
}

TBAG_CONSTEXPR MqEvent createUserMqEvent(unsigned short number) TBAG_NOEXCEPT
{
    return static_cast<MqEvent>(ME_MSG + number);
}

enum class MqType : int
{
    MT_NONE,
    MT_LOCAL,
    MT_PIPE,
    MT_UDP,
    MT_TCP,
};

TBAG_CONSTEXPR char const * const LOCAL_UPPER_NAME = "LOCAL";
TBAG_CONSTEXPR char const * const  PIPE_UPPER_NAME = "PIPE";
TBAG_CONSTEXPR char const * const   UDP_UPPER_NAME = "UDP";
TBAG_CONSTEXPR char const * const   TCP_UPPER_NAME = "TCP";

inline char const * const getTypeName(MqType type) TBAG_NOEXCEPT
{
    switch (type) {
    case MqType::MT_NONE:  return "NONE";
    case MqType::MT_LOCAL: return LOCAL_UPPER_NAME;
    case MqType::MT_PIPE:  return PIPE_UPPER_NAME;
    case MqType::MT_UDP:   return UDP_UPPER_NAME;
    case MqType::MT_TCP:   return TCP_UPPER_NAME;
    default:               return "UNKNOWN";
    }
}

enum class MqMode : int
{
    MM_NONE,
    MM_BIND,
    MM_CONNECT,
};

struct MqBindMode
{
    TBAG_CONSTEXPR static char const * const name = "BIND";
    TBAG_CONSTEXPR static MqMode const mode = MqMode::MM_BIND;
};

struct MqConnectMode
{
    TBAG_CONSTEXPR static char const * const name = "CONNECT";
    TBAG_CONSTEXPR static MqMode const mode = MqMode::MM_CONNECT;
};

// clang-format off
template <typename T> struct IsMqMode      : public std::false_type { /* EMPTY. */ };
template <> struct IsMqMode<MqBindMode>    : public std::true_type  { /* EMPTY. */ };
template <> struct IsMqMode<MqConnectMode> : public std::true_type  { /* EMPTY. */ };
// clang-format on

TBAG_CONSTEXPR MqBindMode    const MQ_BIND    = MqBindMode{};
TBAG_CONSTEXPR MqConnectMode const MQ_CONNECT = MqConnectMode{};

inline char const * const getModeName(MqMode mode) TBAG_NOEXCEPT
{
    switch (mode) {
    case MqMode::MM_NONE:    return "NONE";
    case MqMode::MM_BIND:    return MqBindMode::name;
    case MqMode::MM_CONNECT: return MqConnectMode::name;
    default:                 return "UNKNOWN";
    }
}

enum class MqRequestState
{
    MRS_WAITING,
    MRS_ASYNC,
    MRS_REQUESTING,
};

inline char const * const getRequestStateName(MqRequestState state) TBAG_NOEXCEPT
{
    switch (state) {
    case MqRequestState::MRS_WAITING:    return "WAITING";
    case MqRequestState::MRS_ASYNC:      return "ASYNC";
    case MqRequestState::MRS_REQUESTING: return "REQUESTING";
    default:                             return "UNKNOWN";
    }
}

enum class MqMachineState
{
    MMS_CLOSED,

    /**
     * Initializing...
     */
    MMS_INITIALIZING,

    /**
     * Active machine state.
     *
     * @remarks
     *  - client: The socket is connect state.
     *  - server: The socket is bind state.
     */
    MMS_ACTIVE,

    /**
     * Obtain the close request message.
     */
    MMS_CLOSING,
};

inline char const * const getMachineStateName(MqMachineState state) TBAG_NOEXCEPT
{
    switch (state) {
    case MqMachineState::MMS_CLOSED:        return "CLOSED";
    case MqMachineState::MMS_INITIALIZING:  return "INITIALIZING";
    case MqMachineState::MMS_ACTIVE:        return "ACTIVE";
    case MqMachineState::MMS_CLOSING:       return "CLOSING";
    default:                                return "UNKNOWN";
    }
}

inline bool isActiveState(MqMachineState state) TBAG_NOEXCEPT
{
    return state == MqMachineState::MMS_ACTIVE;
}

inline bool isActiveOrClosingState(MqMachineState state) TBAG_NOEXCEPT
{
    return state == MqMachineState::MMS_ACTIVE || state == MqMachineState::MMS_CLOSING;
}

inline bool isInitializeState(MqMachineState state) TBAG_NOEXCEPT
{
    return state == MqMachineState::MMS_INITIALIZING;
}

inline bool isClosingState(MqMachineState state) TBAG_NOEXCEPT
{
    return state == MqMachineState::MMS_CLOSING;
}

/**
 * MessageQueue data packet.
 *
 * @author zer0
 * @date   2018-11-13
 */
struct MqMsg
{
    using Buffer = libtbag::util::Buffer;
    using Value  = Buffer::value_type;

    static_assert(sizeof(Buffer::value_type) == 1,
                  "The minimum unit of the buffer element must be 1 Byte.");

    /**
     * The event identifier.
     */
    MqEvent event = ME_MSG;

    /**
     * The data buffer corresponding to the event.
     */
    Buffer buffer;

    /**
     * Integer pointer to the stream to be connected to the socket.
     */
    std::intptr_t stream = 0;

    MqMsg()
    { /* EMPTY. */ }

    explicit MqMsg(MqEvent e, std::intptr_t id = 0)
            : event(e), buffer(), stream(id)
    { /* EMPTY. */ }
    explicit MqMsg(std::size_t s, std::intptr_t id = 0)
            : event(ME_MSG), buffer(s), stream(id)
    { /* EMPTY. */ }
    explicit MqMsg(MqEvent e, std::size_t s, std::intptr_t id = 0)
            : event(e), buffer(s), stream(id)
    { /* EMPTY. */ }

    explicit MqMsg(Value const * d, std::size_t s, std::intptr_t id = 0)
            : event(ME_MSG), buffer(d, d + s), stream(id)
    { /* EMPTY. */ }
    explicit MqMsg(MqEvent e, Value const * d, std::size_t s, std::intptr_t id = 0)
            : event(e), buffer(d, d + s), stream(id)
    { /* EMPTY. */ }

    explicit MqMsg(std::string const & str, std::intptr_t id = 0)
            : event(ME_MSG), buffer(str.begin(), str.end()), stream(id)
    { /* EMPTY. */ }
    explicit MqMsg(MqEvent e, std::string const & str, std::intptr_t id = 0)
            : event(e), buffer(str.begin(), str.end()), stream(id)
    { /* EMPTY. */ }

    explicit MqMsg(Buffer const & buffer, std::intptr_t id = 0)
            : event(ME_MSG), buffer(buffer), stream(id)
    { /* EMPTY. */ }
    explicit MqMsg(MqEvent e, Buffer const & buf, std::intptr_t id = 0)
            : event(e), buffer(buf), stream(id)
    { /* EMPTY. */ }

    MqMsg(MqMsg const & obj)
    { *this = obj; }
    MqMsg(MqMsg && obj) TBAG_NOEXCEPT
    { *this = std::move(obj); }

    ~MqMsg()
    { /* EMPTY. */ }

    MqMsg & operator =(MqMsg const & obj)
    {
        if (this != &obj) {
            stream = obj.stream;
            event  = obj.event;
            buffer = obj.buffer;
        }
        return *this;
    }

    MqMsg & operator =(MqMsg && obj) TBAG_NOEXCEPT
    {
        if (this != &obj) {
            std::swap(stream, obj.stream);
            std::swap(event, obj.event);
            buffer.swap(obj.buffer);
        }
        return *this;
    }

    inline char * data() TBAG_NOEXCEPT_SP_OP(buffer.data())
    { return buffer.data(); }
    inline char const * data() const TBAG_NOEXCEPT_SP_OP(buffer.data())
    { return buffer.data(); }

    inline bool empty() const TBAG_NOEXCEPT_SP_OP(buffer.empty())
    { return buffer.empty(); }
    inline std::size_t size() const TBAG_NOEXCEPT_SP_OP(buffer.size())
    { return buffer.size(); }

    inline Buffer::iterator begin() TBAG_NOEXCEPT
    { return buffer.begin(); }
    inline Buffer::const_iterator begin() const TBAG_NOEXCEPT
    { return buffer.begin(); }

    inline Buffer::iterator end() TBAG_NOEXCEPT
    { return buffer.end(); }
    inline Buffer::const_iterator end() const TBAG_NOEXCEPT
    { return buffer.end(); }
};

/**
 * Message copy from.
 *
 * @author zer0
 * @date   2018-11-13
 */
struct TBAG_API MqMsgCopyFrom
{
    MqMsg const & source_msg;

    MqMsgCopyFrom(MqMsg const & msg);
    ~MqMsgCopyFrom();

    bool operator()(MqMsg * msg);
};

/**
 * Message copy to.
 *
 * @author zer0
 * @date   2018-11-13
 */
struct TBAG_API MqMsgCopyTo
{
    MqMsg & destination_msg;

    MqMsgCopyTo(MqMsg & msg);
    ~MqMsgCopyTo();

    bool operator()(MqMsg * msg);
};

enum class MqIsConsume
{
    MIC_PASS,
    MIC_CONSUMED,
};

// Forward declaration.
struct MqInterface;
struct MqInternal;
struct MqParams;

/**
 * Active event.
 *
 * @warning
 *  - stream client: connect socket.
 *  - stream server: bind socket.
 *  - local queue: immediately in the constructor.
 */
using MqOnActive = void(*)(void * parent);

/**
 * Accept filter.
 *
 * @warning
 *  Used only on the server.
 *
 * @remarks
 *  The reason for not using peer as <code>sockaddr</code> is to accommodate the Named-PIPE path as well.
 */
using MqOnAccept = bool(*)(void * node, std::string const & peer, void * parent);

/**
 * Message filter.
 */
using MqOnWrite = MqIsConsume(*)(MqMsg & msg, void * parent);

/**
 * Callback prototype for intercepting Receive events.
 */
using MqOnRecv = MqIsConsume(*)(MqMsg const & msg, void * parent);

/**
 * Close callback.
 */
using MqOnClose = void(*)(void * parent);

/**
 * It operates in the default write mode.
 *
 * @remarks
 *  - client: node is the client stream.
 *  - server: node is the node stream.
 *
 * @return
 *  The number of successful packet transmissions should be returned.
 */
using MqOnDefaultWrite = std::size_t(*)(void * node, MqEvent event, char const * buffer, std::size_t size, void * parent);

/**
 * It operates in the default read mode.
 *
 * @remarks
 *  - client: node is the client stream.
 *  - server: node is the node stream.
 */
using MqOnDefaultRead = void(*)(void * node, char const * buffer, std::size_t size, void * parent);

/**
 * When the node is closed.
 *
 * @warning
 *  Used only on the server.
 */
using MqOnCloseNode = void(*)(void * node, void * parent);

/**
 * Internal option packs.
 */
struct MqInternal
{
    /**
     * Active event callback.
     */
    MqOnActive active_cb = nullptr;

    /**
     * Clients connected to the server are filtered out.
     */
    MqOnAccept accept_cb = nullptr;

    /**
     * Give the user a chance to filter the message.
     */
    MqOnWrite write_cb = nullptr;

    /**
     * Use the receive callback.
     *
     * @remarks
     *  When this option is set, it no longer enqueues to the recv-queue.
     */
    MqOnRecv recv_cb = nullptr;

    /**
     * All work is done.
     */
    MqOnClose close_cb = nullptr;

    /**
     * Use this option to write the original of the packet.
     *
     * @warning
     *  Setting this option will no longer use the default behavior of MessageQueue.
     */
    MqOnDefaultWrite default_write = nullptr;

    /**
     * Use this option to read the original of the packet.
     *
     * @warning
     *  Setting this option will no longer use the default behavior of MessageQueue.
     */
    MqOnDefaultRead default_read = nullptr;

    /**
     * Called when the client is closed on the server socket.
     *
     * @warning
     *  Used only on the server.
     */
    MqOnCloseNode close_node = nullptr;

    /**
     * Parent object instance pointer.
     */
    void * parent = nullptr;
};

TBAG_CONSTEXPR std::size_t const WAIT_ON_ACTIVATION_INFINITY
        = libtbag::type::TypeInfo<std::size_t>::maximum();
TBAG_CONSTEXPR std::size_t const RECONNECT_INFINITY = 0;

/**
 * Event for loop creation.
 */
using MqOnCreateLoop = void(*)(libtbag::uvpp::Loop & loop, MqParams const & params);

/**
 * User customizable option packs.
 */
struct TBAG_API MqParams
{
    /**
     * Type of stream. must be TCP or PIPE.
     */
    MqType type = MqType::MT_TCP;

    /**
     * Bind or Connect address.
     *
     * @remarks
     *  - tcp: bind socket address.
     *  - pipe: pipe file path.
     */
    std::string address;

    /**
     * Bind port number.
     *
     * @remarks
     *  - tcp: port number.
     *  - pipe: unused.
     */
    int port = 0;

    /**
     * Used with uv_tcp_bind, when an IPv6 address is used.
     *
     * @remarks
     *  - tcp: use this flag.
     *  - pipe: unused.
     */
    bool tcp_ipv6_only = false;

    /**
     * If a pipe file exists, remove it first.
     *
     * @remarks
     *  - tcp: unused.
     *  - pipe: Used in server only.
     */
    bool remove_exists_pipe = true;

    /**
     * The maximum size of the queue for transmission.
     *
     * @warning
     *  This value must be a power of 2.
     */
    std::size_t send_queue_size = 1024;

    /**
     * The default size of the transmission message packet.
     *
     * @remarks
     *  If memory is insufficient, it will be more expanded.
     */
    std::size_t send_msg_size = 256;

    /**
     * The maximum size of the queue for receive
     *
     * @warning
     *  This value must be a power of 2.
     */
    std::size_t recv_queue_size = 1024;

    /**
     * The default size of the receive message packet.
     *
     * @remarks
     *  If memory is insufficient, it will be more expanded.
     */
    std::size_t recv_msg_size = 256;

    /**
     * The number of clients that can be accepted.
     */
    std::size_t max_nodes = 100000;

    /**
     * Temporary buffer size for serialization.
     *
     * @remarks
     *  This value is used by the Builder in FlatBuffers.
     */
    std::size_t packer_size = 1 * 1024 * 1024;

    /**
     * Wait time to closing. If this value is 0, close immediately.
     *
     * @remarks
     *  If you request a shutdown directly, You need time to wait for an idle recv request.
     */
    std::size_t wait_closing_millisec = 1 * 1000;

    /**
     * Verify the restore message.
     */
    bool verify_restore_message = false;

    /**
     * If the consecutive read error is maximum,
     * the connection is forced to close.
     */
    std::size_t continuous_read_error_count = 4;

    /**
     * Connect timeout.
     *
     * @remarks
     *  - Used in client only.
     */
    std::size_t connect_timeout_millisec = 4 * 1000;

    /**
     * Reconnect count.
     *
     * @remarks
     *  If this variable is 0, it tries infinitely.
     */
    std::size_t reconnect_count = 1;

    /**
     * Reconnect delay timer.
     */
    std::size_t reconnect_delay_millisec = 1 * 1000;

    /**
     * Wait until activation is completed.
     *
     * @remarks
     *  This variable is used by the frontend thread to check that the background thread's loop is active. @n
     *  If this variable is 0, disable this feature.
     */
    std::size_t wait_on_activation_timeout_millisec = WAIT_ON_ACTIVATION_INFINITY;

    /**
     * The wait time to not miss the send() requested by another thread.
     *
     * @remarks
     *  At least 10 nanoseconds is recommended.
     */
    std::size_t wait_next_opcode_nanosec = 1000;

    /**
     * Verbose log message.
     */
    bool verbose = false;

    /**
     * Space for user-defined arbitrary data.
     */
    void * user = nullptr;

    /**
     * Provides a callback that allows the user to control the loop.
     */
    MqOnCreateLoop on_create_loop = nullptr;

    MqParams();
    ~MqParams();

    void update(std::string const & uri_string, MqParams const & default_params, bool auto_encode = true);
    void update(std::string const & uri_string, bool auto_encode = true);

    void updateOnlyAddresseAndPort(std::string const & uri_string, bool auto_encode = true);
};

TBAG_CONSTEXPR static char const * const TCP_IPV6_ONLY_NAME    = "tcp_ipv6";
TBAG_CONSTEXPR static char const * const REMOVE_PIPE_NAME      = "remove_pipe";
TBAG_CONSTEXPR static char const * const SEND_QUEUE_SIZE_NAME  = "send_queue";
TBAG_CONSTEXPR static char const * const SEND_MSG_SIZE_NAME    = "send_msg";
TBAG_CONSTEXPR static char const * const RECV_QUEUE_SIZE_NAME  = "recv_queue";
TBAG_CONSTEXPR static char const * const RECV_MSG_SIZE_NAME    = "recv_msg";
TBAG_CONSTEXPR static char const * const MAX_NODES_NAME        = "max_nodes";
TBAG_CONSTEXPR static char const * const PACKER_SIZE_NAME      = "packer_size";
TBAG_CONSTEXPR static char const * const WAIT_CLOSING_NAME     = "wait_closing";
TBAG_CONSTEXPR static char const * const VERIFY_MSG_NAME       = "verify_msg";
TBAG_CONSTEXPR static char const * const READ_ERROR_NAME       = "read_error";
TBAG_CONSTEXPR static char const * const CONNECT_TIMEOUT_NAME  = "connect_timeout";
TBAG_CONSTEXPR static char const * const RECONNECT_COUNT_NAME  = "reconnect";
TBAG_CONSTEXPR static char const * const RECONNECT_DELAY_NAME  = "reconnect_delay";
TBAG_CONSTEXPR static char const * const WAIT_ACTIVATION_NAME  = "wait_activation";
TBAG_CONSTEXPR static char const * const WAIT_NEXT_OPCODE_NAME = "wait_next_opcode";
TBAG_CONSTEXPR static char const * const VERBOSE_NAME          = "verbose";

struct MqInterface
{
    MqInterface() { /* EMPTY. */ }
    virtual ~MqInterface() { /* EMPTY. */ }

    virtual Err exit() = 0;

    virtual Err send(MqMsg const & msg) = 0;
    virtual Err recv(MqMsg & msg) = 0;

    /**
     * Wait for the recv-queue to become active.
     */
    virtual Err waitEnable(uint64_t timeout_nano) = 0;
    virtual Err waitRecv(MqMsg & msg, uint64_t timeout_nano) = 0;
};

// -----------------------
// Miscellaneous utilities
// -----------------------

/**
 * @warning
 *  If it is a PIPE schema, @n
 *  you should clear the slash('/') after parsing.
 */
TBAG_API std::string __append_localhost_if_pipe_schema(std::string const & uri_string);
TBAG_API std::string __append_localhost_if_local_schema(std::string const & uri_string);
TBAG_API std::string __append_localhost_if_pipe_or_local_schema(std::string const & uri_string);

TBAG_API MqParams convertUriToParams(std::string const & uri_string, MqParams const & default_params, bool auto_encode = true);
TBAG_API MqParams convertUriToParams(std::string const & uri_string, bool auto_encode = true);

TBAG_API Err waitOnActivation(MqParams const & params, MqInterface * mq);

} // namespace details
} // namespace mq

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MQ_DETAILS_MQCOMMON_HPP__

