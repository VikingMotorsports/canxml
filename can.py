import argparse
import xml.etree.ElementTree as ET
from mako.template import Template
from mako.runtime import Context
from StringIO import StringIO

# Possible output files;
# <busname>.h : Node-specific driver header
# <busname>.c : Node-specific driver code
# <busname>_messages.h : Generic message header
# <busname>_pack.h : Somewhat-portable message packing functions

def main():
    argparser = argparse.ArgumentParser()
    argparser.add_argument('xml_file', help='XML specification of the bus')
    argparser.add_argument('template_file', help='Mako template file')
    argparser.add_argument('node_name', nargs='?', default=32, help='Node name')
    args = argparser.parse_args()

    xmltree = ET.parse(args.xml_file)
    root = xmltree.getroot()

    bus_template = Template(filename=args.template_file)
    buf = StringIO()
    ctx = Context(buf, xmlroot = root, node_name = args.node_name)
    bus_template.render_context(ctx)
    print(buf.getvalue())

if __name__ == "__main__":
    main()
