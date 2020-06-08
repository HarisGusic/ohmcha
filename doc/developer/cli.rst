
Command Line Interface
======================

The command arguments are passed to ``main``. Since the ``char*[]`` type is
unflexible, we convert it to an ``arglist`` object which is a copy of ``argv``.
The first element is removed because it is just the name of the executable.

    .. doxygentypedef:: arglist

    .. doxygentypedef:: arglist_iter

Next we check if the program should run in GUI mode, i.e. if the ``--gui``
option was specified. In that case, we call the function ``start_gui`` which
takes control of the program.

If we are in CLI mode, the program runs the command whose name is specified as
the second argument by calling ``execute_command``:

    .. doxygenfunction:: execute_command

All defined commands are part of a ``std::map`` called ``commands`` that maps
each command name to a function.
