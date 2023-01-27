#pragma once


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

class RefCount
{
private:
	int m_shared;
	int m_weak;

public:
	RefCount()
	{
		m_shared = 0;
		m_weak = 0;
	}
	
	~RefCount()
	{
		
	}

	void IncrementShared()
	{
		++m_shared;
	}

	bool DecrementShared()
	{
		--m_shared;

		if (m_shared <= 0)
		{
			if (m_weak <= 0)
			{
				return false;
			}

			m_shared = 0;

			return true;
		}
	}
};

template< class T>
class SharedPtr
{
private:
	T* m_ptr;
	int* m_useCount;

public:

	explicit SharedPtr(T* ptr)
	{
		m_ptr = ptr;
		m_useCount = new int(0);

		if (ptr != nullptr)
		{

		}

		++m_useCount;
	}

	SharedPtr(std::nullptr_t nPtr) noexcept
	{
		m_ptr = nullptr;
		m_useCount = 0;
	}

	SharedPtr() noexcept
	{
		m_ptr = nullptr;
		m_useCount = 0;
	}

	~SharedPtr()
	{
		delete m_ptr;
	}

	//SharedPtr(WeakPtr& sharedPtr)
	//{

	//}

	SharedPtr(SharedPtr& sharedPtr)
	{
		m_ptr = sharedPtr.get();
		m_useCount = sharedPtr.use_count();
	}

	SharedPtr(SharedPtr&& sharedPtr)
	{

	}

	T* get()
	{
		return m_ptr;
	}

	size_t use_count()
	{
		return m_useCount;
	}

	bool Invariant()
	{
		return true;
	}
};