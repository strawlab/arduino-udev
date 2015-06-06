#!/usr/bin/env python

from distutils.core import setup
from catkin_pkg.python_setup import generate_distutils_setup

setup_args = generate_distutils_setup(
    packages=['arduinoudev'],
    scripts=[],
    package_dir={'': 'src'},
    install_requires = ['arduino-mk']
    )

setup(**setup_args)
