CircuitView
============

src/ui/circuitview.h
    ..

This is where the schematic is drawn.

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
