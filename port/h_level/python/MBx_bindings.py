r"""Wrapper for MBx_api.h

Generated with:
E:\PY64_PROJECT\CDLL_TEST\.venv\Scripts\ctypesgen -I . -o MBx_bindings.py MBx_api.h MBx_port.h MBx_utility.h MBx_ExHeader_py.h

Do not modify this file.
"""

__docformat__ = "restructuredtext"

# Begin preamble for Python

import ctypes
import sys
from ctypes import *  # noqa: F401, F403

_int_types = (ctypes.c_int16, ctypes.c_int32)
if hasattr(ctypes, "c_int64"):
    # Some builds of ctypes apparently do not have ctypes.c_int64
    # defined; it's a pretty good bet that these builds do not
    # have 64-bit pointers.
    _int_types += (ctypes.c_int64,)
for t in _int_types:
    if ctypes.sizeof(t) == ctypes.sizeof(ctypes.c_size_t):
        c_ptrdiff_t = t
del t
del _int_types



class UserString:
    def __init__(self, seq):
        if isinstance(seq, bytes):
            self.data = seq
        elif isinstance(seq, UserString):
            self.data = seq.data[:]
        else:
            self.data = str(seq).encode()

    def __bytes__(self):
        return self.data

    def __str__(self):
        return self.data.decode()

    def __repr__(self):
        return repr(self.data)

    def __int__(self):
        return int(self.data.decode())

    def __long__(self):
        return int(self.data.decode())

    def __float__(self):
        return float(self.data.decode())

    def __complex__(self):
        return complex(self.data.decode())

    def __hash__(self):
        return hash(self.data)

    def __le__(self, string):
        if isinstance(string, UserString):
            return self.data <= string.data
        else:
            return self.data <= string

    def __lt__(self, string):
        if isinstance(string, UserString):
            return self.data < string.data
        else:
            return self.data < string

    def __ge__(self, string):
        if isinstance(string, UserString):
            return self.data >= string.data
        else:
            return self.data >= string

    def __gt__(self, string):
        if isinstance(string, UserString):
            return self.data > string.data
        else:
            return self.data > string

    def __eq__(self, string):
        if isinstance(string, UserString):
            return self.data == string.data
        else:
            return self.data == string

    def __ne__(self, string):
        if isinstance(string, UserString):
            return self.data != string.data
        else:
            return self.data != string

    def __contains__(self, char):
        return char in self.data

    def __len__(self):
        return len(self.data)

    def __getitem__(self, index):
        return self.__class__(self.data[index])

    def __getslice__(self, start, end):
        start = max(start, 0)
        end = max(end, 0)
        return self.__class__(self.data[start:end])

    def __add__(self, other):
        if isinstance(other, UserString):
            return self.__class__(self.data + other.data)
        elif isinstance(other, bytes):
            return self.__class__(self.data + other)
        else:
            return self.__class__(self.data + str(other).encode())

    def __radd__(self, other):
        if isinstance(other, bytes):
            return self.__class__(other + self.data)
        else:
            return self.__class__(str(other).encode() + self.data)

    def __mul__(self, n):
        return self.__class__(self.data * n)

    __rmul__ = __mul__

    def __mod__(self, args):
        return self.__class__(self.data % args)

    # the following methods are defined in alphabetical order:
    def capitalize(self):
        return self.__class__(self.data.capitalize())

    def center(self, width, *args):
        return self.__class__(self.data.center(width, *args))

    def count(self, sub, start=0, end=sys.maxsize):
        return self.data.count(sub, start, end)

    def decode(self, encoding=None, errors=None):  # XXX improve this?
        if encoding:
            if errors:
                return self.__class__(self.data.decode(encoding, errors))
            else:
                return self.__class__(self.data.decode(encoding))
        else:
            return self.__class__(self.data.decode())

    def encode(self, encoding=None, errors=None):  # XXX improve this?
        if encoding:
            if errors:
                return self.__class__(self.data.encode(encoding, errors))
            else:
                return self.__class__(self.data.encode(encoding))
        else:
            return self.__class__(self.data.encode())

    def endswith(self, suffix, start=0, end=sys.maxsize):
        return self.data.endswith(suffix, start, end)

    def expandtabs(self, tabsize=8):
        return self.__class__(self.data.expandtabs(tabsize))

    def find(self, sub, start=0, end=sys.maxsize):
        return self.data.find(sub, start, end)

    def index(self, sub, start=0, end=sys.maxsize):
        return self.data.index(sub, start, end)

    def isalpha(self):
        return self.data.isalpha()

    def isalnum(self):
        return self.data.isalnum()

    def isdecimal(self):
        return self.data.isdecimal()

    def isdigit(self):
        return self.data.isdigit()

    def islower(self):
        return self.data.islower()

    def isnumeric(self):
        return self.data.isnumeric()

    def isspace(self):
        return self.data.isspace()

    def istitle(self):
        return self.data.istitle()

    def isupper(self):
        return self.data.isupper()

    def join(self, seq):
        return self.data.join(seq)

    def ljust(self, width, *args):
        return self.__class__(self.data.ljust(width, *args))

    def lower(self):
        return self.__class__(self.data.lower())

    def lstrip(self, chars=None):
        return self.__class__(self.data.lstrip(chars))

    def partition(self, sep):
        return self.data.partition(sep)

    def replace(self, old, new, maxsplit=-1):
        return self.__class__(self.data.replace(old, new, maxsplit))

    def rfind(self, sub, start=0, end=sys.maxsize):
        return self.data.rfind(sub, start, end)

    def rindex(self, sub, start=0, end=sys.maxsize):
        return self.data.rindex(sub, start, end)

    def rjust(self, width, *args):
        return self.__class__(self.data.rjust(width, *args))

    def rpartition(self, sep):
        return self.data.rpartition(sep)

    def rstrip(self, chars=None):
        return self.__class__(self.data.rstrip(chars))

    def split(self, sep=None, maxsplit=-1):
        return self.data.split(sep, maxsplit)

    def rsplit(self, sep=None, maxsplit=-1):
        return self.data.rsplit(sep, maxsplit)

    def splitlines(self, keepends=0):
        return self.data.splitlines(keepends)

    def startswith(self, prefix, start=0, end=sys.maxsize):
        return self.data.startswith(prefix, start, end)

    def strip(self, chars=None):
        return self.__class__(self.data.strip(chars))

    def swapcase(self):
        return self.__class__(self.data.swapcase())

    def title(self):
        return self.__class__(self.data.title())

    def translate(self, *args):
        return self.__class__(self.data.translate(*args))

    def upper(self):
        return self.__class__(self.data.upper())

    def zfill(self, width):
        return self.__class__(self.data.zfill(width))


class MutableString(UserString):
    """mutable string objects

    Python strings are immutable objects.  This has the advantage, that
    strings may be used as dictionary keys.  If this property isn't needed
    and you insist on changing string values in place instead, you may cheat
    and use MutableString.

    But the purpose of this class is an educational one: to prevent
    people from inventing their own mutable string class derived
    from UserString and than forget thereby to remove (override) the
    __hash__ method inherited from UserString.  This would lead to
    errors that would be very hard to track down.

    A faster and better solution is to rewrite your program using lists."""

    def __init__(self, string=""):
        self.data = string

    def __hash__(self):
        raise TypeError("unhashable type (it is mutable)")

    def __setitem__(self, index, sub):
        if index < 0:
            index += len(self.data)
        if index < 0 or index >= len(self.data):
            raise IndexError
        self.data = self.data[:index] + sub + self.data[index + 1 :]

    def __delitem__(self, index):
        if index < 0:
            index += len(self.data)
        if index < 0 or index >= len(self.data):
            raise IndexError
        self.data = self.data[:index] + self.data[index + 1 :]

    def __setslice__(self, start, end, sub):
        start = max(start, 0)
        end = max(end, 0)
        if isinstance(sub, UserString):
            self.data = self.data[:start] + sub.data + self.data[end:]
        elif isinstance(sub, bytes):
            self.data = self.data[:start] + sub + self.data[end:]
        else:
            self.data = self.data[:start] + str(sub).encode() + self.data[end:]

    def __delslice__(self, start, end):
        start = max(start, 0)
        end = max(end, 0)
        self.data = self.data[:start] + self.data[end:]

    def immutable(self):
        return UserString(self.data)

    def __iadd__(self, other):
        if isinstance(other, UserString):
            self.data += other.data
        elif isinstance(other, bytes):
            self.data += other
        else:
            self.data += str(other).encode()
        return self

    def __imul__(self, n):
        self.data *= n
        return self


class String(MutableString, ctypes.Union):

    _fields_ = [("raw", ctypes.POINTER(ctypes.c_char)), ("data", ctypes.c_char_p)]

    def __init__(self, obj=b""):
        if isinstance(obj, (bytes, UserString)):
            self.data = bytes(obj)
        else:
            self.raw = obj

    def __len__(self):
        return self.data and len(self.data) or 0

    def from_param(cls, obj):
        # Convert None or 0
        if obj is None or obj == 0:
            return cls(ctypes.POINTER(ctypes.c_char)())

        # Convert from String
        elif isinstance(obj, String):
            return obj

        # Convert from bytes
        elif isinstance(obj, bytes):
            return cls(obj)

        # Convert from str
        elif isinstance(obj, str):
            return cls(obj.encode())

        # Convert from c_char_p
        elif isinstance(obj, ctypes.c_char_p):
            return obj

        # Convert from POINTER(ctypes.c_char)
        elif isinstance(obj, ctypes.POINTER(ctypes.c_char)):
            return obj

        # Convert from raw pointer
        elif isinstance(obj, int):
            return cls(ctypes.cast(obj, ctypes.POINTER(ctypes.c_char)))

        # Convert from ctypes.c_char array
        elif isinstance(obj, ctypes.c_char * len(obj)):
            return obj

        # Convert from object
        else:
            return String.from_param(obj._as_parameter_)

    from_param = classmethod(from_param)


def ReturnString(obj, func=None, arguments=None):
    return String.from_param(obj)


# As of ctypes 1.0, ctypes does not support custom error-checking
# functions on callbacks, nor does it support custom datatypes on
# callbacks, so we must ensure that all callbacks return
# primitive datatypes.
#
# Non-primitive return values wrapped with UNCHECKED won't be
# typechecked, and will be converted to ctypes.c_void_p.
def UNCHECKED(type):
    if hasattr(type, "_type_") and isinstance(type._type_, str) and type._type_ != "P":
        return type
    else:
        return ctypes.c_void_p


# ctypes doesn't have direct support for variadic functions, so we have to write
# our own wrapper class
class _variadic_function(object):
    def __init__(self, func, restype, argtypes, errcheck):
        self.func = func
        self.func.restype = restype
        self.argtypes = argtypes
        if errcheck:
            self.func.errcheck = errcheck

    def _as_parameter_(self):
        # So we can pass this variadic function as a function pointer
        return self.func

    def __call__(self, *args):
        fixed_args = []
        i = 0
        for argtype in self.argtypes:
            # Typecheck what we can
            fixed_args.append(argtype.from_param(args[i]))
            i += 1
        return self.func(*fixed_args + list(args[i:]))


def ord_if_char(value):
    """
    Simple helper used for casts to simple builtin types:  if the argument is a
    string type, it will be converted to it's ordinal value.

    This function will raise an exception if the argument is string with more
    than one characters.
    """
    return ord(value) if (isinstance(value, bytes) or isinstance(value, str)) else value

# End preamble

_libs = {}
_libdirs = []

# Begin loader

"""
Load libraries - appropriately for all our supported platforms
"""
# ----------------------------------------------------------------------------
# Copyright (c) 2008 David James
# Copyright (c) 2006-2008 Alex Holkner
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
#  * Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#  * Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in
#    the documentation and/or other materials provided with the
#    distribution.
#  * Neither the name of pyglet nor the names of its
#    contributors may be used to endorse or promote products
#    derived from this software without specific prior written
#    permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
# COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
# ----------------------------------------------------------------------------

import ctypes
import ctypes.util
import glob
import os.path
import platform
import re
import sys


def _environ_path(name):
    """Split an environment variable into a path-like list elements"""
    if name in os.environ:
        return os.environ[name].split(":")
    return []


class LibraryLoader:
    """
    A base class For loading of libraries ;-)
    Subclasses load libraries for specific platforms.
    """

    # library names formatted specifically for platforms
    name_formats = ["%s"]

    class Lookup:
        """Looking up calling conventions for a platform"""

        mode = ctypes.DEFAULT_MODE

        def __init__(self, path):
            super(LibraryLoader.Lookup, self).__init__()
            self.access = dict(cdecl=ctypes.CDLL(path, self.mode))

        def get(self, name, calling_convention="cdecl"):
            """Return the given name according to the selected calling convention"""
            if calling_convention not in self.access:
                raise LookupError(
                    "Unknown calling convention '{}' for function '{}'".format(
                        calling_convention, name
                    )
                )
            return getattr(self.access[calling_convention], name)

        def has(self, name, calling_convention="cdecl"):
            """Return True if this given calling convention finds the given 'name'"""
            if calling_convention not in self.access:
                return False
            return hasattr(self.access[calling_convention], name)

        def __getattr__(self, name):
            return getattr(self.access["cdecl"], name)

    def __init__(self):
        self.other_dirs = []

    def __call__(self, libname):
        """Given the name of a library, load it."""
        paths = self.getpaths(libname)

        for path in paths:
            # noinspection PyBroadException
            try:
                return self.Lookup(path)
            except Exception:  # pylint: disable=broad-except
                pass

        raise ImportError("Could not load %s." % libname)

    def getpaths(self, libname):
        """Return a list of paths where the library might be found."""
        if os.path.isabs(libname):
            yield libname
        else:
            # search through a prioritized series of locations for the library

            # we first search any specific directories identified by user
            for dir_i in self.other_dirs:
                for fmt in self.name_formats:
                    # dir_i should be absolute already
                    yield os.path.join(dir_i, fmt % libname)

            # check if this code is even stored in a physical file
            try:
                this_file = __file__
            except NameError:
                this_file = None

            # then we search the directory where the generated python interface is stored
            if this_file is not None:
                for fmt in self.name_formats:
                    yield os.path.abspath(os.path.join(os.path.dirname(__file__), fmt % libname))

            # now, use the ctypes tools to try to find the library
            for fmt in self.name_formats:
                path = ctypes.util.find_library(fmt % libname)
                if path:
                    yield path

            # then we search all paths identified as platform-specific lib paths
            for path in self.getplatformpaths(libname):
                yield path

            # Finally, we'll try the users current working directory
            for fmt in self.name_formats:
                yield os.path.abspath(os.path.join(os.path.curdir, fmt % libname))

    def getplatformpaths(self, _libname):  # pylint: disable=no-self-use
        """Return all the library paths available in this platform"""
        return []


# Darwin (Mac OS X)


class DarwinLibraryLoader(LibraryLoader):
    """Library loader for MacOS"""

    name_formats = [
        "lib%s.dylib",
        "lib%s.so",
        "lib%s.bundle",
        "%s.dylib",
        "%s.so",
        "%s.bundle",
        "%s",
    ]

    class Lookup(LibraryLoader.Lookup):
        """
        Looking up library files for this platform (Darwin aka MacOS)
        """

        # Darwin requires dlopen to be called with mode RTLD_GLOBAL instead
        # of the default RTLD_LOCAL.  Without this, you end up with
        # libraries not being loadable, resulting in "Symbol not found"
        # errors
        mode = ctypes.RTLD_GLOBAL

    def getplatformpaths(self, libname):
        if os.path.pathsep in libname:
            names = [libname]
        else:
            names = [fmt % libname for fmt in self.name_formats]

        for directory in self.getdirs(libname):
            for name in names:
                yield os.path.join(directory, name)

    @staticmethod
    def getdirs(libname):
        """Implements the dylib search as specified in Apple documentation:

        http://developer.apple.com/documentation/DeveloperTools/Conceptual/
            DynamicLibraries/Articles/DynamicLibraryUsageGuidelines.html

        Before commencing the standard search, the method first checks
        the bundle's ``Frameworks`` directory if the application is running
        within a bundle (OS X .app).
        """

        dyld_fallback_library_path = _environ_path("DYLD_FALLBACK_LIBRARY_PATH")
        if not dyld_fallback_library_path:
            dyld_fallback_library_path = [
                os.path.expanduser("~/lib"),
                "/usr/local/lib",
                "/usr/lib",
            ]

        dirs = []

        if "/" in libname:
            dirs.extend(_environ_path("DYLD_LIBRARY_PATH"))
        else:
            dirs.extend(_environ_path("LD_LIBRARY_PATH"))
            dirs.extend(_environ_path("DYLD_LIBRARY_PATH"))
            dirs.extend(_environ_path("LD_RUN_PATH"))

        if hasattr(sys, "frozen") and getattr(sys, "frozen") == "macosx_app":
            dirs.append(os.path.join(os.environ["RESOURCEPATH"], "..", "Frameworks"))

        dirs.extend(dyld_fallback_library_path)

        return dirs


# Posix


class PosixLibraryLoader(LibraryLoader):
    """Library loader for POSIX-like systems (including Linux)"""

    _ld_so_cache = None

    _include = re.compile(r"^\s*include\s+(?P<pattern>.*)")

    name_formats = ["lib%s.so", "%s.so", "%s"]

    class _Directories(dict):
        """Deal with directories"""

        def __init__(self):
            dict.__init__(self)
            self.order = 0

        def add(self, directory):
            """Add a directory to our current set of directories"""
            if len(directory) > 1:
                directory = directory.rstrip(os.path.sep)
            # only adds and updates order if exists and not already in set
            if not os.path.exists(directory):
                return
            order = self.setdefault(directory, self.order)
            if order == self.order:
                self.order += 1

        def extend(self, directories):
            """Add a list of directories to our set"""
            for a_dir in directories:
                self.add(a_dir)

        def ordered(self):
            """Sort the list of directories"""
            return (i[0] for i in sorted(self.items(), key=lambda d: d[1]))

    def _get_ld_so_conf_dirs(self, conf, dirs):
        """
        Recursive function to help parse all ld.so.conf files, including proper
        handling of the `include` directive.
        """

        try:
            with open(conf) as fileobj:
                for dirname in fileobj:
                    dirname = dirname.strip()
                    if not dirname:
                        continue

                    match = self._include.match(dirname)
                    if not match:
                        dirs.add(dirname)
                    else:
                        for dir2 in glob.glob(match.group("pattern")):
                            self._get_ld_so_conf_dirs(dir2, dirs)
        except IOError:
            pass

    def _create_ld_so_cache(self):
        # Recreate search path followed by ld.so.  This is going to be
        # slow to build, and incorrect (ld.so uses ld.so.cache, which may
        # not be up-to-date).  Used only as fallback for distros without
        # /sbin/ldconfig.
        #
        # We assume the DT_RPATH and DT_RUNPATH binary sections are omitted.

        directories = self._Directories()
        for name in (
            "LD_LIBRARY_PATH",
            "SHLIB_PATH",  # HP-UX
            "LIBPATH",  # OS/2, AIX
            "LIBRARY_PATH",  # BE/OS
        ):
            if name in os.environ:
                directories.extend(os.environ[name].split(os.pathsep))

        self._get_ld_so_conf_dirs("/etc/ld.so.conf", directories)

        bitage = platform.architecture()[0]

        unix_lib_dirs_list = []
        if bitage.startswith("64"):
            # prefer 64 bit if that is our arch
            unix_lib_dirs_list += ["/lib64", "/usr/lib64"]

        # must include standard libs, since those paths are also used by 64 bit
        # installs
        unix_lib_dirs_list += ["/lib", "/usr/lib"]
        if sys.platform.startswith("linux"):
            # Try and support multiarch work in Ubuntu
            # https://wiki.ubuntu.com/MultiarchSpec
            if bitage.startswith("32"):
                # Assume Intel/AMD x86 compat
                unix_lib_dirs_list += ["/lib/i386-linux-gnu", "/usr/lib/i386-linux-gnu"]
            elif bitage.startswith("64"):
                # Assume Intel/AMD x86 compatible
                unix_lib_dirs_list += [
                    "/lib/x86_64-linux-gnu",
                    "/usr/lib/x86_64-linux-gnu",
                ]
            else:
                # guess...
                unix_lib_dirs_list += glob.glob("/lib/*linux-gnu")
        directories.extend(unix_lib_dirs_list)

        cache = {}
        lib_re = re.compile(r"lib(.*)\.s[ol]")
        # ext_re = re.compile(r"\.s[ol]$")
        for our_dir in directories.ordered():
            try:
                for path in glob.glob("%s/*.s[ol]*" % our_dir):
                    file = os.path.basename(path)

                    # Index by filename
                    cache_i = cache.setdefault(file, set())
                    cache_i.add(path)

                    # Index by library name
                    match = lib_re.match(file)
                    if match:
                        library = match.group(1)
                        cache_i = cache.setdefault(library, set())
                        cache_i.add(path)
            except OSError:
                pass

        self._ld_so_cache = cache

    def getplatformpaths(self, libname):
        if self._ld_so_cache is None:
            self._create_ld_so_cache()

        result = self._ld_so_cache.get(libname, set())
        for i in result:
            # we iterate through all found paths for library, since we may have
            # actually found multiple architectures or other library types that
            # may not load
            yield i


# Windows


class WindowsLibraryLoader(LibraryLoader):
    """Library loader for Microsoft Windows"""

    name_formats = ["%s.dll", "lib%s.dll", "%slib.dll", "%s"]

    class Lookup(LibraryLoader.Lookup):
        """Lookup class for Windows libraries..."""

        def __init__(self, path):
            super(WindowsLibraryLoader.Lookup, self).__init__(path)
            self.access["stdcall"] = ctypes.windll.LoadLibrary(path)


# Platform switching

# If your value of sys.platform does not appear in this dict, please contact
# the Ctypesgen maintainers.

loaderclass = {
    "darwin": DarwinLibraryLoader,
    "cygwin": WindowsLibraryLoader,
    "win32": WindowsLibraryLoader,
    "msys": WindowsLibraryLoader,
}

load_library = loaderclass.get(sys.platform, PosixLibraryLoader)()


def add_library_search_dirs(other_dirs):
    """
    Add libraries to search paths.
    If library paths are relative, convert them to absolute with respect to this
    file's directory
    """
    for path in other_dirs:
        if not os.path.isabs(path):
            path = os.path.abspath(path)
        load_library.other_dirs.append(path)


del loaderclass

# End loader

add_library_search_dirs([])

# No libraries

# No modules

int8_t = c_char# ./MBx_port.h: 22

int16_t = c_short# ./MBx_port.h: 23

int32_t = c_int# ./MBx_port.h: 24

int64_t = c_longlong# ./MBx_port.h: 25

uint8_t = c_ubyte# ./MBx_port.h: 26

uint16_t = c_ushort# ./MBx_port.h: 27

uint32_t = c_uint# ./MBx_port.h: 28

uint64_t = c_ulonglong# ./MBx_port.h: 29

MBX_SEND_PTR = CFUNCTYPE(UNCHECKED(uint32_t), POINTER(None), c_size_t)# ./MBx_port.h: 157

MBX_GTEC_PTR = CFUNCTYPE(UNCHECKED(uint32_t), POINTER(uint8_t))# ./MBx_port.h: 163

MBX_MAP_REG_HANDLE = CFUNCTYPE(UNCHECKED(uint32_t), POINTER(None))# ./MBx_port.h: 175

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 138
class struct_anon_1(Structure):
    pass

struct_anon_1.__slots__ = [
    'Addr',
    'Memory',
    'Type',
    'Handle',
]
struct_anon_1._fields_ = [
    ('Addr', uint32_t),
    ('Memory', POINTER(None)),
    ('Type', uint8_t),
    ('Handle', MBX_MAP_REG_HANDLE),
]

_MBX_MAP_LIST_ENTRY = struct_anon_1# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 138

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 151
class struct_anon_2(Structure):
    pass

struct_anon_2.__slots__ = [
    'Buffer',
    'Len',
    'LenMAX',
]
struct_anon_2._fields_ = [
    ('Buffer', POINTER(uint8_t)),
    ('Len', uint16_t),
    ('LenMAX', uint16_t),
]

_MBX_EXIST = struct_anon_2# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 151

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 164
class struct_anon_3(Structure):
    pass

struct_anon_3.__slots__ = [
    'ModbusModel',
    'ModbusMode',
    'T1_5_Cycs',
    'T3_5_Cycs',
]
struct_anon_3._fields_ = [
    ('ModbusModel', uint8_t, 7),
    ('ModbusMode', uint8_t, 1),
    ('T1_5_Cycs', uint32_t),
    ('T3_5_Cycs', uint32_t),
]

_MBX_COMMON_CONFIG = struct_anon_3# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 164

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 174
class struct_anon_4(Structure):
    pass

struct_anon_4.__slots__ = [
    'Send',
    'Getc',
]
struct_anon_4._fields_ = [
    ('Send', MBX_SEND_PTR),
    ('Getc', MBX_GTEC_PTR),
]

_MBX_COMMON_FUNCTION = struct_anon_4# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 174

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 188
class struct_anon_5(Structure):
    pass

struct_anon_5.__slots__ = [
    'TimeCnt',
    'NoComNum',
    'State',
    'StatePast',
    'StateFlow',
    'reg',
]
struct_anon_5._fields_ = [
    ('TimeCnt', uint32_t),
    ('NoComNum', uint32_t, 4),
    ('State', uint32_t, 4),
    ('StatePast', uint32_t, 4),
    ('StateFlow', uint32_t, 1),
    ('reg', uint32_t, 19),
]

_MBX_COMMON_RUNTIME = struct_anon_5# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 188

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 199
class struct_anon_6(Structure):
    pass

struct_anon_6.__slots__ = [
    'SlaveID',
    'Map',
    'MapNum',
]
struct_anon_6._fields_ = [
    ('SlaveID', uint8_t),
    ('Map', POINTER(_MBX_MAP_LIST_ENTRY)),
    ('MapNum', uint16_t),
]

_MBX_SLAVE_CONFIG = struct_anon_6# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 199

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 210
class struct_anon_7(Structure):
    pass

struct_anon_7.__slots__ = [
    'Func',
    'RegData',
    'AddrStart',
    'RegNum',
]
struct_anon_7._fields_ = [
    ('Func', uint8_t),
    ('RegData', uint16_t),
    ('AddrStart', uint16_t),
    ('RegNum', uint16_t),
]

_MBX_SLAVE_PARSE_VALUE = struct_anon_7# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 210

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 219
class struct_anon_8(Structure):
    pass

struct_anon_8.__slots__ = [
    'SlaveID',
]
struct_anon_8._fields_ = [
    ('SlaveID', uint8_t),
]

_MBX_MASTER_CONFIG = struct_anon_8# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 219

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 224
class struct__MBX_MASTER_TEAM_MEMBER(Structure):
    pass

struct__MBX_MASTER_TEAM_MEMBER.__slots__ = [
    'SlaveID',
    'Map',
    'MapNum',
    'Next',
]
struct__MBX_MASTER_TEAM_MEMBER._fields_ = [
    ('SlaveID', uint8_t),
    ('Map', POINTER(_MBX_MAP_LIST_ENTRY)),
    ('MapNum', uint16_t),
    ('Next', POINTER(struct__MBX_MASTER_TEAM_MEMBER)),
]

_MBX_MASTER_TEAM_MEMBER = struct__MBX_MASTER_TEAM_MEMBER# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 233

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 262
class struct_anon_9(Structure):
    pass

struct_anon_9.__slots__ = [
    'Func',
    'RegData',
    'AddrStart',
    'RegNum',
    'SlaveID',
    'SendFunc',
    'SendAddrStart',
    'SendRegNum',
    'SendValue',
]
struct_anon_9._fields_ = [
    ('Func', uint8_t),
    ('RegData', uint16_t),
    ('AddrStart', uint16_t),
    ('RegNum', uint16_t),
    ('SlaveID', uint8_t),
    ('SendFunc', uint8_t),
    ('SendAddrStart', uint16_t),
    ('SendRegNum', uint16_t),
    ('SendValue', uint8_t * int((0x7F * 2))),
]

_MBX_MASTER_PARSE_VALUE = struct_anon_9# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 262

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 273
class struct_anon_10(Structure):
    pass

struct_anon_10.__slots__ = [
    'SendFunc',
    'ErrorCode',
    'SendAddrStart',
    'SendRegNum',
]
struct_anon_10._fields_ = [
    ('SendFunc', uint8_t),
    ('ErrorCode', uint8_t),
    ('SendAddrStart', uint16_t),
    ('SendRegNum', uint16_t),
]

_MBX_MASTER_ERROR_RING_NODE = struct_anon_10# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 273

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 280
class struct_anon_11(Structure):
    pass

struct_anon_11.__slots__ = [
    'Queue',
    'Head',
    'Tail',
]
struct_anon_11._fields_ = [
    ('Queue', _MBX_MASTER_ERROR_RING_NODE * int(10)),
    ('Head', uint8_t),
    ('Tail', uint8_t),
]

_MBX_MASTER_ERROR_RING = struct_anon_11# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 280

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 292
class struct_anon_12(Structure):
    pass

struct_anon_12.__slots__ = [
    'SlaveID',
    'Func',
    'AddrStart',
    'RegNum',
    'Value',
]
struct_anon_12._fields_ = [
    ('SlaveID', uint8_t),
    ('Func', uint8_t),
    ('AddrStart', uint16_t),
    ('RegNum', uint16_t),
    ('Value', uint8_t * int((0x7F * 2))),
]

_MBX_MASTER_REQUEST_RING_NODE = struct_anon_12# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 292

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 299
class struct_anon_13(Structure):
    pass

struct_anon_13.__slots__ = [
    'Queue',
    'Head',
    'Tail',
]
struct_anon_13._fields_ = [
    ('Queue', _MBX_MASTER_REQUEST_RING_NODE * int(40)),
    ('Head', uint8_t),
    ('Tail', uint8_t),
]

_MBX_MASTER_REQUEST_RING = struct_anon_13# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 299

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 305
class struct__MBX_SLAVE(Structure):
    pass

struct__MBX_SLAVE.__slots__ = [
    'Runtime',
    'Attr',
    'TxExist',
    'RxExist',
    'Func',
    'Config',
    'Parse',
    'Next',
]
struct__MBX_SLAVE._fields_ = [
    ('Runtime', _MBX_COMMON_RUNTIME),
    ('Attr', _MBX_COMMON_CONFIG),
    ('TxExist', _MBX_EXIST),
    ('RxExist', _MBX_EXIST),
    ('Func', _MBX_COMMON_FUNCTION),
    ('Config', _MBX_SLAVE_CONFIG),
    ('Parse', _MBX_SLAVE_PARSE_VALUE),
    ('Next', POINTER(struct__MBX_SLAVE)),
]

_MBX_SLAVE = struct__MBX_SLAVE# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 319

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 324
class struct__MBX_MASTER(Structure):
    pass

struct__MBX_MASTER.__slots__ = [
    'Runtime',
    'Attr',
    'Config',
    'SlaveChainRoot',
    'TxExist',
    'RxExist',
    'Func',
    'Parse',
    'Error',
    'Request',
    'Next',
]
struct__MBX_MASTER._fields_ = [
    ('Runtime', _MBX_COMMON_RUNTIME),
    ('Attr', _MBX_COMMON_CONFIG),
    ('Config', _MBX_MASTER_CONFIG),
    ('SlaveChainRoot', POINTER(_MBX_MASTER_TEAM_MEMBER)),
    ('TxExist', _MBX_EXIST),
    ('RxExist', _MBX_EXIST),
    ('Func', _MBX_COMMON_FUNCTION),
    ('Parse', _MBX_MASTER_PARSE_VALUE),
    ('Error', _MBX_MASTER_ERROR_RING),
    ('Request', _MBX_MASTER_REQUEST_RING),
    ('Next', POINTER(struct__MBX_MASTER)),
]

_MBX_MASTER = struct__MBX_MASTER# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 341

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 347
for _lib in _libs.values():
    if not _lib.has("MBx_Ticks", "cdecl"):
        continue
    MBx_Ticks = _lib.get("MBx_Ticks", "cdecl")
    MBx_Ticks.argtypes = [uint32_t]
    MBx_Ticks.restype = None
    break

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 349
for _lib in _libs.values():
    if not _lib.has("MBx_Slave_RTU_Init", "cdecl"):
        continue
    MBx_Slave_RTU_Init = _lib.get("MBx_Slave_RTU_Init", "cdecl")
    MBx_Slave_RTU_Init.argtypes = [POINTER(_MBX_SLAVE), uint8_t, POINTER(_MBX_MAP_LIST_ENTRY), MBX_SEND_PTR, MBX_GTEC_PTR, uint32_t, POINTER(uint8_t), uint32_t, POINTER(uint8_t), uint32_t]
    MBx_Slave_RTU_Init.restype = uint32_t
    break

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 350
for _lib in _libs.values():
    if not _lib.has("MBx_Slave_TCP_Init", "cdecl"):
        continue
    MBx_Slave_TCP_Init = _lib.get("MBx_Slave_TCP_Init", "cdecl")
    MBx_Slave_TCP_Init.argtypes = [POINTER(_MBX_SLAVE), uint8_t, POINTER(_MBX_MAP_LIST_ENTRY), MBX_SEND_PTR, MBX_GTEC_PTR, POINTER(uint8_t), uint32_t, POINTER(uint8_t), uint32_t]
    MBx_Slave_TCP_Init.restype = uint32_t
    break

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 352
for _lib in _libs.values():
    if not _lib.has("MBx_Master_RTU_Init", "cdecl"):
        continue
    MBx_Master_RTU_Init = _lib.get("MBx_Master_RTU_Init", "cdecl")
    MBx_Master_RTU_Init.argtypes = [POINTER(_MBX_MASTER), MBX_SEND_PTR, MBX_GTEC_PTR, uint32_t, POINTER(uint8_t), uint32_t, POINTER(uint8_t), uint32_t]
    MBx_Master_RTU_Init.restype = uint32_t
    break

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 353
for _lib in _libs.values():
    if not _lib.has("MBx_Master_TCP_Init", "cdecl"):
        continue
    MBx_Master_TCP_Init = _lib.get("MBx_Master_TCP_Init", "cdecl")
    MBx_Master_TCP_Init.argtypes = [POINTER(_MBX_MASTER), MBX_SEND_PTR, MBX_GTEC_PTR, POINTER(uint8_t), uint32_t, POINTER(uint8_t), uint32_t]
    MBx_Master_TCP_Init.restype = uint32_t
    break

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 354
for _lib in _libs.values():
    if not _lib.has("MBx_Master_Member_Add", "cdecl"):
        continue
    MBx_Master_Member_Add = _lib.get("MBx_Master_Member_Add", "cdecl")
    MBx_Master_Member_Add.argtypes = [POINTER(_MBX_MASTER), POINTER(_MBX_MASTER_TEAM_MEMBER), uint8_t, POINTER(_MBX_MAP_LIST_ENTRY)]
    MBx_Master_Member_Add.restype = uint32_t
    break

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 355
for _lib in _libs.values():
    if not _lib.has("MBx_Master_Error_Get", "cdecl"):
        continue
    MBx_Master_Error_Get = _lib.get("MBx_Master_Error_Get", "cdecl")
    MBx_Master_Error_Get.argtypes = [POINTER(_MBX_MASTER), POINTER(uint8_t), POINTER(uint8_t), POINTER(uint16_t), POINTER(uint16_t)]
    MBx_Master_Error_Get.restype = uint32_t
    break

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 356
for _lib in _libs.values():
    if not _lib.has("MBx_Master_Read_Coil_Request", "cdecl"):
        continue
    MBx_Master_Read_Coil_Request = _lib.get("MBx_Master_Read_Coil_Request", "cdecl")
    MBx_Master_Read_Coil_Request.argtypes = [POINTER(_MBX_MASTER), uint8_t, uint16_t, uint16_t]
    MBx_Master_Read_Coil_Request.restype = uint32_t
    break

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 357
for _lib in _libs.values():
    if not _lib.has("MBx_Master_Read_Disc_Input_Request", "cdecl"):
        continue
    MBx_Master_Read_Disc_Input_Request = _lib.get("MBx_Master_Read_Disc_Input_Request", "cdecl")
    MBx_Master_Read_Disc_Input_Request.argtypes = [POINTER(_MBX_MASTER), uint8_t, uint16_t, uint16_t]
    MBx_Master_Read_Disc_Input_Request.restype = uint32_t
    break

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 358
for _lib in _libs.values():
    if not _lib.has("MBx_Master_Read_Reg_Request", "cdecl"):
        continue
    MBx_Master_Read_Reg_Request = _lib.get("MBx_Master_Read_Reg_Request", "cdecl")
    MBx_Master_Read_Reg_Request.argtypes = [POINTER(_MBX_MASTER), uint8_t, uint16_t, uint16_t]
    MBx_Master_Read_Reg_Request.restype = uint32_t
    break

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 359
for _lib in _libs.values():
    if not _lib.has("MBx_Master_Read_Input_Reg_Request", "cdecl"):
        continue
    MBx_Master_Read_Input_Reg_Request = _lib.get("MBx_Master_Read_Input_Reg_Request", "cdecl")
    MBx_Master_Read_Input_Reg_Request.argtypes = [POINTER(_MBX_MASTER), uint8_t, uint16_t, uint16_t]
    MBx_Master_Read_Input_Reg_Request.restype = uint32_t
    break

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 360
for _lib in _libs.values():
    if not _lib.has("MBx_Master_Write_Coil_Request", "cdecl"):
        continue
    MBx_Master_Write_Coil_Request = _lib.get("MBx_Master_Write_Coil_Request", "cdecl")
    MBx_Master_Write_Coil_Request.argtypes = [POINTER(_MBX_MASTER), uint8_t, uint16_t, uint16_t]
    MBx_Master_Write_Coil_Request.restype = uint32_t
    break

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 361
for _lib in _libs.values():
    if not _lib.has("MBx_Master_Write_Reg_Request", "cdecl"):
        continue
    MBx_Master_Write_Reg_Request = _lib.get("MBx_Master_Write_Reg_Request", "cdecl")
    MBx_Master_Write_Reg_Request.argtypes = [POINTER(_MBX_MASTER), uint8_t, uint16_t, uint16_t]
    MBx_Master_Write_Reg_Request.restype = uint32_t
    break

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 362
for _lib in _libs.values():
    if not _lib.has("MBx_Master_Write_Coil_Mul_Request", "cdecl"):
        continue
    MBx_Master_Write_Coil_Mul_Request = _lib.get("MBx_Master_Write_Coil_Mul_Request", "cdecl")
    MBx_Master_Write_Coil_Mul_Request.argtypes = [POINTER(_MBX_MASTER), uint8_t, uint16_t, uint16_t, POINTER(uint8_t), uint16_t]
    MBx_Master_Write_Coil_Mul_Request.restype = uint32_t
    break

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 363
for _lib in _libs.values():
    if not _lib.has("MBx_Master_Write_Reg_Mul_Request", "cdecl"):
        continue
    MBx_Master_Write_Reg_Mul_Request = _lib.get("MBx_Master_Write_Reg_Mul_Request", "cdecl")
    MBx_Master_Write_Reg_Mul_Request.argtypes = [POINTER(_MBX_MASTER), uint8_t, uint16_t, uint16_t, POINTER(uint8_t), uint16_t]
    MBx_Master_Write_Reg_Mul_Request.restype = uint32_t
    break

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 367
for _lib in _libs.values():
    if not _lib.has("MBx_Init_Runtime", "cdecl"):
        continue
    MBx_Init_Runtime = _lib.get("MBx_Init_Runtime", "cdecl")
    MBx_Init_Runtime.argtypes = [POINTER(_MBX_COMMON_RUNTIME)]
    MBx_Init_Runtime.restype = None
    break

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 368
for _lib in _libs.values():
    if not _lib.has("MBx_Init_Attr", "cdecl"):
        continue
    MBx_Init_Attr = _lib.get("MBx_Init_Attr", "cdecl")
    MBx_Init_Attr.argtypes = [POINTER(_MBX_COMMON_CONFIG), uint8_t, uint8_t, uint32_t, uint32_t]
    MBx_Init_Attr.restype = None
    break

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 369
for _lib in _libs.values():
    if not _lib.has("MBx_Init_Slave_Parse", "cdecl"):
        continue
    MBx_Init_Slave_Parse = _lib.get("MBx_Init_Slave_Parse", "cdecl")
    MBx_Init_Slave_Parse.argtypes = [POINTER(_MBX_SLAVE_PARSE_VALUE)]
    MBx_Init_Slave_Parse.restype = None
    break

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 370
for _lib in _libs.values():
    if not _lib.has("MBx_Init_Slave_Config", "cdecl"):
        continue
    MBx_Init_Slave_Config = _lib.get("MBx_Init_Slave_Config", "cdecl")
    MBx_Init_Slave_Config.argtypes = [POINTER(_MBX_SLAVE_CONFIG), uint8_t, POINTER(_MBX_MAP_LIST_ENTRY)]
    MBx_Init_Slave_Config.restype = uint32_t
    break

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 371
for _lib in _libs.values():
    if not _lib.has("MBx_Init_Master_Parse", "cdecl"):
        continue
    MBx_Init_Master_Parse = _lib.get("MBx_Init_Master_Parse", "cdecl")
    MBx_Init_Master_Parse.argtypes = [POINTER(_MBX_MASTER_PARSE_VALUE)]
    MBx_Init_Master_Parse.restype = None
    break

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 372
for _lib in _libs.values():
    if not _lib.has("MBx_Init_Master_Error", "cdecl"):
        continue
    MBx_Init_Master_Error = _lib.get("MBx_Init_Master_Error", "cdecl")
    MBx_Init_Master_Error.argtypes = [POINTER(_MBX_MASTER_ERROR_RING)]
    MBx_Init_Master_Error.restype = None
    break

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 373
for _lib in _libs.values():
    if not _lib.has("MBx_Init_Master_Request", "cdecl"):
        continue
    MBx_Init_Master_Request = _lib.get("MBx_Init_Master_Request", "cdecl")
    MBx_Init_Master_Request.argtypes = [POINTER(_MBX_MASTER_REQUEST_RING)]
    MBx_Init_Master_Request.restype = None
    break

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 374
for _lib in _libs.values():
    if not _lib.has("MBx_Init_Master_Config", "cdecl"):
        continue
    MBx_Init_Master_Config = _lib.get("MBx_Init_Master_Config", "cdecl")
    MBx_Init_Master_Config.argtypes = [POINTER(_MBX_MASTER_TEAM_MEMBER), uint8_t, POINTER(_MBX_MAP_LIST_ENTRY)]
    MBx_Init_Master_Config.restype = uint32_t
    break

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_utility.h: 243
class struct_anon_14(Structure):
    pass

struct_anon_14.__slots__ = [
    'L8',
    'H8',
]
struct_anon_14._fields_ = [
    ('L8', uint8_t),
    ('H8', uint8_t),
]

_MBX_CRC16_H_L = struct_anon_14# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_utility.h: 243

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_utility.h: 248
class union__MBX_CRC16_R_(Union):
    pass

union__MBX_CRC16_R_.__slots__ = [
    'Val',
    'H_L',
]
union__MBX_CRC16_R_._fields_ = [
    ('Val', uint16_t),
    ('H_L', _MBX_CRC16_H_L),
]

_MBX_CRC16 = union__MBX_CRC16_R_# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_utility.h: 248

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_utility.h: 260
class struct_anon_15(Structure):
    pass

struct_anon_15.__slots__ = [
    'SlaveID',
    'mode',
    'State',
    'reg',
    'ErrCode',
]
struct_anon_15._fields_ = [
    ('SlaveID', uint8_t),
    ('mode', uint8_t, 1),
    ('State', uint8_t, 4),
    ('reg', uint8_t, 3),
    ('ErrCode', uint32_t),
]

_MBX_ERR_TRACE = struct_anon_15# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_utility.h: 260

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_utility.h: 264
for _lib in _libs.values():
    try:
        MBxErrTraceStack = (POINTER(_MBX_ERR_TRACE)).in_dll(_lib, "MBxErrTraceStack")
        break
    except:
        pass

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_utility.h: 268
for _lib in _libs.values():
    if not _lib.has("MBx_utility_crc16", "cdecl"):
        continue
    MBx_utility_crc16 = _lib.get("MBx_utility_crc16", "cdecl")
    MBx_utility_crc16.argtypes = [POINTER(uint8_t), uint16_t]
    MBx_utility_crc16.restype = uint16_t
    break

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_utility.h: 270
for _lib in _libs.values():
    if not _lib.has("MBx_utility_MemMove", "cdecl"):
        continue
    MBx_utility_MemMove = _lib.get("MBx_utility_MemMove", "cdecl")
    MBx_utility_MemMove.argtypes = [POINTER(None), POINTER(None), c_size_t]
    MBx_utility_MemMove.restype = None
    break

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_utility.h: 272
for _lib in _libs.values():
    if not _lib.has("MBxErrTraceAdd", "cdecl"):
        continue
    MBxErrTraceAdd = _lib.get("MBxErrTraceAdd", "cdecl")
    MBxErrTraceAdd.argtypes = [uint8_t, uint8_t, uint8_t, uint32_t]
    MBxErrTraceAdd.restype = None
    break

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_utility.h: 273
for _lib in _libs.values():
    if not _lib.has("MBxErrTraceGet", "cdecl"):
        continue
    MBxErrTraceGet = _lib.get("MBxErrTraceGet", "cdecl")
    MBxErrTraceGet.argtypes = [POINTER(uint8_t), POINTER(uint8_t), POINTER(uint8_t), POINTER(uint32_t)]
    MBxErrTraceGet.restype = uint32_t
    break

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_utility.h: 275
for _lib in _libs.values():
    if not _lib.has("MBx_utility_map_addr_data_read", "cdecl"):
        continue
    MBx_utility_map_addr_data_read = _lib.get("MBx_utility_map_addr_data_read", "cdecl")
    MBx_utility_map_addr_data_read.argtypes = [POINTER(_MBX_MAP_LIST_ENTRY), uint16_t, uint16_t, POINTER(uint16_t), uint8_t]
    MBx_utility_map_addr_data_read.restype = uint32_t
    break

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_utility.h: 276
for _lib in _libs.values():
    if not _lib.has("MBx_utility_map_r_continuity_review", "cdecl"):
        continue
    MBx_utility_map_r_continuity_review = _lib.get("MBx_utility_map_r_continuity_review", "cdecl")
    MBx_utility_map_r_continuity_review.argtypes = [POINTER(_MBX_MAP_LIST_ENTRY), uint16_t, uint16_t, uint16_t]
    MBx_utility_map_r_continuity_review.restype = uint32_t
    break

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_utility.h: 277
for _lib in _libs.values():
    if not _lib.has("MBx_utility_map_addr_data_write", "cdecl"):
        continue
    MBx_utility_map_addr_data_write = _lib.get("MBx_utility_map_addr_data_write", "cdecl")
    MBx_utility_map_addr_data_write.argtypes = [POINTER(_MBX_MAP_LIST_ENTRY), uint16_t, uint16_t, uint16_t, uint8_t]
    MBx_utility_map_addr_data_write.restype = uint32_t
    break

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_utility.h: 278
for _lib in _libs.values():
    if not _lib.has("MBx_utility_map_addr_data_write_cast", "cdecl"):
        continue
    MBx_utility_map_addr_data_write_cast = _lib.get("MBx_utility_map_addr_data_write_cast", "cdecl")
    MBx_utility_map_addr_data_write_cast.argtypes = [POINTER(_MBX_MAP_LIST_ENTRY), uint16_t, uint16_t, uint16_t, uint8_t]
    MBx_utility_map_addr_data_write_cast.restype = uint32_t
    break

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_utility.h: 279
for _lib in _libs.values():
    if not _lib.has("MBx_utility_map_w_continuity_review", "cdecl"):
        continue
    MBx_utility_map_w_continuity_review = _lib.get("MBx_utility_map_w_continuity_review", "cdecl")
    MBx_utility_map_w_continuity_review.argtypes = [POINTER(_MBX_MAP_LIST_ENTRY), uint16_t, uint16_t, uint16_t]
    MBx_utility_map_w_continuity_review.restype = uint32_t
    break

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_utility.h: 280
for _lib in _libs.values():
    if not _lib.has("MBx_utility_map_w_cooperate_review", "cdecl"):
        continue
    MBx_utility_map_w_cooperate_review = _lib.get("MBx_utility_map_w_cooperate_review", "cdecl")
    MBx_utility_map_w_cooperate_review.argtypes = []
    MBx_utility_map_w_cooperate_review.restype = uint32_t
    break

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_utility.h: 282
for _lib in _libs.values():
    if not _lib.has("MBxMasterErrortAdd", "cdecl"):
        continue
    MBxMasterErrortAdd = _lib.get("MBxMasterErrortAdd", "cdecl")
    MBxMasterErrortAdd.argtypes = [POINTER(_MBX_MASTER), uint8_t, uint8_t, uint16_t, uint16_t]
    MBxMasterErrortAdd.restype = uint32_t
    break

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_utility.h: 284
for _lib in _libs.values():
    if not _lib.has("MBxMasterRequestToTx", "cdecl"):
        continue
    MBxMasterRequestToTx = _lib.get("MBxMasterRequestToTx", "cdecl")
    MBxMasterRequestToTx.argtypes = [POINTER(_MBX_MASTER)]
    MBxMasterRequestToTx.restype = None
    break

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_utility.h: 285
for _lib in _libs.values():
    if not _lib.has("MBxMasterRequestAdd", "cdecl"):
        continue
    MBxMasterRequestAdd = _lib.get("MBxMasterRequestAdd", "cdecl")
    MBxMasterRequestAdd.argtypes = [POINTER(_MBX_MASTER), uint8_t, uint8_t, uint16_t, uint16_t, POINTER(uint8_t), uint16_t]
    MBxMasterRequestAdd.restype = uint32_t
    break

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_utility.h: 287
for _lib in _libs.values():
    if not _lib.has("MBx_Master_Member_Find", "cdecl"):
        continue
    MBx_Master_Member_Find = _lib.get("MBx_Master_Member_Find", "cdecl")
    MBx_Master_Member_Find.argtypes = [POINTER(_MBX_MASTER), uint8_t]
    MBx_Master_Member_Find.restype = POINTER(_MBX_MASTER_TEAM_MEMBER)
    break

MBX_MASTER = _MBX_MASTER# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_ExHeader_py.h: 19

MBX_SLAVE = _MBX_SLAVE# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_ExHeader_py.h: 20

MBX_MASTER_TEAM_MEMBER = _MBX_MASTER_TEAM_MEMBER# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_ExHeader_py.h: 21

MBX_MAP_LIST_ENTRY = _MBX_MAP_LIST_ENTRY# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_ExHeader_py.h: 22

# ./MBx_port.h: 42
try:
    MBX_SLAVE_ENABLE = 1
except:
    pass

# ./MBx_port.h: 46
try:
    MBX_MASTER_ENABLE = 1
except:
    pass

# ./MBx_port.h: 50
try:
    MBX_MASTER_REQUEST_QUEUE_MAX = 40
except:
    pass

# ./MBx_port.h: 57
try:
    MBX_MODULE_TCP_MASTER_ENABLE = 0
except:
    pass

# ./MBx_port.h: 61
try:
    MBX_MASTER_ERROR_QUEUE_MAX = 10
except:
    pass

# ./MBx_port.h: 68
try:
    MBX_MASTER_MULTI_REG_MAX = 0x7F
except:
    pass

# ./MBx_port.h: 75
try:
    MBX_MASTER_RESPONSE_TIMEOUT_US = 100000
except:
    pass

# ./MBx_port.h: 79
try:
    MBX_MODULE_ERR_TRACE_ENABLE = 1
except:
    pass

# ./MBx_port.h: 83
try:
    MBX_ERR_TRACE_MAX_NUM = 20
except:
    pass

# ./MBx_port.h: 87
try:
    MBX_EXTEN_REG_HANDLE_ENABLE = 0
except:
    pass

# ./MBx_port.h: 91
try:
    MBX_SENDERR_TIMEOUT_US = 10000
except:
    pass

# ./MBx_port.h: 95
try:
    MBX_SEND_MODE_BYTES = 1
except:
    pass

# ./MBx_port.h: 99
try:
    MBX_EXTEN_API_ENABLE = 0
except:
    pass

# ./MBx_port.h: 103
try:
    MBX_BUILT_UTILTY_CRC_ENABLE = 0
except:
    pass

# ./MBx_port.h: 107
try:
    MBX_ENDIAN_MODE_BIG = 0
except:
    pass

# ./MBx_port.h: 114
try:
    MBX_PORT_RETURN_DEFAULT = 0x00
except:
    pass

# ./MBx_port.h: 115
try:
    MBX_PORT_RETURNT_ERR_INDEFINITE = 0x01
except:
    pass

# ./MBx_port.h: 116
try:
    MBX_PORT_RETURN_BUFFER_FULL = 0x20
except:
    pass

# ./MBx_port.h: 117
try:
    MBX_PORT_RETURN_BUFFER_EMPTY = 0x21
except:
    pass

# ./MBx_port.h: 118
try:
    MBX_PORT_RETURN_CABLE_BUSY = 0x41
except:
    pass

# ./MBx_port.h: 119
try:
    MBX_PORT_RETURN_CABLE_LOST = 0x41
except:
    pass

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 26
try:
    MBX_MODEL_RTU = 1
except:
    pass

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 27
try:
    MBX_MODEL_TCP = 2
except:
    pass

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 31
try:
    MBX_MODE_SLAVE = 0
except:
    pass

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 32
try:
    MBX_MODE_MASTER = 1
except:
    pass

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 35
try:
    MBX_REG_HOLDING = 1
except:
    pass

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 36
try:
    MBX_REG_INPUT = 2
except:
    pass

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 37
try:
    MBX_REG_COIL = 3
except:
    pass

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 38
try:
    MBX_REG_DISCRETE = 4
except:
    pass

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 41
try:
    MBX_FUNC_READ_COIL = 1
except:
    pass

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 42
try:
    MBX_FUNC_READ_DISC_INPUT = 2
except:
    pass

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 43
try:
    MBX_FUNC_READ_REG = 3
except:
    pass

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 44
try:
    MBX_FUNC_READ_INPUT_REG = 4
except:
    pass

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 45
try:
    MBX_FUNC_WRITE_COIL = 5
except:
    pass

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 46
try:
    MBX_FUNC_WRITE_REG = 6
except:
    pass

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 47
try:
    MBX_FUNC_WRITE_COIL_MUL = 15
except:
    pass

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 48
try:
    MBX_FUNC_WRITE_REG_MUL = 16
except:
    pass

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 49
try:
    MBX_FUNC_EXCEPTION_OFFSET = 128
except:
    pass

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 52
try:
    MBX_EXCEPTION_NONE = 0
except:
    pass

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 53
try:
    MBX_EXCEPTION_UNFUNC = 1
except:
    pass

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 54
try:
    MBX_EXCEPTION_UNADDR = 2
except:
    pass

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 55
try:
    MBX_EXCEPTION_DATA = 3
except:
    pass

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 56
try:
    MBX_EXCEPTION_FAULT = 4
except:
    pass

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 58
try:
    MBX_EXCEPTION_BUSY = 7
except:
    pass

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 59
try:
    MBX_EXCEPTION_PARITY = 8
except:
    pass

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 64
try:
    MBX_EXCEPTION_SAFE = 32
except:
    pass

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 65
try:
    MBX_EXCEPTION_CRC = 64
except:
    pass

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 66
try:
    MBX_EXCEPTION_TIMEOUT = 128
except:
    pass

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 67
try:
    MBX_EXCEPTION_SENDERR = 129
except:
    pass

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 70
try:
    MBX_REG_TYPE_U8 = 1
except:
    pass

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 71
try:
    MBX_REG_TYPE_U16 = 2
except:
    pass

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 72
try:
    MBX_REG_TYPE_U32_L = 3
except:
    pass

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 73
try:
    MBX_REG_TYPE_U32_H = 4
except:
    pass

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 74
try:
    MBX_REG_TYPE_U64_0 = 5
except:
    pass

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 75
try:
    MBX_REG_TYPE_U64_1 = 6
except:
    pass

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 76
try:
    MBX_REG_TYPE_U64_2 = 7
except:
    pass

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 77
try:
    MBX_REG_TYPE_U64_3 = 8
except:
    pass

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 78
try:
    MBX_REG_TYPE_BIT_ONLY = 15
except:
    pass

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 79
try:
    MBX_REG_TYPE_BIT_U8_BASE = 16
except:
    pass

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 80
try:
    MBX_REG_TYPE_BIT_U16_BASE = 32
except:
    pass

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 81
try:
    MBX_REG_TYPE_BIT_U32_BASE = 64
except:
    pass

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 82
try:
    MBX_REG_TYPE_BIT_U64_BASE = 128
except:
    pass

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 85
try:
    MBX_STATE_IDLE = 0
except:
    pass

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 86
try:
    MBX_STATE_ERROR = 1
except:
    pass

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 87
try:
    MBX_STATE_WAIT = 2
except:
    pass

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 88
try:
    MBX_STATE_WRITE = 3
except:
    pass

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 89
try:
    MBX_STATE_READ = 4
except:
    pass

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 105
try:
    MBX_PARA_NULL = 0
except:
    pass

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 109
try:
    MBX_API_RETURN_DEFAULT = 0x00
except:
    pass

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 110
try:
    MBX_API_RETURN_ERR_INDEFINITE = 0x01
except:
    pass

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 111
try:
    MBX_API_RETURN_ERR_PARAM = 0x02
except:
    pass

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 113
try:
    MBX_API_RETURN_DATA_ABOVE_LIMIT = 0x10
except:
    pass

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 114
try:
    MBX_API_RETURN_DATA_BELOW_LIMIT = 0x11
except:
    pass

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 115
try:
    MBX_API_RETURN_DATA_LOSS = 0x12
except:
    pass

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 117
try:
    MBX_API_RETURN_BUFFER_FULL = 0x20
except:
    pass

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 118
try:
    MBX_API_RETURN_BUFFER_EMPTY = 0x21
except:
    pass

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 120
try:
    MBX_API_RETURN_MAP_UNFORMAT = 0x101
except:
    pass

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 121
try:
    MBX_API_RETURN_MAP_UNFIND = 0x102
except:
    pass

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_utility.h: 198
def MBxMasterErrorEmptyQ(pMBX):
    return (((((pMBX.contents.Error).value).Head).value) == ((((pMBX.contents.Error).value).Tail).value))

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_utility.h: 204
def MBxMasterErrorFullQ(pMBX):
    return (((((((pMBX.contents.Error).value).Head).value) + 1) % MBX_MASTER_ERROR_QUEUE_MAX) == ((((pMBX.contents.Error).value).Tail).value))

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_utility.h: 210
def MBxMasterRequestEmptyQ(pMBX):
    return (((((pMBX.contents.Request).value).Head).value) == ((((pMBX.contents.Request).value).Tail).value))

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_utility.h: 216
def MBxMasterRequestFullQ(pMBX):
    return (((((((pMBX.contents.Request).value).Head).value) + 1) % MBX_MASTER_REQUEST_QUEUE_MAX) == ((((pMBX.contents.Request).value).Tail).value))

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_utility.h: 223
def MBx_MODULE_TRACE_ADD_ERR(pMB, err):
    return (MBxErrTraceAdd (((pMB.contents.Config).SlaveID), ((pMB.contents.Attr).ModbusMode), ((pMB.contents.Runtime).State), err))

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_utility.h: 229
try:
    MBX_MAP_FIND_MODE_FIRST = 0
except:
    pass

# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_utility.h: 230
try:
    MBX_MAP_FIND_MODE_CONTINUOUS = 1
except:
    pass

_MBX_MASTER_TEAM_MEMBER = struct__MBX_MASTER_TEAM_MEMBER# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 224

_MBX_SLAVE = struct__MBX_SLAVE# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 305

_MBX_MASTER = struct__MBX_MASTER# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_api.h: 324

_MBX_CRC16_R_ = union__MBX_CRC16_R_# E:\\PY64_PROJECT\\CDLL_TEST\\h\\MBx_utility.h: 248

# No inserted files

# No prefix-stripping

