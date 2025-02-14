# mySimpleSTL
C++ STL的学习和简单地实现
## 文件目录说明

|——mySimpleSTL
    |
    |── header         -->头文件
    |
    |──source          -->源文件
    |
    |── STL源码剖析     --> 存放STL源码剖析的笔记



## 顺序容器

### thin_vector

#### 数据成员
* _m_start：指向动态数组的起始位置。
* _m_finish：指向当前最后一个元素的下一个位置。
* _m_end_of_storage：指向分配的内存末尾。