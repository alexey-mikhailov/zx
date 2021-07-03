Properties
==========

Properties are get & set method pairs to encapsulate data. 
They look like this in C#: 

```c#
public class PropertyUser
{
	private readonly Dependency _dependency;
	private readonly Owned _owned = new();

	// Expose them read only to deprecate reseat reference. 
	public Dependency Dependency => _dependency;
	public Owned Owned => _owned;

	public float Radius
	{
		get => _radius;
		set 
		{
			PreActionIfNeeded();
			_radius = value; 
			_cached_circumference = 2 * pi * _radius;
			_cached_area = pi * pi * _radius;
			PosActionIdNeeded();
		}
	}

	PropertyUser(Dependency dependency)
	{
		_dependency = dependency;
	}
}

// Usage
var user = new PropertyUser();
user.Radius = 10.0f;
```

The property mechanism gives possibility to use fields like a get & set method pair. Usually data encapsulation via property pattern needs exactly these pair (get & set). We will name it further as _getter_ and _setter_ hidden functions. Of course, the real type of property is not type of aggregated data. But with help of them you can: 
- get the data via operator* or operator->
- set the data via precedig = (equal sign)

Access behaviour classification
-------------------------------

We know that some properties let you only get value without permission (_read only_), other of them let you do both of _read and write_ operations. We do not consider _write only_ properties because they are used very seldom and sometimes people consider them as antipattern. 

 Access Behaviour | Description
------------------|------------------------------------------------------------------------------------
 Getter           | __Read Only.__    <br>To get value, it will be refer to memory where field data is. 
 Formula          | __Read Only.__    <br>To get value, it will be computed during every access. 
 Computer         | __Read & Write.__ <br>To get/set value, it will be computed during every access. 
 Wrapper          | __Read & Write.__ <br>To get value, it will be refer to memory where field data is. <br>To set value, it will be computed during every access. 

Dereference behaviour classification
---------------------

The next classification of properties is dereference behaviour. To tell about them, let's check use case first. 
Let the perfect C++ service class is looking like:

```c++
class t, polymorphic_t;

class api_perfect_receiver_and_exposer
{
public:
	// Expose sealed type instance by value
	int get_by_value() const { return _value; }

	// Expose polymorphic type instance by value
	std::unique_ptr<polymorphic_t> get_polymorphic_by_value() const
    {
        return std::make_unique(*_polymorphic_value);
    }

	// Expose new generated object (by val/ref no matter)
	std::unique_ptr<polymorphic_t> concat() const
    {
        return std::make_unique<...>() { ... + ... };
    }

	// Expose by reference
	std::shared_ptr<t> get_by_reference() const 
    {
        return _reference;
    }

	// Take arg with ownership by reference
	// To use and forget
	void do_job1(std::unique_ptr<t> arg);

	// Take arg  w/o ownership by reference
	void do_job2(const t& arg);

	// Take arg with ownership by reference
	// to keep in instance of this type
	void store_reference(std::shared_ptr<t> arg);

private: 
	int _value; 
	std::unique_ptr<polymorphic_t> _polymorphic_value;	
	std::shared_ptr<t> _reference;
};
```

This code sample is based on several aspects of Modern C++. Corresponding to [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines) and [Law of Demeter](https://stackoverflow.com/questions/23112178/what-is-the-right-way-to-expose-resources-owned-by-a-class/23114713), no reason to expose class internals via unique_ptr. 

Namely, these rules of Modern C++ influenced this properties concept: 

- C.9: Minimize exposure of members
- F.7: For general use, take T* or T& arguments rather than smart pointers
- F.26: Use a unique_ptr<T> to transfer ownership where a pointer is needed
- F.27: Use a shared_ptr<T> to share ownership

So, shortly we need to expose the following types of properties: 

Dereference Behaviour | Semantic                                                               | Getter Return Value
----------------------|------------------------------------------------------------------------|---------------------------------
By Value              | I own it. It's cheap to copy. Take it.                                 | Derefers to value (object copy). 
By Reference          | I own it. It's either expensive to copy, or polymorphic or reseatable. | Derefers to reference. 
By Shared Pointer     | I don't own it.                                                        | Derefers to pointer previously being derefered to shared pointer. 
~~By Unique Pointer~~ | ~~Oops~~       	                                                       | Deprecated due to broken Law of Demeter. 

Full list of property types: 

Type Name        | Access Behaviour | Dereference Behaviour
-----------------|------------------|----------------------------
zx_ref_getter    | Getter           | By Reference
zx_ref_formula   | Formula          | By Reference
zx_ref_computer  | Computer         | By Reference
zx_ref_wrapper   | Wrapper          | By Reference
zx_shr_getter    | Getter           | By Shared Pointer
zx_shr_formula   | Formula          | By Shared Pointer
zx_shr_computer  | Computer         | By Shared Pointer
zx_shr_wrapper   | Wrapper          | By Shared Pointer
zx_val_getter    | Getter           | By Value
zx_val_formula   | Formula          | By Value
zx_val_computer  | Computer         | By Value
zx_val_wrapper   | Wrapper          | By Value

Thus, properties are no more than syntax sugar for get/set method pairs. 

Fore more information see [./core_test/propertytest](./core_test/propertytest) folder. 
