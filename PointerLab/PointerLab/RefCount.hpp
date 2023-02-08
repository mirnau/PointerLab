#pragma once
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

	//WEAK
	size_t WeakPtrSize()
	{
		return m_weak;
	}

	void IncrementWeak()
	{
		++m_weak;
	}

	void DecrementWeak()
	{
		--m_weak;
	}

	//SHARED
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
	}

private:
	size_t m_shared;
	size_t m_weak;
};

