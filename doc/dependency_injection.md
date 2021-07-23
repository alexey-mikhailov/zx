Dependency Injection
--------------------

Many languages already have this feature in restricted mode. Passing arguments in constructor is already dependency injection. Even more, passing service abstractions in constructor is already almost all you need. The problem is if you want to detach a couple of dependencies from whole program, you may feel lot of pain. Constructor DI is brief and graceful enough but doesn't offer robust refactoring protection. 

For example, in C#/.NET stack, starting with .NET 5, dependency injection is becoming out of the box feature. It provides injection of dependencies with different lifetimes: transient, scoped and singleton. In terms of ASP.NET and web terminology, _Transient_ objects are always different; a new instance is provided to every controller and every service. _Scoped_ objects are the same within a request, but different across different requests. _Singleton_ objects are the same for every object and every request. 

```c#
// DI setup
public void ConfigureServices(IServiceCollection services)
{
    // ...
    services.AddTransient     <ISomeInterface1, SomeImplementation1>();
    services.AddScoped        <ISomeInterface2, SomeImplementation2>();
    services.AddSingleton     <ISomeInterface3, SomeImplementation3>();
    services.AddHostedService <MainLoop>();
    // ...
}

// Entry point
static async Task Main(string[] args)
{
	using IHost host = 
		Host.CreateDefaultBuilder(args)
			.ConfigureServices(ConfigureServices).Build();

	await host.RunAsync();
}

// Client code
public class MainLoop : BackgroundService
{
    private readonly ISomeInterface3 _instance;

    public MainLoop(ISomeInterface3 instance) => _instance = instance;

    protected override async Task ExecuteAsync(CancellationToken stoppingToken)
    {
        // Voila. That's dependency we use inside. 
		_instance.DoSomething();
		// ...
    }
}
```

Similar code with `zx` will look like this: 

```c++ 
// DI setup
void some_di_container::bind_all()
{
	bind_singleton<i_some_interface_t, some_implementation_t>();
}

class main_loop
{
	std::shared_ptr<i_some_interface_t> _interface_inst;

public:
    main_loop()
    {
        zx::inject_dependencies(this);
    }
};

main_loop::ctor::ctor()
{
	zx::meta::registry::add_field(
		nameof(_interface_inst),
		&some_injectee::_interface_inst,
		zx::make_fieldpawn_of_singleton());
}

// Client code
void main_loop::execute()
{
    // Voila. That's dependency we use inside. 
	_interface_inst->do_something();
    // ...
}
```

The whole injection process is splitted to three stages:
- **Pawning**. When you manually define _pawns_ (e.g., fields) in your types, which will be affected by DI. 
- **Binding**. when you create mapping between abstract and concrete types. 
- **Adoption**. when instances are created and are put in _container_. 
- **Injection**. when adopted instances are injected into _injectees_ and became _pawns_. 

In its turn, 

- **Pawns** are targets of injection. 
- **Injectee** is type which uses DI to resolve dependency by using container. 
- **Container** is injector itself. It resolves dependency and assigns it to pawn. 

As you definitely noticed (did you?), `zx::di_container` provides only _singleton_ lifetime, there are no _scoped_ or _transient_ bindings. Probably, we will add it in future releases. 

Let's talk about lifetime of singletons. By default, singleton instance of bound type created when `zx::inject_dependencies()` be invoked. But you can do it instantly from your `zx::di_container::bind_all()` overload. Anyway, you can provide arguments for future instance as parameters inside of `zx::bind_singleton()` call: 

```c++
void test_di_container::bind_all()
{
    // Just bind it. It will be instantiated on demand. 
	bind_singleton<some_concrete1_t>(singeton_arg1);

    // Create instance at binding time. 
	bind_singleton<some_concrete1_t>(singeton_arg1).create();
}
```

From another side, singleton will be alive until counter of `std::shared_ptr` becomes zeroed. Since the last reference to it is `zx::di_container` itself, singleton surely lives until owning `zx::di_container` dies. 

Container also allows named singleton (so called 'nameleton')—one instance of type per name. This may be useful when you want to bind several long-living instances across single container. Look at this: 

```c++
void test_di_container::bind_all()
{
	bind_nameleton<the_same_concrete_type>("instance 1", nameleton1_arg);
	bind_nameleton<the_same_concrete_type>("instance 2", nameleton2_arg);
	bind_nameleton<i_same_interface, the_same_implementation>("instance 1");
	bind_nameleton<i_same_interface, the_same_implementation>("instance 2");
}
```
A couple of words about signal packs. Signal packs are items which will be pawned to injectee to corresponding fieldpawn. Using signal pack you can subscribe to its delegates (_signals_), which are treated as container-wide signals to fire from another side. 

```c++
class some_signal_pack
{
public:
	explicit some_signal_pack() = default;
	virtual ~some_signal_pack() = default;

	zx::multicast_delegate<void(arg1_t, arg2_t)> something_happened;
};
```

`zx::bind_signal_pack` is no more than shortcut to binding singleton of `zx::signal_pack` instance. 


Fore more information see unit test folder: [./core_test/dicontainertest](./core_test/dicontainertest/)