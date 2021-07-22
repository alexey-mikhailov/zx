# pragma once
# include "zx_iterables.h"

namespace zx
{
	template <class MemFn>
	size_t memfn_hash(MemFn ptr, void* owner)
	{
		const auto size = sizeof(size_t);
		size_t dest = 0;
		memcpy(&dest, &ptr, size);
		const auto owner_u64 = reinterpret_cast<size_t>(owner);
		return dest ^ owner_u64;
	}

	template<class Sig>
	struct mem_fn
	{
		template <class Owner>
		mem_fn(Owner* owner, Sig memfn);
	};

	template <class Owner, class Ret, class... Args>
	struct mem_fn<Ret(Owner::*)(Args...)>
	{
		Owner* owner;
		Ret(Owner::* memfn)(Args...);

		mem_fn(Owner* owner, Ret(Owner::* memfn)(Args...)) : owner(owner), memfn(memfn) {}
	};

	template <class Owner, class Ret, class... Args>
	struct mem_fn<Ret(Owner::*)(Args...) const>
	{
		Owner* owner;
		Ret(Owner::* memfn)(Args...) const;

		mem_fn(Owner* owner, Ret(Owner::* memfn)(Args...) const) : owner(owner), memfn(memfn) {}
	};

	template <class Sig>
	struct functor_info
	{
		functor_info(const std::string& key,
					 const std::function<Sig>& functor);
	};

	template <class Ret, class... Args>
	struct functor_info<Ret(Args...)>
	{
		std::string key;
		std::function<Ret(Args...)> functor;

		functor_info(const std::string& key,
					 const std::function<Ret(Args...)>& functor) :
			key(key), functor(functor) {  }
	};


	template <class Sig>
	class multicast_delegate;

	/// <summary>
	/// Multicast delegate.
	/// </summary>
	/// <typeparam name="Ret">Return value type. </typeparam>
	/// <typeparam name="...Args">Argument types. </typeparam>
	/// <remarks>
	/// Implemented as functor with public parentheses operator. 
	/// Designed to be private field or a temporary. 
	/// </remarks>
	template <class Ret, class... Args>
	class multicast_delegate<Ret(Args...)>
	{
		struct i_delegate
		{
			virtual ~i_delegate() = default;
			virtual Ret invoke(Args...) = 0;
		};

		struct delegate_map
		{
			std::unordered_map<size_t, i_delegate*> map;

			delegate_map() = default;

			~delegate_map()
			{
				for (auto del : map)
				{
					delete del.second;
				}
			}
		};

		struct raw_delegate : i_delegate
		{
			Ret(*callback)(Args...);

			raw_delegate(Ret(*callback)(Args...)) :
				callback(callback)
			{
			}

			Ret invoke(Args... args) override
			{
				return callback(args...);
			}
		};

		struct mem_fn_delegate : i_delegate
		{
			std::function<Ret(Args...)> functor;

			template <class Owner>
			mem_fn_delegate(Owner* owner, Ret(Owner::* cb)(Args...)) :
				functor([owner, cb](Args... args) { return (owner->*cb)(args...); })
			{
			}

			template <class Owner>
			mem_fn_delegate(Owner* owner, Ret(Owner::* cb)(Args...) const) :
				functor([owner, cb](Args... args) { return (owner->*cb)(args...); })
			{
			}

			Ret invoke(Args... args) override
			{
				return functor(args...);
			}
		};

		struct functor_delegate : i_delegate
		{
			std::function<Ret(Args...)> functor;

			template <class Functor>
			functor_delegate(const Functor& functor) :
				functor(functor)
			{
			}

			Ret invoke(Args... args) override
			{
				return functor(args...);
			}
		};

		std::shared_ptr<delegate_map> _delegates = std::make_shared<delegate_map>();
		std::shared_ptr<std::mutex> _mutex = std::make_shared<std::mutex>();

	public:
		multicast_delegate() = default;
		multicast_delegate(const multicast_delegate& other)
		{
			static_assert(
				false, 
				"You cannot implicitly copy the `zx::multicast_delegate`. "
				"Use method `copy` instead. ");
		}

		multicast_delegate(multicast_delegate&& other) :
			_delegates(std::move(other._delegates)),
			_mutex(std::move(other._mutex))
		{
		}

		/// <summary>
		/// Explicitly copies the instance of `multicast_delegate`. 
		/// Both of them will refer to the same delegate list. 
		/// </summary>
		/// <returns>New copied instance. </returns>
		multicast_delegate copy() const
		{
			multicast_delegate result;
			result._delegates = _delegates;
			result._mutex = _mutex;
			return std::move(result);
		}

		operator bool()
		{
			std::lock_guard<std::mutex> lk(*_mutex);
			return !!_delegates->map.size();
		}

		void operator +=(Ret(*cb)(Args...))
		{
			std::lock_guard<std::mutex> lk(*_mutex);

			_delegates->map.insert(
				{
					std::hash<Ret(*)(Args...)>()(cb),
					new raw_delegate(cb)
				});
		}

		template <class Owner>
		void operator +=(const mem_fn<Ret(Owner::*)(Args...)>& info)
		{
			std::lock_guard<std::mutex> lk(*_mutex);

			auto hash = memfn_hash(info.memfn, info.owner);
			_delegates->map.insert(
				{
					hash,
					new mem_fn_delegate(info.owner, info.memfn)
				});
		}

		template <class Owner>
		void operator +=(const mem_fn<Ret(Owner::*)(Args...) const>& info)
		{
			std::lock_guard<std::mutex> lk(*_mutex);

			auto hash = memfn_hash(info.memfn, info.owner);
			_delegates->map.insert(
				{
					hash,
					new mem_fn_delegate(info.owner, info.memfn)
				});
		}

		void operator +=(const functor_info<Ret(Args...)>& info)
		{
			std::lock_guard<std::mutex> lk(*_mutex);

			_delegates->map.insert(
				{
					std::hash<std::string>()(info.key),
					new functor_delegate(info.functor)
				});
		}

		void operator -=(Ret(*cb)(Args...))
		{
			std::lock_guard<std::mutex> lk(*_mutex);

			_delegates->map.erase(std::hash<Ret(*)(Args...)>()(cb));
		}

		template <class Owner>
		void operator -=(const mem_fn<Ret(Owner::*)(Args...)>& info)
		{
			std::lock_guard<std::mutex> lk(*_mutex);

			auto hash = memfn_hash(info.memfn, info.owner);
			_delegates->map.erase(hash);
		}

		template <class Owner>
		void operator -=(const mem_fn<Ret(Owner::*)(Args...) const>& info)
		{
			std::lock_guard<std::mutex> lk(*_mutex);

			auto hash = memfn_hash(info.memfn, info.owner);
			_delegates->map.erase(hash);
		}

		void operator -=(const std::string& key)
		{
			std::lock_guard<std::mutex> lk(*_mutex);

			_delegates->map.erase(std::hash<std::string>()(key));
		}

		auto get_delegates()
		{
			return iterable::imm::
				unordered_map<size_t, i_delegate*>
			(
				_delegates->map
			);
		}

		void handle_every(const std::function<void(Ret)>& routine, 
						  Args... args)
		{
			std::lock_guard<std::mutex> lk(*_mutex);

			for (auto it = _delegates->map.begin();
				 it != _delegates->map.end();
				 ++it)
			{
				routine(it->second->invoke(args...));
			}
		}

		Ret operator()(Args... args)
		{
			std::lock_guard<std::mutex> lk(*_mutex);

			Ret result = {};

			for (auto it = _delegates->map.begin();
				 it != _delegates->map.end();
				 ++it)
			{
				result = it->second->invoke(args...);
			}

			return result;
		}
	};

	/// <summary>
	/// Multicast delegate.
	/// </summary>
	/// <typeparam name="...Args">Argument types</typeparam>
	/// <remarks>
	/// Implemented as functor with public parentheses operator. 
	/// Designed to be private field or a temporary. 
	/// </remarks>
	template <class... Args>
	class multicast_delegate<void(Args...)>
	{
		struct delegate_map
		{
			std::unordered_map<size_t, struct i_delegate*> map;

			delegate_map() = default;

			~delegate_map()
			{
				for (auto del : map)
				{
					delete del.second;
				}
			}
		};

		struct i_delegate
		{
			virtual ~i_delegate() {}
			virtual void invoke(Args...) = 0;
		};

		struct raw_delegate : i_delegate
		{
			void(*callback)(Args...);

			raw_delegate(void(*callback)(Args...)) :
				callback(callback)
			{
			}

			void invoke(Args... args) override
			{
				return callback(args...);
			}
		};

		struct mem_fn_delegate : i_delegate
		{
			std::function<void(Args...)> functor;

			template <class Owner>
			mem_fn_delegate(Owner* owner, void(Owner::* cb)(Args...)) :
				functor([owner, cb](Args... args) { return (owner->*cb)(args...); })
			{
			}

			void invoke(Args... args) override
			{
				return functor(args...);
			}
		};

		struct functor_delegate : i_delegate
		{
			std::function<void(Args...)> functor;

			template <class Functor>
			functor_delegate(const Functor& functor) :
				functor(functor)
			{
			}

			void invoke(Args... args) override
			{
				return functor(args...);
			}
		};

		std::shared_ptr<delegate_map> _delegates = std::make_shared<delegate_map>();
		std::shared_ptr<std::mutex> _mutex = std::make_shared<std::mutex>();

	public:
		multicast_delegate() = default;
		multicast_delegate(const multicast_delegate& other)
		{
			static_assert(
				false, 
				"You cannot implicitly copy the `zx::multicast_delegate`. "
				"Use method `copy` instead. ");
		}

		multicast_delegate(multicast_delegate&& other) :
			_delegates(std::move(other._delegates)), 
			_mutex(std::move(other._mutex))
		{
		}

		/// <summary>
		/// Explicitly copies the instance of `multicast_delegate`. 
		/// Both of them will refer to the same delegate list. 
		/// </summary>
		/// <returns>New copied instance. </returns>
		multicast_delegate copy() const
		{
			multicast_delegate result;
			result._delegates = _delegates;
			result._mutex = _mutex;
			return std::move(result);
		}

		operator bool()
		{
			std::lock_guard<std::mutex> lk(*_mutex);

			return !!_delegates->map.size();
		}

		void operator +=(void(*cb)(Args...))
		{
			std::lock_guard<std::mutex> lk(*_mutex);

			_delegates->map.insert(
				{
					std::hash<void(*)(Args...)>()(cb),
					new raw_delegate(cb)
				});
		}

		template <class Owner>
		void operator +=(const mem_fn<void(Owner::*)(Args...)>& info)
		{
			std::lock_guard<std::mutex> lk(*_mutex);

			auto hash = memfn_hash(info.memfn, info.owner);
			_delegates->map.insert(
				{
					hash,
					new mem_fn_delegate(info.owner, info.memfn)
				});
		}

		template <class Owner>
		void operator +=(const mem_fn<void(Owner::*)(Args...) const>& info)
		{
			std::lock_guard<std::mutex> lk(*_mutex);

			auto hash = memfn_hash(info.memfn, info.owner);
			_delegates->map.insert(
				{
					hash,
					new mem_fn_delegate(info.owner, info.memfn)
				});
		}

		void operator +=(const functor_info<void(Args...)>& info)
		{
			std::lock_guard<std::mutex> lk(*_mutex);

			_delegates->map.insert(
				{
					std::hash<std::string>()(info.key),
					new functor_delegate(info.functor)
				});
		}

		void operator -=(void(*cb)(Args...))
		{
			std::lock_guard<std::mutex> lk(*_mutex);

			_delegates->map.erase(std::hash<void(*)(Args...)>()(cb));
		}

		template <class Owner>
		void operator -=(const mem_fn<void(Owner::*)(Args...)>& info)
		{
			std::lock_guard<std::mutex> lk(*_mutex);

			auto hash = memfn_hash(info.memfn, info.owner);
			_delegates->map.erase(hash);
		}

		template <class Owner>
		void operator -=(const mem_fn<void(Owner::*)(Args...) const>& info)
		{
			std::lock_guard<std::mutex> lk(*_mutex);

			auto hash = memfn_hash(info.memfn, info.owner);
			_delegates->map.erase(hash);
		}

		void operator -=(const std::string& key)
		{
			std::lock_guard<std::mutex> lk(*_mutex);

			_delegates->map.erase(std::hash<std::string>()(key));
		}

		void operator()(Args... args)
		{
			std::lock_guard<std::mutex> lk(*_mutex);

			for (auto it = _delegates->map.begin();
				 it != _delegates->map.end();
				 ++it)
			{
				it->second->invoke(args...);
			}
		}
	};


	/// <summary>
	/// Event is an interface wrapping multicast delegate. 
	/// Provides subscribe, unsubscribe operators. 
	/// </summary>
	/// <typeparam name="Signature"></typeparam>
	/// <remarks>
	/// Designed to be a public field. Not a functor. 
	/// </remarks>
	template <class Signature> 
	class event;

	/// <summary>
	/// Event is an interface wrapping multicast delegate. 
	/// Provides subscribe, unsubscribe operators. 
	/// </summary>
	/// <typeparam name="R">Return value type. </typeparam>
	/// <typeparam name="...Args">Argument types. </typeparam>
	/// <remarks>
	/// Designed to be a public field. Not a functor. 
	/// </remarks>
	template <class R, class ... Args>
	class event<R(Args...)>
	{
		multicast_delegate<R(Args...)>& _del;

	public:
		event(multicast_delegate<R(Args ...)>& del) :
			_del(del)
		{
		}

		event(const event& other) : _del(other._del)
		{
			static_assert(
				false,
				"You cannot copy instance of `zx::event`. "
				"Consider to override copy constructor of `zx::event` owning type properly. ");
		}

		void operator+=(R(*value)(Args...)) const 
		{
			(_del).operator+=(value);
		}

		template <class Owner>
		void operator+=(const mem_fn<R(Owner::*)(Args...)>& value) const
		{
			(_del).operator+=(value);
		}

		void operator +=(const functor_info<void(Args...)>& value) const 
		{
			(_del).operator+=(value);
		}
		
		void operator-=(R(*value)(Args...)) const 
		{
			(_del).operator-=(value);
		}

		template <class Owner>
		void operator-=(const mem_fn<void(Owner::*)(Args...)>& value) const
		{
			(_del).operator-=(value);
		}

		void operator -=(const std::string& value) const
		{
			(_del).operator-=(value);
		}
	};
}
