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
- [x] reverse_iterator rbegin() noexcept;
- [x] const_reverse_iterator rbegin() const noexcept;
- [x] reverse_iterator rend() noexcept;// 反向迭代器：指向反向尾后位置（即原列表第一个元素的前驱）
- [x] const_reverse_iterator rend() const noexcept;
- [x] const_iterator cbegin() const noexcept;// 常量迭代器版本（C++11起）
- [x] const_iterator cend() const noexcept;
- [x] const_reverse_iterator crbegin() const noexcept;
- [x] const_reverse_iterator crend() const noexcept;
***
#### 容量
- [x] bool empty() const noexcept; //检查列表是否为空
- [x] size_type size() const noexcept;// 返回当前元素数量（时间复杂度 O(n)，C++11 前可能为 O(1)）
- [x] size_type max_size() const noexcept;// 返回列表可容纳的最大元素数量（与系统或实现相关）
- [x] void resize(size_type count);// 调整列表大小：若 `count > size()`，添加默认构造的元素；否则删除多余元素
- [x] void resize(size_type count, const T& value);// 调整列表大小：若 `count > size()`，添加 `value` 的副本；否则删除多余元素
***
#### 修改器
- [x] void push_back(const T& value);// 尾部插入元素
- [ ] void push_back(T&& value); // C++11起
- [x] void pop_back();// 删除尾部元素（列表为空时行为未定义）
- [x] void push_front(const T& value);// 头部插入元素（拷贝或移动语义）
- [ ] void push_front(T&& value); // C++11起
- [x] void pop_front();// 删除头部元素（列表为空时行为未定义）
- [x] template <class... Args> void emplace_front(Args&&... args);// 在头部就地构造元素（C++11起，避免拷贝）
- [x] template <class... Args> void emplace_back(Args&&... args);// 在尾部就地构造元素（C++11起）
- [ ] template <class... Args> iterator emplace(const_iterator pos, Args&&... args);// 在迭代器 `pos` 前就地构造元素，返回新元素的迭代器（C++11起）
- [ ] iterator insert(const_iterator pos, const T& value);// 在 `pos` 前插入元素，返回插入的第一个元素的迭代器
- [ ] iterator insert(const_iterator pos, T&& value); // C++11起
- [ ] iterator insert(const_iterator pos, size_type count, const T& value); // 插入 `count` 个 `value`
- [ ] template <class InputIt> iterator insert(const_iterator pos, InputIt first, InputIt last);// 在 `pos` 前插入范围 `[first, last)` 的元素
- [ ] iterator insert(const_iterator pos, std::initializer_list<T> ilist);// 插入初始化列表中的元素（C++11起）
- [ ] iterator erase(const_iterator pos);// 删除 `pos` 处的元素，返回被删元素的下一个位置的迭代器
- [ ] iterator erase(const_iterator first, const_iterator last);// 删除 `[first, last)` 范围的元素，返回 `last` 的迭代器
- [ ] void swap(list& other) noexcept;// 交换两个列表的内容（O(1) 时间复杂度）
- [ ] void clear() noexcept;// 清空列表中的所有元素
- [ ] void assign(size_type count, const T& value);// 替换列表内容为 `count` 个 `value`
- [ ] template <class InputIt> void assign(InputIt first, InputIt last);// 替换内容为迭代器范围 `[first, last)` 的元素
- [ ] void assign(std::initializer_list<T> ilist);// 替换内容为初始化列表 `ilist` 中的元素（C++11起）
***
#### 操作
- [ ] void merge(list& other);// 合并两个有序列表：将 `other` 的元素移动到当前列表，合并后 `other` 为空// 前提：当前列表和 `other` 必须已按相同顺序排序
- [ ] void merge(list&& other); // C++11起
- [ ] template <class Compare> void merge(list& other, Compare comp);// 使用自定义比较器 `comp` 合并两个有序列表
- [ ] template <class Compare> void merge(list&& other, Compare comp); // C++11起
- [ ] void splice(const_iterator pos, list& other);// 将 `other` 的所有元素移动到当前列表的 `pos` 前（`other` 必须与当前列表不同）
- [ ] void splice(const_iterator pos, list&& other); // C++11起
- [ ] void splice(const_iterator pos, list& other, const_iterator it);// 将 `other` 中 `it` 指向的元素移动到当前列表的 `pos` 前
- [ ] void splice(const_iterator pos, list&& other, const_iterator it); // C++11起
- [ ] void splice(const_iterator pos, list& other, const_iterator first, const_iterator last);// 将 `other` 中 `[first, last)` 范围的元素移动到当前列表的 `pos` 前
- [ ] void splice(const_iterator pos, list&& other, const_iterator first, const_iterator last); // C++11起
- [ ] void remove(const T& value);// 删除所有值等于 `value` 的元素
- [ ] template <class UnaryPredicate> void remove_if(UnaryPredicate p);// 删除所有满足谓词 `p` 的元素（`p(element)` 返回 `true` 时删除）
- [ ] void reverse() noexcept;// 反转列表元素的顺序（O(n) 时间复杂度）
- [ ] void unique();// 删除连续重复元素（保留第一个）
- [ ] template <class BinaryPredicate> void unique(BinaryPredicate p);// 使用二元谓词 `p` 判断是否重复（例如自定义相等条件）
- [ ] void sort();// 对列表元素进行升序排序（默认使用 `<` 运算符）
- [ ] template <class Compare> void sort(Compare comp);// 使用自定义比较器 `comp` 排序
***
#### 赋值运算符
- [ ] list& operator=(const list& other);// 拷贝赋值：用 `other` 的内容替换当前列表
- [ ] list& operator=(list&& other) noexcept;// 移动赋值：将 `other` 的资源移动到当前列表（C++11起）
- [ ] list& operator=(std::initializer_list<T> ilist);// 用初始化列表 `ilist` 的内容赋值（C++11起）

















