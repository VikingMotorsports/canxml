import argparse
import xml.etree.ElementTree as ET
from mako.template import Template
from mako.runtime import Context
from StringIO import StringIO

# Output files;
# <busname>.h
# <busname>_<arch>.h
# <busname>_<arch>.c

def test():
    return "Hello, World!"

def main():
    argparser = argparse.ArgumentParser()
    argparser.add_argument('xml_file', help='XML specification of the bus')
    argparser.add_argument('template_file', help='Mako template file')
    argparser.add_argument('word_size', nargs='?', default=32, help='Machine word size')
    args = argparser.parse_args()

    xmltree = ET.parse(args.xml_file)
    root = xmltree.getroot()

    bus_template = Template(filename=args.template_file)
    buf = StringIO()
    ctx = Context(buf, xmlroot = root, wordsize=int(args.word_size))
    bus_template.render_context(ctx)
    print(buf.getvalue())

if __name__ == "__main__":
    main()
