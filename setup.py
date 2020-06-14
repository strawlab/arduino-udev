from setuptools import setup

import io

# read the contents of your README file
from os import path

this_directory = path.abspath(path.dirname(__file__))
with io.open(path.join(this_directory, "README.md"), encoding="utf-8") as f:
    long_description = f.read()

setup(
    name="arduino-udev",
    description="Get and set information by querying serial devices",
    long_description=long_description,
    long_description_content_type="text/markdown",
    version="0.2.5",
    py_modules=["arduinoudev"],
    package_dir={"": "src"},
    url="https://github.com/strawlab/arduino-udev",
    license="MIT",
    maintainer="Andrew Straw",
    maintainer_email="strawman@astraw.com",
    classifiers=[
        "Development Status :: 5 - Production/Stable",
        "License :: OSI Approved :: MIT License",
        "Programming Language :: Python :: 2",
        "Programming Language :: Python :: 2.7",
        "Programming Language :: Python :: 3",
        "Programming Language :: Python :: 3.5",
        "Programming Language :: Python :: 3.6",
        "Programming Language :: Python :: 3.7",
        "Programming Language :: Python :: 3.8",
    ],
)
