#ifndef USER_FULLCLASS_HPP
#define USER_FULLCLASS_HPP

class FullClass {
public:
    FullClass(void);
    FullClass(int);
    FullClass(int, int*);
    FullClass(const FullClass&);
    void tell(void);
    void setNthElement(int, int);
    ~FullClass(void);
    
    // Note: No access to private members objectId, data, size
};

#endif