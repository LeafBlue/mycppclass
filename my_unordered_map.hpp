#pragma once
#include"my_open_hashtable.hpp"



template<class Tkey, class Tval>
class my_unordered_map :public my_open_hashtable<Tkey, Tval> {
private:
	using base_class = my_open_hashtable<Tkey, Tval>;
	using _iter = typename base_class::iterator;
	
	/*
		my_open_hashtable����private�ĳ�Ա��������Щ�����޷�����������ʣ��������Ŷ��󴴽�������
		Ϊ�˷�����ã����ǰ�������Ϊprotected��
		��Ϊprotected �󣬿������������ڲ����ʣ����ǲ�����������Ķ�����ͼ���������
		��������͸������ͬ���ĳ�Ա��ֱ�ӷ��ʵõ������������Ա������ĳ�Ա�ᱻ���أ���Ҫ˫ð��::����
	*/
	
public:

	//���캯��
	//���ø���Ĺ��캯��������ͨ�����빹�죬ֻ��ͨ����ʼ���б���
	//Ҳ���Բ����죬��������ø����Ĭ�Ϲ��캯����
	//������Ϊ�����е��������ʱ��������Ѿ�ͨ��Ĭ�Ϻ����������
	my_unordered_map():base_class() {}

	my_unordered_map(size_t size):base_class(size) {}

	//�������������Ҳ����κδ���
	~my_unordered_map() = default;

	//����
	Tval* find(const Tkey& tkey) {
		return base_class::find_key(tkey);
	}

	//���
	void insert(const Tkey& key, const Tval& val) {
		base_class::insertdata(key, val);
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


	void clear() {
		base_class::hash_t.clear();
		base_class::flag.clear();
		base_class::capacity = 13;
		base_class::data_num = 0;
	}

	void swap(my_unordered_map& other_map) {
		base_class::hash_t.swap(other_map.hash_t);
		base_class::flag.swap(other_map.flag);

		size_t temp_capacity = base_class::capacity;
		base_class::capacity = other_map.capacity;
		other_map.capacity = temp_capacity;
		size_t temp_data_num = base_class::data_num;
		base_class::data_num = other_map.data_num;
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