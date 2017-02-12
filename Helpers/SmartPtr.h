#pragma once

template<class T>
class InnerPtr
{
    template<class T> friend class SmartPtr;
public:
    InnerPtr(T* data):
        data_(data),
        refCnt_(0)
    {

    }
    ~InnerPtr()
    {

    }
    void AddRef()
    {
        ++refCnt_;
    }
    void Release()
    {
        --refCnt_;
        if (refCnt_ == 0) {
            delete data_;
        }
    }
private:
    T* data_;
    int refCnt_;
};

template <class T>
class SmartPtr
{
public:
    SmartPtr() :
        inner_(NULL)
    {

    }
    SmartPtr(T* data):
        inner_(new InnerPtr(data))
    {

    }
    SmartPtr(SmartPtr<T>& other):
        inner_(other.inner_)
    {
        AddRef();
    }
    ~SmartPtr()
    {
        Release();
    }
    SmartPtr<T>& operator=(SmartPtr<T>& other)
    {
        inner_ = other.inner_;
        AddRef();
        return *this;
    }
    SmartPtr<T>& operator=(T* data)
    {
        inner_ = new InnerPtr<T>(data);
        AddRef();
        return *this;
    }
    void AddRef()
    {
        inner_->AddRef();
    }

    void Release()
    {
        inner_->Release();
    }
    operator T* ()
    {
        return inner_->data_;
    }
    T* operator -> ()
    {
        return inner_->data_;
    }
private:
    InnerPtr<T>* inner_;
};