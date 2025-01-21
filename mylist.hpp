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
    // 构造函数和析构函数
    mylist() 
        :head(nullptr),tail(nullptr),node_count(0)
    {}
    ~mylist() {
        clear();
    }

    // 插入操作
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

    //在这里，链表并没有一个真正意义上的链，它实际上是通过成员变量，把一个个独立变量链接起来了。
    //所以在这里我们可以直接通过position指针，直接找到对应的值并进行一个插入
    void insertAfter(Node* position, const T& value) {
        if (position == nullptr) {
            //尾部插入
            insertBack(value);
            return;
        }
        if (position->next == nullptr) {
            //尾部插入
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
            //头插法
            insertFront(value);
            return;
        }
        if (position->prev == nullptr) {
            //头插法
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

    // 删除操作
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

    // 查找操作
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

    // 访问操作
    T& front();
    const T& front() const;
    T& back();
    const T& back() const;

    // 大小和状态
    size_t size() const {
        return node_count;
    }
    bool empty() const {
        return node_count == 0;
    }


    // 迭代器支持（可选）
    class iterator {
    private:
        Node* ptr;
        iterator(Node *node)
        {
            ptr = node;
        };
    public:
        iterator() {};
        //解引用操作符
        T& operator*() const {
            return ptr->data;
        }
        T* operator->() const {
            return ptr->data;
        }
        //递增操作符
        //前置递增：Iterator& operator++()
        iterator& operator++() {
            ptr = ptr->next;
            return *this;
        }
        //后置递增：Iterator operator++(int)
        iterator operator++(int) {
            iterator temp = iterator(ptr);
            ptr = ptr->next;
            return temp;
        }
        //递减操作符
        //前置递减：Iterator& operator--()
        iterator& operator--() {
            ptr = ptr->prev;
            return *this;
        }
        //后置递减：Iterator operator--(int)
        iterator operator--(int) {
            iterator temp = iterator(ptr);
            ptr = ptr->prev;
            return temp;
        }
        //比较操作符
        //相等性比较：bool operator==(const Iterator& other) const
        bool operator==(const iterator& other)const {
            return ptr == other.ptr;
        }
        //不等性比较：bool operator!=(const Iterator& other) const
        bool operator!=(const iterator& other)const {
            return ptr != other.ptr;
        }

        friend class mylist<T>;
    };
    iterator begin() {
        return iterator(head);
    }
    //end()函数，用来表示逻辑上“超出最后一位元素”的位置
    //如果拿tail来计算很容易导致未定义行为
    iterator end() {
        return iterator(nullptr);
    }
    //在这里不要再使用find，迭代器已经找过了，会增加工作量
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











