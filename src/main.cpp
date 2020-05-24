#include <iostream>
#include <vector>
#include <Eigen/Dense>
#include <map>

#include "program/program.h"
#include "test/test.h"

// Just for debugging purposes
// #define NO_GUI

#ifndef NO_GUI

#include "ui/mainwindow.h"
#include <QApplication>

#endif

#include "model/component.h"
#include "model/kirchhoff.h"
#include "fileio/xml.h"

using namespace Ohmcha;
using namespace Eigen;


// TODO, just a placeholder for now
std::map<std::string, int (*)(const arglist_iter &, const arglist_iter &)> commands = {};

/**
 * Execute a command, or print a message if it does not exist.
 * @param it_arg Iterator to the first argument in the list.
 * @param end - End of the argument list
 * @return Status code of the executed command.
 */
int execute_command(const arglist_iter &it_arg, const arglist_iter &end)
{
	// Can the argument be found among the defined commands?
	if (commands.find(*it_arg) != commands.end())
	{ // Yes: execute it
		return commands[*it_arg](it_arg + 1, end);
	}
	else
	{ // No: print an error message
		unrecognized_opt_cmd(*it_arg);
		return -1;
	}
}

int start_gui(int argc, char *argv[])
{
#ifndef NO_GUI
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
#else
    return 0;
#endif
}

int main(int argc, char *argv[])
{
    return test();
    arglist args = arglist(argv + 1, argv + argc);
    auto it_gui = std::find(args.begin(), args.end(), "--gui");

    // This is here temporarily
    return start_gui(0, nullptr);

    // Non-GUI mode: execute command and exit
	if (it_gui == args.end())
    {
        if (argc < 2)
            return std::cerr << "No command specified.", 0;
        return execute_command(args.begin(), args.end());
    }
    // GUI mode
    return start_gui(0, nullptr);
}
