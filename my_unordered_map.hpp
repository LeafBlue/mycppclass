#pragma once
#include"my_open_hashtable.hpp"



template<class Tkey, class Tval>
class my_unordered_map :public my_open_hashtable<Tkey, Tval> {
	using base_class = my_open_hashtable<Tkey, Tval>;
	using _iter = typename base_class::iterator;
	
	/*
		my_open_hashtable����private�ĳ�Ա��������Щ�����޷�����������ʣ��������Ŷ��󴴽�������
		Ϊ�˷�����ã����ǰ�������Ϊprotected��
		��Ϊprotected �󣬿������������ڲ����ʣ����ǲ�����������Ķ�����ͼ���������
		��������͸������ͬ���ĳ�Ա��ֱ�ӷ��ʵõ������������Ա������ĳ�Ա�ᱻ���أ���Ҫ˫ð��::����
	*/
	

	//���캯��
	//���ø���Ĺ��캯��������ͨ�����빹�죬ֻ��ͨ����ʼ���б���
	//Ҳ���Բ����죬��������ø����Ĭ�Ϲ��캯����
	//������Ϊ�����е��������ʱ��������Ѿ�ͨ��Ĭ�Ϻ����������
	my_unordered_map():base_class() {}

	my_unordered_map(size_t size):base_class(size) {}

	//�������������Ҳ����κδ���
	~my_unordered_map() = default;

	//����
	Tval find(const Tkey& tkey) {
		return base_class::find_key(tkey);
	}

	//���
	void insert(Tkey& key,Tval& val) {
		base_class::insertdata(key, val);
	}

	//ɾ��
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