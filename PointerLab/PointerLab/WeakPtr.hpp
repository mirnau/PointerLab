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
	template <class T>
	friend class SharedPtr;

	T* m_ptr;
	RefCount* m_counter;

public:

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
		m_ptr(sharedPtr.m_ptr),
		m_counter(sharedPtr.m_counter)
	{
		if (m_counter)
			++m_counter->m_weak;

		CHECK;
	}

	~WeakPtr()
	{
		CHECK;

		if (!expired())
			--m_counter->m_weak;
	}

	WeakPtr& operator=(const WeakPtr& weakPtr)
	{
		if (weakPtr.m_ptr == m_ptr)
			return *this;

		if (!expired())
		{
			if (m_counter)
				--m_counter->m_weak;
		}

		m_counter = weakPtr.m_counter;
		m_ptr = weakPtr.m_ptr;

		if (m_counter)
			++m_counter->m_weak;

		CHECK;

		return *this;
	}


	WeakPtr<T>& operator=(const SharedPtr<T>& sharedPtr)
	{

		if (!expired())
		{
			if(m_counter)
				--m_counter->m_shared;
		}

		m_ptr = sharedPtr.m_ptr;
		m_counter = sharedPtr.m_counter;
		
		if (m_counter)
			++m_counter->m_weak;

		CHECK;

		return *this;
	}

	void reset()
	{
		if (!expired());
		{
			--m_counter->m_weak;
			m_counter = nullptr;
			m_ptr = nullptr;
		}

		CHECK;
	}

	SharedPtr<T> lock()
	{
		return expired() ? SharedPtr<T>() : SharedPtr<T>(*this);
	}

	bool expired() const
	{
		if (m_counter == nullptr)
			return true;

		if (m_counter->m_shared == 0) {

			--m_counter->m_weak;

			if (m_counter->m_weak == 0)
				delete m_counter;

			const_cast<WeakPtr*>(this)->m_counter = nullptr;
			return true;
		}

		return false;
	}

	bool Invariant() const noexcept
	{
		if (m_ptr == nullptr)
				return true;
	
		return m_counter->m_weak;
	}

	friend void swap(WeakPtr<T>& lhs, WeakPtr<T>& rhs) noexcept
	{
		WeakPtr<T> temp = lhs;
		lhs = rhs;
		rhs = temp;
	}
};
