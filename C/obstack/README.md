### obstacks 介绍
> obstack是包含一堆对象的内存池。您可以创建任意数量的单独的obstack，然后在指定的obstack中分配对象。
> 在每个obstack中，分配的最后一个对象必须始终是第一个被释放的对象，但是不同的obstack彼此独立。

