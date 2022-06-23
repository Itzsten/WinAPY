from distutils.core import setup, Extension
setup(name='winapy', version='1.0', ext_modules=[Extension('winapy', ['wrapper.c'])])
