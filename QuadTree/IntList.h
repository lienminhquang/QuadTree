#pragma once
#include <cstdint>

enum 
{
	fixed_cap = 128
};

class IntList
{
public:
	IntList(int8_t num_fields);
	~IntList();


	int get_field(int idx, int n) const;
	void set_field(int idx, int n, int val);
	int insert();
	int get_num_elt() const;
	void remove(int idx);
	void clear();

	int32_t* data;
	int32_t num_elt;
	int8_t num_fields;


public:
	int32_t fixed[fixed_cap];

	int32_t first_free;
	int32_t cap;
};

