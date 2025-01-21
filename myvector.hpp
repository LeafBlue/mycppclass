#pragma once
#include<iostream>
#include<iterator>

template<typename T>
class myvector {
private:
	//���� ���� ����
	
	size_t capability;
	size_t size;
	T* t_arr;

public:
	//���캯��

	//�޲Σ�����ʼ��С10
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

	//������Ԫ�س�ʼ��������ֵ
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

	//��������
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

	//�ƶ�����
	myvector(myvector&& other) noexcept
		:t_arr(other.t_arr)
		,capability(other.capability)
		,size(other.size) 
	{
		other.t_arr = nullptr;
	}

	//����
	~myvector() {
		delete[] t_arr;
		/*
		* ���󼴽������٣��˲������Բ���
		capability = 0;
		size = 0;
		*/
	}

	class iterator {
	private:
		//�����Ա�����洢������ָ��ĵ�ǰԪ��
		T* ptr;
		//������캯������ȡ����Tָ�뽻����Աptr����
		//������캯�����ܱ�¶������ʹ�ã�����������һ��T���͵�����myvectorԪ�أ��ᵼ��δ������Ϊ
		iterator(T *t) {
			ptr = t;
		}
	public:
		iterator() {}
		//operator*�����ص�������ָ���Ԫ�ص����á�
		T& operator*() const {
			return *ptr;
		}
		//operator->������ָ���Ա��ָ�루����ָ�����͵ĵ�������˵����ͨ��ֱ�ӷ��� ptr->����
		T* operator->()const {
			return ptr;
		}
		//���ص����͵ݼ�������(operator++ �� operator--)
		//operator++()��ǰ�õ��������ص�����ĵ�������
		iterator operator++() noexcept {
			++ptr;
			return *this;
		}
		//operator++(int)�����õ�����
		//���ص���ǰ�ĵ������������ٶ�ԭ��������޸�
		iterator operator++(int)noexcept {
			iterator temp = iterator(ptr);
			++ptr;
			return temp;
		}
		//operator--() �� operator--(int)�����Ƶأ�ʵ��ǰ�úͺ��õݼ���������
		iterator& operator--() noexcept {
			--ptr;
			return *this;
		}
		iterator operator--(int) noexcept {
			iterator temp = iterator(ptr);
			--ptr;
			return temp;
		}
		//��������ԺͲ����ԱȽϲ�����(operator== �� operator!=)
		//��Щ�����������ж������������Ƿ�ָ��ͬһλ�ã������ѭ����ֹ�����ǳ���Ҫ��
		bool operator==(const iterator &_it)const noexcept {
			return this->ptr == _it.ptr;
		}
		bool operator!=(const iterator& _it)const noexcept {
			return this->ptr != _it.ptr;
		}
		//operator+ �� operator-��֧����Ծʽ�ƶ���
		iterator operator+(int n) {
			return iterator(ptr+n);
		}
		iterator operator-(int n) {
			return iterator(ptr - n);
		}
		//std::ptrdiff_t ����һ����׼�����ڱ�ʾָ���ֵ�ķ���
		std::ptrdiff_t operator-(const iterator &_it)const noexcept  {
			return ptr - _it.ptr;
		}
		//operator+= �� operator-=��ԭ�����ӻ����ָ�������Ĳ�����
		iterator& operator+=(std::ptrdiff_t index)noexcept {
			ptr = ptr + index;
			return *this;
		}
		iterator& operator-=(std::ptrdiff_t index)noexcept {
			ptr = ptr - index;
			return *this;
		}
		//��ϵ���������� <�� <= ��>�� >= ����֧�ֵ�����֮��ıȽϡ�
		bool operator<(const iterator& _it)const noexcept { return ptr < _it.ptr; }
		bool operator<=(const iterator& _it)const noexcept { return ptr <= _it.ptr; }
		bool operator>(const iterator& _it)const noexcept { return ptr > _it.ptr; }
		bool operator>=(const iterator& _it)const noexcept { return ptr >= _it.ptr; }
		//myvector��begin��endҪ��������Ĺ��캯��������һ����Ԫ
		friend class myvector<T>;
	};

	//��ֵ����������
	//ʹ��new�����׳�bad_alloc ���������״δ��������
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
				//�ȴ������׳�
				delete[] temp;
				t_arr = oldarr;
				throw;
			}
		}
	}

	//�±���������أ������޸�Ԫ��
	//����Ҫ����Ƿ�Խ�磬��at����
	T& operator[](size_t index) {
		return t_arr[index];
	}

	//��� vector �Ƿ�Ϊ��
	bool empty() {
		return size == 0;
	}
	//���ص�ǰ vector ��Ԫ�ص�������
	size_t size_() {
		return size;
	}

	//���ص�ǰ���������
	size_t capacity() {
		return capability;
	}

	//��Ԥ�������� new_cap ��Ԫ�صĿռ䡣�������˺���������С����
	void reserve(size_t new_cap) {
		if (new_cap > capability) {
			T* temp;
			try {
				temp = new T[new_cap];
				for (size_t i = 0; i < size; i++)
				{
					temp[i] = t_arr[i];
				}
				//deleteֻ�����new�Ķ��������������������е�Ԫ������placement new����������޷��չ˵���
				//���������ʹ����placement new��������飬����Ҫ���¿���
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
	
	//shrink_to_fit()�������ܼ����ѷ��䵫δʹ�õĿռ䣨C++11 �����ϣ���
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
	
	//��ȫ�ط���ָ��λ�õ�Ԫ�أ����߽��飬�׳��쳣��
	//������˵������[]�ǲ���Ҫ����±�Խ��ģ���������������
	T& at(size_t index) {
		if (index > size - 1) {
			throw std::out_of_range("index out of range");
		}
		return t_arr[index];
	}
	//���ʵ�һ��Ԫ�ء�
	T& front() {
		if (size>0) {
			return t_arr[0];
		}
		throw std::out_of_range("front out of range");
	}
	//�������һ��Ԫ�ء�
	T& back() {
		if (size > 0) {
			return t_arr[size - 1];
		}
		throw std::out_of_range("back out of range");
	}
	
	
	
	//	push_back(const value_type& value)����ĩβ���һ��Ԫ�ء�
	void push_back(const T& value) {
		if (size < capability) {
			//����δ�ﵽ����
			t_arr[size] = value;
			size++;
		}
		else {
			//�����ﵽ���ޣ����������
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
	//	push_back(value_type&& value)����ĩβ���һ��Ԫ�أ���ֵ���ð汾��C++11 �����ϣ���
	// ����ͨ������ֱ���ƶ���Դ���������п���
	//ͨ��ʹ����ֵ���ã�������յ���һ����ʱ��Դ�����������Դ���Ա�����
	//���Ⲣ����ζ����ֵ����ֻ�ܰ���ʱ��Դ���������Ǻ�����һ����ͼ
	void push_back(const T&& value) {
		if (size < capability) {
			//����δ�ﵽ����
			//move���������Ը�Ч�ؽ���Դ���ƶ�����Ȩ����ʹ��ǰ�����ÿգ�ʹ�����Դ�����������������������һ������
			//ͬʱ������������ɡ�δ���� ���ա�״̬
			t_arr[size] = move(value);
			size++;
		}
		else {
			//�����ﵽ���ޣ����������
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
	//	pop_back()���Ƴ����һ��Ԫ�ء�
	void pop_back() {
		if (size == 0) {
			return;
		}
		size--;
		//���ﲻ������д�����ڷ�ָ������ =nullptr����д�������
		//t_arr[size] = nullptr;
		//��Ҳ���ܰ������Ų��ܣ�Ȼ����浱û����һ��ֱ�Ӹ�����������ܻᵼ�¶����޷���ȷ���������������Ӷ������޷�Ԥ�ϵ��鷳���⡣
		// �˴���Ҫ��ʽ����������������

		//is_trivially_destructible_v �����ж�T�ǲ�����ͨ���ƻ�����
		if constexpr (!std::is_trivially_destructible_v <T>) {
			t_arr[size].~T();
		}
	}


	//	insert(iterator pos, const value_type & value)����ָ��λ�ò���һ��Ԫ�ء�
	//���������ƶ�һλʵ���ϻ����ڸ��ƣ����������ʹ����move��������Tʵ�����ƶ���ֵ�����ʱ������ƶ�
	//��ʹ���ƶ����ᵼ�±��ƶ���λ�ñ����Ч��δ����״̬��һ��Ҫȷ�����λ�ñ��µ�������䣬���һ��Ҫ��ϸд�����ѭ��
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

	//	insert(iterator pos, size_type count, const value_type & value)����ָ��λ�ò�������ͬԪ�ء�
	//��δ�����ʹ����size+count���±꣬����һ��Ҫȷ���µ�����������������±�
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
	//	erase(iterator pos)���Ƴ�ָ��λ�õ�Ԫ�ء�
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
		//�������Ѿ������Ч����˲���Ҫ����
		//t_arr[size - 1] = new T();
		size--;
	}
	//	erase(iterator first, iterator last)���Ƴ�ָ����Χ�ڵ�Ԫ�ء�
	//���ﲻ����lastָ���ֵ������������±�������ţ�Ǽ⣬Ҫȷ���Լ����õ�ÿһ����������ȷ���ĺ���
	//��first > lastʱ��Ӧ���׳��쳣���������������Ž�����������������Ϊ�û��ܿ���Ҫ�õ����������������������
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
	//	clear()���Ƴ�����Ԫ�ء�
	void clear() {
		//clear��Ӧ�����·����µ����飬����Ҫ����ÿ��Ԫ�ص���������
		for (size_t i = 0; i < size; i++)
		{
			if constexpr (!std::is_trivially_destructible_v <T>) {
				t_arr[i].~T();
			}
		}
		size = 0;
	}
	//	swap(vector & other)���������� vector �����ݡ�
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

	//begin() �� end()������ָ���һ�������һ��Ԫ��֮��ĵ��������ǳ����汾����
	iterator begin() noexcept {
		return myvector<T>::iterator(t_arr);
	}

	iterator end() noexcept {
		return t_arr + size;
	}

};