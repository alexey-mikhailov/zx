ZX Booster
==========

C++ booster. Dependency Injection, properties, delegates, reflection. 

The main intentions of this library are:
- Forward features of languages such C# in C++, 
- Enrich functionality of language with inversion of control via modern Dependency Injection approach. It will help to solve Law of Demeter with injected dependency abstractions not via constructors, yet via injection container. 

Brief Overview
--------------

If you are not familiar with all these missing in C++ language features, then I recommend you to jump to [Deeper Overview](#deeper-overview) section below in this document. 

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
u.dependency = ..; // forbidden

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

### Delegates and Events

```c++
class cargo, location;

// Event provider (observer, dispatcher)
class cargo_dispatcher
{
	zx::multicast_delegate<void(const cargo&, const location&)> _cargo_arrived
public:
	zx::event<void(const cargo&, const location&)> cargo_arrived = _cargo_arrived;

private:
	void watch_cargo()
	{
		while (_is_running)
		{
			if (there_is_new_cargo)
			{
				auto cargo = get_arrived_cargo();
				auto where_from = get_department_location();

				_cargo_arrived(cargo, where_from);
			}
		}
	}
}

// Event consumer (performer)
class unloader
{
	unloader(const cargo_dispatcher& dispatcher)
	{
		dispatcher.cargo_arrived += on_cargo_arrived;
	}

	void on_cargo_arrived(const cargo&    cargo, 
						  const location& where_from)
	{
		if (is_trusted_location(where_from))
		{
			unload(cargo);
		}
	}
}
```

### Reflection



### Dependency Injection



Deeper Overview
---------------

Each following section will consider every dedicated missing language/framework feature in C++/STL. For simplicity, we will start with comparison to existing language solutions, describe known problems and try to supply graceful replacement. 



Properties
----------

Properties are get & set method pairs to encapsulate data. 
They look like this in C#: 

```c#
public class Circle
{
	// Fields:
	float _radius;
	float _cachedCircumference;
	float _cachedArea;
	private readonly ShapeDispatcher _dispatcher;
	private readonly CircleDrawer _drawer = new();

	// Read only properties,
	// to deprecate reference reseat:
	public ShapeDispatcher Dispatcher => _dispatcher;
	public CircleDrawer Drawer => _drawer;

	// Read-write property:
	public float Radius
	{
		get => _radius;
		set 
		{
			PreActionIfNeeded();
			_radius = value; 
			_cachedCircumference = 2 * pi * _radius;
			_cachedArea = pi * pi * _radius;
			PosActionIdNeeded();
		}
	}

	Circle(ShapeDispatcher dispatcher)
	{
		_dispatcher = dispatcher;
	}
}

// Usage
var circle = new Circle();
circle.Radius = 10.0f;
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

But, from another hand, this use case may be justified. Consider class `Circle` from above example:

```c++
class circle
{
	float _radius;
	float _cached_circumference;
	float _cached_area;
	...

public:
	...

	float radius() const { return _radius; }

	void set_radius(float value)
	{
		// It is not just only about set new value. 
		// Do not treat it verbatim, look: 
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
	lib::dependency& _dependency;
	lib::owned&      _owned;

public:
	zx::ref_getter<lib::dependency> dependency = &_dependency;
};

// client code:
property_user u;
u.dependency->do_something();
u.dependency = ..; // forbidden

// or
class circle final
{
	float _radius;
	float _cached_area;
	std::shared_ptr<shape_dispatcher> _dispatcher;
	std::unique_ptr<circle_drawer> _drawer = new();

public:
	explicit circle(std::shared_ptr<shape_dispatcher> dispatcher) :
		_dispatcher(dispatcher)
	{
	}

	zx::shr_getter<>

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

For details, see [under the hood](doc/properties.md). 


Delegates and Events
--------------------

Delegates (arch: callbacks) are invented to notify (feedback to) notification receiver. 

For example, in C#, qualifiers _delegate_ and _event_ becomes a little messy in terms of C++, because delegates are types (signatures) of callback, yet events are instances of some delegate, i.e. callbacks with subscription/unsubscription functionality designed to be exposed in the class. Delegates are types (signatures) of events, they are multicasted by default. That means they may consume many subscribers to be notified. Both of them are using like this in C#: 

```c#
// Event of this delegate will
// receive Cargo and Location instances
// and return nothing. 
public delegate void CargoArrivedDelegate(Cargo cargo,
										  Location whereFrom);

// Observer
public class CargoDispatcher
{
	public event CargoArrivedDelegate CargoArrived;

	private void WatchCargo()
	{
		while (_isRunning)
		{
			if (thereIsNewCargo)
			{
				var cargo = GetArrivedCargo();
				var whereFrom = GetDepartmentLocation();

				CargoArrived?.Invoke(cargo, whereFrom);
			}
		}
	}
}

// Notification consumer
public class Unloader
{
	public Unloader(CargoDispatcher dispatcher)
	{
		dispatcher.CargoArrived += OnCargoArrived;
	}

	private void OnCargoArrived(Cargo cargo,
								Location whereFrom)
	{
		if (IsTrustedLocation(whereFrom))
		{
			Unload(cargo);
		}
	}
}
```

Fore more information see unit test folder: [./core_test/eventtest](./core_test/eventtest/)

Reflection
----------



Dependency Injection
--------------------

