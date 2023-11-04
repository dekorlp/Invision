#ifndef I_WINDOW_H
#define I_WINDOW_H

class IWindow 
{
protected:
	virtual bool isWindowActive() = 0;
};

#endif // I_WINDOW_H