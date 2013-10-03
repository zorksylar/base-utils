APPNAME="b0"
VERSION="0.1"

import os
import sys
from waflib import Logs

# use clang++ as default compiler (for c++11 support on mac)
if sys.platform == 'darwin' and not os.environ.has_key("CXX"):
    os.environ["CXX"] = "clang++"

def options(opt):
    opt.load("compiler_cxx")

def configure(conf):
    conf.load("compiler_cxx")
    _enable_cxx11(conf)
    _enable_debug(conf)
    conf.env.LIB_PTHREAD = 'pthread'

def build(bld):
    bld.stlib(source=bld.path.ant_glob("b0/*.cc"), target="b0", includes="b0", lib="pthread")
    bld.program(source=bld.path.ant_glob("test/test*.cc"), target="testharness", includes=".", use="b0 PTHREAD")

#
# waf helper functions
#

def _enable_cxx11(conf):
    Logs.pprint("PINK", "C++11 features enabled")
    if sys.platform == "darwin":
        conf.env.append_value("CXXFLAGS", "-stdlib=libc++")
        conf.env.append_value("LINKFLAGS", "-stdlib=libc++")
    conf.env.append_value("CXXFLAGS", "-std=c++11")

def _enable_debug(conf):
    if os.getenv("DEBUG") in ["true", "1"]:
        Logs.pprint("PINK", "Debug support enabled")
        conf.env.append_value("CXXFLAGS", "-Wall -pthread -ggdb".split())
    else:
        conf.env.append_value("CXXFLAGS", "-Wall -pthread -O3 -ggdb -fno-omit-frame-pointer -DNDEBUG".split())
