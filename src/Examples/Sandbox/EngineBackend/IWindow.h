#ifndef I_WINDOW_H
#define I_WINDOW_H

enum EWindowStatus;

class IWindow 
{
protected:
	virtual EWindowStatus getWindowStatus() = 0;
	virtual void setWindowTitle(const char* title) = 0;
};

#endif // I_WINDOW_H