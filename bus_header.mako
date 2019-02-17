#ifndef ${bus_name.upper()}_MESSAGES_H
#define ${bus_name.upper()}_MESSAGES_H

#include <stdint.h>
% for message in messages.values():

#define ${message.name}_SID ${hex(message.id)}
#define ${message.name}_DLC 8

struct ${message.name}_t {
    % for signal in message.signals:
    uint${signal.type_len}_t ${signal.name};
    % endfor
};
% endfor

#endif // ${bus_name.upper()}_MESSAGES_H
