#pragma once
#include "my_open_hashtable.hpp"


template<class Tkey, class Tval>
class my_unordered_set :public my_open_hashtable<Tkey, Tval> {
private:
	using base_class = my_open_hashtable<Tkey, Tval>;
	using _iter = typename base_class::iterator;


public:
	my_unordered_set() :base_class() {}

	my_unordered_set(size_t size) :base_class(size) {}

	//析构函数，暂且不做任何处理
	~my_unordered_map() = default;


	//查找
	_iter find(const Tkey& tkey) {
		return set_find(tkey);
	}

	//插入
	void insert(const Tkey& tkey) {
		base_class::insertdata(key, 0);
	}

	//删除
	void erase(const Tkey& key) {
		base_class::delete_key(key);
	}

	size_t size() {
		return base_class::data_num;
	}

	bool empty() {
		return base_class::data_num == 0;
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