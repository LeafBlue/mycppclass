#pragma once
#include<iostream>
#include<iterator>



template<typename T>
class mylist {
private:
	struct Node {
		T data;
		Node* prev;
		Node* next;

		Node(const T& val) :data(val), prev(nullptr), next(nullptr) {};
	};

	Node* head;
	Node* tail;
	size_t node_count;
public:
    // ���캯������������
    mylist() 
        :head(nullptr),tail(nullptr),node_count(0)
    {}
    ~mylist() {
        clear();
    }

    // �������
    void insertFront(const T& value) {
        Node* temp_node = new Node(value);
        if (node_count == 0) {
            head = temp_node;
            tail = temp_node;
        }
        else {
            temp_node->next = head;
            head->prev = temp_node;
            head = temp_node;
        }
        node_count++;
    }
    void insertBack(const T& value) {
        Node* temp_node = new Node(value);
        if (node_count == 0) {
            head = temp_node;
            tail = temp_node;
        }
        else {
            temp_node->prev = tail;
            tail->next = temp_node;
            tail = temp_node;
        }
        node_count++;
    }

    //���������û��һ�����������ϵ�������ʵ������ͨ����Ա��������һ���������������������ˡ�
    //�������������ǿ���ֱ��ͨ��positionָ�룬ֱ���ҵ���Ӧ��ֵ������һ������
    void insertAfter(Node* position, const T& value) {
        if (position == nullptr) {
            //β������
            insertBack(value);
            return;
        }
        if (position->next == nullptr) {
            //β������
            insertBack(value);
            return;
        }
        Node* newnode = new Node(value);
        Node* posi_next = position->next;
        newnode->next = posi_next;
        newnode->prev = position;
        posi_next->prev = newnode;
        position->next = newnode;
        node_count++;
    }
    void insertBefore(Node* position, const T& value) {
        if (position == nullptr) {
            //ͷ�巨
            insertFront(value);
            return;
        }
        if (position->prev == nullptr) {
            //ͷ�巨
            insertFront(value);
            return;
        }
        Node* newnode = new Node(value);
        Node* posi_prev = position->prev;
        newnode->next = position;
        newnode->prev = posi_prev;
        position->prev = newnode;
        posi_prev->next = newnode;
        node_count++;
    }

    // ɾ������
    bool remove(const T& value) {
        Node* temp = find(value);
        if (temp == nullptr) {
            return false;
        }
        removeNode(temp);
        return true;
    }
    void removeNode(Node* node) {
        if (node == nullptr) {
            return;
        }
        if (node_count == 1) {
            head = nullptr;
            tail = nullptr;
        }
        else {
            if (node->prev == nullptr) {
                head = node->next;
                head->prev = nullptr;
            }
            else {
                node->prev->next = node->next;
            }
            if (node->next == nullptr) {
                tail = node->prev;
                tail->next = nullptr;
            }
            else {
                node->next->prev = node->prev;
            }
        }
        delete node;
        node_count--;
    }
    void clear() {
        Node* temp = head;
        while (temp != nullptr) {
            Node *temp_1 = temp->next;
            delete temp;
            temp = temp_1;
        }
        head = nullptr;
        tail = nullptr;
        node_count = 0;
    }

    // ���Ҳ���
    bool contains(const T& value) const {
        Node* temp = head;
        while (temp != nullptr) {
            if (temp->data == value) {
                return true;
            }
            temp = temp->next;
        }
        return false;
    }
    Node* find(const T& value) const {
        Node* temp = head;
        while (temp != nullptr) {
            if (temp->data == value) {
                return temp;
            }
            temp = temp->next;
        }
        return nullptr;
    }

    // ���ʲ���
    T& front();
    const T& front() const;
    T& back();
    const T& back() const;

    // ��С��״̬
    size_t size() const {
        return node_count;
    }
    bool empty() const {
        return node_count == 0;
    }


    // ������֧�֣���ѡ��
    class iterator {
    private:
        Node* ptr;
        iterator(Node *node)
        {
            ptr = node;
        };
    public:
        iterator() {};
        //�����ò�����
        T& operator*() const {
            return ptr->data;
        }
        T* operator->() const {
            return ptr->data;
        }
        //����������
        //ǰ�õ�����Iterator& operator++()
        iterator& operator++() {
            ptr = ptr->next;
            return *this;
        }
        //���õ�����Iterator operator++(int)
        iterator operator++(int) {
            iterator temp = iterator(ptr);
            ptr = ptr->next;
            return temp;
        }
        //�ݼ�������
        //ǰ�õݼ���Iterator& operator--()
        iterator& operator--() {
            ptr = ptr->prev;
            return *this;
        }
        //���õݼ���Iterator operator--(int)
        iterator operator--(int) {
            iterator temp = iterator(ptr);
            ptr = ptr->prev;
            return temp;
        }
        //�Ƚϲ�����
        //����ԱȽϣ�bool operator==(const Iterator& other) const
        bool operator==(const iterator& other)const {
            return ptr == other.ptr;
        }
        //�����ԱȽϣ�bool operator!=(const Iterator& other) const
        bool operator!=(const iterator& other)const {
            return ptr != other.ptr;
        }

        friend class mylist<T>;
    };
    iterator begin() {
        return iterator(head);
    }
    //end()������������ʾ�߼��ϡ��������һλԪ�ء���λ��
    //�����tail����������׵���δ������Ϊ
    iterator end() {
        return iterator(nullptr);
    }
    //�����ﲻҪ��ʹ��find���������Ѿ��ҹ��ˣ������ӹ�����
    iterator erase(iterator pos) {
        if (pos == end()) {
            throw;
        }
        iterator next = pos;
        removeNode(pos.ptr);
        ++next;
        return next;
    }

    
};











