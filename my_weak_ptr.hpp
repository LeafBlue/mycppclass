#pragma once
#include<atomic>


/*
	构造函数（默认构造、从 shared_ptr 构造、拷贝构造、移动构造）。
	析构函数。
	赋值运算符（拷贝赋值、移动赋值）。
	lock() 函数。
	expired() 函数。
	use_count() 函数。
	reset() 函数
*/

template<typename T>
class my_share_ptr;

#include"my_share_ptr.hpp"

template<typename T>
class my_weak_ptr {
private:
	T* obj;
	std::atomic<std::size_t>* ref_count = nullptr;

	std::atomic<std::size_t>* weak_count = nullptr;

public:
	my_weak_ptr() {};
	my_weak_ptr(const my_share_ptr<T>& other)
		:obj(other.obj)
		, ref_count(other.ref_count)
		, weak_count(other.weak_count)//这个会被初始化为0，所以不用担心没数字
	{
		++(*weak_count);
	}

	my_weak_ptr(const my_weak_ptr& other)
		:obj(other.obj)
		,ref_count(other.ref_count)
		,weak_count(other.weak_count)
	{
		++(*weak_count);
	}

	my_weak_ptr(my_weak_ptr&& other) noexcept 
		:obj(other.obj)
		, ref_count(other.ref_count)
		, weak_count(other.weak_count)
	{
		other.obj = nullptr;
		other.ref_count = nullptr;
		other.weak_count = nullptr;
	}

	~my_weak_ptr() {
		if (weak_count && --(*weak_count) == 0) {
			if (ref_count && *ref_count == 0) {
				delete ref_count;
				delete weak_count;
			}
		}
	}


	my_weak_ptr& operator=(const my_weak_ptr& other) {
		obj = other.obj;
		ref_count = other.ref_count;
		weak_count = other.weak_count;

		++(*weak_count);
		return *this;
	}

	my_weak_ptr& operator=(my_weak_ptr&& other) noexcept {
		obj = other.obj;
		ref_count = other.ref_count;
		weak_count = other.weak_count;

		other.obj = nullptr;
		other.ref_count = nullptr;
		other.weak_count = nullptr;

		return *this;
	}

	//请注意，lock函数是基于当前对像构建一个shareptr，而不是将当前对像转换为shareptr，所以这里不需要--弱引用计数
	my_share_ptr<T> lock() {
		if (expired()) {
			return my_share_ptr<T>();
		}
		else {
			++(*ref_count);
			//--(*weak_count);
			return my_share_ptr<T>(obj,ref_count,weak_count);
		}

	}

	//检查对象是否“不存在”，如果不存在则返回true
	bool expired() {
		return !ref_count||(*ref_count)==0;
	}

	size_t use_count() {
		if (ref_count) {
			return *ref_count;
		}
		else {
			return 0;
		}
	}

	void reset() {
		if (weak_count&& --(*weak_count) == 0) {
			if (*ref_count == 0) {
				delete obj;
				delete ref_count;
				delete weak_count;
			}
			obj = nullptr;
			ref_count = nullptr;
			weak_count = nullptr;
		}
		
	}
};




