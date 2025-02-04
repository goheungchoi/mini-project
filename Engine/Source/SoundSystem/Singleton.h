// Singleton.h
#pragma once

#include "framework.h"

template<class C>
class Singleton {
	static std::mutex _mutex;

protected:
	static std::unique_ptr<C> _instance;
	Singleton() {}
	Singleton(const Singleton&) = delete;
	Singleton(Singleton&&) noexcept = delete;
	Singleton& operator=(const Singleton&) = delete;
	Singleton& operator=(Singleton&&) noexcept = delete;
	virtual ~Singleton() {}

public:

	template< typename... Args >
	static C& CreateInstance(Args... args);

	static void DestroyInstance();

	static C& GetInstance();
};

// Static member variable definitions
template< class C >
std::unique_ptr<C> Singleton<C>::_instance{ nullptr };
template< class C >
std::mutex Singleton<C>::_mutex;

// Static member funtion definitions
template< class C >
template< typename... Args >
C& Singleton<C>::CreateInstance(Args... args) {
	std::lock_guard<std::mutex> lock(_mutex);
	if (_instance)
		throw std::runtime_error("Double-creation of singleton detected!");

	_instance = std::make_unique<C>(std::forward<Args>(args)...);
	return *_instance;
}

template< class C >
void Singleton<C>::DestroyInstance() {
	if (!_instance)
		throw std::runtime_error("Destruction of non-exist singleton detected!");

	// Deallocate the instance
	_instance.reset(nullptr);
}

template< class C >
C& Singleton<C>::GetInstance() {
	return *_instance;
}
