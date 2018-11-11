#! /usr/bin/python
#
# convert the swig -debug-lsymbols output text file format into
# a simple list of lua module names and classes
#
# Dan Wilcox <danomatika@gmail.com> 2017
#
import sys
import re

if len(sys.argv) < 2:
    print("USAGE: lua_syntax.py MODULENAME INFILE")
    exit(0)

module = sys.argv[1]
infile = sys.argv[2]
sections = []
sectionMatches = [
    "string",           # swig std::string wrappers
    "string.SwigStatic" # swig std::string wrappers
]
sectionEnds = [
    "Vector" # swig std::vector wrappers
]
lineMatches = [ 
    "string",    # swig std::string wrappers
    "lua:cdata", # c pointers
]
lineStarts = [
    "~",  # destructors
    "__", # lua metatable __add, __sub, etc
    "of", # of core type prefixes
    "ofx" # ofx addon type prefixes
]
lineEnds = [
    "Vector" # swig std::vector wrappers
]

# any other user-supplied section ignores
for arg in sys.argv[3:]:
    sectionIgnores.append(arg)

# check if a string matches one in an array
def matches(needle, haystack):
    for straw in haystack:
        if needle == straw:
            return True
    return False

# append a section to the sections array if the name passes muster
def appendSection(section):
    # drop static classes which don't have any symbols
    if len(section) < 2:
        return
    # section names are followed by a " -", so double check
    if not section[0].endswith("-"):
        print("warning: section name does not end with -: "+section[0])
        return
    # grab first non-whitespace name ie. "Color" from "Color -"
    match = re.match("\S+", section[0])
    if match:
        if section[0] == "-": # main module is just a "-"
            section[0] = module
        else: # class name
            section[0] = match.group(0)
    else:
        print("warning: section name had no non-whitespace match: "+section[0])
        return
    # drop sections which match certain strings
    if matches(section[0], sectionMatches):
        return
    # drop sections which contain certain strings
    if any(section[0].endswith(x) for x in sectionEnds):
        return
    # if got this far, the section must be good...
    sections.append(section)

# parse swig output into sections
file = open(infile)
section = []
for line in file:
    # strip whitespace
    line = line.strip()
    # ignore beginning and end lines
    if line.startswith("LANGUAGE"):
        continue
    # section headers are a series of = chars, ie. ==========
    if line.startswith("="):
        appendSection(section)
        section = []
    # append line within a section
    else:
        # empty line
        if len(line) == 0:
            continue
        # drop lines with certain prefixes
        if any(line.startswith(x) for x in lineStarts):
            continue
        # drop lines with certain suffixes
        if any(line.endswith(x) for x in lineEnds):
            continue
        # drop lines which match certain strings
        if matches(line, lineMatches):
            continue
        # line must be good
        section.append(line)
appendSection(section) # catch any left overs
file.close()
section = []

# for section in sections:
#     print(section)
# exit(0)

# output module & section names to each section line
file = open(module+"_syntax.txt", "w")
num = 0
for section in sections:

    # grab name from first line and output
    prefix = "  "
    name = section[0]
    if name == module: # main module
        prefix = module+"."
        file.write(module+"\n")
    elif name.endswith(".SwigStatic"): # static members
        name = name.split(".")[0] # drop SwigStatic suffix
        prefix = module+"."+name+"."
    else: # class instance members
        file.write(module+"."+name+"\n")

    # sort remaining lines
    lines = section[1:]
    lines.sort()

    # output with module.class prefix
    for line in lines:
        if not line.endswith(".SwigStatic"): # drop statics from main module
            file.write(prefix+line+"\n")
    num = num + 1

    # linebreak between sections
    if num < len(sections):
        file.write("\n")
file.close()
