# mySimpleSTL
C++ STL的学习和简单地实现

## 一、顺序容器

### vector-->thin_vector

* 数据成员
  * m_start：指向动态数组的起始位置。
  * m_finish：指向当前最后一个元素的下一个位置。
  * m_end_of_storage：指向分配的内存末尾。

#### 构造函数
- [x] 默认构造函数
- [x] 复制构造函数
- [x] 移动构造函数
- [x] 初始化列表构造函数
- [x] 范围构造函数
- [x] 填充构造函数

#### 析构函数
- [x] ~vector()

#### 赋值操作
- [x] 拷贝赋值：`vector& operator=(const vector& other)`
- [x] 移动赋值：`vector& operator=(vector&& other)`
- [x] 初始化列表赋值：`vector& operator=(initializer_list<T> init)`

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

#### 迭代器

##### 正向迭代器
- [x] `iterator begin()`, `const_iterator begin() const`
- [x] `iterator end()`, `const_iterator end() const`
- [x] `const_iterator cbegin() const`  (C++11)
- [x] `const_iterator cend() const`  (C++11)

##### 反向迭代器
- [ ] `reverse_iterator rbegin()`, ` const_reverse_iterator rbegin() const`
- [ ] `reverse_iterator rend()`, `const_reverse_iterator rend() const`
- [ ] `const_reverse_iterator crbegin() const`  (C++11)
- [ ] `const_reverse_iterator crend() const` (C++11)

### 容量管理
- [x] `bool empty() const`
- [x] `size_type size() const`
- [x] `size_type max_size() const`
- [x] `void reserve(size_type new_cap)` （预分配内存）
- [x] `size_type capacity() const`
- [x] `void shrink_to_fit()` (C++11)（缩减内存至适应大小）

### 修改容器
- [x] `void clear()`
- [x] `iterator insert(const_iterator pos, const T& value)`
- [x] `iterator insert(const_iterator pos, T&& value)` (C++11)
- [x] `iterator insert(const_iterator pos, size_type count, const T& value)`
- [x] `template <class InputIt> iterator insert(const_iterator pos, InputIt first, InputIt last)`
- [x] `iterator insert(const_iterator pos, initializer_list<T> ilist)` (C++11)
- [ ] `iterator erase(const_iterator pos)`
- [ ] `iterator erase(const_iterator first, const_iterator last)`
- [ ] `void push_back(const T& value)`
- [ ] `void push_back(T&& value)` (C++11)
- [ ] `template <class... Args> void emplace_back(Args&&... args)` (C++11, 就地构造元素)
- [ ] `void pop_back()`
- [ ] `void resize(size_type count)`
- [ ] `void resize(size_type count, const T& value)`
- [x] `void swap(vector& other)`






















