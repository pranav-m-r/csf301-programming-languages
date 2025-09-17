#ifndef FULLCLASS_HPP
#define FULLCLASS_HPP

class FullClass {
private:
    int objectId, *data, size;
    
public:
    FullClass(void);
    FullClass(int);
    FullClass(int, int*);
    FullClass(const FullClass&);
    void tell(void);
    void setNthElement(int, int);
    ~FullClass(void);
};

#endif