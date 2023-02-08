#pragma once
#include "RefCount.hpp"
#include "SharedPtr.hpp"
#include <cassert>
#include <algorithm>
#define CHECK assert(Invariant())

template<class T> class SharedPtr;
template<class T>

class WeakPtr
{
private:
	T* m_ptr;
	RefCount* m_counter;
public:

	//Void Constructor
	WeakPtr() noexcept :
		m_ptr(nullptr), m_counter(nullptr) {
		CHECK;
	}

	WeakPtr(WeakPtr& weakPtr) :
		m_ptr(weakPtr.m_ptr), m_counter(weakPtr.m_counter)
	{
		if (m_counter == nullptr)
		{
			m_counter = new RefCount();
		}
		
		m_counter->IncrementWeak();
		
		CHECK;
	}

	WeakPtr(SharedPtr<T>& sharedPtr)
	{

		*this = sharedPtr;

		if (m_counter)
		{
			m_counter->IncrementWeak();
		}

		CHECK;
	}

	~WeakPtr()
	{
		if (expired())
		{
			delete m_counter;
		}
		else
		{
			m_counter->DecrementWeak();
		}
	}


	//WeakPtr& operator=(WeakPtr& weakPtr)
	//{
	//	m_ptr = weakPtr.m_ptr;
	//	m_counter = weakPtr.m_counter;
	//	m_counter->IncrementWeak();
	//}


	WeakPtr& operator=(SharedPtr<T>& sharedPtr)
	{
		m_ptr = sharedPtr.get();
		m_counter = sharedPtr.counter();
		m_counter->IncrementWeak();

		return *this;
	}

	//void reset()
	//{

	//}

	T* get()
	{
		return m_ptr;
	}

	RefCount* counter()
	{
		return m_counter;
	}

	SharedPtr<T> lock()
	{
		return expired() ? SharedPtr<T>() : SharedPtr<T>(*this);
	}

	bool expired()
	{
		if (m_counter == nullptr)
		{
			return true;
		}

		if (m_counter->WeakPtrSize() == 0)
		{
			return true;
		}

		return false;
	}

	bool Invariant()
	{
		if (m_ptr == nullptr)
		{
			return true;
		}

		return m_counter->WeakPtrSize();
	}

	//void swap(WeakPtr lhs, WeakPtr rhs)
	//{

	//}
};
