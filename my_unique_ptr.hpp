#pragma once
#include<iostream>



template<typename T>
class my_unique_ptr {
private:
	//独立指针
	T* obj;

	my_unique_ptr(T* _obj) {
		obj = _obj;
	}
public:
	/*
	在这里，可以把构造函数隐藏，来确保创建的指针都是通过工厂创建的。

	new T(std::forward<Args>(args)...) 通过这个我们可以得到一个new T。
	但这还不够，我们要的是my_unique_ptr的实例，所以外层还需要调用一遍my_unique_ptr的构造函数。
	得到的这个new T是什么呢？
	它实际上是一个未被赋值给任何指针的new对象，它会赋值给my_unique_ptr的构造函数参数。因此构造函数可以用一个T*来接收。

	但是这个函数，本意是创建一个my_unique_ptr对象，我们不能指望一个已存在的my_unique_ptr去调用它，
	所以这个函数必须写成类内部的静态函数，用类名调用。当写到类内部时，这个T会与已存在的T冲突，在这里就不需要写了。
	*/
	//工厂
	template<typename... Args>
	static my_unique_ptr<T> makemy_unique_ptr(Args&&...args) {
		return my_unique_ptr<T>(new T(std::forward<Args>(args)...))
	}

	//移动构造函数
	my_unique_ptr(my_unique_ptr&& other)noexcept {
		obj = other.obj;
		other.obj = nullptr;
	}
	//移动赋值符号
	my_unique_ptr& operator=(my_unique_ptr&& other)noexcept {
		if (this == other) {
			return *this;
		}
		//处理原this
		reset();
		obj = other.obj;
		other.obj = nullptr;
		return *this;
	}

	//禁用拷贝构造函数
	my_unique_ptr(my_unique_ptr& other) = delete;
	my_unique_ptr& operator=(my_unique_ptr& other) = delete;

	//析构函数
	~my_unique_ptr() {
		delete obj;
	}

	//重置指针
	/*
	* 这里原先的参数是T* other，要求开发者自己规范传入的参数必须是一个裸指针
	* 这里对参数做了更多限制，让开发者只能传入右值指针
	*/
	void reset(T*&& other = nullptr) {
		if (other != this->obj) {
			T* temp = obj;
			obj = other;
			delete temp;
		}
		//相等，不处理
	}
	//释放所有权
	T* release() noexcept {
		T* temp = obj;
		obj = nullptr;
		return temp;
	}

	//get：返回内部保存的指针（不转移所有权）
	//所谓独占，指的是同一时刻只有一个my_unique_ptr管理，并不妨碍有普通指针也指向它
	// 实际上这个函数只要写好了移动，关闭复制就算是写好了
	T* get()const noexcept {
		return obj;
	}
	//operator*：解引用操作符
	//通常我们会根据解引用对原对象进行修改，所以这里不能返回一个T，造成T的复制
	T& operator*() const noexcept{
		return *obj;
	}
	//operator->：成员访问操作符
	//不要想得太复杂了
	T* operator->() const noexcept {
		return obj;
	}
	
	//explicit operator bool() const：检查是否持有非空指针
	//这是一个特殊重载，实现这个可以把它放进if和while条件直接使用
	explicit operator bool() const noexcept {
		return obj != nullptr;
	}
	
	//swap(my_unique_ptr& other)：交换两个 my_unique_ptr 实例之间的资源
	void swap(my_unique_ptr& other) noexcept {
		if (obj!=other.obj) {
			T* temp = obj;
			obj = other.obj;
			other.obj = temp;
		}
	}
	
};



