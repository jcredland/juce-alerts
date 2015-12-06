#!/usr/bin/env python
#
# Credland Technical Limited.  Contact Jim; contact@credland.net.
#
# This code takes the variables.less file from the FontAwesome
# repo and converts it into a C++ class with constants defined 
# for all the glyphs
#
import fileinput
import re
import sys

def convert_name_to_camelcase(name):
    formatted_name = ""
    should_uppercase = True
    for c in name:
        if c is "-":
            should_uppercase = True
        else:
            if should_uppercase:
                formatted_name += c.upper()
                should_uppercase = False
            else:
                formatted_name += c

    return formatted_name

def process(line):
    if not "@fa-var-" in line:
        return

    r = re.search(r'@fa-var-(.*): "\\([0-9a-f]+)";', line)

    if not r:
        print "Error: format not recognised"
        sys.exit(1)

    name = convert_name_to_camelcase(r.groups()[0])
    unicode_hex = r.groups()[1]

    code = "    static const int fa" + name
    code += " = 0x" + unicode_hex + ";"
    print code


print "class FontAwesomeIcons {"
print "public:"

for line in fileinput.input():
        process(line)


print "};"
