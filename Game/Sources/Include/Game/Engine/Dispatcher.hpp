#include <Game/Engine/DeviceHolder.hpp>
#include <Game/Looper/Looper.hpp>
#include <Game/Utils/WindowRect.hpp>

class Dispatcher
{

public:

	static void Initialize ();

	Dispatcher ();

	void Tick ();

	void Destroy ();

	void SetWindow (GAME_NATIVE_WINDOW_T window, WindowSize size, WindowRotation rotation);

	void Size (WindowSize size, WindowRotation rotation);

	void Suspend ();

	void Resume ();

	void ValidateDevice ();

	~Dispatcher ();

private:

	static double s_TimerFreq;
	bool m_bLastTimeValid { false };
	LARGE_INTEGER m_LastTime;
	DeviceHolder m_DeviceHolder;
	Looper m_Looper { m_DeviceHolder };

	void Render ();

	void Update ();

};