//#pragma once
//#include <vector>
//#include <utility>
//
//class sandbox
//{
//
//	SharedPtr& operator=(SharedPtr&& sharedPtr)
//	{
//		if (sharedPtr.m_ptr == m_ptr)
//			return *this;
//
//		delete m_ptr;
//
//		if (sharedPtr.m_counter->SharedPtrSize() > 1)
//		{
//			sharedPtr.m_counter->DecrementShared();
//		}
//		else
//		{
//			delete m_counter;
//		}
//
//		if (&sharedPtr != nullptr)
//		{
//			m_counter = sharedPtr.m_counter;
//			m_ptr = sharedPtr.m_ptr;
//
//			if (m_ptr != nullptr)
//				m_counter->IncrementShared();
//		}
//		else
//		{
//			m_ptr = nullptr;
//			m_counter = nullptr;
//		}
//	}
//
//	return *this;
//
