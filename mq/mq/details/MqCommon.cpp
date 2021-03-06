/**
 * @file   MqCommon.cpp
 * @brief  MqCommon class implementation.
 * @author zer0
 * @date   2018-11-10
 */

#include <libtbag/mq/details/MqCommon.hpp>
#include <libtbag/net/Ip.hpp>
#include <libtbag/net/Uri.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/log/Log.hpp>
#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace mq      {
namespace details {

// -----------------------------
// MqMsgCopyFrom implementation.
// -----------------------------

MqMsgCopyFrom::MqMsgCopyFrom(MqMsg const & msg) : source_msg(msg)
{
    // EMPTY.
}

MqMsgCopyFrom::~MqMsgCopyFrom()
{
    // EMPTY.
}

bool MqMsgCopyFrom::operator()(MqMsg * msg)
{
    assert(msg != nullptr);
    *msg = source_msg;
    return true;
}

// ---------------------------
// MqMsgCopyTo implementation.
// ---------------------------

MqMsgCopyTo::MqMsgCopyTo(MqMsg & msg) : destination_msg(msg)
{
    // EMPTY.
}

MqMsgCopyTo::~MqMsgCopyTo()
{
    // EMPTY.
}

bool MqMsgCopyTo::operator()(MqMsg * msg)
{
    destination_msg = *msg;
    return true;
}

// ------------------------
// MqParams implementation.
// ------------------------

MqParams::MqParams()
{
    // EMPTY.
}

MqParams::~MqParams()
{
    // EMPTY.
}

void MqParams::update(std::string const & uri_string, MqParams const & default_params, bool auto_encode)
{
    (*this) = convertUriToParams(uri_string, default_params, auto_encode);
}

void MqParams::update(std::string const & uri_string, bool auto_encode)
{
    (*this) = convertUriToParams(uri_string, auto_encode);
}

void MqParams::updateOnlyAddresseAndPort(std::string const & uri_string, bool auto_encode)
{
    auto const PARAMS = convertUriToParams(uri_string, auto_encode);
    address = PARAMS.address;
    port = PARAMS.port;
}

// -----------------------
// Miscellaneous utilities
// -----------------------

std::string __append_localhost_if_pipe_schema(std::string const & uri_string)
{
    if (uri_string.size() < 7) {
        return uri_string;
    }
    if (uri_string[0] != 'p' && uri_string[0] != 'P') {
        return uri_string;
    }
    if (uri_string[1] != 'i' && uri_string[1] != 'I') {
        return uri_string;
    }
    if (uri_string[2] != 'p' && uri_string[2] != 'P') {
        return uri_string;
    }
    if (uri_string[3] != 'e' && uri_string[3] != 'E') {
        return uri_string;
    }
    if (uri_string[4] != ':' && uri_string[5] != '/' && uri_string[6] != '/') {
        return uri_string;
    }

    return uri_string.substr(0, 7) +
           "localhost" +
           "/" + /* clear the slash after parsing */
           uri_string.substr(7);
}

std::string __append_localhost_if_local_schema(std::string const & uri_string)
{
    if (uri_string.size() < 8) {
        return uri_string;
    }
    if (uri_string[0] != 'l' && uri_string[0] != 'L') {
        return uri_string;
    }
    if (uri_string[1] != 'o' && uri_string[1] != 'O') {
        return uri_string;
    }
    if (uri_string[2] != 'c' && uri_string[2] != 'C') {
        return uri_string;
    }
    if (uri_string[3] != 'a' && uri_string[3] != 'A') {
        return uri_string;
    }
    if (uri_string[4] != 'l' && uri_string[4] != 'L') {
        return uri_string;
    }
    if (uri_string[5] != ':' && uri_string[6] != '/' && uri_string[7] != '/') {
        return uri_string;
    }

    return uri_string.substr(0, 8) +
           "localhost" +
           "/" + /* clear the slash after parsing */
           uri_string.substr(8);
}

std::string __append_localhost_if_pipe_or_local_schema(std::string const & uri_string)
{
    std::string result = uri_string;
    result = __append_localhost_if_pipe_schema(result);
    result = __append_localhost_if_local_schema(result);
    return result;
}

MqParams convertUriToParams(std::string const & uri_string, MqParams const & default_params, bool auto_encode)
{
    using namespace libtbag::net;
    using namespace libtbag::string;
    auto const INPUT_URI = __append_localhost_if_pipe_or_local_schema(uri_string);

    Uri uri;
    bool parse_result;
    if (auto_encode) {
        parse_result = uri.encodeParse(INPUT_URI);
    } else {
        parse_result = uri.parse(INPUT_URI);
    }

    if (!parse_result) {
        return MqParams();
    }

    MqParams params = default_params;
    auto const SCHEMA = libtbag::string::upper(uri.getSchema());
    if (SCHEMA == LOCAL_UPPER_NAME) {
        params.type = MqType::MT_LOCAL;
    } else if (SCHEMA == PIPE_UPPER_NAME) {
        params.type = MqType::MT_PIPE;
    } else if (SCHEMA == TCP_UPPER_NAME) {
        params.type = MqType::MT_TCP;
    } else if (SCHEMA == UDP_UPPER_NAME) {
        params.type = MqType::MT_UDP;
    } else {
        tDLogW("convertUriToParams() {} applies as a TCP type.", SCHEMA);
        params.type = MqType::MT_TCP;
    }

    if (params.type == MqType::MT_LOCAL || params.type == MqType::MT_PIPE) {
        params.address = uri.getPath();
        if (auto_encode) {
            params.address = Uri::decodePercent(params.address);
        }
        params.address = params.address.substr(1); // Remove the slash('/')
        params.port = 0;
    } else {
        auto const INFO = libtbag::net::getAddrInfo(uri);
        params.address = INFO.address;
        params.port = INFO.port;
    }

    std::map<std::string, std::string> queries;
    if (auto_encode) {
        queries = uri.decodeQueryMap();
    } else {
        queries = uri.getQueryMap();
    }

    auto itr = queries.find(TCP_IPV6_ONLY_NAME);
    if (itr != queries.end()) {
        params.tcp_ipv6_only = toValue<bool>(itr->second);
    }

    itr = queries.find(REMOVE_PIPE_NAME);
    if (itr != queries.end()) {
        params.remove_exists_pipe = toValue<bool>(itr->second);
    }

    itr = queries.find(SEND_QUEUE_SIZE_NAME);
    if (itr != queries.end()) {
        params.send_queue_size = toValue<unsigned>(itr->second);
    }

    itr = queries.find(SEND_MSG_SIZE_NAME);
    if (itr != queries.end()) {
        params.send_msg_size = toValue<unsigned>(itr->second);
    }

    itr = queries.find(RECV_QUEUE_SIZE_NAME);
    if (itr != queries.end()) {
        params.recv_queue_size = toValue<unsigned>(itr->second);
    }

    itr = queries.find(RECV_MSG_SIZE_NAME);
    if (itr != queries.end()) {
        params.recv_msg_size = toValue<unsigned>(itr->second);
    }

    itr = queries.find(MAX_NODES_NAME);
    if (itr != queries.end()) {
        params.max_nodes = toValue<unsigned>(itr->second);
    }

    itr = queries.find(PACKER_SIZE_NAME);
    if (itr != queries.end()) {
        params.packer_size = toValue<unsigned>(itr->second);
    }

    itr = queries.find(WAIT_CLOSING_NAME);
    if (itr != queries.end()) {
        params.wait_closing_millisec = toValue<unsigned>(itr->second);
    }

    itr = queries.find(VERIFY_MSG_NAME);
    if (itr != queries.end()) {
        params.verify_restore_message = toValue<bool>(itr->second);
    }

    itr = queries.find(READ_ERROR_NAME);
    if (itr != queries.end()) {
        params.continuous_read_error_count = toValue<unsigned>(itr->second);
    }

    itr = queries.find(CONNECT_TIMEOUT_NAME);
    if (itr != queries.end()) {
        params.connect_timeout_millisec = toValue<unsigned>(itr->second);
    }

    itr = queries.find(RECONNECT_COUNT_NAME);
    if (itr != queries.end()) {
        params.reconnect_count = toValue<unsigned>(itr->second);
    }

    itr = queries.find(RECONNECT_DELAY_NAME);
    if (itr != queries.end()) {
        params.reconnect_delay_millisec = toValue<unsigned>(itr->second);
    }

    itr = queries.find(WAIT_ACTIVATION_NAME);
    if (itr != queries.end()) {
        params.wait_on_activation_timeout_millisec = toValue<unsigned>(itr->second);
    }

    itr = queries.find(WAIT_NEXT_OPCODE_NAME);
    if (itr != queries.end()) {
        params.wait_next_opcode_nanosec = toValue<bool>(itr->second);
    }

    itr = queries.find(VERBOSE_NAME);
    if (itr != queries.end()) {
        params.verbose = toValue<bool>(itr->second);
    }

    return params;
}

MqParams convertUriToParams(std::string const & uri_string, bool auto_encode)
{
    return convertUriToParams(uri_string, MqParams(), auto_encode);
}

Err waitOnActivation(MqParams const & params, MqInterface * mq)
{
    if (params.wait_on_activation_timeout_millisec == 0) {
        return E_ALREADY;
    }

    Err code;
    if (params.wait_on_activation_timeout_millisec == WAIT_ON_ACTIVATION_INFINITY) {
        tDLogIfI(params.verbose, "waitOnActivation() Waiting enable: infinity ...");
        code = mq->waitEnable(0);
    } else {
        auto const TIMEOUT = params.wait_on_activation_timeout_millisec * MILLISECONDS_TO_NANOSECONDS;
        tDLogIfI(params.verbose, "waitOnActivation() Waiting enable: {}ms ...",
                 params.wait_on_activation_timeout_millisec);
        code = mq->waitEnable(TIMEOUT);
    }

    if (isSuccess(code)) {
        tDLogIfD(params.verbose, "waitOnActivation() Wait done.");
    } else {
        assert(code == E_TIMEOUT);
        tDLogW("waitOnActivation() Connection timeout.");
    }

    return code;
}

} // namespace details
} // namespace mq

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

