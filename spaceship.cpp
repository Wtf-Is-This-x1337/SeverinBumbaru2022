#include <windows.h>
#include <iostream>
#include <winuser.h>
#include <stdio.h>
#include <string>
#include <chrono>
#include <stdlib.h>
#include "renderidk.cpp"
int a = 500, b = 500,k;
void* buffer_memory;
bool manu = false;
int buffer_width;
int buffer_height;
bool running = true;
int ma[1280];
int s[10000];
int m[110] = { 0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,1,1,0,1,0,1,1,0,0,0,1,1,0,0,1,0,0,1,1,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,1,0,1,0,1,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,0,0,0,0,0 };
std::string test1 = std::to_string(buffer_height);
BITMAPINFO buffer_bitmap_info;

int v[6] = {100,300,500,700,900,1100};
using namespace std::chrono;
int close()
{
	int msgboxID = MessageBox(
		NULL,
		(LPCWSTR)L"Are you sure you want to exit ?",
		(LPCWSTR)L"Spaceship Battle",
		 MB_ICONQUESTION  | MB_YESNO | MB_DEFBUTTON2 | MB_TASKMODAL 
	);

	switch (msgboxID)
	{
	case 7:
		break;
	case 6:
		running = false;
		break;
	}

	return msgboxID;
}
int sec = 0;
std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
LRESULT CALLBACK wind_callback(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;
	switch (Msg)
	{	case WM_CLOSE:
	case WM_DESTROY: {
		close();
		break;
	}
		case WM_SIZE:
		{
			RECT rect;
			//debugare();
			GetWindowRect(hwnd, &rect);
			//MoveWindow(hwnd, rect.left, rect.top, 1280, 720, TRUE);
			buffer_width = rect.right - rect.left;
			buffer_height = rect.bottom - rect.top;
			int buffer_size = buffer_width * buffer_height * sizeof(unsigned int);
			if (buffer_memory) { VirtualFree(buffer_memory, 0, MEM_RELEASE); }
			buffer_memory = VirtualAlloc(0, buffer_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
			buffer_bitmap_info.bmiHeader.biSize = sizeof(buffer_bitmap_info.bmiHeader);
			buffer_bitmap_info.bmiHeader.biWidth = buffer_width;
			buffer_bitmap_info.bmiHeader.biHeight = buffer_height;
			buffer_bitmap_info.bmiHeader.biPlanes = 1;
			buffer_bitmap_info.bmiHeader.biBitCount = 32;
			buffer_bitmap_info.bmiHeader.biCompression = BI_RGB; }
	
		default:
			result = DefWindowProc(hwnd, Msg, wParam, lParam);
	}
		return result;
}
void format()
{
	unsigned int* pixel = (unsigned int*)buffer_memory;
	for (int i = 0; i < buffer_height; i++)
	{
		for (int j = 0; j < buffer_width; j++)
			*(pixel++) = 0;
	}
	return;
}
int cl = 0;
void pp(int x, int y, int q)
{
	int k = 0;
	int s = q*q;
	unsigned int* pixel = (unsigned int*)buffer_memory;
	pixel += (x)*buffer_width+y-1;
	for (int i = 0; i <q ; i++)
	{
		for (int j = 0; j <q; j++)
				*pixel++ = 0xFFFFFF; k++;
		pixel += buffer_width - q;
	}
	return;
}
int menu()
{
	int msgboxID = MessageBox(
		NULL,
		(LPCWSTR)L"Do you want to continue ?",
		(LPCWSTR)L" ",
		MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON1 | MB_TASKMODAL
	);
	switch (msgboxID)
	{
	case IDNO:
		running = false;
		break;
	case IDYES:
		break;
	}

	return msgboxID;
}bool da, ds, dd, dw, dsp;
int speed = 1;
void sfarsit()
{
	int msgboxID = MessageBox(
		NULL,
		(LPCWSTR)L"You Won !",
		(LPCWSTR)L"Game Over",
		MB_ICONASTERISK | MB_OK | MB_TASKMODAL
	);
	return;
}
int  WinMain(	HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR  lpCmdLine, int nShowCmd) 
{
	//create windows  class
	WNDCLASS window_class = {};
	COLORREF NewColor = RGB(255, 0, 0);
	int a1=1, a2 = 1, a3 = 1, a4 = 1, a5 = 1, a6 = 1;
	window_class.lpszClassName = L"Game Windows Class";
	window_class.lpfnWndProc = wind_callback;
	//register winows class
	RegisterClass(&window_class);
	//create windows
	HWND window =CreateWindowA("Game Windows Class", "Spaceship Battle", WS_TILEDWINDOW ^ WS_SIZEBOX^ WS_MAXIMIZEBOX | WS_VISIBLE,CW_USEDEFAULT, CW_USEDEFAULT,1280,720,0,0,hInstance, 0);
	HDC hdc = GetDC(window);
	ShowWindow( NULL, SW_SHOWNORMAL);
	int r = 7;
	int sf = 1;
	while (running)
	{
		sf = 1;
		
		for (int i = 0; i < 6; i++)if (v[i])sf = 0;
		if (sf)
		{
			sfarsit(); break;;
		}
		ds = da = dw = dd = dsp = 0;
			MSG message;
			while (PeekMessage(&message, window, 0, 0, PM_REMOVE))
			{
				switch (message.message)
				{
				case WM_KEYDOWN:
				{
					unsigned int vk_code = (unsigned int)message.wParam;
					bool is_down = ((message.lParam & (1 << 31)) == 0);
					switch (vk_code) {
					case VK_ESCAPE: {
						manu = true;
						menu();

					}break;
					}

					break;
				}
				default:
				{TranslateMessage(&message);
				DispatchMessage(&message); }
				}
			}
			k = 110;
			format();
			if (GetKeyState(0x41) & 0x8000)
				da = 1;
			if (GetKeyState(0x53) & 0x8000)
				ds = 1;
			if (GetKeyState(0x44) & 0x8000)
				dd = 1;
			if (GetKeyState(0x57) & 0x8000)
				dw = 1;
				if(!cl)
				{
						if (!ma[b + 6 * r + 1])
						{
							ma[b + 6 * r + 1] = a - r;
					
						}
				}
			if (da)
				if (b > 40)
					b -= speed;
			if (dw)
				if (a > buffer_height / 2)
					a -= speed;
			if (dd)
				if (b < buffer_width - 150)
					b += speed;
			if (ds)
				if (a < buffer_height - 120)
					a += speed;
			for (int i = 0; i < 6; i++)if (v[i])pp(600, v[i], r * 4);
			for (int i = a + 10 * r - 2; i >= a; i -= r)
			{
				for (int j = b + 1; j < b + 11 * r + 1; j += r)

					if (m[k--] == 1)
					{
						pp(buffer_height - i, j, r + 1);
					}
			}
			for (int i = 0; i < 1280; i++)
			{
				if (ma[i])
				{
					pp(buffer_height - ma[i], i, r); ma[i] -= 2 * r;
					if (i > 85 && i < 130 && ma[i] < 50)
					{
						if (a1)
						{
							v[0] = 0; format(); a1 = 0;
						}
					}
					if (i > 285 && i < 330 && ma[i] < 50)
					{
						if (a2)
						{
							v[1] = 0; format(); a2 = 0;
						}

					}
					if (i > 485 && i < 530 && ma[i] < 50)
					{
						if (a3)
						{
							v[2] = 0; format(); a3 = 0;
						}
					}
					if (i > 685 && i < 730 && ma[i] < 50)
					{
						if (a4)
						{
							v[3] = 0; format(); a4 = 0;
						}
					}
					if (i > 885 && i < 930 && ma[i] < 50)
					{
						if (a5)
						{
							v[4] = 0; format(); a5 = 0;
						}
					}
					if (i > 1085 && i < 1130 && ma[i] < 50)
					{
						if (a6)
						{
							v[5] = 0; format(); a6 = 0;
						}
					}
				}
				if (ma[i] < 50)
					ma[i] = 0;
			}
			StretchDIBits(hdc, 0, 0, buffer_width, buffer_height, 0, 0, buffer_width, buffer_height, buffer_memory, &buffer_bitmap_info, DIB_RGB_COLORS, SRCCOPY);
			cl = 1;
			std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
			int duration = std::chrono::duration_cast<std::chrono::seconds>(end - begin).count();
			if (duration % 2 == 0 && s[duration]==0)
			{
				cl = 0; s[duration] = 1;
			}

			
	}
}