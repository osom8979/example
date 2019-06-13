
#ifndef __INCLUDE__DATA_TO_STRING__msg_H__
#define __INCLUDE__DATA_TO_STRING__msg_H__

char const * __data_to_string__msg__[] = {
        R"_msg_(// tbag packet protocols.

namespace libtbag.proto.fbs.msg;

table MsgPacket
{
    event : int;
    data  : [byte];
}

root_type MsgPacket;

)_msg_",
        0
    };

#endif /* __INCLUDE__DATA_TO_STRING__msg_H__ */
