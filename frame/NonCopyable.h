class NonCopyable
{
protected:
    /** Default Constructor*/
    NonCopyable()=default;
    ~NonCopyable()=default;
    /** Deleted Copy Constructor and Copy Operator */
    NonCopyable(const NonCopyable&)=delete;
    NonCopyable& operator = (const NonCopyable&)=delete;
};
