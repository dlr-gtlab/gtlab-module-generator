# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

## [1.0.1] - 2021-06-22
## Added
- High DPI scaling #11 #13
- Application icon #13 

## Changed
- Made some labels more descriptive #12
- the path to GTlab must be entered instead of the install directory (settings page) #10

## Removed
- file system completers (settings page)

## Fixed
- Spelling errors #12
- `DESTDIR` in module.pro supports custom GTlab builds (eg. $${GTLAB_DIR}/build/GTlab.exe) #10

## [1.0.0] - 2021-06-21
## Added
- Support for GTlab v1.7.x
- Support for all core interfaces
- Modular class generation

## Changed
- Refactored the main GUI layout
- Enhanced the module generation process
- Autocompletion for file and class name can be toggled #8
