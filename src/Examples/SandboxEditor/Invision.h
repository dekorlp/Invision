#ifndef INVISION_H
#define INVISION_H


class Invision
{
private:

public:
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void Destroy() = 0;

	void Run();
};

#endif //INVISION_H

#ifdef _WIN32
#define INVISION_MAIN(InvisionGameClass) \
	void main() { \
		Invision *gameInstance = new InvisionGameClass(); \
		gameInstance->Run(); \
	 } \

#endif