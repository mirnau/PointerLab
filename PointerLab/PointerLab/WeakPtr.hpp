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


	WeakPtr& operator=(WeakPtr& weakPtr)
	{
		if (weakPtr.m_ptr == m_ptr)
			return *this;

		if (m_ptr)
		{
			m_counter->DecrementWeak();

			if (m_counter->WeakPtrSize() == 0) {
				delete m_ptr;
				delete m_counter;
			}
		}
		m_counter = weakPtr.m_counter;
		m_ptr = weakPtr.m_ptr;

		if (m_ptr != nullptr)
			m_counter->WeakPtrSize();

		return *this;
	}


	WeakPtr& operator=(SharedPtr<T>& sharedPtr)
	{
		m_ptr = sharedPtr.get();
		m_counter = sharedPtr.counter();
		m_counter->IncrementWeak();

		return *this;
	}

	void reset()
	{
		m_counter->DecrementWeak();
		delete m_counter;
		delete m_ptr;
		m_counter = nullptr;
		m_ptr = nullptr;
	}

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

	friend void swap(WeakPtr<T>& lhs, WeakPtr<T>& rhs)
	{
		WeakPtr<T> temp = lhs;
		lhs = rhs;
		rhs = temp;
	}
};
