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

	//friend class RefCount;

	//Void Constructor
	WeakPtr() noexcept :
		m_ptr(nullptr), m_counter(nullptr) {
		CHECK;
	}

	WeakPtr(const WeakPtr& weakPtr) :
		m_ptr(weakPtr.m_ptr), 
		m_counter(weakPtr.m_counter)
	{
		if (m_counter != nullptr)
		{
			++m_counter->m_weak;
		}
		
		CHECK;
	}

	WeakPtr(const SharedPtr<T>& sharedPtr) noexcept :
		m_ptr(sharedPtr.cget()), 
		m_counter(sharedPtr.counter())
	{
		if (m_counter)
		{
			++m_counter->m_weak;
		}

		CHECK;
	}

	~WeakPtr()
	{
		if (expired())
		{
			if (m_counter)
			{
				delete m_counter;
			}
		}
		else
		{
			--m_counter->m_weak;
		}
	}

	WeakPtr& operator=(const WeakPtr& weakPtr)
	{
		if (weakPtr.m_ptr == m_ptr)
			return *this;

		if (m_ptr)
		{
			--m_counter->m_weak;

			if (m_counter->m_weak == 0) {
				delete m_ptr;
				delete m_counter;
			}
		}
		m_counter = weakPtr.m_counter;
		m_ptr = weakPtr.m_ptr;

		if (m_ptr != nullptr)
			m_counter->m_weak;

		return *this;
	}


	WeakPtr<T>& operator=(const SharedPtr<T>& sharedPtr)
	{
		m_ptr = sharedPtr.cget();
		m_counter = sharedPtr.counter();

		if (m_ptr != nullptr)
		{
			++m_counter->m_weak;
		}

		return *this;
	}

	void reset()
	{
		--m_counter->m_weak;

		/*if (m_counter->m_shared > 0)
		{
			delete m_counter;
			delete m_ptr;
		}*/

		m_counter = nullptr;
		m_ptr = nullptr;
	}

	T* get()
	{
		return m_ptr;
	}

	T* get() const
	{
		return m_ptr;
	}

	RefCount* counter()
	{
		return m_counter;
	}

	RefCount* counter() const
	{
		return m_counter;
	}

	SharedPtr<T> lock()
	{
		return expired() ? SharedPtr<T>() : SharedPtr<T>(*this);
	}

	bool expired()
	{
		if (m_counter)
		{
			if (m_counter->m_shared > 0)
			{
				return false;
			}
		}

		return true;
	}

	bool Invariant()
	{
		if (m_ptr == nullptr)
		{
			return true;
		}

		return m_counter->m_weak;
	}

	friend void swap(WeakPtr<T>& lhs, WeakPtr<T>& rhs)
	{
		WeakPtr<T> temp = lhs;
		lhs = rhs;
		rhs = temp;
	}
};
