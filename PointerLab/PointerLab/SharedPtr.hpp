#pragma once
#include <utility>
#include <cassert>
#include <compare>

#define CHECK assert(Invariant())

template<class T>
class WeakPtr
{
public:
	WeakPtr()
	{

	}

	~WeakPtr()
	{

	}
};

struct RefCount
{
	RefCount()
	{
		m_shared = 0;
		m_weak = 0;
	}

	RefCount(bool isShared)
	{
		if (true)
		{
			m_shared = 1;
			m_weak = 0;
		}
		else
		{
			m_shared = 0;
			m_weak = 1;
		}
	}
	
	~RefCount()
	{
		
	}

	size_t SharedPtrSize()
	{
		return m_shared;
	}

	void IncrementShared()
	{
		++m_shared;
	}

	void DecrementShared()
	{
		--m_shared;

		//if (m_shared <= 0)
		//{
		//	if (m_weak <= 0)
		//	{
		//		return false;
		//	}

		//	m_shared = 0;

		//	return true;
		//}
	}

private:
	size_t m_shared;
	size_t m_weak;
};

template< class T>
//using IfNotSharedPtr = std::enable_if_t <(std::is_same<T, SharedPtr>)>;
class SharedPtr
{
private:
	T* m_ptr;
	RefCount* m_counter;

public:

	explicit SharedPtr(T* ptr) :
		m_ptr(ptr)
	{
		m_counter = new RefCount(true);
	}

	SharedPtr(std::nullptr_t nPtr) noexcept :
		m_ptr(nullptr) {CHECK;}

	SharedPtr() noexcept :
		m_ptr(nullptr) {CHECK;}

	~SharedPtr()
	{
		CHECK;
		if (m_counter != nullptr)
		{
			m_counter->DecrementShared();

			if (m_counter->SharedPtrSize() == 0)
			{
				delete m_ptr;
				delete m_counter;
			}
		}
	}

	SharedPtr(SharedPtr& sharedPtr) noexcept :
		m_ptr(sharedPtr.get()),
		m_counter(sharedPtr.m_counter) 
	{
		m_counter->IncrementShared();

		CHECK;
	}

	SharedPtr(SharedPtr&& sharedPtr) :
		m_counter(sharedPtr.m_counter),
		m_ptr(std::move(sharedPtr.m_ptr))
	{
		sharedPtr.m_ptr = nullptr;
		sharedPtr.m_counter = nullptr;

		CHECK;
	}

	T* get()
	{
		return m_ptr;
	}

	size_t use_count() const
	{
		if (m_counter == nullptr)
		{
			return 0;
		}
		else
		{
			return m_counter->SharedPtrSize();
		}

		CHECK;
	}

	void reset()
	{
		m_counter->DecrementShared();
		delete m_counter;
		delete m_ptr;
		m_counter = nullptr;
		m_ptr = nullptr;
	}

	bool Invariant() const noexcept
	{
		if (m_ptr == nullptr)
		{
			return true;
		}

		return m_counter->SharedPtrSize();
	}

	SharedPtr& operator=(const SharedPtr& sharedPtr)
	{
		if (&sharedPtr == this)
		{
			return *this;
		}

		delete m_ptr;
		delete m_counter;

		m_counter = sharedPtr.m_counter;
		m_counter->IncrementShared();
		m_ptr = sharedPtr.m_ptr;
		return *this;
	}

	SharedPtr& operator=(SharedPtr&& sharedPtr)
	{

		delete m_ptr;
		delete m_counter;

		m_counter = sharedPtr.m_counter;
		m_ptr = sharedPtr.m_ptr;

		sharedPtr.m_counter = nullptr;
		sharedPtr.m_ptr = nullptr;
		
		CHECK;
		return *this;
	}

	T& operator*()
	{
		return *m_ptr;
	}

	T* operator->()
	{
		return m_ptr;
	}

    operator bool() const
	{
		return (m_ptr != nullptr);
	}

	friend int operator<=>(SharedPtr& lhs, SharedPtr& rhs)
	{
		if (lhs.m_ptr < rhs.m_ptr)
		{
			return -1;
		}
		else if (lhs.m_ptr > rhs.m_ptr)
		{
			return 1;
		}

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

	//SharedPtr(WeakPtr& sharedPtr) //VG
	//{

	//}
};