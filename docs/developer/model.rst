
Model
=====

This is the most abstract part of the project. It is also the part where
efficiency is most important. This includes both processing performance and
memory usage. The model should never depend on the GUI, CLI, or file IO. There
are multiple reasons for this:

    * This module might become its own library
    * It is the common denominator between the CLI and GUI and those should be
      separate. It is planned that these can be compiled separately.
    * The project is easier to maintain this way

.. note::
   While we plan to make Ohmcha support nonlinear circuits and custom user
   circuits of any kind, only linear circuits are being implemented in the
   early stages. Everything that follows relates strictly to linear circuits.

Modelling a component
---------------------

Everything that has at least a current and a voltage associated with it, is
represented as a component.  A special case of a component is a branch. 
``Component`` is an abstract class. It contains a struct ``MetaInfo`` which
includes information about the component that is not needed for solving the
circuit, such as the component name.

For implementation details, see :ref:`Component`.

Modelling a branch
------------------

A Branch is a subclass of Component. While it is not an abstract class in
C++ terminology, it is fairly abstract in that every two-terminal element and 
subcircuit can be represented as a branch. For example, a resistor and emf
connected in series are to be represented as a single branch. Or, a single
current source. Even parallel and combined connections can be represented this
way, as long as that connection effectively has 2 terminals. It is advisable
to combine connections into a single branch wherever possible, for increased
performance.

In order to explain the model, an example is useful.

Example
*******

Let's look at an example that is used throughout the documentation:

.. image:: ../examples/example_1/example_1.svg
   :align: center

Consider branch 1 in isolation -- the branch number and the index of its current
are identical in this representation.

The branch can be mathematically represented as:

.. math::
   V_1 - V_3 = E_1 + R_1 \cdot I_1
   
We can rearrange this a bit:

.. math::
   V_1 - V_3 - R_1 \cdot I_1 = E_1

or, in matrix form:

.. math::
   [ \begin{array}{ccc} 1 & -1 & -R_1 \end{array} ]
        \cdot
   [ \begin{array}{ccc} V_1 & V_3 & I_1 \end{array} ]^T
        = E_1

In fact, an arbitrary linear branch can be represented in this way:

.. math::
   \mathbf A \cdot 
   [ \begin{array}{ccc} V_1 & V_2 & I \end{array} ]^T
   = B

where :math:`V_1` is the voltage of the first node, and :math:`V_2` of the
second node this branch is connected to, and :math:`I` is the current through.
this branch. The order of these variables is determined using the following
convention: *current flows through a branch from node 1 to node 2*. We will
refer to this last equation as the constitutive relation of the branch.

For completeness, we will display the results for the other branches:

.. math::
   \mathbf A_2 = [ \begin{array}{ccc} 1 & -1 & -R_2 \end{array} ],\ B_2 = 0

   \mathbf A_3 = [ \begin{array}{ccc} 1 & -1 & -R_3 \end{array} ],\ B_3 = 0

   \mathbf A_4 = [ \begin{array}{ccc} 0 & 0 & 1 \end{array} ],\ B_4 = I_4
   
   \mathbf A_5 = [ \begin{array}{ccc} 1 & -1 & 0 \end{array} ],\ B_5 = E_5


**To sum up, the only things that an algorithm needs to know
about this branch are: node 1, node 2, matrix** :math:`\mathbf A` **and**
:math:`B`.

Note that by combining multiple components into a single branch, the branch
itself does not retain information of each component that is part of it. For
example, if a branch represents a series connection of a resistor and emf,
you can use the algorithms to calculate the current through both of them, since
it is a property of the branch. However, you can't calculate the voltage drop
on the resistor by merely knowing the mathematical model of the branch.

This is not a problem. Firstly, the algorithms don't care. The algorithms' only
job is to find an explicit representation of the system. Secondly, since each
component keeps a reference to its branch, by iterating through all components,
one can find all variables, in a way that is less costly than the algorithms.
Additionally, the user often won't care about all the variables in the system.
This is another performance boost.

Classes
-------

Component
*********

.. doxygenclass:: Ohmcha::Component
   :members:
   :protected-members:
   :private-members:
   :undoc-members:

Branch
******

.. doxygenclass:: Ohmcha::Branch
   :members:
   :protected-members:
   :private-members:
   :undoc-members:

Resistor
********

.. doxygenclass:: Ohmcha::Resistor
   :members:
   :protected-members:
   :private-members:
   :undoc-members:

Custom Component
****************

Custom user components are all represented as instances of CustomComponent. The
attributes of this class are the number of terminals, the graphical
representation (e.g. an image), and the mathematical model.

.. todo::
   Custom user components are not implemented yet.

