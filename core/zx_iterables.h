# pragma once

namespace zx
{
	/// <summary>
	/// Iterables (read-only enumerables). 
	/// Provides read-only interface for containers to use them in range based loops. 
	/// </summary>
	namespace iterable
	{
		/// Immutable version of iterables. 
		namespace imm
		{
			/// Read-only enumerable for any iterator range. 
			template <class Container>
			class generic final
			{
				Container::template const_iterator _begin;
				Container::template const_iterator _end;

			public:
				generic(Container& container) :
					_begin(container.begin()),
					_end(container.end())
				{
				}

				Container::template const_iterator begin() const
				{
					return _begin;
				}

				Container::template const_iterator end() const
				{
					return _end;
				}
			};

			/// Iterable interface for std::vector
			/// to immutable (const) element. 
			template <class T>
			using vector = generic<const std::vector<T>>;

			/// Iterable interface for std::deque
			/// to immutable (const) element. 
			template <class T>
			using deque = generic<const std::deque<T>>;

			/// Iterable interface for std::list
			/// to immutable (const) element. 
			template <class T>
			using list = generic<const std::list<T>>;

			/// Iterable interface for std::set
			/// to immutable (const) element. 
			template <class T>
			using set = generic<const std::set<T>>;

			/// Iterable interface for std::unordered_map
			/// to immutable (const) element. 
			template <class K, class V>
			using unordered_map = generic<const std::unordered_map<K, V>>;

			/// Iterable interface for std::map
			/// to immutable (const) element. 
			template <class K, class V>
			using map = generic<const std::map<K, V>>;
		}

		/// Mutable version of iterables. 
		namespace mut
		{
			/// Read-only enumerable for any iterator range. 
			template <class Container>
			class generic final
			{
				Container::template iterator _begin;
				Container::template iterator _end;

			public:
				generic(const Container& container)
				{
					static_assert (
						false,
						"Namespace 'mut' allows only non-const instances of container to view. "
						"Const instances will provide only const_iterators. ");
				}

				generic(Container& container) :
					_begin(container.begin()),
					_end(container.end())
				{
				}

				Container::template iterator begin() const
				{
					return _begin;
				}

				Container::template iterator end() const
				{
					return _end;
				}
			};

			/// Iterable interface for std::vector
			/// to immutable (const) element. 
			template <class T>
			using vector = generic<std::vector<T>>;

			/// Iterable interface for std::deque
			/// to immutable (const) element. 
			template <class T>
			using deque = generic<std::deque<T>>;

			/// Iterable interface for std::list
			/// to immutable (const) element. 
			template <class T>
			using list = generic<std::list<T>>;

			/// Iterable interface for std::set
			/// to immutable (const) element. 
			template <class T>
			using set = generic<std::set<T>>;

			/// Iterable interface for std::unordered_map
			/// to immutable (const) element. 
			template <class K, class V>
			using unordered_map = generic<std::unordered_map<K, V>>;

			/// Iterable interface for std::map
			/// to immutable (const) element. 
			template <class K, class V>
			using map = generic<std::map<K, V>>;
		}
	}

	/// <summary>
	/// Iterator ranges. 
	/// Provides interface for part of whole container iterator range. 
	/// </summary>
	namespace ranges
	{
		/// Read-only enumerable for any iterator range. 
		template <class Iterator>
		class generic final
		{
			Iterator _begin;
			Iterator _end;

		public:
			generic(Iterator begin,
					Iterator end) :
				_begin(begin),
				_end(end)
			{
			}

			Iterator begin() const
			{
				return _begin;
			}

			Iterator end() const
			{
				return _end;
			}
		};

		/// Immutable version of iterator ranges. 
		namespace imm
		{
			/// Iterator range for part of whole iterator range
			/// of std::vector to immutable (const) element. 
			template <class T>
			using vector = generic<std::vector<T>::template const_iterator>;

			/// Iterator range for part of whole iterator range
			/// of std::deque to immutable (const) element. 
			template <class T>
			using deque = generic<std::deque<T>::template const_iterator>;

			/// Iterator range for part of whole iterator range
			/// of std::list to immutable (const) element. 
			template <class T>
			using list = generic<std::list<T>::template const_iterator>;

			/// Iterator range for part of whole iterator range
			/// of std::set to immutable (const) element. 
			template <class T>
			using set = generic<std::set<T>::template const_iterator>;

			/// Iterator range for part of whole iterator range
			/// of std::unordered_map to immutable (const) element. 
			template <class K, class V>
			using unordered_map = generic<std::unordered_map<K, V>::template const_iterator>;

			/// Iterator range for part of whole iterator range
			/// of std::map to immutable (const) element. 
			template <class K, class V>
			using map = generic<std::map<K, V>::template const_iterator>;
		}

		/// Mutable version of iterator ranges. 
		namespace mut
		{
			/// Iterator range for part of whole iterator range
			/// of std::vector to mutable (non-const) element. 
			template <class T>
			using vector = generic<std::vector<T>::template iterator>;

			/// Iterator range for part of whole iterator range
			/// of std::deque to mutable (non-const) element. 
			template <class T>
			using deque = generic<std::deque<T>::template iterator>;

			/// Iterator range for part of whole iterator range
			/// of std::list to mutable (non-const) element. 
			template <class T>
			using list = generic<std::list<T>::template iterator>;

			/// Iterator range for part of whole iterator range
			/// of std::set to mutable (non-const) element. 
			template <class T>
			using set = generic<std::set<T>::template iterator>;

			/// Iterator range for part of whole iterator range
			/// of std::unordered_map to mutable (non-const) element. 
			template <class K, class V>
			using unordered_map = generic<std::unordered_map<K, V>::template iterator>;

			/// Iterator range for part of whole iterator range
			/// of std::map to mutable (non-const) element. 
			template <class K, class V>
			using map = generic<std::map<K, V>::template iterator>;
		}
	}
}
