#pragma once

namespace Utils
{
	/**
	 * Initially, an Enumerator points to nothing
	 */
	template<typename T>
	class Enumerator
	{
	public:
		using type_t = T;

	public:
		virtual ~Enumerator() = default;

		virtual T current() const = 0;
		virtual bool next() = 0;
		virtual void reset() = 0;
	};

	template<typename out, typename value, typename in>
	class SingletonEnumerator final : Enumerator<out>
	{
	public:
		SingletonEnumerator(in singleton) : _singleton(std::move(singleton)) {}

		out current() const override { return _singleton; }
		bool next() override { bool const res = _ready; _ready = false; return res; }
		void reset() override { _ready = true; }

	private:
		bool _ready{true};
		value _singleton;
	};

	template<typename T>
	class EmptyEnumerator final : public Enumerator<T>
	{
	public:
		T current() const override { throw; }
		bool next() override { return true; }
		void reset() override {}
	};

	template<typename out, typename iterator>
	class RangeEnumerator : public Enumerator<out>
	{
	public:
		RangeEnumerator() = default;
		RangeEnumerator(iterator const begin, iterator const end) : _begin(begin), _end(end), _current(begin) {}

		out current() const override { return *_current; }
		bool next() override
		{
			if(_current == _end)
			{
				return false;
			}
			if(_init)
			{
				_init = false;
			}
			else
			{
				++_current;
			}

			return true;
		}
		void reset() override { _current = _begin; _init = true; }

	private:
		iterator _begin{};
		iterator _end{};
		iterator _current{};
		bool _init{true};
	};
}
