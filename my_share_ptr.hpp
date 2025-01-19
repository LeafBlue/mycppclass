#pragma once
#include<iostream>

//共享指针
template<typename T>
class my_share_ptr {
private:
	T* obj;
	//int ptr_num;
	//通过创建一个原子对象指针，指向一块堆中的特定区域。每次复制时，只复制指针，不复制指针指向的内容
	//atomic的特点：操作原子，线程安全
	std::atomic<std::size_t>* ref_count = nullptr;

public:
	my_share_ptr() :obj(nullptr), ref_count(nullptr){}

	//explicit关键字：可以禁止隐式转换
	explicit my_share_ptr(T* other = nullptr) 
		:obj(other)
	{	
		if (obj) {
			ref_count = new std::atomic<std::size_t>(1);
		}
	}

	//复制构造
	my_share_ptr(const my_share_ptr& other) 
		:obj(other.obj)
		,ref_count(other.ref_count) 
	{
		++(*ref_count);
	}
	
	//移动构造
	my_share_ptr(my_share_ptr&& other) noexcept
		:obj(other.obj)
		, ref_count(other.ref_count) 
	{
		other.obj = nullptr;
		other.ref_count = nullptr;
	}

	//析构函数
	//析构函数要对ref_count-1，需要考虑ref_count是否存在
	//当ref_count-1是0后，可以安全删除资源
	~my_share_ptr() {
		if (ref_count && --(*ref_count) == 0) {
			delete obj;
			delete ref_count;
		}
	}

	//复制操作符
	my_share_ptr& operator=(const my_share_ptr& other) {
		obj = other.obj;
		ref_count = other.ref_count;
		++(*ref_count);
		return *this;
	}
	//移动操作符
	my_share_ptr& operator=(my_share_ptr&& other)noexcept {
		obj = other.obj;
		ref_count = other.ref_count;
		other.obj = nullptr;
		other.ref_count = nullptr;
		return *this;
	}

	//reset()：
	//重置 my_shared_ptr，使其不再管理当前对象（如果有），可以选择传递一个新的对象来管理。
	void reset(T *other = nullptr){
		release();
		if (other != nullptr) {
			obj = other;
			ref_count = new std::atomic<std::size_t>(1);
		}
	}
	//release()：
	//释放对对象的所有权，返回原始指针（注意：这与 std::shared_ptr 的行为不同，通常不推荐在 std::shared_ptr 中使用此方法，因为这会导致悬空指针问题）。
	void release() {
		if (ref_count && --(*ref_count) == 0) {
			delete obj;
			delete ref_count;
		}
		obj = nullptr;
		ref_count = nullptr;
	}
	//get()：
	//返回内部存储的裸指针，但不会转移所有权。
	//const noexcept  顺序不能错，否则会导致编译器识别后面的代码出错
	T* get()const noexcept  {
		return obj;
	}
	//use_count()：
	//返回当前对象的引用计数。
	//理论上这里返回size_t 也是副本，不会修改ref_count，但出于安全考虑，直接读取ref_count行为不推荐
	size_t use_count() const noexcept {
		if (ref_count) {
			return ref_count->load();
		}
		//return *ref_count;
		return 0;
	}
	//swap(my_shared_ptr& other)：交换两个 my_shared_ptr 实例的内容。
	void swap(my_share_ptr& other)noexcept  {
		if (obj != other.obj) {
			T* temp = obj;
			std::atomic<std::size_t>* temp_data = ref_count;

			obj = other.obj;
			ref_count = other.ref_count;

			other.obj = temp;
			other.ref_count = temp_data;
		}
	}
	//operator bool()：
	//隐式转换为布尔值，用于检查 my_shared_ptr 是否为空。
	explicit operator bool()const noexcept {
		return obj != nullptr;
	}
	//解引用操作符(*和 ->)：
	T& operator*()noexcept {
		if (obj == nullptr) {
			throw;
		}
		return *obj;
	}
	T* operator->()noexcept {
		if (obj == nullptr) {
			throw;
		}
		return obj;
	}
	
	//operator== 和 operator!=：
	bool operator==(const my_share_ptr& other)const noexcept {
		return obj == other.obj;
	}

	bool operator!=(const my_share_ptr& other)const noexcept {
		return obj != other.obj;
	}
	
};