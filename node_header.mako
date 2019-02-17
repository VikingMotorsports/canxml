<%
assert(node is not None)
%>\
#ifndef ${bus_name.upper()}_H
#define ${bus_name.upper()}_H

#include <stdint.h>
#include "${bus_name}_messages.h"

extern uint16_t ${bus_name}_buffer[][8];

struct ${bus_name}_subscriptions_t {
% for message in node.subscribes:
    struct ${message.name}_t ${message.name};
% endfor
};

int ${bus_name}_init(enum ecan_speed, enum ecan_mode);

int ${bus_name}_check_subscriptions(struct ${bus_name}_subscriptions_t *);
% for message in node.publishes:

int ${bus_name}_publish_${message.name}(struct ${message.name}_t *);
% endfor

#endif // ${bus_name.upper()}_H
