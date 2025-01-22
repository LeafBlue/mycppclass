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

	//�������������Ҳ����κδ���
	~my_unordered_map() = default;


	//����
	_iter find(const Tkey& tkey) {
		return set_find(tkey);
	}

	//����
	void insert(const Tkey& tkey) {
		base_class::insertdata(key, 0);
	}

	//ɾ��
	void erase(const Tkey& key) {
		base_class::delete_key(key);
	}

	size_t size() {
		return base_class::data_num;
	}

	bool empty() {
		return base_class::data_num == 0;
	}

	//��ȡָ��Ԫ��ֵ��Ԫ�ظ���
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