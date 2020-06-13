
ComponentPreview
================

src/ui/component_preview.h
    ..

src/ui/component_preview.ui
    ..

Quick Overview
--------------

* The object is constructed
* Main window calls ``initialize``
* The user selects a component from the list of components

    * Main window makes this widget visible (if not already)
    * ``initializeNewComponent``

        * Remove the previous item from the scene
        * ``synchronize``
        * Set the component immovable and unselectable

* The user edits the item
* The user clicks the *Add* button

    * ``CircuitView::initiateInsertComponent``
    * ``initializeNewComponent``

* The user selects an item in the circuit view

    * Slot ``setEditExisting`` is called

Detailed Explanation
--------------------

This widget has two purposes. One is to edit a new item that has yet
to be placed into the active :ref:`CircuitView`, and the other is to edit an
existing item. In the first case, a small preview window is shown.
The widget that shows the component preview is a plain ``QGraphicsView``.
The user cannot interact with the component in it as they can with components in
the circuit view. For example, the component cannot be selected nor
moved by mouse. Since these are properties of the component and not of the view,
this has to be disabled and enabled **each time the component is copied back and
forth between the preview and the circuit view.**

When the user selects a component from the component list (see
:ref:`MainWindow`), ``initializeNewComponent`` is called. This creates a new
component, with the same properties as the last inserted component. When the
user selects an existing component in the circuit view, ``setEditExisting`` is
called with the selected component. The method ``synchronize`` is used to
synchronize the text boxes and buttons with the actual properties of the
component.

New components are added into the circuit view when the user clicks on the *Add*
button. The method ``CircuitView::initiateInsertComponent`` is called. A new
component is initialized in the preview immediately. The ``GraphicComponent`` instance in the preview is copied using 
``GraphicComponent::copy``.

.. note:: ``initialize`` can be called only after the member ``circuitView`` has
   been initialized. This is done by the main window.

.. doxygenclass:: Ohmcha::ComponentPreview
   :members:
   :private-members:
   :protected-members:
   :undoc-members:
