#include "IntList.h"
#include <string>




IntList::IntList(int8_t num_fields) :
    fixed(),
    num_fields(num_fields),
    num_elt(0),
    cap(fixed_cap),
    data(fixed),
    first_free(-1)
{
}

IntList::~IntList()
{
    if (fixed != data)
	   delete data;
}

int IntList::get_field(int idx, int n) const
{
    return data[idx * num_fields + n];
}

void IntList::set_field(int idx, int n, int val)
{
    data[idx * num_fields + n] = val;
}

int IntList::insert()
{
    //
    if (first_free >= 0)
    {
	   int rs = first_free;
	   first_free = get_field(first_free, 0);
	   ++num_elt;

	   return rs;
    }

    //neu vuot qua cap thi cap phat lai
    if ((num_elt + 1) * num_fields >= cap)
    {
	   cap <<= 1;
	   if (data == fixed)
	   {
		  data = new int32_t[cap];
		  data = (int32_t*)memcpy(data, fixed, sizeof(fixed));
	   }
	   else
	   {
		  data = (int32_t*)realloc(data, cap * sizeof(int32_t));
	   }
    }

    ++num_elt;

    return num_elt - 1;
}

int IntList::get_num_elt() const
{
    return num_elt;
}

void IntList::remove(int idx)
{
    set_field(idx, 0, first_free);
    first_free = idx;
    --num_elt;
}

void IntList::clear()
{
    first_free = -1;
    num_elt = 0;
}
