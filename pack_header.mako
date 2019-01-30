<%
bus_name = xmlroot.attrib.get('name').lower()

def boundary_size(start, end):
    res = 8
    while start//res != end//res:
        res *= 2
    return res

def minimum_size(len):
    size = 0
    for i in range(3, 6):
        if len <= 2**i:
            return 2**i
    assert(false)
    
%>
#ifndef ${bus_name.upper()}_PACK_H
#define ${bus_name.upper()}_PACK_H

#include <stdint.h>

#include "${bus_name}.h"

#define MASK(n) ((1UL<<(n))-1)

% for message in xmlroot.findall('message'):
<%
message_name = message.attrib.get('name')
%>\
void
${bus_name}_pack_${message_name}(void *data, struct ${message_name}_t *m)
{
    memset(data, 0, ${message_name}_DLC);
    % for signal in message.findall('signal'):
<%
    signal_name = signal.attrib.get('name')
    signal_len = int(signal.attrib.get('len'))
    signal_size = minimum_size(signal_len)
    signal_start = 0
    if 'start' in signal.attrib:
        signal_start += 8 * int(signal.attrib.get('start'))
    if 'sbit' in signal.attrib:
        signal_start += int(signal.attrib.get('sbit'))
    signal_end = signal_start + int(signal.attrib.get('len')) - 1
    boundary = boundary_size(signal_start, signal_end)
    masked = True
    aligned = False
    if signal_len == signal_size:
        masked = False
    if signal_start % signal_size == 0:
        aligned = True
    index = signal_start//boundary
    offset = signal_start % boundary
%>\
    % if aligned and not masked:
    *((uint${boundary}_t *)data + ${index}) = (uint${boundary}_t)m->${signal_name};
    % elif not masked:
    *((uint${boundary}_t *)data + ${index}) |= ((uint${boundary}_t)m->${signal_name}) << ${offset};
    % else:
    *((uint${boundary}_t *)data + ${index}) |= ((uint${boundary}_t)m->${signal_name} & MASK(${signal_len}) << ${offset};
    % endif
    % endfor
}

void
${bus_name}_unpack_${message_name}(void *data, struct ${message_name}_t *m)
{
    % for signal in message.findall('signal'):
<%
    signal_name = signal.attrib.get('name')
    signal_len = int(signal.attrib.get('len'))
    signal_size = minimum_size(signal_len)
    signal_start = 0
    if 'start' in signal.attrib:
        signal_start += 8 * int(signal.attrib.get('start'))
    if 'sbit' in signal.attrib:
        signal_start += int(signal.attrib.get('sbit'))
    signal_end = signal_start + int(signal.attrib.get('len')) - 1
    boundary = boundary_size(signal_start, signal_end)
    masked = True
    aligned = False
    if signal_len == signal_size:
        masked = False
    if signal_start % signal_size == 0:
        aligned = True
    index = signal_start//boundary
    offset = signal_start % boundary
%>\
    % if aligned and not masked:
    m->${signal_name} = *((uint${boundary}_t *)data + ${index});
    % elif not masked:
    m->${signal_name} = *((uint${boundary}_t *)data + ${index}) >> ${offset};
    % else:
    m->${signal_name} = (*((uint${boundary}_t *)data + ${index}) >> ${offset}) & MASK(${signal_len});
    % endif
    % endfor
}
% endfor

#endif // ${bus_name.upper()}_PACK_H
