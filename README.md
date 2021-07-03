ZX Booster
==========

C++ booster. Dependency Injection, properties, delegates, reflection. 

The main intentions of this library are:
- Forward features of languages such C# in C++, 
- Enrich functionality of language with inversion of control via modern Dependency Injection approach. It will help to solve Law of Demeter with injected dependency abstractions not via constructors, yet via injection container. 

Brief Overview
--------------

If you are not familiar with all these missing in C++ language features, jump to Deeper Overview section below in this document. 

### Properties

```c++
class property_user
{
	lib::dependency& _dependency;
	lib::owned&      _owned;

public:
	zx::ref_getter<lib::dependency> dependency = &_dependency;
};

// client code:
property_user u;
u.dependency->do_something();
u.dependency = ... // forbidden

// or
class circle
{
	float _radius;
	float _cached_area;

public:
	zx::val_wrapper<circle, float> radius =
	{
		this, &_radius,

		[](circle& user, const float& value)
		{
			user._radius = value;
			_cached_area = pi * pi * _radius;
		}
	};  
};
```

### Delegates



### Reflection



### Dependency Injection



Deeper Overwiew
---------------





Properties
----------

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
```


This funcionality of C# has a lot of critique because in real life sometimes things go different. We actually not often have permissions to get or set something in pair or even at least one  of them.  

For example,

```c++
class cargo
{
	lib::weight _weight;

public:
	// Yes, we can obtain current amount of weight. 
	// But, sometimes, may be forbidden. 
	lib::weight weight() const { return _weight; }
	// (Another naming convention, the same stuff)
	lib::weight get_weight() const { return _weight; }

	// Leverage encapsulated data
	void load(lib::weight amount);
	void unload(lib::weight amount);

	// Looks strange, right?
	void set_weight(lib::weight value);
};
```

But, from another hand, this use case may be justified. For example,

```c++
class circle
{
	float _radius;
	float _cached_circumference;
	float _cached_area;

public:
	float radius() const { return _radius; }

	void set_radius(float value)
	{
		// It is not just only about set new value. 
		// Do not treat it verbatim, thus
		pre_action_if_needed();
		_radius = value;
		_cached_circumference = 2 * pi * _radius;
		_cached_area = pi * pi * _radius;
		pos_action_if_needed();
	}
};
```



Conceptually zx will mirror it like this:

```c++
class property_user
{
	dependency
};
```

For details, see [under the hood](doc/properties.md). 



Delegates
---------
Reflection
----------
Dependency Injection
--------------------

