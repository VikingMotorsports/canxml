import argparse
import xml.etree.ElementTree as ET
from mako.template import Template
from mako.runtime import Context
from mako import exceptions
from StringIO import StringIO


# Possible output files;
# <busname>.h : Node-specific driver header
# <busname>.c : Node-specific driver code
# <busname>_messages.h : Generic message header
# <busname>_pack.h : Somewhat-portable message packing functions

def minimum_size(len):
    size = 0
    for i in range(3, 6):
        if len <= 2**i:
            return 2**i
    assert(false)

def boundary_size(start, end):
    res = 8
    while start//res != end//res:
        res *= 2
    return res

class Signal:
    def __init__(self, xml_node):
        if xml_node.tag != 'signal':
            raise ValueError('XML node was not a valid signal definition')
        self.name = xml_node.attrib.get('name')
        self.len = int(xml_node.attrib.get('len'))
        if self.len > 32 or self.len < 1:
            raise ValueError('signal size out of bounds')
        self.type_len = minimum_size(self.len)
        self.start = 0
        if 'start' in xml_node.attrib:
            self.start += 8 * int(xml_node.attrib.get('start'))
        if 'sbit' in xml_node.attrib:
            self.start += int(xml_node.attrib.get('sbit'))
        self.end = self.start + self.len - 1
        self.boundary = boundary_size(self.start, self.end)
        self.masked = True
        self.aligned = False
        if self.len == self.type_len:
            self.masked = False
        if self.start % self.type_len == 0:
            self.aligned = True
        self.index = self.start // self.boundary
        self.offset = self.start % self.boundary
        self.values = list()

class Message:
    def __init__(self, xml_node):
        if xml_node.tag != 'message':
            raise ValueError('XML node was not a valid message definition')
        self.name = xml_node.attrib.get('name')
        self.id_str = xml_node.attrib.get('id') # keep as a string
        self.id = int(self.id_str, 0)
        if self.id < 0 or self.id >= 4096:
            raise ValueError('invalid message id: %d' % self.id)
        self.signals = [Signal(s) for s in xml_node.iter('signal')]

class Node:
    def __init__(self, xml_node, message_dict):
        if xml_node.tag != 'node':
            raise ValueError('XML node was not a valid CAN node definition')
        self.name = xml_node.attrib.get('name')
        self.publishes = [message_dict[name.strip()] for name in
                          xml_node.find('publish').text.splitlines()
                          if len(name.strip()) > 0]
        self.subscribes = [message_dict[name.strip()] for name in
                           xml_node.find('subscribe').text.splitlines()
                           if len(name.strip()) > 0]
        # message lists need to be sorted by ID
        self.publishes.sort(key=lambda m: m.id);
        self.subscribes.sort(key=lambda m: m.id);

def main():
    argparser = argparse.ArgumentParser(description='CAN driver auto-generation script.' )
    argparser.add_argument('template_file', help='Template file')
    argparser.add_argument('-s', '--spec', dest='xml_file', required=True,
                           help='Filename of the XML bus specification')
    argparser.add_argument('-n', '--node', dest='node_name', 
                           help='Name of the node for which to generate code')
    argparser.add_argument('-o', '--out', dest='output', type=argparse.FileType('w'),
                           help='Output file name')
    args = argparser.parse_args()

    xml_tree = ET.parse(args.xml_file)
    root = xml_tree.getroot()
    messages = dict()
    nodes = list()
    for xml_node in root.iter('message'):
        m = Message(xml_node)
        messages[m.name] = m
    for xml_node in root.iter('node'):
        nodes.append(Node(xml_node, messages))

    node = None
    if args.node_name:
        for n in nodes:
            if n.name == args.node_name:
                node = n

    bus_name = root.attrib.get('name').lower()

    if not args.output:
        buf = StringIO()
    else:
        buf = args.output
    ctx = Context(buf,
                  xml_root = root,
                  node = node,
                  bus_name = bus_name,
                  messages = messages)
    try:
        bus_template = Template(filename=args.template_file)
        bus_template.render_context(ctx)
    except:
        print exceptions.text_error_template().render()

    if not args.output:
        print buf.getvalue()

if __name__ == "__main__":
    main()
