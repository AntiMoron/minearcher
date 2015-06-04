#ifndef IOCONTROL_HPP
#define IOCONTROL_HPP

namespace MEOM{
class IOControl
{
public:
    ~IOControl(){}
    static IOControl* getInstance()
    {
        if(io == nullptr)
            io = new IOControl;
        return io;
    }
    void setPoi(int s,int u)
    {
        x = s;
        y = u;
    }
    int getX()
    {
        return x;
    }
    int getY()
    {
        return y;
    }
private:
    IOControl(){}
    static IOControl* io;
    int x,y;
};
}
#endif // IOCONTROL_HPP
