#ifdef _WINDOWS
    #include <windows.h>
#else
    #include <unistd.h>
#define Sleep(x) usleep((x)*1000)
#endif

int main()
{
	Sleep(5000); // - I would like this to work on Linux and Windows

	return 0;
}