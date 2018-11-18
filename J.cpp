#include <iostream>
#include "permutation.h"

using namespace std;


int main()
{
	//1 2 3 4 5
	//3 4 5 1 2

	//()

	//          2  3  4  0  1     1 2 3 4 5
	int data[] {2, 3, 4, 1, 0}; //3 4 5 2 1
	Permutation perm2(5, data);

	std::cout << (perm2.previous());

	Permutation perm(3);

	std::cout << (perm.next().order() == 2);
	std::cout << (perm.previous().pow(11) == perm.previous());
	std::cout << (perm.next().pow(1) == perm.next());
	std::cout << (perm.next().pow(0) == perm);
	std::cout << (perm.next().pow(-1) == perm.next().inverse());
	std::cout << (perm.pow(10) == perm);
	std::cout << (perm.previous().pow(40) == perm);
	std::cout << (perm.parity() == 1);

	std::cout << (perm.isEven());
	std::cout << (perm.next().isOdd());


	std::cout << (perm2.parity() == -1);
	std::cout << (perm2.previous().parity() == -1);


	//Permutation perm3(1000);
	//std::cout << perm3.next().next().next().next().next().next().next().next().next().next().next().next().pow(1000000007);

	return 0;
}