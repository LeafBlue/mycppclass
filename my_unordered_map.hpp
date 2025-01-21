#pragma once
#include"my_open_hashtable.hpp"



template<class Tkey, class Tval>
class my_unordered_map :public my_open_hashtable<Tkey, Tval> {
	using base_class = my_open_hashtable<Tkey, Tval>;
	using _iter = typename base_class::iterator;
	
	/*
		my_open_hashtable有着private的成员变量，这些变量无法被派生类访问，但会随着对象创建而构造
		为了方便调用，还是把它们设为protected吧
		设为protected 后，可以在派生类内部访问，但是不能用派生类的对象试图把它点出来
		当派生类和父类存在同样的成员，直接访问得到的是派生类成员，父类的成员会被隐藏，需要双冒号::访问
	*/
	

	//构造函数
	//调用父类的构造函数，不能通过代码构造，只能通过初始化列表构造
	//也可以不构造，这样会调用父类的默认构造函数。
	//这是因为在运行到构造代码时，父类就已经通过默认函数构造好了
	my_unordered_map():base_class() {}

	my_unordered_map(size_t size):base_class(size) {}

	//析构函数，暂且不做任何处理
	~my_unordered_map() = default;

	//查找
	Tval find(const Tkey& tkey) {
		return base_class::find_key(tkey);
	}

	//添加
	void insert(Tkey& key,Tval& val) {
		base_class::insertdata(key, val);
	}

	//删除
	void erase(Tkey& key) {
		base_class::delete_key(key);
	}

	size_t size() {
		return data_num;
	}

	bool empty() {
		return data_num == 0;
	}


	void clear() {
		hash_t.clear();
		flag.clear();
		capacity = 13;
		data_num = 0;
	}

	void swap(my_unordered_map& other_map) {
		hash_t.swap(other_map.hash_t);
		flag.swap(other_map.flag);

		size_t temp_capacity = capacity;
		capacity = other_map.capacity;
		other_map.capacity = temp_capacity;
		size_t temp_data_num = data_num;
		data_num = other_map.data_num;
		other_map.data_num = temp_data_num;
	}

	//获取指定元素值的元素个数
	size_t count(Tkey& key) {
		return count(key);
	}

	_iter begin() {
		return base_class::begin();
	}

	_iter end() {
		return base_class::end();
	}
};