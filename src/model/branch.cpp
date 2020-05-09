#include "branch.h"

namespace Ohmcha
{

Branch::Branch(const Branch &branch1, const Branch &branch2)
{
	//TODO implement
}

BResistor::BResistor(float resistance)
		: Branch({1, -1, -resistance}, 0)
{}

BEmf::BEmf(float emf)
		: Branch({1, -1, 0}, emf)
{}

}
