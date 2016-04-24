/** Any Class
*   By qicosmos
*   From https://github.com/qicosmos/cosmos
*/

#ifndef GITHUB_OPENSOURCE_ANY_CLASS_HEAD
#define GITHUB_OPENSOURCE_ANY_CLASS_HEAD

#include <memory>
#include <typeindex>
#include <exception>
#include <iostream>


#include <cxxabi.h>
#include <string>

/** For GCC/G++ Complier
*   Use abi::__cxa_demangle(C-Style String Returned by typeid(T).name(),nullptr,nullptr,nullptr)
*       to get full name of type T.
*
*   For MSVC Complier
*   typeid(T).name() returns the full name. Just repack it.
*/
#ifdef __GNUC__ /// For GCC/G++ Complier
std::string getfulltypename(const char* TypeIDName)
{
    return std::string(abi::__cxa_demangle(TypeIDName,nullptr,nullptr,nullptr));
}
#else /// For MSVC Complier
std::string getfulltypename(const char* TypeIDName)
{
    return std::string(TypeIDName);
}
#endif /// end of ifdef __GNUC__

struct Any
{
	Any(void) : m_tpIndex(std::type_index(typeid(void))) {}
	Any(const Any& that) : m_ptr(that.Clone()), m_tpIndex(that.m_tpIndex) {}
	Any(Any && that) : m_ptr(std::move(that.m_ptr)), m_tpIndex(that.m_tpIndex) {}

	//创建智能指针时，对于一般的类型，通过std::decay来移除引用和cv符，从而获取原始类型
	template<typename U, class = typename std::enable_if<!std::is_same<typename std::decay<U>::type, Any>::value, U>::type> Any(U && value) : m_ptr(new Derived < typename std::decay<U>::type>(std::forward<U>(value))),
		m_tpIndex(std::type_index(typeid(typename std::decay<U>::type))){}

	bool IsNull() const { return !bool(m_ptr); }

	template<class U> bool Is() const
	{
		return m_tpIndex == std::type_index(typeid(U));
	}

	//将Any转换为实际的类型
	template<class U>
	U& AnyCast(bool showmsg=false) throw (std::logic_error)
	{
		if (!Is<U>())
		{
			if(showmsg)
			{
				std::cout << "can not cast " << getfulltypename(typeid(U).name()) << " to " << getfulltypename(m_tpIndex.name()) << std::endl;
			}
			throw std::logic_error{"bad cast"};
		}

		auto derived = dynamic_cast<Derived<U>*> (m_ptr.get());
		return derived->m_value;
	}

	Any& operator=(const Any& a)
	{
		if (m_ptr == a.m_ptr)
			return *this;

		m_ptr = a.Clone();
		m_tpIndex = a.m_tpIndex;
		return *this;
	}

private:
	struct Base;
	typedef std::unique_ptr<Base> BasePtr;

	struct Base
	{
		virtual ~Base() {}
		virtual BasePtr Clone() const = 0;
	};

	template<typename T>
	struct Derived : Base
	{
		template<typename U>
		Derived(U && value) : m_value(std::forward<U>(value)) { }

		BasePtr Clone() const
		{
			return BasePtr(new Derived<T>(m_value));
		}

		T m_value;
	};

	BasePtr Clone() const
	{
		if (m_ptr != nullptr)
			return m_ptr->Clone();

		return nullptr;
	}

	BasePtr m_ptr;
	std::type_index m_tpIndex;
};

#endif /// End of GITHUB_OPENSOURCE_ANY_CLASS_HEAD
