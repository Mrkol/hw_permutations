#include <iostream>
#include <cstring>

class Permutation
{
	int _order;
	int* _data;


	size_t _inversionCount(int* from, int* to, int* buffer) const;
	friend Permutation operator*(const Permutation&, const Permutation&);
public:

	Permutation();

	Permutation(int);

	Permutation(int, int*);

	Permutation(const Permutation& perm);

	int operator[](int) const;

	int order() const;

	Permutation next() const;

	Permutation previous() const;

	Permutation inverse() const;

	size_t inversionCount() const;

	void operator()(int*) const;

	Permutation& operator=(const Permutation& perm);

	~Permutation();
};

Permutation operator*(const Permutation& lhs, const Permutation& rhs);
Permutation& operator*=(Permutation& lhs, const Permutation& rhs);
Permutation& operator++(Permutation& op);
Permutation& operator--(Permutation& op);
Permutation operator++(Permutation& op, int);
Permutation operator--(Permutation& op, int);
bool operator==(const Permutation& lhs, const Permutation& rhs);
bool operator!=(const Permutation& lhs, const Permutation& rhs);
bool operator>(const Permutation& lhs, const Permutation& rhs);
bool operator<(const Permutation& lhs, const Permutation& rhs);
bool operator>=(const Permutation& lhs, const Permutation& rhs);
bool operator<=(const Permutation& lhs, const Permutation& rhs);

inline void checkOrders(const Permutation& first, const Permutation& second);
template<typename T>
inline void swap(T&, T&);


Permutation::Permutation()
{
	_order = 0;
	_data = nullptr;
}

Permutation::Permutation(int ord)
{
	_order = ord;
	_data = new int[_order];
	for (int i = 0; i < _order; i++) _data[i] = i;
}

Permutation::Permutation(int ord, int* data)
{
	_order = ord;
	_data = new int[_order];
	std::memcpy(_data, data, _order*sizeof(int));
}

Permutation::Permutation(const Permutation& perm)
{
	_order = perm._order;
	_data = new int[_order];
	std::memcpy(_data, perm._data, _order*sizeof(int));
}

Permutation::~Permutation()
{
	if (_data != nullptr) delete[] _data;
}

Permutation& Permutation::operator=(const Permutation& perm)
{
	if (_data != nullptr) delete[] _data;
	_order = perm._order;
	_data = new int[_order];
	std::memcpy(_data, perm._data, _order*sizeof(int));
	return *this;
}

int Permutation::operator[](int index) const
{
	return _data[index];
}

void Permutation::operator()(int* array) const
{
	int* tmp = new int[_order];
	std::memcpy(tmp, array, _order*sizeof(int));
	for (int i = 0; i < _order; i++)
		array[_data[i]] = tmp[i];
	delete[] tmp;
}

int Permutation::order() const
{
	return _order;
}

Permutation Permutation::next() const
{
	int last_growing = _order - 1;
	while (last_growing > 0 && _data[last_growing - 1] >= _data[last_growing]) last_growing--;

	if (last_growing == 0) return Permutation(_order);

	int last_bigger = _order - 1;
	while (_data[last_growing - 1] > _data[last_bigger]) last_bigger--;


	Permutation result(*this);

	result._data[last_bigger] = _data[last_growing - 1];
	result._data[last_growing - 1] = _data[last_bigger];

	for (int i = 0; i < (_order - last_growing)/2; i++)
	{
		swap(result._data[last_growing + i], result._data[_order - i - 1]);
	}

	return result;
}

Permutation Permutation::previous() const
{
	int last_decring = _order - 1;
	while (last_decring > 0 && _data[last_decring - 1] <= _data[last_decring]) last_decring--;

	if (last_decring == 0) 
	{
		Permutation result(_order);
		for (int i = 0; i < _order; i++) 
			result._data[i] = _data[_order - i - 1];
		return result;
	}

	int last_smaller = _order - 1;
	while (_data[last_decring - 1] < _data[last_smaller]) last_smaller--;


	Permutation result(*this);

	result._data[last_smaller] = _data[last_decring - 1];
	result._data[last_decring - 1] = _data[last_smaller];

	for (int i = 0; i < (_order - last_decring)/2; i++)
	{
		swap(result._data[last_decring + i],
			result._data[_order - i - 1]);
	}

	return result;
}

Permutation Permutation::inverse() const
{
	Permutation result(_order);
	for (int i = 0; i < _order; i++)
		result._data[_data[i]] = i;
	return result;
}

Permutation operator*(const Permutation& lhs, const Permutation& rhs)
{
	checkOrders(lhs, rhs);
	Permutation result(lhs.order());

	for (int i = 0; i < result.order(); i++)
		result._data[i] = lhs[rhs[i]];

	return result;
}

Permutation& operator*=(Permutation& lhs, const Permutation& rhs)
{
	lhs = lhs * rhs;
	return lhs;
}

Permutation& operator++(Permutation& op)
{
	op = op.next();
	return op;
}

Permutation& operator--(Permutation& op)
{
	op = op.previous();
	return op;
}

Permutation operator++(Permutation& op, int)
{
	Permutation tmp = op;
	op = op.next();
	return tmp;
}

Permutation operator--(Permutation& op, int)
{
	Permutation tmp = op;
	op = op.previous();
	return tmp;
}

bool operator==(const Permutation& lhs, const Permutation& rhs)
{
	checkOrders(lhs, rhs);
	for (int i = 0; i < lhs.order(); i++)
		if (lhs[i] != rhs[i])
			return false;

	return true;
}

bool operator!=(const Permutation& lhs, const Permutation& rhs)
{
	return !(lhs == rhs);
}

bool operator<=(const Permutation& lhs, const Permutation& rhs)
{
	checkOrders(lhs, rhs);
	int i = 0;
	while (i < lhs.order() && lhs[i] == rhs[i]) i++;

	if (i == lhs.order()) return true;
	return lhs[i] < rhs[i];
}

bool operator>=(const Permutation& lhs, const Permutation& rhs)
{
	return rhs <= lhs;
}

bool operator>(const Permutation& lhs, const Permutation& rhs)
{
	return !(lhs <= rhs);
}

bool operator<(const Permutation& lhs, const Permutation& rhs)
{
	return !(lhs >= rhs);
}

inline void checkOrders(const Permutation& first, const Permutation& second)
{
	if (first.order() == second.order()) return;
	
	std::cerr << "Error: tried to work with permutations of different order" << std::endl;
	throw 1;
}

template<typename T>
inline void swap(T& first, T& second)
{
	T tmp = second;
	second = first;
	first = tmp;
}

size_t Permutation::inversionCount() const
{
	int* buffer = new int[_order];
	int* data_cpy = new int[_order];
	std::memcpy(data_cpy, _data, _order*sizeof(int));
	size_t answer = _inversionCount(data_cpy, data_cpy + _order, buffer);
	delete[] buffer;
	delete[] data_cpy;
	return answer;
}

size_t Permutation::_inversionCount(int* from, int* to, int* buffer) const
{
	if (to - from <= 1) return 0;
	if (to - from == 2) 
	{
		if (from[0] < from[1]) return 0;
		swap(from[0], from[1]);
		return 1;
	}

	int* middle = from + (to - from)/2;

	size_t left = _inversionCount(from, middle, buffer);
	size_t right = _inversionCount(middle, to, buffer);

	int* i = from;
	int* j = middle;
	int* t = buffer;

	size_t answer = left + right;
	while (i < middle || j < to)
	{
		if (i < middle && (*i < *j || j >= to))
		{
			*(t++) = *(i++);
		}
		else if (j < to) 
		{
			answer += middle - i;
			*(t++) = *(j++);
		}
	}

	std::memcpy(from, buffer, (to - from)*sizeof(int));
	return answer;
}

std::ostream& operator<<(std::ostream& out, const Permutation& perm)
{
	out << "(";
	for (int i = 0; i < perm.order(); i++)
	{
		out << (perm[i] + 1);
		if (i != perm.order() - 1) out << " ";
	}
	out << ")";
	return out;
}


int main()
{
	size_t ord = 0;
	std::cin >> ord;
	int* data = new int[ord];
	for (size_t i = 0; i < ord; i++) std::cin >> data[i];

	Permutation perm(ord, data);
	
	size_t answ = perm.inversionCount();
	std::cout << answ << std::endl;
	delete[] data;

	return 0;
}