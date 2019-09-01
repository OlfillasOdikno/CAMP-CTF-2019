from distutils.core import setup, Extension
import os
ice_module = Extension('ice',
                    sources = ['icemodule.c','ice.c'])

setup (name = 'ICEpy',
       version = '1.0',
       description = 'ICE Module',
       ext_modules = [ice_module])