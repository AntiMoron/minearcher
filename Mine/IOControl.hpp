#ifndef IOCONTROL_HPP
#define IOCONTROL_HPP
#include"Point.hpp"
namespace MEOM
{
	class IOControl
	{
	public:
		~IOControl(){}
		static IOControl& getInstance()
		{
			static IOControl inst;
			return inst;
		}
		void setMousePosition(Point pt)
		{
			mousePosition = pt;
		}
		const Point& getMousePosition()const
		{
			return mousePosition;
		}
	private:
		IOControl(){}
		Point mousePosition;
	};
}
#define IOCONTROL (IOControl::getInstance())
#endif // IOCONTROL_HPP
