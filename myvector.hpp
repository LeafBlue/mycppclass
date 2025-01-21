#pragma once
#include<iostream>
#include<iterator>

template<typename T>
class myvector {
private:
	//数组 长度 容量
	
	size_t capability;
	size_t size;
	T* t_arr;

public:
	//构造函数

	//无参，给初始大小10
	myvector()
		:t_arr(new T[10])
		, capability(10)
		, size(0)
	{}

	myvector(size_t _capability) 
		:t_arr(new T[_capability])
		,capability(_capability)
		,size(0)
	{}

	//将所有元素初始化给给定值
	myvector(size_t _size,T _t) 
		:t_arr(new T[_size * 2])
		,capability(_size * 2)
		, size(_size)
	{
		for (size_t i = 0; i < _size; i++)
		{
			t_arr[i] = _t;
		}
	}

	//拷贝构造
	myvector(myvector & other) 
		:capability(other.capability)
		,size(other.size)
		,t_arr(new T[capability])
		{
			for (size_t i = 0; i < size; i++)
			{
				t_arr[i] = other.t_arr[i];
			}
		}

	//移动构造
	myvector(myvector&& other) noexcept
		:t_arr(other.t_arr)
		,capability(other.capability)
		,size(other.size) 
	{
		other.t_arr = nullptr;
	}

	//析构
	~myvector() {
		delete[] t_arr;
		/*
		* 对象即将被销毁，此操作可以不做
		capability = 0;
		size = 0;
		*/
	}

	class iterator {
	private:
		//这个成员用来存储迭代器指向的当前元素
		T* ptr;
		//这个构造函数将获取到的T指针交给成员ptr保存
		//这个构造函数不能暴露给外面使用，如果传入的是一个T类型但并非myvector元素，会导致未定义行为
		iterator(T *t) {
			ptr = t;
		}
	public:
		iterator() {}
		//operator*：返回迭代器所指向的元素的引用。
		T& operator*() const {
			return *ptr;
		}
		//operator->：返回指向成员的指针（对于指针类型的迭代器来说，这通常直接返回 ptr->）。
		T* operator->()const {
			return ptr;
		}
		//重载递增和递减操作符(operator++ 和 operator--)
		//operator++()：前置递增，返回递增后的迭代器。
		iterator operator++() noexcept {
			++ptr;
			return *this;
		}
		//operator++(int)：后置递增，
		//返回递增前的迭代器副本，再对原对象进行修改
		iterator operator++(int)noexcept {
			iterator temp = iterator(ptr);
			++ptr;
			return temp;
		}
		//operator--() 和 operator--(int)：类似地，实现前置和后置递减操作符。
		iterator& operator--() noexcept {
			--ptr;
			return *this;
		}
		iterator operator--(int) noexcept {
			iterator temp = iterator(ptr);
			--ptr;
			return temp;
		}
		//重载相等性和不等性比较操作符(operator== 和 operator!=)
		//这些操作符用于判断两个迭代器是否指向同一位置，这对于循环终止条件非常重要。
		bool operator==(const iterator &_it)const noexcept {
			return this->ptr == _it.ptr;
		}
		bool operator!=(const iterator& _it)const noexcept {
			return this->ptr != _it.ptr;
		}
		//operator+ 和 operator-：支持跳跃式移动。
		iterator operator+(int n) {
			return iterator(ptr+n);
		}
		iterator operator-(int n) {
			return iterator(ptr - n);
		}
		//std::ptrdiff_t 这是一个标准库用于表示指针差值的符号
		std::ptrdiff_t operator-(const iterator &_it)const noexcept  {
			return ptr - _it.ptr;
		}
		//operator+= 和 operator-=：原地增加或减少指定数量的步长。
		iterator& operator+=(std::ptrdiff_t index)noexcept {
			ptr = ptr + index;
			return *this;
		}
		iterator& operator-=(std::ptrdiff_t index)noexcept {
			ptr = ptr - index;
			return *this;
		}
		//关系操作符（如 <、 <= 、>、 >= ）：支持迭代器之间的比较。
		bool operator<(const iterator& _it)const noexcept { return ptr < _it.ptr; }
		bool operator<=(const iterator& _it)const noexcept { return ptr <= _it.ptr; }
		bool operator>(const iterator& _it)const noexcept { return ptr > _it.ptr; }
		bool operator>=(const iterator& _it)const noexcept { return ptr >= _it.ptr; }
		//myvector的begin和end要调用这里的构造函数，给他一个友元
		friend class myvector<T>;
	};

	//赋值操作符重载
	//使用new容易抛出bad_alloc ，在这里首次处理此问题
	myvector& operator=(myvector &other) {
		if (this != &other) {
			T* oldarr = t_arr;
			T* temp;
			try {
				temp = new T[other.capability];
				for (size_t i = 0; i < other.size; i++)
				{
					temp[i] = other.t_arr[i];
				}

				capability = other.capability;
				size = other.size;
				t_arr = temp;
				delete[] oldarr;
			}
			catch (...) {
				//先处理再抛出
				delete[] temp;
				t_arr = oldarr;
				throw;
			}
		}
	}

	//下标操作符重载，可以修改元素
	//不需要检查是否越界，比at更快
	T& operator[](size_t index) {
		return t_arr[index];
	}

	//检查 vector 是否为空
	bool empty() {
		return size == 0;
	}
	//返回当前 vector 中元素的数量。
	size_t size_() {
		return size;
	}

	//返回当前分配的容量
	size_t capacity() {
		return capability;
	}

	//：预分配至少 new_cap 个元素的空间。这里假设此函数不会缩小容量
	void reserve(size_t new_cap) {
		if (new_cap > capability) {
			T* temp;
			try {
				temp = new T[new_cap];
				for (size_t i = 0; i < size; i++)
				{
					temp[i] = t_arr[i];
				}
				//delete只会针对new的对象调用析构函数，如果有的元素用了placement new，这句代码就无法照顾到了
				//如果代码中使用了placement new来填充数组，则需要重新考虑
				delete[] t_arr;
				t_arr = temp;
				capability = new_cap;
			}
			catch (...) {
				delete[] temp;
				throw;
			}
		}
	}
	
	//shrink_to_fit()：尽可能减少已分配但未使用的空间（C++11 及以上）。
	bool shrink_to_fit() {
		if (capability > size) {
			T* oldarr = t_arr;
			try {
				t_arr = new T[size];
				for (size_t i = 0; i < size; i++)
				{
					t_arr[i] = oldarr[i];
				}
				delete[] oldarr;
				capability = size;
				return true;
			}
			catch (...) {
				t_arr = oldarr;
				throw;
			}
		}
		return false;
	}
	
	//安全地访问指定位置的元素（带边界检查，抛出异常）
	//正常来说，重载[]是不需要检查下标越界的，这是两个的区别
	T& at(size_t index) {
		if (index > size - 1) {
			throw std::out_of_range("index out of range");
		}
		return t_arr[index];
	}
	//访问第一个元素。
	T& front() {
		if (size>0) {
			return t_arr[0];
		}
		throw std::out_of_range("front out of range");
	}
	//访问最后一个元素。
	T& back() {
		if (size > 0) {
			return t_arr[size - 1];
		}
		throw std::out_of_range("back out of range");
	}
	
	
	
	//	push_back(const value_type& value)：在末尾添加一个元素。
	void push_back(const T& value) {
		if (size < capability) {
			//容量未达到上限
			t_arr[size] = value;
			size++;
		}
		else {
			//容量达到上限，不能添加了
			T* temparr = nullptr;
			try {
				temparr = new T[static_cast<size_t>(capability * 1.5)];
				for (size_t i = 0; i < size; i++)
				{
					temparr[i] = t_arr[i];
				}
				temparr[size] = value;
				delete[] t_arr;
				t_arr = temparr;
				size++;
				capability = static_cast<size_t>(capability * 1.5);
			}
			catch (...) {
				delete[] temparr;
				throw;
			}
		}
	}
	//	push_back(value_type&& value)：在末尾添加一个元素（右值引用版本，C++11 及以上）。
	// 可以通过引用直接移动资源，而不进行拷贝
	//通常使用右值引用，代表接收的是一个临时资源，或者这个资源可以被销毁
	//但这并不意味着右值引用只能绑定临时资源，它表达的是函数的一个意图
	void push_back(const T&& value) {
		if (size < capability) {
			//容量未达到上限
			//move函数，可以高效地将资源“移动所有权”，使当前变量置空，使这个资源不再属于这个变量，属于另一个对象
			//同时，这个变量会变成“未定义 悬空”状态
			t_arr[size] = move(value);
			size++;
		}
		else {
			//容量达到上限，不能添加了
			T* temparr = nullptr;
			try {
				temparr = new T[static_cast<size_t>(capability * 1.5)];
				for (size_t i = 0; i < size; i++)
				{
					temparr[i] = t_arr[i];
				}
				temparr[size] = move(value);
				delete[] t_arr;
				t_arr = temparr;
				size++;
				capability = static_cast<size_t>(capability * 1.5);
			}
			catch (...) {
				delete[] temparr;
				throw;
			}
		}
	}
	//	pop_back()：移除最后一个元素。
	void pop_back() {
		if (size == 0) {
			return;
		}
		size--;
		//这里不能这样写，对于非指针类型 =nullptr这样写会出问题
		//t_arr[size] = nullptr;
		//但也不能把它放着不管，然后后面当没事人一样直接覆盖它。这可能会导致对象无法正确调用析构函数，从而导致无法预料的麻烦问题。
		// 此处需要显式调用析构函数才行

		//is_trivially_destructible_v 用来判断T是不是普通可破坏类型
		if constexpr (!std::is_trivially_destructible_v <T>) {
			t_arr[size].~T();
		}
	}


	//	insert(iterator pos, const value_type & value)：在指定位置插入一个元素。
	//这里往后移动一位实际上还是在复制，因此我这里使用了move函数，当T实现了移动赋值运算符时会调用移动
	//但使用移动，会导致被移动的位置变成有效但未定义状态，一定要确保这个位置被新的数据填充，因此一定要仔细写好这个循环
	void insert(iterator pos, const T& value) {
		if (pos < begin() || pos >= end()) {
			throw std::out_of_range("Iterator out of range");
		}
		if (size == capability) {
			reserve(static_cast<size_t>(capability * 1.5));
		}
		size_t index = pos - t_arr;
		for (size_t i = size; i > index; i --) {
			t_arr[i] = std::move(t_arr[i - 1]);
		}
		t_arr[index] = value;
		size++;
	}

	//	insert(iterator pos, size_type count, const value_type & value)：在指定位置插入多个相同元素。
	//这段代码中使用了size+count的下标，所以一定要确保新的容量可以满足这个下标
	void insert(iterator pos, size_t count, const T& value) {
		if (pos < begin() || pos >= end()) {
			throw std::out_of_range("Iterator out of range");
		}
		if (size + count > capability) {
			reserve(static_cast<size_t>(capability * 1.5 + count));
		}
		size_t index = pos - t_arr;
		for (size_t i = size; i > index; i--) {
			t_arr[i + count] = std::move(t_arr[i - 1]);
		}
		for (size_t i = 0; i < count; i++) {
			t_arr[index + i] = value;
		}
		size+=count;
	}
	//	erase(iterator pos)：移除指定位置的元素。
	void erase(iterator pos) {
		if (pos < begin() || pos >= end()) {
			throw std::out_of_range("Iterator out of range");
		}
		size_t index = pos - t_arr;
		if constexpr (!std::is_trivially_destructible_v <T>) {
			t_arr[index].~T();
		}
		for (size_t i = index; i <  size - 1; i++) {
			t_arr[i] = std::move(t_arr[i + 1]);
		}
		//此数据已经变的无效，因此不需要这样
		//t_arr[size - 1] = new T();
		size--;
	}
	//	erase(iterator first, iterator last)：移除指定范围内的元素。
	//这里不包括last指向的值，在这里计算下标容易钻牛角尖，要确保自己设置的每一个变量都明确它的含义
	//当first > last时，应该抛出异常，而不是自作主张交换两个迭代器，因为用户很可能要用到这两个迭代器，会出问题
	void erase(iterator first, iterator last) {
		if (first == last) {
			return;
		}
		if (first > last) {
			throw std::out_of_range("Iterator out of range");
		}
		if (first < begin() || last >  end()) {
			throw std::out_of_range("Iterator out of range");
		}

		size_t first_index = first - t_arr;
		size_t last_index = last - t_arr;
		size_t del_count = last_index - first_index;

		for (size_t i = first_index; i < last_index; i ++) {
			if constexpr (!std::is_trivially_destructible_v <T>) {
				t_arr[i].~T();
			}
		}
		for (size_t i = last_index; i < size; i++)
		{
			t_arr[i - del_count] = std::move(t_arr[i]);
		}
		size -= del_count;
	}
	//	clear()：移除所有元素。
	void clear() {
		//clear不应该重新分配新的数组，并且要处理每个元素的析构函数
		for (size_t i = 0; i < size; i++)
		{
			if constexpr (!std::is_trivially_destructible_v <T>) {
				t_arr[i].~T();
			}
		}
		size = 0;
	}
	//	swap(vector & other)：交换两个 vector 的内容。
	void swap(myvector& other)  noexcept{
		T* temp = t_arr;
		size_t temp_capability = capability;
		size_t temp_size = size;

		t_arr = other.t_arr;
		capability = other.capability;
		size = other.size;

		other.t_arr = temp;
		other.capability = temp_capability;
		other.size = temp_size;
	}

	//begin() 和 end()：返回指向第一个和最后一个元素之后的迭代器（非常量版本）。
	iterator begin() noexcept {
		return myvector<T>::iterator(t_arr);
	}

	iterator end() noexcept {
		return t_arr + size;
	}

};