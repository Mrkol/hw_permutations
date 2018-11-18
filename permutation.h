#include <iostream>
#include <cstring>


const char* INCORRECT_ORDERS_MSG = "Error: tried to work with permutations of different group order";
const int INCORRECT_ORDERS = 1;

class Permutation
{
	int _grp_order;
	int* _data;
	mutable int _order_saved;
	mutable int _parity_saved;

	void clearSaved() const;

	size_t _inversionCount(int* from, int* to, int* buffer) const;
	friend const Permutation operator*(const Permutation&, const Permutation&);
	friend Permutation& operator++(Permutation&);
	friend Permutation& operator--(Permutation&);

	static const int ORDER_NONE = 0;
	static const int PARITY_NONE = 0;

public:

	Permutation() = delete;

	explicit Permutation(int);

	Permutation(int, int*);

	Permutation(const Permutation& perm);

	int operator[](int) const;

	int groupOrder() const;

	int order() const;

	int parity() const;

	const Permutation next() const;

	const Permutation previous() const;

	const Permutation inverse() const;

	const Permutation pow(int) const;

	bool isEven() const;
	
	bool isOdd() const;

	size_t inversionCount() const;

	void operator()(int*) const;

	Permutation& operator=(const Permutation& perm);

	~Permutation();
};

const Permutation operator*(const Permutation&, const Permutation&);
Permutation& operator*=(Permutation&, const Permutation&);
Permutation& operator++(Permutation&);
Permutation& operator--(Permutation&);
Permutation operator++(Permutation&, int);
Permutation operator--(Permutation&, int);
bool operator==(const Permutation&, const Permutation&);
bool operator!=(const Permutation&, const Permutation&);
bool operator>(const Permutation&, const Permutation&);
bool operator<(const Permutation&, const Permutation&);
bool operator>=(const Permutation&, const Permutation&);
bool operator<=(const Permutation&, const Permutation&);

inline void checkGroupOrders(const Permutation&, const Permutation&);
template<typename T>
inline void swap(T&, T&);


Permutation::Permutation(int ord)
{
	_grp_order = ord;
	_order_saved = 1;
	_parity_saved = 1;
	_data = new int[_grp_order];
	for (int i = 0; i < _grp_order; i++) _data[i] = i;
}

Permutation::Permutation(int ord, int* data)
{
	_grp_order = ord;
	_order_saved = ORDER_NONE;
	_parity_saved = PARITY_NONE;
	_data = new int[_grp_order];
	std::memcpy(_data, data, _grp_order*sizeof(int));
}

Permutation::Permutation(const Permutation& perm)
{
	_grp_order = perm._grp_order;
	_order_saved = perm._order_saved;
	_parity_saved = perm._parity_saved;
	_data = new int[_grp_order];
	std::memcpy(_data, perm._data, _grp_order*sizeof(int));
}

Permutation::~Permutation()
{
	if (_data != nullptr) delete[] _data;
}

Permutation& Permutation::operator=(const Permutation& perm)
{
	if (this == &perm) return *this;

	if (_data != nullptr) delete[] _data;
	_grp_order = perm._grp_order;
	_order_saved = perm._order_saved;
	_parity_saved = perm._parity_saved;
	_data = new int[_grp_order];
	std::memcpy(_data, perm._data, _grp_order*sizeof(int));

	return *this;
}

int Permutation::operator[](int index) const
{
	return _data[index];
}

void Permutation::operator()(int* array) const
{
	int* tmp = new int[_grp_order];
	std::memcpy(tmp, array, _grp_order*sizeof(int));
	for (int i = 0; i < _grp_order; i++)
		array[_data[i]] = tmp[i];
	delete[] tmp;
}

void Permutation::clearSaved() const
{
	_order_saved = ORDER_NONE;
	_parity_saved = PARITY_NONE;
}

int Permutation::groupOrder() const
{
	return _grp_order;
}

int Permutation::order() const
{
	if (_order_saved != ORDER_NONE) return _order_saved;

	Permutation id = Permutation(_grp_order);

	_order_saved = 1;
	for (Permutation curr(*this); curr != id; curr *= *this) 
		_order_saved++;

	return _order_saved;
}

bool Permutation::isEven() const
{
	return parity() == 1;
}

bool Permutation::isOdd() const
{
	return parity() == -1;
}

int Permutation::parity() const
{
	if (_parity_saved != PARITY_NONE) return _parity_saved;

	bool* checked = new bool[_grp_order] {false};

	int number_of_even_cycles = 0;
	for (int i = 0; i < _grp_order; i++)
	{
		if (checked[i]) continue;
		int cycle_length = 1;
		checked[i] = true;
		for (int j = _data[i]; j != i; j = _data[j])
		{
			checked[j] = true;
			cycle_length++;
		}
		if (!(cycle_length % 2)) number_of_even_cycles++;
	}

	delete[] checked;

	_parity_saved = !(number_of_even_cycles % 2) ? 1 : -1;
	return _parity_saved;
}

const Permutation Permutation::next() const
{
	Permutation result(*this);
	return ++result;
}

const Permutation Permutation::previous() const
{
	Permutation result(*this);
	return --result;
}

const Permutation Permutation::inverse() const
{
	Permutation result(_grp_order);
	for (int i = 0; i < _grp_order; i++)
		result._data[_data[i]] = i;
	result.clearSaved();
	return result;
}

size_t Permutation::inversionCount() const
{
	int* buffer = new int[_grp_order];
	int* data_cpy = new int[_grp_order];
	std::memcpy(data_cpy, _data, _grp_order*sizeof(int));
	size_t answer = _inversionCount(data_cpy, data_cpy + _grp_order, buffer);
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

const Permutation Permutation::pow(int n) const
{
	if (n == 0) return Permutation(_grp_order);
	if (n < 0)
	{
		return this->inverse().pow(-n);
	}

	Permutation result(*this);

	bool* checked = new bool[_grp_order] {false};

	for (int i = 0; i < _grp_order; i++)
	{
		if (checked[i]) continue;

		int cycle_length = 1;
		checked[i] = true;
		for (int j = _data[i]; j != i; j = _data[j])
		{
			checked[j] = true;
			cycle_length++;
		}

		int* cycle = new int[cycle_length];
		cycle[0] = i;
		for (int j = _data[i], k = 1; j != i; j = _data[j], k++)
		{
			cycle[k] = j;
		}

		int offset = (n - 1) % cycle_length;
		for (int j = 0; j < cycle_length; j++)
		{
			result._data[cycle[j]]
				= _data[cycle[(j + offset) % cycle_length]];
		}

		delete[] cycle;
	}

	delete[] checked;

	return result;
}

const Permutation operator*(const Permutation& lhs, const Permutation& rhs)
{
	checkGroupOrders(lhs, rhs);
	Permutation result(lhs._grp_order);

	for (int i = 0; i < result._grp_order; i++)
		result._data[i] = lhs[rhs[i]];
	result.clearSaved();
	return result;
}

Permutation& operator*=(Permutation& lhs, const Permutation& rhs)
{
	lhs = lhs * rhs;
	return lhs;
}

Permutation& operator++(Permutation& op)
{
	int last_growing = op._grp_order - 1;
	while (last_growing > 0 && op._data[last_growing - 1] >= op._data[last_growing]) last_growing--;

	if (last_growing == 0)
	{
		for (int i = 0; i < op._grp_order; i++) op._data[i] = i;
		op.clearSaved();
		return op;
	}

	int last_bigger = op._grp_order - 1;
	while (op._data[last_growing - 1] > op._data[last_bigger]) last_bigger--;

	swap(op._data[last_bigger], op._data[last_growing - 1]);

	for (int i = 0; i < (op._grp_order - last_growing)/2; i++)
	{
		swap(op._data[last_growing + i], op._data[op._grp_order - i - 1]);
	}

	op.clearSaved();
	return op;
}

Permutation& operator--(Permutation& op)
{
	int last_decring = op._grp_order - 1;
	while (last_decring > 0 && op._data[last_decring - 1] <= op._data[last_decring]) last_decring--;

	if (last_decring == 0) 
	{
		for (int i = 0; i < op._grp_order; i++) op._data[i] = op._grp_order - i - 1;
		op.clearSaved();
		return op;
	}

	int last_smaller = op._grp_order - 1;
	while (op._data[last_decring - 1] < op._data[last_smaller]) last_smaller--;

	swap(op._data[last_smaller], op._data[last_decring - 1]);

	for (int i = 0; i < (op._grp_order - last_decring)/2; i++)
	{
		swap(op._data[last_decring + i],
			op._data[op._grp_order - i - 1]);
	}

	op.clearSaved();
	return op;
}

Permutation operator++(Permutation& op, int)
{
	Permutation tmp = op;
	++op;
	return tmp;
}

Permutation operator--(Permutation& op, int)
{
	Permutation tmp = op;
	--op;
	return tmp;
}

bool operator==(const Permutation& lhs, const Permutation& rhs)
{
	checkGroupOrders(lhs, rhs);
	for (int i = 0; i < lhs.groupOrder(); i++)
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
	checkGroupOrders(lhs, rhs);
	int i = 0;
	while (i < lhs.groupOrder() && lhs[i] == rhs[i]) i++;

	if (i == lhs.groupOrder()) return true;
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

inline void checkGroupOrders(const Permutation& first, const Permutation& second)
{
	if (first.groupOrder() == second.groupOrder()) return;
	
	std::cerr << INCORRECT_ORDERS_MSG << std::endl;
	throw INCORRECT_ORDERS;
}

template<typename T>
inline void swap(T& first, T& second)
{
	T tmp = second;
	second = first;
	first = tmp;
}

std::ostream& operator<<(std::ostream& out, const Permutation& perm)
{
	out << "(";
	for (int i = 0; i < perm.groupOrder(); i++)
	{
		out << (perm[i] + 1);
		if (i != perm.groupOrder() - 1) out << " ";
	}
	out << ")";
	return out;
}