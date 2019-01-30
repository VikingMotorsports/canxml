<%bus_name = xmlroot.attrib.get('name').lower()%>
#ifndef ${bus_name.upper()}_H
#define ${bus_name.upper()}_H

#include <stdint.h>

% for message in xmlroot.findall('message'):
<%
message_name = message.attrib.get('name')
message_id = int(message.attrib.get('id'), 16)
%>
#define ${message_name}_SID ${hex(message_id)}
#define ${message_name}_DLC 8

struct ${message_name}_t {
    % for signal in message.findall('signal'):
    <%
    signal_name = signal.attrib.get('name')
    signal_len = int(signal.attrib.get('len'))
    if signal_len <= 8:
        signal_size = 8
    elif signal_len <= 16:
        signal_size = 16
    elif signal_len <= 32:
        signal_size = 32
    elif signal_len <= 64:
        signal_size = 64
    %>\
    uint${signal_size}_t ${signal_name};
    % endfor
};
% endfor

#endif // ${bus_name.upper()}_H
