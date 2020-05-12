#include <iostream>
#include <vector>
#include <Eigen/Dense>
#include <map>

#include "program/program.h"

// Just for debugging purposes
#define NO_GUI

#ifndef NO_GUI

#include "ui/mainwindow.h"
#include <QApplication>

#endif

#include "model/component.h"
#include "model/kirchhoff.h"
#include "fileio/xml.h"

using namespace Ohmcha;
using namespace Eigen;

void simple_RE_example()
{
	// Declare nodes
	std::vector<Node> nodes(2);
	Node &n1 = nodes[0], &n2 = nodes[1];

	// Declare elements
	BResistor R(10, n1, n2);
	BEmf E(20, n1, n2);

	std::cout << khoffSolve({R, E}, nodes);
	// Expected result: [ 20, 2, -2 ]
}

void E_2parallelR_example()
{
	// Declare nodes
	std::vector<Node> nodes(2);
	Node &n1 = nodes[0], &n2 = nodes[1];

	// Declare elements
	BResistor R1(10, n1, n2);
	BResistor R2(20, n2, n1);
	BEmf E(10, n1, n2);

	std::cout << khoffSolve({R1, R2, E}, nodes);
	// Expected: [ 10, 1, -0.5, -1.5 ]
}

void E_NparallelR_example()
{
	// Declare nodes
	std::vector<Node> nodes(2);
	Node &n1 = nodes[0], &n2 = nodes[1];

	// Declare elements
	std::vector<Branch> branches(1001, BResistor(10, n1, n2));
	branches[0] = BEmf(10, n1, n2);

	std::cout << khoffSolve(branches, nodes);
	// Expected: [ 10, -1000, 1, 1,..., 1 ]
}

void daisy_chain_example()
{
	// Declare nodes
	std::vector<Node> nodes(1001);

	// Declare elements
	std::vector<Branch> branches(1001, BResistor(1, nodes[0], nodes[1000]));
	branches[0] = BEmf(1);
	for (int i = 0; i < 1001; ++i)
	{
		branches[i].setNode1(nodes[i]);
		branches[i].setNode2(nodes[(i + 1) % 1001]);
	}

	auto result = khoffSolve(branches, nodes);
	std::cout << "Voltages: \n" << result.head(1001) << "Currents: \n" << result.tail(1001);
	// Expected: [ 0.001, -0.999, -0.998, ..., -0.001, -0.001, ..., -0.001 ]
	// The results may not be exact
}

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

/**
  * Use for quick tests. Just put
  * 	return test();
  * as the first statement in main, to override the program.
  */
int test()
{
    Schematic schem = xmlParseSchematic("../../staging/test.xml");
    auto v = schem.getComponents();
    auto resistor = *((Resistor*)v[0]);
}

int main(int argc, char *argv[])
{
    return test();
    arglist args = arglist(argv + 1, argv + argc);
    auto it_gui = std::find(args.begin(), args.end(), "--gui");

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
