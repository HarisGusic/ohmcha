

.. toctree::
   : maxdepth: 2
   
Getting Started
===============

Ohmcha is written in C++, and uses the Qt framework for GUI and some file IO.
The project is structured in the following way:

.. include:: tree.txt
   :literal:

**Model**

    src/model/
        ..

    Low-level mathematical model of electrical circuits and algorithms. It does
    not depend on any GUI representations and doesn't use the Qt framework.

**File IO**
    
    src/fileio/
        ..

    Anything that is related to reading from and writing to files. This module
    uses the Qt framework, but it is completely decoupled from the GUI.

**UI**
    
    src/ui/
        ..

    The user interface.

**Program**

    src/program/
        ..
   
    Implementation of the Command Line Interface.

**Test**
    
    src/test/
        ..

    Any tests or examples. These are mostly used to test if a new feature
    has broken existing features.

.. note::

   The above components correspond to the directory structure inside src/

