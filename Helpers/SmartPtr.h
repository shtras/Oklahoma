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
        inner_(nullptr)
    {

    }
    SmartPtr(T* data):
        inner_(new InnerPtr<T>(data))
    {
        AddRef();
    }
    SmartPtr(const SmartPtr<T>& other):
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
        if (inner_) {
            inner_->Release();
        }
    }
    operator T* ()
    {
        if (!inner_) {
            return nullptr;
        }
        return inner_->data_;
    }
    T* operator -> ()
    {
        return inner_->data_;
    }

    bool operator == (const SmartPtr<T>& other) const
    {
        return inner_ == other.inner_;
    }

    bool operator == (T* data) const
    {
        return inner_->data_ == data;
    }
private:
    InnerPtr<T>* inner_;
};
