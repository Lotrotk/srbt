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
}
