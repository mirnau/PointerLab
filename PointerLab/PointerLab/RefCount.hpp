#pragma once

class RefCount
{
	template <class T>
	friend class SharedPtr;

	template <class T>
	friend class WeakPtr;

	RefCount() : 
		m_shared(0),
		m_weak(0) {}

	size_t m_shared;
	size_t m_weak;
};

