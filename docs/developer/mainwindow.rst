
MainWindow
==========

src/ui/mainwindow.h
    ..

src/ui/mainwindow.ui
    ..

The usual MainWindow class found in Qt GUI applications.

By default, the main window contains a :ref:`CircuitView` where the user can
edit a schematic. It also contains the following docked widgets:

    * A ``QListWidget`` of components that can be inserted
    * A :ref:`ComponentPreview`.

Note that the component preview is initially invisible, until a component is
selected from the list.

The ``MainWindow`` object handles any events from the list of components,
whereas ``CircuitView`` and ``ComponentPreview`` handle their own events.

.. doxygenclass:: Ohmcha::MainWindow
   :members:
   :protected-members:
   :private-members:
   :undoc-members:

Test sentence.

.. doxygenclass:: Ui_MainWindow
   :members:
   :protected-member:
   :private-members:
   :undoc-members:

