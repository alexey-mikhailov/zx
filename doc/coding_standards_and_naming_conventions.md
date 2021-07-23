# ZX Coding Standards and Naming Conventions


| Object Name               | Notation   | Length | Plural | Prefix | Suffix | Abbreviation | Char Mask          | Underscores |
|:--------------------------|:-----------|-------:|:-------|:-------|:-------|:-------------|:-------------------|:------------|
| Namespace name            | snake_case |    128 | Yes    | Yes    | No     | No           | [A-z][0-9]         | Yes         |
| Class name                | snake_case |    128 | No     | No     | Yes    | No           | [A-z][0-9]         | Yes         |
| Constructor name          | snake_case |    128 | No     | No     | Yes    | No           | [A-z][0-9]         | Yes         |
| Method name               | snake_case |    128 | Yes    | No     | No     | Yes          | [A-z][0-9]         | Yes         |
| Method arguments          | snake_case |    128 | Yes    | No     | No     | Yes          | [A-z][0-9]         | Yes         |
| Local variables           | snake_case |     50 | Yes    | No     | No     | Yes          | [A-z][0-9]         | Yes         |
| Constants name            | snake_case |     50 | No     | No     | No     | Yes          | [A-z][0-9]         | Yes         |
| Field name                | snake_case |     50 | Yes    | No     | No     | Yes          | [A-z][0-9]         | Yes         |
| Properties name           | snake_case |     50 | Yes    | No     | No     | Yes          | [A-z][0-9]         | Yes         |
| Delegate name             | snake_case |    128 | No     | No     | Yes    | Yes          | [A-z]              | Yes         |
| Enum type name            | snake_case |    128 | Yes    | No     | No     | Yes          | [A-z]              | Yes         |

<br>

#### 1. Do use snake_casing for class names and method names:

```c++
class client_activity
{
	void client_statistics();
	void calculate_statistics();
}
```

> **WHY?** 
>
> Consistent with the C++ STL and easy to read.

<br>

#### 2. Do use snake_casing for method arguments and local variables:

```c++
class user_log
{
	void add(log_event log_event)
	{
		int item_count = log_event.items.count();
		// ...
	}
}
```

> **WHY?** 
>
> Consistent with the C++ STL and easy to read.

<br>

#### 3. Do not use Hungarian notation or any other type identification in identifiers

```c++
// Correct
int counter;
std::string name;    

// Avoid
int i_counter;
std::string str_name;
```

> **WHY?** 
>
> Consistent with the C++ STL and Visual Studio IDE makes determining types very easy (via tooltips). In general, you want to avoid type indicators in any identifier.

<br>

#### 4. Do not use Screaming Caps for constants or readonly variables:

```c++
// Correct
const std::string shipping_type = "DropShip";
// Avoid
const std::string SHIPPING_TYPE = "DropShip";
```

> **WHY?** 
>
> Consistent with the C++ STL. Caps grab too much attention.

<br>

#### 5. Use meaningful names for variables. The following example uses seattle_customers for customers who are located in Seattle:

```c++
auto seattle_customers = 
	database.customers
	.where {
			   city: "Seattle", 
		   }
	.select(c => c.get_name())
	.execute();
```

> **WHY?** 
>
> Consistent with the C++ STL and easy to read.

<br>

#### 6. Avoid using Abbreviations. Exceptions: abbreviations commonly used as names, such as Id, Xml, Ftp, Uri.

```c++    
// Correct
user_group user_group;
assignment employee_assignment;     

// Avoid
user_group usr_grp;
assignment emp_assignment; 

// Exceptions
customer_id customer_id;
xml_document xml_document;
ftp_helper ftp_helper;
uri_part uri_part;
```

> **WHY?** 
>
> Consistent with the C++ STL and prevents inconsistent abbreviations.

<br>

#### 7. Do use snake_casing for any abbreviations.:

```c++  
html_helper html_helper;
ftp_transfer ftp_transfer, fast_ftp_transfer;
ui_constrol ui_control, next_ui_control;
```

> **WHY?** 
>
> Consistent with the C++ STL. Caps would grab visually too much attention.

<br>

#### 8. Do use underscores in identifiers everywhere. Prefix private fields with an underscore:

```c++ 
// Correct
date_time client_appointment;
time_span time_left; 

// Avoid
DateTime clientAppointment;
TimeSpan timeLeft;    

// Private fields
date_time _registration_date;
```

> **WHY?** 
>
> Consistent with the C++ STL, that means, easy to read.

<br>

#### 9. Do use typedefed type names (aliases) like `zx::u64`, `zx::i32` for local, parameter and member declarations. 

```c++
// Correct
zx::i32 offset;
zx::u64 size;
// Avoid
int offset;
size_t size;
```

> **WHY?** 
>
> Explicitly declared size and presence of sign.

<br>

#### 10. Do use implicit type var for local variable declarations. Exception: primitive types (`zx::i32`, `zx::u08`, `zx::f64`, etc) may use predefined names. 

```c++ 
auto stream = create_file(path);
auto customers = std::make_unique<customer_list_t>();

// Exceptions
zx::u16 index = 100ui16;
zx::i08 offset = 192i8;
zx::f32 vector_length = 1.0f;
zx::f64 precise_amount = zx::zero::f64;
```

> **WHY?** 
>
> Removes clutter, particularly with complex template types. Type is easily detected with Visual Studio tooltips.

<br>

#### 11. Do use noun or noun phrases to name a class. 

```c++ 
class employee;
class business_location;
class document_collection;
```

> **WHY?** 
>
> Consistent with C++ STL and easy to remember.

<br>

#### 12. Do prefix interfaces with the letter `i_`. Interface names are noun (phrases) or adjectives.

```c++     
struct i_shape
{
	virtual ~i_shape = default;
	virtual return_type some_method() = 0;
};

struct i_shape_collection
{
	virtual ~i_shape_collection = default;
	virtual return_type some_method() = 0;
};

struct i_groupable
{
	virtual ~i_groupable = default;
	virtual return_type some_method() = 0;
};
```

> **WHY?** 
>
> Consistent with C++ STL.

<br>

#### 13. Do name source files according to their main classes. Exception: bunch of small types (no more than promising 50 lines) in single file, module enums in single file. 

```c++ 
// Located in some_class.h
class some_class
{
};

// Located in some_namespace_structs.h
namespace some_namespace
{
	struct operation1_result
	{
		// A bit of code :)
	};

	struct operation2_result
	{
		// A bit of code :)
	};

	struct operation3_result
	{
		// A bit of code :)
	};
}
```

> **WHY?** 
>
> Consistent with the common practices. Files are alphabetically sorted, so types may be easy to find.

<br>

#### 14. Do organize namespaces with a clearly defined structure: 

```c++ 
// For example
namespace framework
{
	namespace layer
	{
		namespace feature
		{
			namespace group { }
		}
	}
}
```

> **WHY?** 
>
> Maintains good organization of your code base.

<br>

#### 15. Do vertically align curly brackets: 

```c++ 
// Correct
namespace framework
{
	void function()
	{
		//...
	}
}
```

> **WHY?** 
>
> Developers have overwhelmingly preferred vertically aligned brackets.

<br>

#### 16. Do declare all member variables at the top of a class, with static variables at the very top.

```c++ 
// Correct
class account
{
	static std::string bank_name;
	static zx::i32 reserves;
	std::string number;
	date_time date_opened;
	date_time date_closed;
	zx::f32 Balance;

public: 
	// Constructor
	explicit account()
	{
		// ...
	}
}
```

> **WHY?** 
>
> C++ class has default private section where private field fits perfectly. Besides of that, class or struct declaration assigns offsets in memory for every member depending on how early they were declared. Pushing data down to the bottom leads to problems with constructor initializer list. Constructor initializer list will initialize once per instance if they are aligned corresponding to declaration order.

<br>

#### 17. Do use singular names for enums. Exception: bit field enums.

```c++ 
// Correct
enum class color
{
	red,
	green,
	blue,
	yellow,
	magenta,
	cyan
};

// Exception
enum class dockings
{
	none = 0,
	top = 1,
	right = 2, 
	bottom = 4,
	left = 8
};
```

> **WHY?** 
>
> Makes the code more natural to read. Plural flags because enum can hold multiple values (using bitwise operations).

<br>

#### 18. Do not explicitly specify a type of an enum or values of enums (except bit fields):

```c++ 
// Don't
enum class direction : zx::u64
{
	north = 1ui64,
	east = 2ui64,
	south = 3ui64,
	west = 4ui64
};

// Correct
enum class direction
{
	north,
	east,
	south,
	west
};

// Exception
enum class dockings : zx::u64
{
	none = 0ui64,
	top = 1ui64,
	right = 2ui64, 
	bottom = 4ui64,
	left = 8ui64,
	// ...
	// very big flags
};
```

> **WHY?** 
>
> Can create confusion when relying on actual types and values.

<br>

#### 19. Do not use an "enum" suffix in enum type names:

```c++     
// Don't
enum class coin_enum
{
	penny,
	nickel,
	dime,
	quarter,
	dollar
};

// Correct
enum class coin
{
	penny,
	nickel,
	dime,
	quarter,
	dollar
};
```

> **WHY?** 
>
> Consistent with the Microsoft's .NET Framework and consistent with prior rule of no type indicators in identifiers.

<br>

#### 20. Prefer to not to use "flag" or "flags" suffixes in enum type names:

```c++ 
// Correct
enum class docking_flags
{
	none = 0,
	top = 1,
	right = 2, 
	bottom = 4,
	left = 8
}

// Preferable
enum class dockings
{
	none = 0,
	top = 1,
	right = 2, 
	bottom = 4,
	left = 8
}
```

> **WHY?** 
>
> Consistent with prior rule of no type indicators in identifiers.

<br>

#### 21. Do use suffix `_exception` at creation of the new classes comprising the information on exception:

```c++ 
// Correct
class barcode_read_exception : public std::exception {};
```

> **WHY?**
>
> Easy to read.

<br>

#### 22. Do use prefix `any_`, `is_`, `has_` or similar keywords for boolean identifier:

```c++ 
// Correct
static bool is_null_or_empty(std::string value) 
{
	return (value || value.length());
}
```

> **WHY?** 
>
> Easy to determine boolean return type.


