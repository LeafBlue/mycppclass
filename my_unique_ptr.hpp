#pragma once
#include<iostream>



template<typename T>
class my_unique_ptr {
private:
	//����ָ��
	T* obj;

	my_unique_ptr(T* _obj) {
		obj = _obj;
	}
public:
	/*
	��������԰ѹ��캯�����أ���ȷ��������ָ�붼��ͨ�����������ġ�

	new T(std::forward<Args>(args)...) ͨ��������ǿ��Եõ�һ��new T��
	���⻹����������Ҫ����my_unique_ptr��ʵ����������㻹��Ҫ����һ��my_unique_ptr�Ĺ��캯����
	�õ������new T��ʲô�أ�
	��ʵ������һ��δ����ֵ���κ�ָ���new�������ḳֵ��my_unique_ptr�Ĺ��캯����������˹��캯��������һ��T*�����ա�

	������������������Ǵ���һ��my_unique_ptr�������ǲ���ָ��һ���Ѵ��ڵ�my_unique_ptrȥ��������
	���������������д�����ڲ��ľ�̬���������������á���д�����ڲ�ʱ�����T�����Ѵ��ڵ�T��ͻ��������Ͳ���Ҫд�ˡ�
	*/
	//����
	template<typename... Args>
	static my_unique_ptr<T> makemy_unique_ptr(Args&&...args) {
		return my_unique_ptr<T>(new T(std::forward<Args>(args)...))
	}

	//�ƶ����캯��
	my_unique_ptr(my_unique_ptr&& other)noexcept {
		obj = other.obj;
		other.obj = nullptr;
	}
	//�ƶ���ֵ����
	my_unique_ptr& operator=(my_unique_ptr&& other)noexcept {
		if (this == other) {
			return *this;
		}
		//����ԭthis
		reset();
		obj = other.obj;
		other.obj = nullptr;
		return *this;
	}

	//���ÿ������캯��
	my_unique_ptr(my_unique_ptr& other) = delete;
	my_unique_ptr& operator=(my_unique_ptr& other) = delete;

	//��������
	~my_unique_ptr() {
		delete obj;
	}

	//����ָ��
	/*
	* ����ԭ�ȵĲ�����T* other��Ҫ�󿪷����Լ��淶����Ĳ���������һ����ָ��
	* ����Բ������˸������ƣ��ÿ�����ֻ�ܴ�����ֵָ��
	*/
	void reset(T*&& other = nullptr) {
		if (other != this->obj) {
			T* temp = obj;
			obj = other;
			delete temp;
		}
		//��ȣ�������
	}
	//�ͷ�����Ȩ
	T* release() noexcept {
		T* temp = obj;
		obj = nullptr;
		return temp;
	}

	//get�������ڲ������ָ�루��ת������Ȩ��
	//��ν��ռ��ָ����ͬһʱ��ֻ��һ��my_unique_ptr����������������ָͨ��Ҳָ����
	// ʵ�����������ֻҪд�����ƶ����رո��ƾ�����д����
	T* get()const noexcept {
		return obj;
	}
	//operator*�������ò�����
	//ͨ�����ǻ���ݽ����ö�ԭ��������޸ģ��������ﲻ�ܷ���һ��T�����T�ĸ���
	T& operator*() const noexcept{
		return *obj;
	}
	//operator->����Ա���ʲ�����
	//��Ҫ���̫������
	T* operator->() const noexcept {
		return obj;
	}
	
	//explicit operator bool() const������Ƿ���зǿ�ָ��
	//����һ���������أ�ʵ��������԰����Ž�if��while����ֱ��ʹ��
	explicit operator bool() const noexcept {
		return obj != nullptr;
	}
	
	//swap(my_unique_ptr& other)���������� my_unique_ptr ʵ��֮�����Դ
	void swap(my_unique_ptr& other) noexcept {
		if (obj!=other.obj) {
			T* temp = obj;
			obj = other.obj;
			other.obj = temp;
		}
	}
	
};



