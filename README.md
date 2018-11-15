# C List Types

There are numerous C libraries that provide support for a list, but
none (that I found) that support stong typing. Of couse, this is not
possible with C, but the code here provides some close approximation.

Where standard list implementations act as containers that hold `void
*` objects, this implementation is essentially a set of macros that
builds type-specific list data structures, prototypes, and functions.

For example, say we want a list that supports objects of type
`uint8_t`, we can create the prototypes for such a list by including
these two lines in a header file:

```
#include "c_list_types.h"

create_c_list_headers(uint8_t_list, uint8_t)
```

Here the macro `create_c_list_headers` takes two arguments, the first
is the name given to the type of list, the second is the type of
element the list will hold. This macro creates the data structure and
prototypes for such a list.

Next we add this line in a corresponding source file:

```
create_c_list_type(uint8_t_list, uint8_t)
```

The macro `create_c_list_type` also takes two arguments, the first is
the name given to the type of list, the second is the type of element
the list will hold. These arguments must be the same as those given to
`create_c_list_headers`.

This macro creates the functions used to initialize, build, inspect, and free this type of list object.

The following lines create an object of the new type `uint8_t_list`, add some elements to the list, and then free it.

```
uint8_t_list *myList = uint8_t_list_alloc(0, 0);

for(uint32_t i = 0; i < 10000; i++) {
  uint8_t_list_push(myList, (uint8_t) i);
}
  
uint8_t_list_free(myList, NULL);
```

An example is provided in `test.c`. Compile and run it by typing `make && test/test`

More documentation to come.
