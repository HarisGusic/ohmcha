
Algorithms
==========

.. toctree::
   :maxdepth: 2
   :local:

Basic Kirchhoff
---------------

src/model/kirchhoff.h
    ..

This is the most basic algorithm. It is not very efficient, and should only be
used for debugging. Suppose we have a circuit with :math:`b` branches and 
:math:`n` nodes. We take one node (the last one, by convention) as reference.
The corresponding system of equations should be written as:

.. math::

   \left[
    \begin{array}{c}
        &\\ % empty row
        \text{Constitutive equations} \\
        b \times (b+n-1) \\
        &\\ % empty row
        \begin{array}{c|c}
            \hline \\
            \hspace{0.7cm} \mathbf{0}_{(n-1) \times (n-1)} \hspace{0.7cm} &
            \hspace{0.7cm} \text{KCL}_{(n-1) \times b} \hspace{0.7cm} \\
            \\ % empty row
        \end{array}
    \end{array}
   \right]
   %############################################################
   \cdot
   %############################################################
   \left[
    \begin{array}{c}
        V_1 \\ \vdots \\ V_{n-1} \\
        I_1 \\ \vdots \\ I_b
    \end{array}
   \right]
   %############################################################
   =
   %############################################################
   \left[
    \begin{array}{c}
        B_1 \\ \vdots \\ B_b \\
        0 \\ \vdots \\ 0
    \end{array}
   \right]

KCL = Kirchhoff's Current Law

Then we just apply one of Eigen's solvers to do the dirty work for us.

Example
*******

.. image:: ../examples/example_1/example_1.svg
   :align: center

Suppose that the first row of the system matrix is

.. math::
   [ \begin{array}{ccc} a_1 & a_2 & b_1 & b_2 & b_3 & b_4 & b_5 \end{array} ]

Since the first branch is only connected to nodes 1 and 3, and :math:`I_1` is
the current that flows through it, :math:`a_1=1,\ b_1=-R_1`, and all
other elements are zero. The lower part of the matrix is easy to populate based
on the KCL.

Finally, the system of equations for this circuit is:

.. math::
   \left[
    \begin{array}{ccccccc}
        1 & 0 & -R_1 & 0 & 0 & 0 & 0 \\
        1 & 0 & 0 & -R_2 & 0 & 0 & 0 \\
        1 & -1 & 0 & 0 & -R_3 & 0 & 0\\
        0 & 0 & 0 & 0 & 0 & 1 & 0 \\
        0 & 1 & 0 & 0 & 0 & 0 & 0 \\ \hline
        0 & 0 & 1 & 1 & 1 & -1 & 0 \\
        0 & 0 & 0 & 0 & -1 & 1 & 1 \\
    \end{array}
   \right]
        \cdot
   \left[
    \begin{array}{c}
        V_1 \\ V_2 \\
        I_1 \\ I_2 \\ I_3 \\ I_4 \\ I_5
    \end{array}
   \right]
        =
   \left[
    \begin{array}{c}
        E_1 \\ 0 \\ 0 \\ I_4 \\ E_5 \\
        0 \\ 0
    \end{array}
   \right]
