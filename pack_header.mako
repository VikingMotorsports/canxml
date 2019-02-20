#ifndef ${bus_name.upper()}_PACK_H
#define ${bus_name.upper()}_PACK_H

#include <stdint.h>
#include <string.h>
#include "${bus_name}_messages.h"

#define MASK(n) ((1UL<<(n))-1)

% for message in messages.values():
void
${bus_name}_pack_${message.name}(void *data, struct ${message.name}_t *m)
{
    memset(data, 0, ${message.name}_DLC);
% for sig in message.signals:
    % if sig.aligned and not sig.masked:
    *((uint${sig.boundary}_t *)data + ${sig.index}) = (uint${sig.boundary}_t)m->${sig.name};
    % elif not masked:
    *((uint${sig.boundary}_t *)data + ${sig.index}) |= ((uint${sig.boundary}_t)m->${sig.name}) << ${sig.offset};
    % else:
    *((uint${sig.boundary}_t *)data + ${sig.index}) |= ((uint${sig.boundary}_t)m->${sig.name} & MASK(${sig.len}) << ${sig.offset};
    % endif
% endfor
}

void
${bus_name}_unpack_${message.name}(void *data, struct ${message.name}_t *m)
{
% for sig in message.signals:
    % if sig.aligned and not sig.masked:
    m->${sig.name} = *((uint${sig.boundary}_t *)data + ${sig.index});
    % elif not masked:
    m->${sig.name} = *((uint${sig.boundary}_t *)data + ${sig.index}) >> ${sig.offset};
    % else:
    m->${sig.name} = (*((uint${sig.boundary}_t *)data + ${sig.index}) >> ${sig.offset}) & MASK(${sig.len});
    % endif
% endfor
}
% endfor

#endif // ${bus_name.upper()}_PACK_H
