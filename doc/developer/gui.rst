
GUI
===

The GUI is started by calling ``start_gui``.

    .. doxygenfunction:: start_gui

This creates an instance of ``MainWindow``.

MainWindow
----------

src/ui/mainwindow.h
    ..

The usual MainWindow class found in Qt GUI applications.

.. doxygenclass:: Ohmcha::MainWindow
   :members:
   :protected-members:
   :private-members:
   :undoc-members:

Circuit View
------------

This is where the schematic is drawn. It is a subclass of QGraphicsView.

.. note::

   Zooming does not behave as intended. What is intended is that when you scroll
   your mouse wheel, the scene should scale around the point where your cursor
   is. Instead, the current implementation scales with the center of the view as
   the pivot. We are working on a fix for this. We might even ditch the
   QGraphicsView and use a lower-level framework.

.. doxygenclass:: Ohmcha::CircuitView
   :members:
   :protected-members:
   :private-members:
   :undoc-members:
  
