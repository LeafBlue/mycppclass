#pragma once
#include<iostream>

//����ָ��
template<typename T>
class my_share_ptr {
private:
	T* obj;
	//int ptr_num;
	//ͨ������һ��ԭ�Ӷ���ָ�룬ָ��һ����е��ض�����ÿ�θ���ʱ��ֻ����ָ�룬������ָ��ָ�������
	//atomic���ص㣺����ԭ�ӣ��̰߳�ȫ
	std::atomic<std::size_t>* ref_count = nullptr;

public:
	my_share_ptr() :obj(nullptr), ref_count(nullptr){}

	//explicit�ؼ��֣����Խ�ֹ��ʽת��
	explicit my_share_ptr(T* other = nullptr) 
		:obj(other)
	{	
		if (obj) {
			ref_count = new std::atomic<std::size_t>(1);
		}
	}

	//���ƹ���
	my_share_ptr(const my_share_ptr& other) 
		:obj(other.obj)
		,ref_count(other.ref_count) 
	{
		++(*ref_count);
	}
	
	//�ƶ�����
	my_share_ptr(my_share_ptr&& other) noexcept
		:obj(other.obj)
		, ref_count(other.ref_count) 
	{
		other.obj = nullptr;
		other.ref_count = nullptr;
	}

	//��������
	//��������Ҫ��ref_count-1����Ҫ����ref_count�Ƿ����
	//��ref_count-1��0�󣬿��԰�ȫɾ����Դ
	~my_share_ptr() {
		if (ref_count && --(*ref_count) == 0) {
			delete obj;
			delete ref_count;
		}
	}

	//���Ʋ�����
	my_share_ptr& operator=(const my_share_ptr& other) {
		obj = other.obj;
		ref_count = other.ref_count;
		++(*ref_count);
		return *this;
	}
	//�ƶ�������
	my_share_ptr& operator=(my_share_ptr&& other)noexcept {
		obj = other.obj;
		ref_count = other.ref_count;
		other.obj = nullptr;
		other.ref_count = nullptr;
		return *this;
	}

	//reset()��
	//���� my_shared_ptr��ʹ�䲻�ٹ���ǰ��������У�������ѡ�񴫵�һ���µĶ���������
	void reset(T *other = nullptr){
		release();
		if (other != nullptr) {
			obj = other;
			ref_count = new std::atomic<std::size_t>(1);
		}
	}
	//release()��
	//�ͷŶԶ��������Ȩ������ԭʼָ�루ע�⣺���� std::shared_ptr ����Ϊ��ͬ��ͨ�����Ƽ��� std::shared_ptr ��ʹ�ô˷�������Ϊ��ᵼ������ָ�����⣩��
	void release() {
		if (ref_count && --(*ref_count) == 0) {
			delete obj;
			delete ref_count;
		}
		obj = nullptr;
		ref_count = nullptr;
	}
	//get()��
	//�����ڲ��洢����ָ�룬������ת������Ȩ��
	//const noexcept  ˳���ܴ�����ᵼ�±�����ʶ�����Ĵ������
	T* get()const noexcept  {
		return obj;
	}
	//use_count()��
	//���ص�ǰ��������ü�����
	//���������ﷵ��size_t Ҳ�Ǹ����������޸�ref_count�������ڰ�ȫ���ǣ�ֱ�Ӷ�ȡref_count��Ϊ���Ƽ�
	size_t use_count() const noexcept {
		if (ref_count) {
			return ref_count->load();
		}
		//return *ref_count;
		return 0;
	}
	//swap(my_shared_ptr& other)���������� my_shared_ptr ʵ�������ݡ�
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
	//operator bool()��
	//��ʽת��Ϊ����ֵ�����ڼ�� my_shared_ptr �Ƿ�Ϊ�ա�
	explicit operator bool()const noexcept {
		return obj != nullptr;
	}
	//�����ò�����(*�� ->)��
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
	
	//operator== �� operator!=��
	bool operator==(const my_share_ptr& other)const noexcept {
		return obj == other.obj;
	}

	bool operator!=(const my_share_ptr& other)const noexcept {
		return obj != other.obj;
	}
	
};