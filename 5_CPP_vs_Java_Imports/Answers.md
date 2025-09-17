#### 1. Find an example of two headers (C/C++ .h files) in the libc/libc++ implementation on any platform (your laptop or any other) that shows the same type declared differently, with one declaration consistent with the type’s implementation in the binary library and the other consistent with but restricted to the API available to you (as programmers). If there are no such headers (or there cannot be) in your opinion and search, argue why.

The FILE structure in C demonstrates perfect information hiding through different header declarations. In the public header (stdio.h), FILE is declared as an opaque pointer, typically `typedef struct _FILE FILE;` without revealing the internal structure. This declaration is "consistent with but restricted to the API available to programmers, allowing only function-based access like fopen(), fprintf(), and fclose().

However, in the internal implementation headers used by the C library itself, FILE is declared as a complete structure containing members like _ptr (buffer pointer), _cnt (character count), _flags (status flags), and _file (file descriptor). This complete declaration is "consistent with the type's implementation in the binary library" because the actual library functions need direct access to these internal fields for efficient I/O operations.

This dual declaration strategy provides encapsulation (users cannot accidentally corrupt internal state), Application Binary Interface stability (internal structure can change without breaking user code), and performance (library functions can access internals directly). The public header restricts programmers to the safe API while the internal header enables the actual implementation.

We can see public implementation of FILE here: https://github.com/bminor/glibc/blob/master/libio/stdio.h

And the actual struct here: https://github.com/bminor/glibc/blob/master/libio/bits/types/struct_FILE.h

#### 2. Answer for C, C++ and Java each: An array “a” is declared as “int a[11][3];”. When all its elements are 0, and then the statement “a[12][1] = 1;” is encountered at that time, what will be the outcome? The possibilities are: (a) a compilation error, or (b) a runtime crash--no exception catch, or (c) a runtime exception, or (d) some "n"th element becomes 1. In cases your answer is (d), also give that “n” each time. Each answer must be justified.

For C, the answer is: some "n"th element becomes 1 or a runtime crash occurs.
This is because C performs no bounds checking. a[12][1] accesses memory 37 integers past the start of the array, or 4 integers past the end of the array [n = 37]. This is undefined behavior that could crash, corrupt memory, or appear to work.

For C++, the answer is: some "n"th element becomes 1 or a runtime crash occurs if we use C-style arrays, while we can catch a runtime exception if we use library array.
This is because C++ behaves exactly like C with standard arrays, while the library class implements safety checks.

For Java, the answer is: a runtime exception occurs.
This is because Java performs automatic bounds checking and an ArrayIndexOutOfBoundsException is thrown. The program can continue after catching the exception.

#### 3. Find examples of a class in the Java sdk on your platform that has two different implementations (or dissimilar .class files) in two different packages.

The Date class has two different implementations with the same class name in the util package and the sql package. One is a general purpose implementation for dates in Java, while the other is a part of the larger SQL support package for Java. The example shows an example where we use fully qualified names to import both types of dates into a program. If we uncomment the import statements, we will have a conflict error and the compilation will fail due to two classes having the same name. In Java, imports are handled during compilation using its package system, so a class is only loaded once, and all conflicts are handled directly.

#### 4. Can each implementation of such a class be instantiated into two distinct objects inside the same method and block?

As explained in the previous question, we can instantiate two distinct objects from the two different classes inside the same method and block using fully qualified identifiers. Without using these, it is not possible to avoid the conflict.

#### 5. Is it possible to do so (3,4 above) in C++ as well? How? Without namespaces?

In C++, there is no automatic disambiguation without namespaces. C++ linker will produce an error or choose one definition, two definitions of the same symbol name cannot exist. Conflicts are resolved during linking and not at the preprocessing step. So, it is not possible to do the same in C++ without using namespaces.