#ifndef CONTAINER_H
#define CONTAINER_H

template <typename T>
class Container {
    T element;
public:
    // metodas nustatyti elementą
    void setElement(T elem) {
        element = elem;
    }
    // metodas gauti elementą
    T getElement() {
        return element;
    }
};

#endif // CONTAINER_H