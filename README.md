# mySimpleSTL
C++ STL的学习和简单地实现

## 顺序容器

### vector-->thin_vector
***
* 数据成员
  * m_start：指向动态数组的起始位置。
  * m_finish：指向当前最后一个元素的下一个位置。
  * m_end_of_storage：指向分配的内存末尾。
***
#### 构造函数
- [x] 默认构造函数
- [x] 复制构造函数
- [x] 移动构造函数
- [x] 初始化列表构造函数
- [x] 范围构造函数
- [x] 填充构造函数
***
#### 析构函数
- [x] ~vector()
***
#### 赋值操作
- [x] 拷贝赋值：`vector& operator=(const vector& other)`
- [x] 移动赋值：`vector& operator=(vector&& other)`
- [x] 初始化列表赋值：`vector& operator=(initializer_list<T> init)`
***
#### 元素访问 
- [x] `reference operator[](size_type pos)`
- [x] `const_reference operator[](size_type pos) const`
- [x] `reference at(size_type pos)（边界检查）`
- [x] `const_reference at(size_type pos) const`
- [x] `reference front()（首元素）`
- [x] `const_reference front() const`
- [x] `reference back()（尾元素）`
- [x] `const_reference back() const`
- [x] `T* data() noexcept（底层数组指针）`
- [x] `const T* data() const noexcept`
***
#### 迭代器
***
##### 正向迭代器
- [x] `iterator begin()`, `const_iterator begin() const`
- [x] `iterator end()`, `const_iterator end() const`
- [x] `const_iterator cbegin() const`  (C++11)
- [x] `const_iterator cend() const`  (C++11)
***
##### 反向迭代器
- [ ] `reverse_iterator rbegin()`, ` const_reverse_iterator rbegin() const`
- [ ] `reverse_iterator rend()`, `const_reverse_iterator rend() const`
- [ ] `const_reverse_iterator crbegin() const`  (C++11)
- [ ] `const_reverse_iterator crend() const` (C++11)
***
### 容量管理
- [x] `bool empty() const`
- [x] `size_type size() const`
- [x] `size_type max_size() const`
- [x] `void reserve(size_type new_cap)` （预分配内存）
- [x] `size_type capacity() const`
- [x] `void shrink_to_fit()` (C++11)（缩减内存至适应大小）
***
### 修改容器
- [x] `void clear()`
- [x] `iterator insert(const_iterator pos, const T& value)`
- [x] `iterator insert(const_iterator pos, T&& value)` (C++11)
- [x] `iterator insert(const_iterator pos, size_type count, const T& value)`
- [x] `template <class InputIt> iterator insert(const_iterator pos, InputIt first, InputIt last)`
- [x] `iterator insert(const_iterator pos, initializer_list<T> ilist)` (C++11)
- [x] `iterator erase(const_iterator pos)`
- [x] `iterator erase(const_iterator first, const_iterator last)`
- [x] `void push_back(const T& value)`
- [x] `void push_back(T&& value)` (C++11)
- [x] `template <class... Args> void emplace_back(Args&&... args)` (C++11, 就地构造元素)
- [x] `void pop_back()`
- [x] `void resize(size_type count)`
- [x] `void resize(size_type count, const T& value)`
- [x] `void swap(vector& other)`
***
### list --> thin_list
***
#### 构造函数
- [x] list();
- [x] explicit list(size_type count, const T& value);//填充构造函数
- [x] template <class InputIt> list(InputIt first, InputIt last); //范围构造
- [x] list(const list& other) //拷贝构造
- [x] list(list&& other) //移动构造
- [x] list(std::initializer_list<T> init); //初始化列表构造
- [x] ~list(); //析构函数
***
#### 元素访问
- [x] reference front(); // 返回首元素的引用（列表为空时行为未定义）
- [x] const_reference front() const;
- [x] reference back();// 返回尾元素的引用（列表为空时行为未定义）
- [x] const_reference back() const;
***
#### 迭代器
- [x] iterator begin() noexcept;// 返回指向首元素的迭代器
- [x] const_iterator begin() const noexcept;
- [x] iterator end() noexcept;// 返回指向尾后位置的迭代器
- [x] const_iterator end() const noexcept;// 反向迭代器：指向反向首元素（即原列表的最后一个元素）
- [ ] reverse_iterator rbegin() noexcept;
- [ ] const_reverse_iterator rbegin() const noexcept;
- [ ] reverse_iterator rend() noexcept;// 反向迭代器：指向反向尾后位置（即原列表第一个元素的前驱）
- [ ] const_reverse_iterator rend() const noexcept;
- [ ] const_iterator cbegin() const noexcept;// 常量迭代器版本（C++11起）
- [ ] const_iterator cend() const noexcept;
- [ ] const_reverse_iterator crbegin() const noexcept;
- [ ] const_reverse_iterator crend() const noexcept;




















