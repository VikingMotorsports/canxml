<%
node_name = arg_node_name.lower()
bus_name = xmlroot.attrib.get('name').lower()
for node_element in xmlroot.iter('node'):
    assert(node_element.get('name') is not None)
    if node_element.get('name') == node_name:
        node = node_element
assert(node is not None)
%>\
#ifndef ${bus_name.upper()}_${node_name.upper()}_H
#define ${bus_name.upper()}_${node_name.upper()}_H

#include <stdint.h>

#include "${bus_name}.h"

extern uint16_t ${bus_name}_buffer[][8];

struct ${bus_name}_subscriptions_t {
    struct CAN_Test_Message_t CAN_Test_Message;
};

int ${bus_name}_init(enum ecan_speed, enum ecan_mode);

int ${bus_name}_check_subscriptions(struct ${bus_name}_subscriptions_t *);

int ${bus_name}_publish_CAN_Test_Message(struct CAN_Test_Message_t *);

#endif // ${bus_name.upper()}_${node_name.upper()}_H
