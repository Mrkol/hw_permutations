#include <iostream>
#include "permutation.h"

using namespace std;


int main()
{
	Permutation perm(3);

	std::cout << (perm.next().next() > perm);
	std::cout << (perm >= perm);
	std::cout << (perm.next().previous() == perm);
	std::cout << (perm < perm.previous());
	std::cout << (++perm == perm.next());
	std::cout << (--(++perm) == perm++);

	return 0;
}