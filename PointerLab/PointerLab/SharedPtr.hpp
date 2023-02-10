#pragma once
#include <utility>
#include <cassert>
#include <compare>
#include <memory>
#include "RefCount.hpp"
#include "WeakPtr.hpp"
#define CHECK assert(Invariant())

template< class T>

class SharedPtr
{
	template <class T>
	friend class WeakPtr;

	T* m_ptr;
	RefCount* m_counter;

public:

	explicit SharedPtr(T* ptr) :
		m_ptr(ptr)
	{
		m_counter = new RefCount();
		++m_counter->m_shared;
	}

	SharedPtr(std::nullptr_t nPtr) noexcept :
		m_ptr(nullptr), m_counter(nullptr) {CHECK;}

	SharedPtr() noexcept :
		m_ptr(nullptr), m_counter(nullptr) {CHECK;}

	~SharedPtr()
	{
		CHECK;
		if (m_ptr != nullptr) {

			if (m_counter->m_shared != 0)
				--m_counter->m_shared;

			if (m_counter->m_shared == 0) 
			{
				delete m_ptr;
				delete m_counter;
			}
		}
	}

	SharedPtr(const SharedPtr& sharedPtr) noexcept :
		m_ptr(sharedPtr.m_ptr),
		m_counter(sharedPtr.m_counter) 
	{
		if (m_ptr != nullptr)
		{
			++m_counter->m_shared;
		}

		CHECK;
	}
	
	explicit SharedPtr(WeakPtr<T>& weakPtr) :
		m_ptr(weakPtr.m_ptr), 
		m_counter(weakPtr.m_counter)
	{
		if (weakPtr.expired())
			throw std::bad_weak_ptr();

		if(m_counter)
			++m_counter->m_shared;

		CHECK;
	}

	SharedPtr(SharedPtr&& sharedPtr) noexcept :
		m_counter(sharedPtr.m_counter),
		m_ptr(sharedPtr.m_ptr)
	{
		sharedPtr.m_ptr = nullptr;
		sharedPtr.m_counter = nullptr;
		CHECK;
	}

	constexpr T* get() const
	{
		return m_ptr;
	}

	size_t use_count() const
	{
		if (m_counter == nullptr)
			return 0;
		else
			return m_counter->m_shared;

		CHECK;
	}

	void reset()
	{
		--m_counter->m_shared;

		if (m_counter->m_shared == 0)
		{
			delete m_ptr;
			
			if (m_counter->m_weak == 0)
				delete m_counter;
		}

		m_counter = nullptr;
		m_ptr = nullptr;
	}

	bool Invariant() const noexcept
	{
		if (m_ptr == nullptr)
			return true;

		return m_counter->m_shared;
	}

	SharedPtr& operator=(const SharedPtr& sharedPtr) noexcept
	{
		if (sharedPtr.m_ptr == m_ptr)
			return *this;

		if (m_counter)
		{
			--m_counter->m_shared;

			if (m_counter->m_shared == 0)
			{
				delete m_counter;
				delete m_ptr;
			}
		}

		m_counter = sharedPtr.m_counter;
		m_ptr = sharedPtr.m_ptr;

		if (m_ptr != nullptr)
			++m_counter->m_shared;

		return *this;
	}

	SharedPtr& operator=(SharedPtr&& sharedPtr) noexcept
	{
		if (sharedPtr == *this)
			return * this;

		delete m_ptr;
		delete m_counter;

		m_counter = sharedPtr.m_counter;
		m_ptr = sharedPtr.m_ptr;

		sharedPtr.m_counter = nullptr;
		sharedPtr.m_ptr = nullptr;
		
		CHECK;
		return *this;
	}

	T& operator*() const
	{
		return *m_ptr;
	}

	T* operator->() const
	{
		return m_ptr;
	}

    operator bool() const
	{
		return (m_ptr != nullptr);
	}

	friend int operator<=>(const SharedPtr& lhs, const SharedPtr& rhs)
	{
		if (lhs.m_ptr < rhs.m_ptr)
			return -1;

		else if (lhs.m_ptr > rhs.m_ptr)
			return 1;

		return 0;
	}

	friend bool operator==(const SharedPtr& lhs, const SharedPtr& rhs) 
	{
		return lhs.m_ptr == rhs.m_ptr;
	}

	friend void swap(SharedPtr& lhs, SharedPtr& rhs)
	{
		std::swap(lhs, rhs);
	}
};