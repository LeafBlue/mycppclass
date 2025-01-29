#pragma once
#include<atomic>


/*
	���캯����Ĭ�Ϲ��졢�� shared_ptr ���졢�������졢�ƶ����죩��
	����������
	��ֵ�������������ֵ���ƶ���ֵ����
	lock() ������
	expired() ������
	use_count() ������
	reset() ����
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
		, weak_count(other.weak_count)//����ᱻ��ʼ��Ϊ0�����Բ��õ���û����
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

	//��ע�⣬lock�����ǻ��ڵ�ǰ���񹹽�һ��shareptr�������ǽ���ǰ����ת��Ϊshareptr���������ﲻ��Ҫ--�����ü���
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

	//�������Ƿ񡰲����ڡ�������������򷵻�true
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




