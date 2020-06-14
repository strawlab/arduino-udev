from setuptools import setup

# read the contents of your README file
from os import path
this_directory = path.abspath(path.dirname(__file__))
with open(path.join(this_directory, 'README.md'), encoding='utf-8') as f:
    long_description = f.read()

setup(name='arduino-udev',
      description="Get and set information by querying serial devices",
      long_description=long_description,
      long_description_content_type='text/markdown',
      version='0.2.4',
      py_modules=['arduinoudev'],
      package_dir = {'': 'src'},
      url = "https://github.com/strawlab/arduino-udev",
      maintainer = "Andrew Straw",
      maintainer_email = "strawman@astraw.com",
      )

