#include <iostream>
#include "permutation.h"

using namespace std;


int main(int argc, char* argw[])
{
	if (argc < 2) return 1;

	int length = stod(argw[1]);

	if (argc < 2 + length) return 1;

	int* data = new int[length];
	for (int i = 0; i < length; i++) data[i] = stod(argw[2 + i]);
	Permutation perm(length, data);

	if (argc == 2 + length)
	{
		cout << perm.parity();
		return 0;
	}

	int power = stod(argw[2 + length]);
	cout << perm.pow(power);
	return 0;
}