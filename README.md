# GTlab Module Generator

This tool generates a basic module code structure for GTlab. It can be used as s first starting point when generating modules, as it creates a whole skeleton, including
 - Source Code
 - Build System
 - and uses the GTlab file structure

## Usage


1. **Settings**: 
    * select an ouput path for the module. An additional subfolder will be created using the module name.
    * set the path to your local GTlab application `GTlab(.exe)`. The GTlab installation should also contain the include files and libraries.
</p>

2. **Module Specifications**:
    * define an optional prefix for all file and class names
    * enter the name of the module to generate
    * file and class name are auto generated using the module name, but their values can be locked
    * set the desired module description 

</p>

3. **Interface Selection**: select the desired interfaces to extend 


4. **Interface Specifications**: depending on the previous interface selection different tabs are generated, in which interface specific data and classes can be registered. The function name and the return value are displayed on the left, while data can be entered on the right. Example for generating a custom `Exporter` class:

    <figure class="image">
        <img src="/images/add_class_example.jpg" alt="Adding a custom exporter class">
        <figcaption> <i>Fig. 1: Adding a custom exporter class</i></figcaption>
    </figure>

    Some functions require the name of a 'linked class' (eg. uiItems - Mdi Interface). The name of the linked class can be entered in the lineedit accordingly.

    _Note: depending on the linked class entered, the module may not be able to compile, as the include statement for that class is not added automatically! (eg. defining a ui item for `GtProject` may fail, as `gt_project.h` is not included automatically)_

5. **Module Dependencies**: select or enter the desired module dependencies. A list of avaibale modules is created automatically at runtime (this process may take a few seconds and is run concurrently). 

</p>

6. **File Signature**: Enter your name and email address to create a file signature

7. **Summary**: Displays an overview about the files to generate and the module specifications. Click on `Generate` to generate the module at the ouput directory
