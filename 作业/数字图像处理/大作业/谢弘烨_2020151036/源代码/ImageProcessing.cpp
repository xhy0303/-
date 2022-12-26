// ImageProcessing.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"
#include <direct.h>
#include <commdlg.h>
#include<cmath>
#include<random>
#include<algorithm>
#include<cstring>

using namespace std;

#define MAX_LOADSTRING 100
#define HISTOGRAMHIEGHT	150


BOOL ReadImage(LPSTR, char *, int, int); //读取图像信息并保存在Image[][]中
void ShowImage(char *, int, int, int, int);
BOOL ReadBmpImage(LPSTR, char *);
void ShowBmpImage(char *, int, int, int, int);
void OpenImageFileDlg(char *);

	void ShowGrayToLow(char* Image, int wImage, int hImage, int xPos, int yPos);//淡出
	void ShowGrayToHigh(char* Image, int wImage, int hImage, int xPos, int yPos);//淡出
	void ShowLeftRightOpen(char* Image, int wImage, int hImage, int xPos, int yPos);//左右开门
	void ShowLeftRightClose(char* Image, int wImage, int hImage, int xPos, int yPos);
	void ShowTopBottomOpen(char* Image, int wImage, int hImage, int xPos, int yPos);
	void ShowTopBottomClose(char* Image, int wImage, int hImage, int xPos, int yPos);

	void ShowTopBottomShutter(char* Image, int wImage, int hImage, int xPos, int yPos);//向下百叶窗
	void ShowBottomTopShutter(char* Image, int wImage, int hImage, int xPos, int yPos);
	void ShowLeftRightShutter(char* Image, int wImage, int hImage, int xPos, int yPos);
	void ShowRightLeftShutter(char* Image, int wImage, int hImage, int xPos, int yPos);

	void ShowImageRect(char* Image, int wImage, int hImage, int xPos, int yPos, RECT ShowRect);//

	void ImageHistogramShowProcessing(char* oImage, char* nImage, int wImage, int hImage);//显示直方图
	void ImageGrayReversionProcessing(char* Image, int wImage, int hImage);//灰度反转
	void ThresholdImage(char* oImage, int wImage, int hImage);//二值化

	void ImageHistogramEvenProcess(char* oImage, int wImage, int hImage);//直方图均衡化

	void ImageChangeByLog(char* oImage, int wImage, int hImage);//对数变换
	void ImageChangeByPow(char* oImage, int wImage, int hImage, double k);//幂次变换
	void ImageChangeBySeptFunc(char* oImage, int wImage, int hImage, float r1, float s1, float r2, float s2);//分段函数

	void ImageChangeByBits(char* oImage, char* nImage, int wImage, int hImage, int k);//位图分割

	void ImageMaskProcessing(char* oImage, char* nImage, int wImage, int hImage, int* Mask, int MaskWH, int MaskCoff);//图像处理函数 3*3
	void ImageMaskProcessing(char* oImage, char* nImage, int wImage, int hImage, int MaskWH, int sign);
	void ImageMaskProcessing(char* oImage, char* nImage, int wImage, int hImage, int* Mask1, int* Mask2, int MaskWH, int MaskCoff, int sign);
	void AverageMaskProcessing(char* oImage, char* nImage, int wImage, int hImage);//均值滤波
	void GuassAverageMaskProcessing(char* oImage, char* nImage, int wImage, int hImage);//高斯滤波
	void MaxFilterProcessing(char* oImage, char* nImage, int wImage, int hImage);//极大值滤波
	void MinFilterProcessing(char* oImage, char* nImage, int wImage, int hImage);
	void MidFilterProcessing(char* oImage, char* nImage, int wImage, int hImage);

	void SALTNOISEGENERATOR(char* oImage, int wImage, int hImage, double rate);
	void PEPPERNOISEGENERATOR(char* oImage, int wImage, int hImage, double rate);

	void LaplaceEdgeProcessing(char* oImage, char* nImage, int wImage, int hImage);
	void PrewittEdgeProcessing(char* oImage, char* nImage, int wImage, int hImage, int sign);
	void SobelEdgeProcessing(char* oImage, char* nImage, int wImage, int hImage, int sign);
	void RobertsEdgeProcessing(char* oImage, char* nImage, int wImage, int hImage);

	void SimpleGeometricTranslation(char* oImage, char* nImage, int wImage, int hImage, double* sgt_Array);
	void ImageTranslation(char* oImage, char* nImage, int wImage, int hImage, int xPos, int yPos);
	void ImageScaling(char* oImage, char* nImage, int wImage, int hImage, double xScaling, double yScaling);
	void ImageRotation(char* oImage, char* nImage, int wImage, int hImage, double iAngle);

	void ShowBMPToLow(char* Image, int wImage, int hImage, int xPos, int yPos);
	void ShowBMPToHigh(char* Image, int wImage, int hImage, int xPos, int yPos);

	void ShowBMPImageRect(char* Image, int wImage, int hImage, int xPos, int yPos, RECT ShowRect);//
	void ShowBMPImageRect1(char* Image, int wImage, int hImage, int xPos, int yPos, RECT ShowRect);//
	
	void DivideBMP(char* rImage, char* gImage, char* bImage, char* nImage, int wImage, int hImage);
	void UnionBMP(char* rImage, char* gImage, char* bImage, char* nImage, int wImage, int hImage);

	void Change2Grey(char* oImage, char* nImage, int wImage, int hImage);



HDC  hWinDC;
int ImageWidth = 256;
int ImageHeight = 256;
char ImgDlgFileName[MAX_PATH];
char ImgDlgFileDir[MAX_PATH];
char OrgImage[1024*1024];
	#define IMAGEWIDTH			ImageWidth
	#define IMAGEHEIGHT			ImageHeight
	#define XPOS				100
	#define YPOS				100
	#define GRAYCHANGENUM	    20
	#define SHUTTERNUM			5

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];								// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];								// The title bar text

// Foward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	Input(HWND, UINT, WPARAM, LPARAM);

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	// MSG.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_IMAGEPROCESSING, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_IMAGEPROCESSING);

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage is only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_IMAGEPROCESSING);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCSTR)IDC_IMAGEPROCESSING;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//

char  NewImage[1024 * 1024];	//新的图像缓存
char* rImage, * gImage, * bImage;
char* NewrImage, * NewgImage, * NewbImage;

HWND hWind;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR szHello[MAX_LOADSTRING];
	switch (message) 
	{
		case WM_CREATE:
			hWinDC = GetWindowDC(hWnd);
			hWind = hWnd;
			break;

		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 
			
			// Parse the menu selections:
			switch (wmId)
			{
				case IDM_TEST:
					Change2Grey(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT);
					ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 50 + IMAGEWIDTH);
					break;

					
				case IDM_TRANSLATION:					//图像平移
					if (IMAGEHEIGHT == IMAGEWIDTH)
					{
						ImageTranslation(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT, 20, 30);
						ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);     	 
						break;
					}
					else
					{
						rImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						gImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						bImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewrImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewgImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewbImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						DivideBMP(rImage, gImage, bImage, OrgImage, IMAGEWIDTH, IMAGEHEIGHT);
						ImageTranslation(rImage, NewrImage, IMAGEWIDTH, IMAGEHEIGHT, 20, -30);
						ImageTranslation(gImage, NewgImage, IMAGEWIDTH, IMAGEHEIGHT, 20, -30);
						ImageTranslation(bImage, NewbImage, IMAGEWIDTH, IMAGEHEIGHT, 20, -30);
						UnionBMP(NewrImage, NewgImage, NewbImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT);
						ShowBmpImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + IMAGEWIDTH + 300); break;
					}
				case IDM_TRANSLATION1:					//图像平移
					if (IMAGEHEIGHT == IMAGEWIDTH)
					{
						ImageTranslation(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT, -20, -30);
						ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);
						break;
					}
					else
					{
						rImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						gImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						bImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewrImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewgImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewbImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						DivideBMP(rImage, gImage, bImage, OrgImage, IMAGEWIDTH, IMAGEHEIGHT);
						ImageTranslation(rImage, NewrImage, IMAGEWIDTH, IMAGEHEIGHT, -20, 30);
						ImageTranslation(gImage, NewgImage, IMAGEWIDTH, IMAGEHEIGHT, -20, 30);
						ImageTranslation(bImage, NewbImage, IMAGEWIDTH, IMAGEHEIGHT, -20, 30);
						UnionBMP(NewrImage, NewgImage, NewbImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT);
						ShowBmpImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + IMAGEWIDTH + 300); break;
					}

				case IDM_SCALING:					//图像缩放
					if (IMAGEHEIGHT == IMAGEWIDTH)
					{
						ImageScaling(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT, 2.0, 0.5);
						ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);	 break;
					}
					else
					{
						rImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						gImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						bImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewrImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewgImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewbImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						DivideBMP(rImage, gImage, bImage, OrgImage, IMAGEWIDTH, IMAGEHEIGHT);
						ImageScaling(rImage, NewrImage, IMAGEWIDTH, IMAGEHEIGHT, 2.0, 0.5);
						ImageScaling(gImage, NewgImage, IMAGEWIDTH, IMAGEHEIGHT, 2.0, 0.5);
						ImageScaling(bImage, NewbImage, IMAGEWIDTH, IMAGEHEIGHT, 2.0, 0.5);
						UnionBMP(NewrImage, NewgImage, NewbImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT);
						ShowBmpImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + IMAGEWIDTH + 300); break;
					}
				case IDM_SCALING1:					//图像缩放
					if (IMAGEHEIGHT == IMAGEWIDTH)
					{
						ImageScaling(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT, 0.5, 2.0);
						ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);	 break;
					}
					else
					{
						rImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						gImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						bImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewrImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewgImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewbImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						DivideBMP(rImage, gImage, bImage, OrgImage, IMAGEWIDTH, IMAGEHEIGHT);
						ImageScaling(rImage, NewrImage, IMAGEWIDTH, IMAGEHEIGHT, 0.5, 2.0);
						ImageScaling(gImage, NewgImage, IMAGEWIDTH, IMAGEHEIGHT, 0.5, 2.0);
						ImageScaling(bImage, NewbImage, IMAGEWIDTH, IMAGEHEIGHT, 0.5, 2.0);
						UnionBMP(NewrImage, NewgImage, NewbImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT);
						ShowBmpImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + IMAGEWIDTH + 300); break;
					}
				case IDM_SCALING2:					//图像缩放
					if (IMAGEHEIGHT == IMAGEWIDTH)
					{
						ImageScaling(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT, 2.0, 1.0);
						ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);	 break;
					}
					else
					{
						rImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						gImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						bImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewrImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewgImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewbImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						DivideBMP(rImage, gImage, bImage, OrgImage, IMAGEWIDTH, IMAGEHEIGHT);
						ImageScaling(rImage, NewrImage, IMAGEWIDTH, IMAGEHEIGHT, 2.0, 1.0);
						ImageScaling(gImage, NewgImage, IMAGEWIDTH, IMAGEHEIGHT, 2.0, 1.0);
						ImageScaling(bImage, NewbImage, IMAGEWIDTH, IMAGEHEIGHT, 2.0, 1.0);
						UnionBMP(NewrImage, NewgImage, NewbImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT);
						ShowBmpImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + IMAGEWIDTH + 300); break;
					}
				case IDM_SCALING3:					//图像缩放
					if (IMAGEHEIGHT == IMAGEWIDTH)
					{
						ImageScaling(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT, 0.5, 1);
						ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);	 break;
					}
					else
					{
						rImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						gImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						bImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewrImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewgImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewbImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						DivideBMP(rImage, gImage, bImage, OrgImage, IMAGEWIDTH, IMAGEHEIGHT);
						ImageScaling(rImage, NewrImage, IMAGEWIDTH, IMAGEHEIGHT, 0.5, 1);
						ImageScaling(gImage, NewgImage, IMAGEWIDTH, IMAGEHEIGHT, 0.5, 1);
						ImageScaling(bImage, NewbImage, IMAGEWIDTH, IMAGEHEIGHT, 0.5, 1);
						UnionBMP(NewrImage, NewgImage, NewbImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT);
						ShowBmpImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + IMAGEWIDTH + 300); break;
					}
				case IDM_SCALING4:					//图像缩放
					if (IMAGEHEIGHT == IMAGEWIDTH)
					{
						ImageScaling(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT, 1, 2.0);
						ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);	 break;
					}
					else
					{
						rImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						gImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						bImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewrImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewgImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewbImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						DivideBMP(rImage, gImage, bImage, OrgImage, IMAGEWIDTH, IMAGEHEIGHT);
						ImageScaling(rImage, NewrImage, IMAGEWIDTH, IMAGEHEIGHT, 1, 2.0);
						ImageScaling(gImage, NewgImage, IMAGEWIDTH, IMAGEHEIGHT, 1, 2.0);
						ImageScaling(bImage, NewbImage, IMAGEWIDTH, IMAGEHEIGHT, 1, 2.0);
						UnionBMP(NewrImage, NewgImage, NewbImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT);
						ShowBmpImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + IMAGEWIDTH + 300); break;
					}
				case IDM_SCALING5:					//图像缩放
					if (IMAGEHEIGHT == IMAGEWIDTH)
					{
						ImageScaling(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT, 1, 0.5);
						ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);	 break;
					}
					else
					{
						rImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						gImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						bImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewrImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewgImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewbImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						DivideBMP(rImage, gImage, bImage, OrgImage, IMAGEWIDTH, IMAGEHEIGHT);
						ImageScaling(rImage, NewrImage, IMAGEWIDTH, IMAGEHEIGHT, 1, 0.5);
						ImageScaling(gImage, NewgImage, IMAGEWIDTH, IMAGEHEIGHT, 1, 0.5);
						ImageScaling(bImage, NewbImage, IMAGEWIDTH, IMAGEHEIGHT, 1, 0.5);
						UnionBMP(NewrImage, NewgImage, NewbImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT);
						ShowBmpImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + IMAGEWIDTH + 300); break;
					}
				case IDM_SCALING6:					//图像缩放
					if (IMAGEHEIGHT == IMAGEWIDTH)
					{
						ImageScaling(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT, 2.0, 2.0);
						ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);	 break;
					}
					else
					{
						rImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						gImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						bImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewrImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewgImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewbImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						DivideBMP(rImage, gImage, bImage, OrgImage, IMAGEWIDTH, IMAGEHEIGHT);
						ImageScaling(rImage, NewrImage, IMAGEWIDTH, IMAGEHEIGHT, 2.0, 2.0);
						ImageScaling(gImage, NewgImage, IMAGEWIDTH, IMAGEHEIGHT, 2.0, 2.0);
						ImageScaling(bImage, NewbImage, IMAGEWIDTH, IMAGEHEIGHT, 2.0, 2.0);
						UnionBMP(NewrImage, NewgImage, NewbImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT);
						ShowBmpImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + IMAGEWIDTH + 300); break;
					}
				case IDM_SCALING7:					//图像缩放
					if (IMAGEHEIGHT == IMAGEWIDTH)
					{
						ImageScaling(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT, 0.5, 0.5);
						ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);	 break;
					}
					else
					{
						rImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						gImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						bImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewrImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewgImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewbImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						DivideBMP(rImage, gImage, bImage, OrgImage, IMAGEWIDTH, IMAGEHEIGHT);
						ImageScaling(rImage, NewrImage, IMAGEWIDTH, IMAGEHEIGHT, 0.5, 0.5);
						ImageScaling(gImage, NewgImage, IMAGEWIDTH, IMAGEHEIGHT, 0.5, 0.5);
						ImageScaling(bImage, NewbImage, IMAGEWIDTH, IMAGEHEIGHT, 0.5, 0.5);
						UnionBMP(NewrImage, NewgImage, NewbImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT);
						ShowBmpImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + IMAGEWIDTH + 300); break;
					}

				case IDM_ROTATION:					//图像旋转
					if (IMAGEWIDTH == IMAGEHEIGHT)
					{
						ImageRotation(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT, 45.0);
						ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);	 break;
					}
					else
					{
						rImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						gImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						bImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewrImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewgImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewbImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						DivideBMP(rImage, gImage, bImage, OrgImage, IMAGEWIDTH, IMAGEHEIGHT);
						ImageRotation(rImage, NewrImage, IMAGEWIDTH, IMAGEHEIGHT, -45.0);
						ImageRotation(gImage, NewgImage, IMAGEWIDTH, IMAGEHEIGHT, -45.0);
						ImageRotation(bImage, NewbImage, IMAGEWIDTH, IMAGEHEIGHT, -45.0);
						UnionBMP(NewrImage, NewgImage, NewbImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT);
						ShowBmpImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + IMAGEWIDTH + 300); break;
					}
				case IDM_ROTATION1:					//图像旋转
					if (IMAGEWIDTH == IMAGEHEIGHT)
					{
						ImageRotation(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT, -45.0);
						ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);	 break;
					}
					else
					{
						rImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						gImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						bImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewrImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewgImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewbImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						DivideBMP(rImage, gImage, bImage, OrgImage, IMAGEWIDTH, IMAGEHEIGHT);
						ImageRotation(rImage, NewrImage, IMAGEWIDTH, IMAGEHEIGHT, 45.0);
						ImageRotation(gImage, NewgImage, IMAGEWIDTH, IMAGEHEIGHT, 45.0);
						ImageRotation(bImage, NewbImage, IMAGEWIDTH, IMAGEHEIGHT, 45.0);
						UnionBMP(NewrImage, NewgImage, NewbImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT);
						ShowBmpImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + IMAGEWIDTH + 300); break;
					}

				case IDM_LAPLACE:					      //Laplace边缘检测滤波
					if (IMAGEHEIGHT == IMAGEWIDTH)
					{
						LaplaceEdgeProcessing(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT);
						ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);  break;
					}
					else
					{
						rImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						gImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						bImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewrImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewgImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewbImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						DivideBMP(rImage, gImage, bImage, OrgImage, IMAGEWIDTH, IMAGEHEIGHT);
						LaplaceEdgeProcessing(rImage, NewrImage, IMAGEWIDTH, IMAGEHEIGHT);
						LaplaceEdgeProcessing(gImage, NewgImage, IMAGEWIDTH, IMAGEHEIGHT);
						LaplaceEdgeProcessing(bImage, NewbImage, IMAGEWIDTH, IMAGEHEIGHT);
						UnionBMP(NewrImage, NewgImage, NewbImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT);
						ShowBmpImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + IMAGEWIDTH + 300); break;
					}

				case IDM_PREWITT1:
					if (IMAGEHEIGHT == IMAGEWIDTH)
					{
						PrewittEdgeProcessing(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT, 1);
						ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);  break;
					}
					else
					{
						rImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						gImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						bImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewrImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewgImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewbImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						DivideBMP(rImage, gImage, bImage, OrgImage, IMAGEWIDTH, IMAGEHEIGHT);
						PrewittEdgeProcessing(rImage, NewrImage, IMAGEWIDTH, IMAGEHEIGHT, 1);
						PrewittEdgeProcessing(gImage, NewgImage, IMAGEWIDTH, IMAGEHEIGHT, 1);
						PrewittEdgeProcessing(bImage, NewbImage, IMAGEWIDTH, IMAGEHEIGHT, 1);
						UnionBMP(NewrImage, NewgImage, NewbImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT);
						ShowBmpImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + IMAGEWIDTH + 300); break;
					}
				case IDM_PREWITT0:
					if (IMAGEHEIGHT == IMAGEWIDTH)
					{
						PrewittEdgeProcessing(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT, 0);
						ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);  break;
					}
					else
					{
						rImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						gImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						bImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewrImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewgImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewbImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						DivideBMP(rImage, gImage, bImage, OrgImage, IMAGEWIDTH, IMAGEHEIGHT);
						PrewittEdgeProcessing(rImage, NewrImage, IMAGEWIDTH, IMAGEHEIGHT, 0);
						PrewittEdgeProcessing(gImage, NewgImage, IMAGEWIDTH, IMAGEHEIGHT, 0);
						PrewittEdgeProcessing(bImage, NewbImage, IMAGEWIDTH, IMAGEHEIGHT, 0);
						UnionBMP(NewrImage, NewgImage, NewbImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT);
						ShowBmpImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + IMAGEWIDTH + 300); break;
					}

				case IDM_SOBEL1:
					if (IMAGEHEIGHT == IMAGEWIDTH)
					{
						SobelEdgeProcessing(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT, 1);
						ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);  break;
					}
					else
					{
						rImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						gImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						bImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewrImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewgImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewbImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						DivideBMP(rImage, gImage, bImage, OrgImage, IMAGEWIDTH, IMAGEHEIGHT);
						SobelEdgeProcessing(rImage, NewrImage, IMAGEWIDTH, IMAGEHEIGHT, 1);
						SobelEdgeProcessing(gImage, NewgImage, IMAGEWIDTH, IMAGEHEIGHT, 1);
						SobelEdgeProcessing(bImage, NewbImage, IMAGEWIDTH, IMAGEHEIGHT, 1);
						UnionBMP(NewrImage, NewgImage, NewbImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT);
						ShowBmpImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + IMAGEWIDTH + 300); break;
					}
				case IDM_SOBEL0:
					if (IMAGEHEIGHT == IMAGEWIDTH)
					{
						SobelEdgeProcessing(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT, 0);
						ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);  break;
					}
					else
					{
						rImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						gImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						bImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewrImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewgImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewbImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						DivideBMP(rImage, gImage, bImage, OrgImage, IMAGEWIDTH, IMAGEHEIGHT);
						SobelEdgeProcessing(rImage, NewrImage, IMAGEWIDTH, IMAGEHEIGHT, 0);
						SobelEdgeProcessing(gImage, NewgImage, IMAGEWIDTH, IMAGEHEIGHT, 0);
						SobelEdgeProcessing(bImage, NewbImage, IMAGEWIDTH, IMAGEHEIGHT, 0);
						UnionBMP(NewrImage, NewgImage, NewbImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT);
						ShowBmpImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + IMAGEWIDTH + 300); break;
					}

				case IDM_ROBERTS:
					if (IMAGEHEIGHT == IMAGEWIDTH)
					{
						RobertsEdgeProcessing(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT);
						ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);  break;
					}
					else
					{
						rImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						gImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						bImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewrImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewgImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewbImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						DivideBMP(rImage, gImage, bImage, OrgImage, IMAGEWIDTH, IMAGEHEIGHT);
						RobertsEdgeProcessing(rImage, NewrImage, IMAGEWIDTH, IMAGEHEIGHT);
						RobertsEdgeProcessing(gImage, NewgImage, IMAGEWIDTH, IMAGEHEIGHT);
						RobertsEdgeProcessing(bImage, NewbImage, IMAGEWIDTH, IMAGEHEIGHT);
						UnionBMP(NewrImage, NewgImage, NewbImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT);
						ShowBmpImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + IMAGEWIDTH + 300); break;
					}


				case IDM_SALTNOISE1:
					SALTNOISEGENERATOR(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, 0.1);
					if (IMAGEHEIGHT == IMAGEWIDTH)
					{
						ShowImage(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS); break;
					}
					else
					{
						ShowBmpImage(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS); break;
					}
				case IDM_SALTNOISE3:
					SALTNOISEGENERATOR(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, 0.075);
					if (IMAGEHEIGHT == IMAGEWIDTH)
					{
						ShowImage(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS); break;
					}
					else
					{
						ShowBmpImage(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS); break;
					}
				case IDM_SALTNOISE5:
					SALTNOISEGENERATOR(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, 0.05);
					if (IMAGEHEIGHT == IMAGEWIDTH)
					{
						ShowImage(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS); break;
					}
					else
					{
						ShowBmpImage(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS); break;
					}
				case IDM_SALTNOISE7:
					SALTNOISEGENERATOR(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, 0.025);
					if (IMAGEHEIGHT == IMAGEWIDTH)
					{
						ShowImage(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS); break;
					}
					else
					{
						ShowBmpImage(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS); break;
					}
				case IDM_SALTNOISE9:
					SALTNOISEGENERATOR(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, 0.01);
					if (IMAGEHEIGHT == IMAGEWIDTH)
					{
						ShowImage(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS); break;
					}
					else
					{
						ShowBmpImage(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS); break;
					}

				case IDM_PEPPERNOISE1:
					PEPPERNOISEGENERATOR(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, 0.1);
					if (IMAGEHEIGHT == IMAGEWIDTH)
					{
						ShowImage(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS); break;
					}
					else
					{
						ShowBmpImage(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS); break;
					}
					
				case IDM_PEPPERNOISE3:
					PEPPERNOISEGENERATOR(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, 0.075);
					if (IMAGEHEIGHT == IMAGEWIDTH)
					{
						ShowImage(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS); break;
					}
					else
					{
						ShowBmpImage(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS); break;
					}
					
				case IDM_PEPPERNOISE5:
					PEPPERNOISEGENERATOR(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, 0.05);
					if (IMAGEHEIGHT == IMAGEWIDTH)
					{
						ShowImage(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS); break;
					}
					else
					{
						ShowBmpImage(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS); break;
					}
				case IDM_PEPPERNOISE7:
					PEPPERNOISEGENERATOR(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, 0.025);
					if (IMAGEHEIGHT == IMAGEWIDTH)
					{
						ShowImage(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS); break;
					}
					else
					{
						ShowBmpImage(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS); break;
					}
				case IDM_PEPPERNOISE9:
					PEPPERNOISEGENERATOR(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, 0.01);
					if (IMAGEHEIGHT == IMAGEWIDTH)
					{
						ShowImage(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS); break;
					}
					else
					{
						ShowBmpImage(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS); break;
					}

				case IDM_AVERAGEFILTER:				//平均滤波器
					if (IMAGEHEIGHT == IMAGEWIDTH)
					{
						AverageMaskProcessing(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT);
						ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300); break;
					}
					else
					{
						rImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						gImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						bImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewrImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewgImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewbImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						DivideBMP(rImage, gImage, bImage, OrgImage, IMAGEWIDTH, IMAGEHEIGHT);
						AverageMaskProcessing(rImage, NewrImage, IMAGEWIDTH, IMAGEHEIGHT);
						AverageMaskProcessing(gImage, NewgImage, IMAGEWIDTH, IMAGEHEIGHT);
						AverageMaskProcessing(bImage, NewbImage, IMAGEWIDTH, IMAGEHEIGHT);
						UnionBMP(NewrImage, NewgImage, NewbImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT);
						ShowBmpImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + IMAGEWIDTH + 300); break;
					}
					
				case IDM_AVERAGEGAUSS:				//高斯滤波器
					if (IMAGEHEIGHT == IMAGEWIDTH)
					{
						GuassAverageMaskProcessing(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT);
						ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300); break;
					}
					else
					{
						rImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						gImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						bImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewrImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewgImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewbImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						DivideBMP(rImage, gImage, bImage, OrgImage, IMAGEWIDTH, IMAGEHEIGHT);
						GuassAverageMaskProcessing(rImage, NewrImage, IMAGEWIDTH, IMAGEHEIGHT);
						GuassAverageMaskProcessing(gImage, NewgImage, IMAGEWIDTH, IMAGEHEIGHT);
						GuassAverageMaskProcessing(bImage, NewbImage, IMAGEWIDTH, IMAGEHEIGHT);
						UnionBMP(NewrImage, NewgImage, NewbImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT);
						ShowBmpImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + IMAGEWIDTH + 300); break;
					}
				case IDM_MAXFILTER:				//最大值滤波
					if (IMAGEHEIGHT == IMAGEWIDTH)
					{
						MaxFilterProcessing(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT);
						ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300); break;
					}
					else
					{
						rImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						gImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						bImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewrImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewgImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewbImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						DivideBMP(rImage, gImage, bImage, OrgImage, IMAGEWIDTH, IMAGEHEIGHT);
						MaxFilterProcessing(rImage, NewrImage, IMAGEWIDTH, IMAGEHEIGHT);
						MaxFilterProcessing(gImage, NewgImage, IMAGEWIDTH, IMAGEHEIGHT);
						MaxFilterProcessing(bImage, NewbImage, IMAGEWIDTH, IMAGEHEIGHT);
						UnionBMP(NewrImage, NewgImage, NewbImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT);
						ShowBmpImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + IMAGEWIDTH + 300); break;
					}
				case IDM_MINFILTER:				
					if (IMAGEHEIGHT == IMAGEWIDTH)
					{
						MinFilterProcessing(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT);
						ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300); break;
					}
					else
					{
						rImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						gImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						bImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewrImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewgImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewbImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						DivideBMP(rImage, gImage, bImage, OrgImage, IMAGEWIDTH, IMAGEHEIGHT);
						MinFilterProcessing(rImage, NewrImage, IMAGEWIDTH, IMAGEHEIGHT);
						MinFilterProcessing(gImage, NewgImage, IMAGEWIDTH, IMAGEHEIGHT);
						MinFilterProcessing(bImage, NewbImage, IMAGEWIDTH, IMAGEHEIGHT);
						UnionBMP(NewrImage, NewgImage, NewbImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT);
						ShowBmpImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + IMAGEWIDTH + 300); break;
					}
				case IDM_MIDFILTER:
					if (IMAGEHEIGHT == IMAGEWIDTH)
					{
						MidFilterProcessing(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT);
						ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300); break;
					}
					else
					{
						rImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						gImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						bImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewrImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewgImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewbImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						DivideBMP(rImage, gImage, bImage, OrgImage, IMAGEWIDTH, IMAGEHEIGHT);
						MidFilterProcessing(rImage, NewrImage, IMAGEWIDTH, IMAGEHEIGHT);
						MidFilterProcessing(gImage, NewgImage, IMAGEWIDTH, IMAGEHEIGHT);
						MidFilterProcessing(bImage, NewbImage, IMAGEWIDTH, IMAGEHEIGHT);
						UnionBMP(NewrImage, NewgImage, NewbImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT);
						ShowBmpImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + IMAGEWIDTH + 300); break;
					}


				case IDM_HISTOGRAMSHOW://直方图
					if (IMAGEHEIGHT == IMAGEWIDTH)
					{
						ImageHistogramShowProcessing(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT);
						ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS + 300, YPOS);   break;
					}
					else
					{
						rImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						gImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						bImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewrImage = new char[256 * 256];
						NewgImage = new char[256 * 256];
						NewbImage = new char[256 * 256];
						DivideBMP(rImage, gImage, bImage, OrgImage, IMAGEWIDTH, IMAGEHEIGHT);
						ImageHistogramShowProcessing(rImage, NewrImage, IMAGEHEIGHT, IMAGEWIDTH);
						ShowImage(NewrImage, 255, 255, XPOS + IMAGEHEIGHT + 50, YPOS);
						ImageHistogramShowProcessing(gImage, NewgImage, IMAGEHEIGHT, IMAGEWIDTH);
						ShowImage(NewgImage, 255, 255, XPOS + IMAGEHEIGHT + 50, YPOS + 300);
						ImageHistogramShowProcessing(bImage, NewbImage, IMAGEHEIGHT, IMAGEWIDTH);
						ShowImage(NewbImage, 255, 255, XPOS + IMAGEHEIGHT + 50, YPOS + 300 * 2);
						break;
					}
					
				case IDM_THRESHOLD://二值化
					if (IMAGEHEIGHT == IMAGEWIDTH)
					{
						ThresholdImage(OrgImage, IMAGEWIDTH, IMAGEHEIGHT);
						ShowImage(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);    break;
					}
					else
					{
						rImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						gImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						bImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewrImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewgImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewbImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						DivideBMP(rImage, gImage, bImage, OrgImage, IMAGEWIDTH, IMAGEHEIGHT);
						ThresholdImage(rImage, IMAGEWIDTH, IMAGEHEIGHT);
						ThresholdImage(gImage, IMAGEWIDTH, IMAGEHEIGHT);
						ThresholdImage(bImage, IMAGEWIDTH, IMAGEHEIGHT);
						UnionBMP(rImage, gImage, bImage, OrgImage, IMAGEWIDTH, IMAGEHEIGHT);
						ShowBmpImage(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + IMAGEWIDTH + 300); break;
					}
					
				case IDM_GRAYREVERSION://灰度反转
					if (IMAGEHEIGHT == IMAGEWIDTH)
					{
						ImageGrayReversionProcessing(OrgImage, IMAGEWIDTH, IMAGEHEIGHT);
						ShowImage(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);    break;
					}
					else
					{
						rImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						gImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						bImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewrImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewgImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewbImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						DivideBMP(rImage, gImage, bImage, OrgImage, IMAGEWIDTH, IMAGEHEIGHT);
						ImageGrayReversionProcessing(rImage, IMAGEWIDTH, IMAGEHEIGHT);
						ImageGrayReversionProcessing(gImage, IMAGEWIDTH, IMAGEHEIGHT);
						ImageGrayReversionProcessing(bImage, IMAGEWIDTH, IMAGEHEIGHT);
						UnionBMP(rImage, gImage, bImage, OrgImage, IMAGEWIDTH, IMAGEHEIGHT);
						ShowBmpImage(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + IMAGEWIDTH + 300); break;
					}
					
				case IDM_HISTOGRAMEVEN://均衡化
					if (IMAGEHEIGHT == IMAGEWIDTH)
					{
						ImageHistogramEvenProcess(OrgImage, IMAGEWIDTH, IMAGEHEIGHT);
						ShowImage(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS);
						break;
					}
					else
					{
						rImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						gImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						bImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewrImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewgImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewbImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						DivideBMP(rImage, gImage, bImage, OrgImage, IMAGEWIDTH, IMAGEHEIGHT);
						ImageHistogramEvenProcess(rImage, IMAGEWIDTH, IMAGEHEIGHT);
						ImageHistogramEvenProcess(gImage, IMAGEWIDTH, IMAGEHEIGHT);
						ImageHistogramEvenProcess(bImage, IMAGEWIDTH, IMAGEHEIGHT);
						UnionBMP(rImage, gImage, bImage, OrgImage, IMAGEWIDTH, IMAGEHEIGHT);
						ShowBmpImage(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + IMAGEWIDTH + 300); break;
					}
					


				case IDM_CHANGEBYLOG:
					if (IMAGEHEIGHT == IMAGEWIDTH)
					{
						ImageChangeByLog(OrgImage, IMAGEWIDTH, IMAGEHEIGHT);
						ShowImage(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);
						break;
					}
					else
					{
						rImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						gImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						bImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewrImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewgImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewbImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						DivideBMP(rImage, gImage, bImage, OrgImage, IMAGEWIDTH, IMAGEHEIGHT);
						ImageChangeByLog(rImage, IMAGEWIDTH, IMAGEHEIGHT);
						ImageChangeByLog(gImage, IMAGEWIDTH, IMAGEHEIGHT);
						ImageChangeByLog(bImage, IMAGEWIDTH, IMAGEHEIGHT);
						UnionBMP(rImage, gImage, bImage, OrgImage, IMAGEWIDTH, IMAGEHEIGHT);
						ShowBmpImage(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + IMAGEWIDTH + 300); break;
					}
					

				case IDM_CHANGEBYPOW_1:
					if (IMAGEHEIGHT == IMAGEWIDTH)
					{
						ImageChangeByPow(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, 1);
						ShowImage(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);
						break;
					}
					else
					{
						rImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						gImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						bImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewrImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewgImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewbImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						DivideBMP(rImage, gImage, bImage, OrgImage, IMAGEWIDTH, IMAGEHEIGHT);
						ImageChangeByPow(rImage, IMAGEWIDTH, IMAGEHEIGHT,1);
						ImageChangeByPow(gImage, IMAGEWIDTH, IMAGEHEIGHT,1);
						ImageChangeByPow(bImage, IMAGEWIDTH, IMAGEHEIGHT,1);
						UnionBMP(rImage, gImage,bImage, OrgImage, IMAGEWIDTH, IMAGEHEIGHT);
						ShowBmpImage(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + IMAGEWIDTH + 300); break;
					}
					
				case IDM_CHANGEBYPOW_5:
					if (IMAGEHEIGHT == IMAGEWIDTH)
					{
						ImageChangeByPow(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, 5);
						ShowImage(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);
						break;
					}
					else
					{
						rImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						gImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						bImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewrImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewgImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewbImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						DivideBMP(rImage, gImage, bImage, OrgImage, IMAGEWIDTH, IMAGEHEIGHT);
						ImageChangeByPow(rImage, IMAGEWIDTH, IMAGEHEIGHT, 5);
						ImageChangeByPow(gImage, IMAGEWIDTH, IMAGEHEIGHT, 5);
						ImageChangeByPow(bImage, IMAGEWIDTH, IMAGEHEIGHT, 5);
						UnionBMP(rImage, gImage, bImage, OrgImage, IMAGEWIDTH, IMAGEHEIGHT);
						ShowBmpImage(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + IMAGEWIDTH + 300); break;
					}
				case IDM_CHANGEBYPOW_05:
					if (IMAGEHEIGHT == IMAGEWIDTH)
					{
						ImageChangeByPow(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, 0.05);
						ShowImage(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);
						break;
					}
					else
					{
						rImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						gImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						bImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewrImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewgImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewbImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						DivideBMP(rImage, gImage, bImage, OrgImage, IMAGEWIDTH, IMAGEHEIGHT);
						ImageChangeByPow(rImage, IMAGEWIDTH, IMAGEHEIGHT, 0.05);
						ImageChangeByPow(gImage, IMAGEWIDTH, IMAGEHEIGHT, 0.05);
						ImageChangeByPow(bImage, IMAGEWIDTH, IMAGEHEIGHT, 0.05);
						UnionBMP(rImage, gImage, bImage, OrgImage, IMAGEWIDTH, IMAGEHEIGHT);
						ShowBmpImage(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + IMAGEWIDTH + 300); break;
					}

				case IDM_CHANGEBYFUNCS:
					if (IMAGEHEIGHT == IMAGEWIDTH)
					{
						ImageChangeBySeptFunc(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, 70, 60, 210, 200);
						ShowImage(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);
						break;
					}
					else
					{
						rImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						gImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						bImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewrImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewgImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewbImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						DivideBMP(rImage, gImage, bImage, OrgImage, IMAGEWIDTH, IMAGEHEIGHT);
						ImageChangeBySeptFunc(rImage, IMAGEWIDTH, IMAGEHEIGHT, 70, 60, 210, 220);
						ImageChangeBySeptFunc(gImage, IMAGEWIDTH, IMAGEHEIGHT, 70, 60, 210, 220);
						ImageChangeBySeptFunc(bImage, IMAGEWIDTH, IMAGEHEIGHT, 70, 60, 210, 220);
						UnionBMP(rImage, gImage, bImage, OrgImage, IMAGEWIDTH, IMAGEHEIGHT);
						ShowBmpImage(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + IMAGEWIDTH + 300); break;
					}
					

				case IDM_CHANGEBYBIT0:
					if (IMAGEHEIGHT == IMAGEWIDTH)
					{
						ImageChangeByBits(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT, 0);
						ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);
						break;
					}
					else
					{
						rImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						gImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						bImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewrImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewgImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewbImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						DivideBMP(rImage, gImage, bImage, OrgImage, IMAGEWIDTH, IMAGEHEIGHT);
						ImageChangeByBits(rImage, NewrImage, IMAGEWIDTH, IMAGEHEIGHT,0);
						ImageChangeByBits(gImage, NewgImage, IMAGEWIDTH, IMAGEHEIGHT,0);
						ImageChangeByBits(bImage, NewbImage, IMAGEWIDTH, IMAGEHEIGHT,0);
						UnionBMP(NewrImage, NewgImage, NewbImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT);
						ShowBmpImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + IMAGEWIDTH + 300); break;
					}					
				case IDM_CHANGEBYBIT1:
					if (IMAGEHEIGHT == IMAGEWIDTH)
					{
						ImageChangeByBits(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT, 1);
						ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);
						break;
					}
					else
					{
						rImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						gImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						bImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewrImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewgImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewbImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						DivideBMP(rImage, gImage, bImage, OrgImage, IMAGEWIDTH, IMAGEHEIGHT);
						ImageChangeByBits(rImage, NewrImage, IMAGEWIDTH, IMAGEHEIGHT, 1);
						ImageChangeByBits(gImage, NewgImage, IMAGEWIDTH, IMAGEHEIGHT, 1);
						ImageChangeByBits(bImage, NewbImage, IMAGEWIDTH, IMAGEHEIGHT, 1);
						UnionBMP(NewrImage, NewgImage, NewbImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT);
						ShowBmpImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + IMAGEWIDTH + 300); break;
					}
				case IDM_CHANGEBYBIT2:
					if (IMAGEHEIGHT == IMAGEWIDTH)
					{
						ImageChangeByBits(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT, 2);
						ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);
						break;
					}
					else
					{
						rImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						gImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						bImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewrImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewgImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewbImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						DivideBMP(rImage, gImage, bImage, OrgImage, IMAGEWIDTH, IMAGEHEIGHT);
						ImageChangeByBits(rImage, NewrImage, IMAGEWIDTH, IMAGEHEIGHT, 2);
						ImageChangeByBits(gImage, NewgImage, IMAGEWIDTH, IMAGEHEIGHT, 2);
						ImageChangeByBits(bImage, NewbImage, IMAGEWIDTH, IMAGEHEIGHT, 2);
						UnionBMP(NewrImage, NewgImage, NewbImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT);
						ShowBmpImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + IMAGEWIDTH + 300); break;
					}
				case IDM_CHANGEBYBIT3:
					if (IMAGEHEIGHT == IMAGEWIDTH)
					{
						ImageChangeByBits(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT, 3);
						ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);
						break;
					}
					else
					{
						rImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						gImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						bImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewrImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewgImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewbImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						DivideBMP(rImage, gImage, bImage, OrgImage, IMAGEWIDTH, IMAGEHEIGHT);
						ImageChangeByBits(rImage, NewrImage, IMAGEWIDTH, IMAGEHEIGHT, 3);
						ImageChangeByBits(gImage, NewgImage, IMAGEWIDTH, IMAGEHEIGHT, 3);
						ImageChangeByBits(bImage, NewbImage, IMAGEWIDTH, IMAGEHEIGHT, 3);
						UnionBMP(NewrImage, NewgImage, NewbImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT);
						ShowBmpImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + IMAGEWIDTH + 300); break;
					}
				case IDM_CHANGEBYBIT4:
					if (IMAGEHEIGHT == IMAGEWIDTH)
					{
						ImageChangeByBits(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT, 4);
						ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);
						break;
					}
					else
					{
						rImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						gImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						bImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewrImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewgImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewbImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						DivideBMP(rImage, gImage, bImage, OrgImage, IMAGEWIDTH, IMAGEHEIGHT);
						ImageChangeByBits(rImage, NewrImage, IMAGEWIDTH, IMAGEHEIGHT, 4);
						ImageChangeByBits(gImage, NewgImage, IMAGEWIDTH, IMAGEHEIGHT, 4);
						ImageChangeByBits(bImage, NewbImage, IMAGEWIDTH, IMAGEHEIGHT, 4);
						UnionBMP(NewrImage, NewgImage, NewbImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT);
						ShowBmpImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + IMAGEWIDTH + 300); break;
					}
				case IDM_CHANGEBYBIT5:
					if (IMAGEHEIGHT == IMAGEWIDTH)
					{
						ImageChangeByBits(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT, 5);
						ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);
						break;
					}
					else
					{
						rImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						gImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						bImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewrImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewgImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewbImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						DivideBMP(rImage, gImage, bImage, OrgImage, IMAGEWIDTH, IMAGEHEIGHT);
						ImageChangeByBits(rImage, NewrImage, IMAGEWIDTH, IMAGEHEIGHT, 5);
						ImageChangeByBits(gImage, NewgImage, IMAGEWIDTH, IMAGEHEIGHT, 5);
						ImageChangeByBits(bImage, NewbImage, IMAGEWIDTH, IMAGEHEIGHT, 5);
						UnionBMP(NewrImage, NewgImage, NewbImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT);
						ShowBmpImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + IMAGEWIDTH + 300); break;
					}
				case IDM_CHANGEBYBIT6:
					if (IMAGEHEIGHT == IMAGEWIDTH)
					{
						ImageChangeByBits(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT, 6);
						ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);
						break;
					}
					else
					{
						rImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						gImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						bImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewrImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewgImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewbImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						DivideBMP(rImage, gImage, bImage, OrgImage, IMAGEWIDTH, IMAGEHEIGHT);
						ImageChangeByBits(rImage, NewrImage, IMAGEWIDTH, IMAGEHEIGHT, 6);
						ImageChangeByBits(gImage, NewgImage, IMAGEWIDTH, IMAGEHEIGHT, 6);
						ImageChangeByBits(bImage, NewbImage, IMAGEWIDTH, IMAGEHEIGHT, 6);
						UnionBMP(NewrImage, NewgImage, NewbImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT);
						ShowBmpImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + IMAGEWIDTH + 300); break;
					}
				case IDM_CHANGEBYBIT7:
					if (IMAGEHEIGHT == IMAGEWIDTH)
					{
						ImageChangeByBits(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT, 7);
						ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);
						break;
					}
					else
					{
						rImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						gImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						bImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewrImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewgImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						NewbImage = new char[IMAGEHEIGHT * IMAGEWIDTH];
						DivideBMP(rImage, gImage, bImage, OrgImage, IMAGEWIDTH, IMAGEHEIGHT);
						ImageChangeByBits(rImage, NewrImage, IMAGEWIDTH, IMAGEHEIGHT, 7);
						ImageChangeByBits(gImage, NewgImage, IMAGEWIDTH, IMAGEHEIGHT, 7);
						ImageChangeByBits(bImage, NewbImage, IMAGEWIDTH, IMAGEHEIGHT, 7);
						UnionBMP(NewrImage, NewgImage, NewbImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT);
						ShowBmpImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + IMAGEWIDTH + 300); break;
					}



				case IDM_GRAYTOLOW:
					if (IMAGEHEIGHT == IMAGEWIDTH)
						ShowGrayToLow(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS);
					else
						ShowBMPToLow(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS);
					break;
				case IDM_GRAYTOHIGH:
					if (IMAGEHEIGHT == IMAGEWIDTH)
						ShowGrayToHigh(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS);
					else
						ShowBMPToHigh(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS);
					break;
				case IDM_LEFTRIGHTOPEN:
					ShowLeftRightOpen(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS);
					break;
				case IDM_LEFTRIGHTCLOSE:
					ShowLeftRightClose(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS);
					break;
				case IDM_TOPBOTTOMOPEN:
					ShowTopBottomOpen(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS);
					break;
				case IDM_TOPBOTTOMCLOSE:
					ShowTopBottomClose(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS);
					break;
				case IDM_TBSHUTTER:
					ShowTopBottomShutter(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS);
					break;
				case IDM_BTSHUTTER:
					ShowBottomTopShutter(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS);
					break;
				case IDM_LRSHUTTER:
					ShowLeftRightShutter(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS);
					break;
				case IDM_RLSHUTTER:
					ShowRightLeftShutter(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS);
					break;
				case IDM_SHOWRAWIMAGE:
					OpenImageFileDlg("打开图像文件");
					InvalidateRgn(hWnd, NULL, TRUE);	UpdateWindow(hWnd);
					ReadImage(ImgDlgFileName, OrgImage, IMAGEWIDTH, IMAGEHEIGHT);
					ShowImage(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS);
				   break;
				case IDM_SHOWBMPIMAGE:
					OpenImageFileDlg("打开图像文件");
					InvalidateRgn(hWnd, NULL, TRUE);	UpdateWindow(hWnd);
					ReadBmpImage(ImgDlgFileName, OrgImage);
					ShowBmpImage(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS);
				   break;
				case IDM_ABOUT:
				   DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
				   break;
				case IDM_EXIT:
				   DestroyWindow(hWnd);
				   break;
				default:
				   return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			// TODO: Add any drawing code here...
			RECT rt;
			GetClientRect(hWnd, &rt);
			//DrawText(hdc, szHello, strlen(szHello), &rt, DT_CENTER);
			EndPaint(hWnd, &ps);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}

// Mesage handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
				return TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}
LRESULT CALLBACK Input(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	}
	return FALSE;
}




/************************************************************************************************
*																								*
*   读无格式RAW图像文件																			*
*																								*
************************************************************************************************/
BOOL ReadImage(LPSTR ImageFileName, char *oImage, int wImage, int hImage) //读取图像信息并保存在Image[][]中
{
	OFSTRUCT of;
	HFILE Image_fp;
	ImageHeight = 256;
	ImageWidth = 256;
	Image_fp = OpenFile(ImageFileName, &of, OF_READ);
	if (Image_fp == HFILE_ERROR) 
	{
		MessageBox(NULL, ImageFileName, "打开文件出错信息", MB_OK);
		return FALSE;
	}

	
	_lread(Image_fp, oImage, wImage*hImage);
	_lclose(Image_fp);

	return TRUE;
}

/************************************************************************************************
*																								*
*   显示RAW图像																					*
*																								*
************************************************************************************************/
void ShowImage(char *Image, int wImage, int hImage, int xPos, int yPos)
{
	int i, j;
	for (i = 0; i < hImage; i++) {
		for (j = 0; j < wImage; j++) {
			SetPixel(hWinDC, j + yPos, i + xPos,
				RGB(Image[i * wImage + j],
					Image[i * wImage + j],
					Image[i * wImage + j]));
		}
	}
}

/************************************************************************************************
*																								*
*   从BMP图像中，读出图像头信息，主要包括图像长度和宽度											*
*																								*
************************************************************************************************/
BOOL ReadBmpImage(LPSTR ImageFileName, char *oImage)
{ 
	BITMAPFILEHEADER bfImage;
	BITMAPINFOHEADER biImage;

	OFSTRUCT of;
	HFILE Image_fp;

	Image_fp = OpenFile(ImageFileName, &of, OF_READ);
	if (Image_fp == HFILE_ERROR) 
	{
		MessageBox(NULL, ImageFileName, "打开文件出错信息", MB_OK);
		return FALSE;
	}

	_llseek(Image_fp, 0, 0);
	_lread(Image_fp, &bfImage, sizeof(BITMAPFILEHEADER));

	if ((bfImage.bfType != 0x4d42)) {		// "BM"
		MessageBox(NULL, NULL, "打开bmp文件出错信息", MB_OK);
		return FALSE;
	}
	
	_lread(Image_fp, &biImage, sizeof(BITMAPINFOHEADER));

	ImageWidth = biImage.biWidth;
	ImageHeight= biImage.biHeight;
	if (biImage.biBitCount != 24) {		// 24位彩色图像
		MessageBox(NULL, NULL, "不是24位bmp图像", MB_OK);
		return FALSE;
	}

	_llseek(Image_fp, bfImage.bfOffBits, 0);
	_lread(Image_fp, oImage, biImage.biWidth * biImage.biHeight * 3);
	_lclose(Image_fp);

	return TRUE;
}

/************************************************************************************************
*																								*
*   显示BMP图像																					*
*																								*
************************************************************************************************/
void ShowBmpImage(char *Image, int wImage, int hImage, int xPos, int yPos)
{
	int i, j;
	char* tmp = Image;
	BYTE b, g, r;
	for (i = 0; i < hImage; i++) {
		for (j = 0; j < wImage; j++) {
			b = *tmp++;
			g = *tmp++;
			r = *tmp++;
			SetPixel(hWinDC, j + yPos, xPos+hImage-i, RGB(r, g, b));
		}
	}
}
/************************************************************************************************
*																								*
*   打开读文件名系统对话框																		*
*																								*
************************************************************************************************/
void OpenImageFileDlg(char *OPDLTitle)
{
	char FileTitle[100];
	OPENFILENAME ofn;

	memset(&ofn,0,sizeof(ofn));
	ofn.lStructSize=sizeof(OPENFILENAME);
	ofn.hwndOwner=NULL;
	ofn.hInstance=NULL;
	ofn.lpstrFilter=TEXT("raw files\0*.raw\0All File\0*.*\0\0");
	ofn.lpstrCustomFilter=NULL;
	ofn.nMaxCustFilter=0;
	ofn.nFilterIndex=1;
	ofn.lpstrFile=ImgDlgFileName;
	ofn.nMaxFile=MAX_PATH;
	ofn.lpstrFileTitle=FileTitle;
	ofn.nMaxFileTitle=99;
	ofn.lpstrInitialDir=ImgDlgFileDir;
	ofn.lpstrTitle=OPDLTitle;
	ofn.Flags=OFN_FILEMUSTEXIST;
	ofn.lpstrDefExt="raw";
	ofn.lCustData=NULL;
	ofn.lpfnHook=NULL;
	ofn.lpTemplateName=NULL;
	ImgDlgFileName[0]='\0';
	GetOpenFileName(&ofn); 

	getcwd(ImgDlgFileDir, MAX_PATH);
}
void ShowGrayToLow(char* Image, int wImage, int hImage, int xPos, int yPos)
{
	int i, j, m;		int r, g, b, gray;
	for (m = 1; m < GRAYCHANGENUM; m++) {
		for (i = 0; i < hImage; i++) {
			for (j = 0; j < wImage; j++) {
				gray = (BYTE)Image[i * wImage + j];
				r = g = b = gray * (GRAYCHANGENUM - m) / GRAYCHANGENUM;
				SetPixel(hWinDC, j + yPos, i + xPos, RGB(r, g, b));
			}
		}
	}
}
void ShowBMPToLow(char* Image, int wImage, int hImage, int xPos, int yPos)
{
	int i, j, m;		BYTE r, g, b;
	BYTE grey_r, grey_g, grey_b;
	for (m = 1; m < GRAYCHANGENUM; m++) {
		char* tmp = Image;
		for (i = 0; i < hImage; i++) {
			for (j = 0; j < wImage; j++) {
				grey_b = *tmp++;
				grey_g = *tmp++;
				grey_r = *tmp++;
				b = grey_b * (GRAYCHANGENUM - m) / GRAYCHANGENUM;
				g = grey_g * (GRAYCHANGENUM - m) / GRAYCHANGENUM;
				r = grey_r * (GRAYCHANGENUM - m) / GRAYCHANGENUM;
				SetPixel(hWinDC, j + yPos, xPos + hImage - i, RGB(r, g, b));
			}
		}
	}
}


void ShowGrayToHigh(char* Image, int wImage, int hImage, int xPos, int yPos)
{
	int i, j, m;		int r, g, b, gray;
	//for (m = 1; m < GRAYCHANGENUM; m++) {
	for (m = GRAYCHANGENUM-1; m >0; m--) {
		for (i = 0; i < hImage; i++) {
			for (j = 0; j < wImage; j++) {
				gray = (BYTE)Image[i * wImage + j];
				r = g = b = gray * (GRAYCHANGENUM - m) / GRAYCHANGENUM;
				SetPixel(hWinDC, j + yPos, i + xPos, RGB(r, g, b));
			}
		}
	}
}
void ShowBMPToHigh(char* Image, int wImage, int hImage, int xPos, int yPos)
{
	int i, j, m;		BYTE r, g, b;
	BYTE grey_r, grey_g, grey_b;
	for (m = GRAYCHANGENUM - 1; m > 0; m--) {
		char* tmp = Image;
		for (i = 0; i < hImage; i++) {
			for (j = 0; j < wImage; j++) {
				grey_b = *tmp++;
				grey_g = *tmp++;
				grey_r = *tmp++;
				b = grey_b * (GRAYCHANGENUM - m) / GRAYCHANGENUM;
				g = grey_g * (GRAYCHANGENUM - m) / GRAYCHANGENUM;
				r = grey_r * (GRAYCHANGENUM - m) / GRAYCHANGENUM;
				SetPixel(hWinDC, j + yPos, xPos + hImage - i, RGB(r, g, b));
			}
		}
	}
}

void ShowLeftRightOpen(char* Image, int wImage, int hImage, int xPos, int yPos)
{
	int i;
	RECT ShowRect;
	InvalidateRgn(hWind, NULL, TRUE);	UpdateWindow(hWind);	// 清空窗口
	if (wImage == hImage)
	{
		ShowRect.top = 0;
		ShowRect.bottom = hImage;
		for (i = 0; i < wImage / 2; i++) {
			ShowRect.left = wImage / 2 - i;
			ShowRect.right = ShowRect.left + 1;				//显示一列
			ShowImageRect(Image, wImage, hImage, xPos, yPos, ShowRect);	//左开门
			ShowRect.left = wImage / 2 + i;
			ShowRect.right = ShowRect.left + 1; 				//显示一列
			ShowImageRect(Image, wImage, hImage, xPos, yPos, ShowRect);	//右开门
			Sleep(1);
		}
	}
	else
	{
		ShowRect.top = 0;
		ShowRect.bottom = hImage;
		for (i = 0; i < wImage / 2; i++) {
			ShowRect.left = wImage / 2 - i;
			ShowRect.right = ShowRect.left + 1;				//显示一列
			ShowBMPImageRect(Image, wImage, hImage, xPos, yPos, ShowRect);	//左开门
			ShowRect.left = wImage / 2 + i;
			ShowRect.right = ShowRect.left + 1; 				//显示一列
			ShowBMPImageRect(Image, wImage, hImage, xPos, yPos, ShowRect);	//右开门
			Sleep(1);
		}
	}
}
void ShowLeftRightClose(char* Image, int wImage, int hImage, int xPos, int yPos)
{
	int i;
	RECT ShowRect;
	InvalidateRgn(hWind, NULL, TRUE);	UpdateWindow(hWind);	// 清空窗口
	if (wImage == hImage)
	{
		ShowRect.top = 0;
		ShowRect.bottom = hImage;
		for (i = wImage / 2 - 1; i >= 0; i--) {
			ShowRect.left = wImage / 2 - i;
			ShowRect.right = ShowRect.left + 1;				//显示一列
			ShowImageRect(Image, wImage, hImage, xPos, yPos, ShowRect);	//左开门
			ShowRect.left = wImage / 2 + i;
			ShowRect.right = ShowRect.left + 1; 				//显示一列
			ShowImageRect(Image, wImage, hImage, xPos, yPos, ShowRect);	//右开门
			Sleep(1);
		}
	}
	else
	{
		ShowRect.top = 0;
		ShowRect.bottom = hImage;
		for (i = wImage / 2 - 1; i >= 0; i--) {
			ShowRect.left = wImage / 2 - i;
			ShowRect.right = ShowRect.left + 1;				//显示一列
			ShowBMPImageRect(Image, wImage, hImage, xPos, yPos, ShowRect);	//左开门
			ShowRect.left = wImage / 2 + i;
			ShowRect.right = ShowRect.left + 1; 				//显示一列
			ShowBMPImageRect(Image, wImage, hImage, xPos, yPos, ShowRect);	//右开门
			Sleep(1);
		}
	}
}
void ShowTopBottomOpen(char* Image, int wImage, int hImage, int xPos, int yPos)
{
	int i;
	RECT ShowRect;
	InvalidateRgn(hWind, NULL, TRUE);	UpdateWindow(hWind);	// 清空窗口
	if (wImage == hImage)
	{
		ShowRect.left = 0;
		ShowRect.right = wImage;
		for (i = 0; i < hImage / 2; i++) {
			ShowRect.top = hImage / 2 - i;
			ShowRect.bottom = ShowRect.top + 1;				//显示一列
			ShowImageRect(Image, wImage, hImage, xPos, yPos, ShowRect);	//左开门
			ShowRect.top = hImage / 2 + i;
			ShowRect.bottom = ShowRect.top + 1; 				//显示一列
			ShowImageRect(Image, wImage, hImage, xPos, yPos, ShowRect);	//右开门
			Sleep(1);
		}
	}
	else
	{
		ShowRect.left = 0;
		ShowRect.right = wImage;
		for (i = 0; i < hImage / 2; i++) {
			ShowRect.top = hImage / 2 - i;
			ShowRect.bottom = ShowRect.top + 1;				//显示一列
			ShowBMPImageRect1(Image, wImage, hImage, xPos, yPos, ShowRect);	//左开门
			ShowRect.top = hImage / 2 + i;
			ShowRect.bottom = ShowRect.top + 1; 				//显示一列
			ShowBMPImageRect1(Image, wImage, hImage, xPos, yPos, ShowRect);	//右开门
			Sleep(1);
		}
	}
}
void ShowTopBottomClose(char* Image, int wImage, int hImage, int xPos, int yPos)
{
	int i;
	RECT ShowRect;
	InvalidateRgn(hWind, NULL, TRUE);	UpdateWindow(hWind);	// 清空窗口
	if (wImage == hImage)
	{
		ShowRect.left = 0;
		ShowRect.right = wImage;
		for (i = hImage / 2 - 1; i >= 0; i--) {
			ShowRect.top = hImage / 2 - i;
			ShowRect.bottom = ShowRect.top + 1;				//显示一列
			ShowImageRect(Image, wImage, hImage, xPos, yPos, ShowRect);	//左开门
			ShowRect.top = hImage / 2 + i;
			ShowRect.bottom = ShowRect.top + 1; 				//显示一列
			ShowImageRect(Image, wImage, hImage, xPos, yPos, ShowRect);	//右开门
			Sleep(1);
		}
	}
	else
	{
		ShowRect.left = 0;
		ShowRect.right = wImage;
		for (i = hImage / 2 - 1; i >= 0; i--) {
			ShowRect.top = hImage / 2 - i;
			ShowRect.bottom = ShowRect.top + 1;				//显示一列
			ShowBMPImageRect1(Image, wImage, hImage, xPos, yPos, ShowRect);	//左开门
			ShowRect.top = hImage / 2 + i;
			ShowRect.bottom = ShowRect.top + 1; 				//显示一列
			ShowBMPImageRect1(Image, wImage, hImage, xPos, yPos, ShowRect);	//右开门
			Sleep(1);
		}
	}
}

void ShowTopBottomShutter(char* Image, int wImage, int hImage, int xPos, int yPos)
{
	int i, j;
	RECT ShowRect;
	InvalidateRgn(hWind, NULL, TRUE); UpdateWindow(hWind);     //清空窗口
	if (wImage == hImage)
	{
		ShowRect.left = 0;
		ShowRect.right = hImage;
		for (i = 0; i < wImage / SHUTTERNUM; i++) {	 //SHUTTERNUM是百叶窗格数
			for (j = 0; j < SHUTTERNUM; j++) {
				ShowRect.top = i + (j * wImage / SHUTTERNUM);
				ShowRect.bottom = ShowRect.top + 1; 	      //显示一行
				ShowImageRect(Image, wImage, hImage, xPos, yPos, ShowRect);
				Sleep(1);
			}
		}
	}
	else
	{
		ShowRect.left = 0;
		ShowRect.right = wImage;
		for (i = 0; i < hImage / SHUTTERNUM; i++) {	 //SHUTTERNUM是百叶窗格数
			for (j = 0; j < SHUTTERNUM; j++) {
				ShowRect.top = i + (j * hImage / SHUTTERNUM);
				ShowRect.bottom = ShowRect.top + 1; 	      //显示一行
				ShowBMPImageRect1(Image, wImage, hImage, xPos, yPos, ShowRect);
				Sleep(1);
			}
		}
	}
}
void ShowBottomTopShutter(char* Image, int wImage, int hImage, int xPos, int yPos)
{
	int i, j;
	RECT ShowRect;
	InvalidateRgn(hWind, NULL, TRUE); UpdateWindow(hWind);     //清空窗口
	if (wImage == hImage)
	{
		ShowRect.left = 0;
		ShowRect.right = hImage;
		//for (i = 0; i < wImage / SHUTTERNUM; i++) {	 //SHUTTERNUM是百叶窗格数
		for (i = wImage / SHUTTERNUM; i >= 0; i--) {
			for (j = 0; j < SHUTTERNUM; j++) {
				ShowRect.top = i + (j * wImage / SHUTTERNUM);
				ShowRect.bottom = ShowRect.top + 1; 	      //显示一行
				ShowImageRect(Image, wImage, hImage, xPos, yPos, ShowRect);
				Sleep(1);
			}
		}
	}
	else
	{
		ShowRect.left = 0;
		ShowRect.right = wImage;
		//for (i = 0; i < wImage / SHUTTERNUM; i++) {	 //SHUTTERNUM是百叶窗格数
		for (i = hImage / SHUTTERNUM; i >= 0; i--) {
			for (j = 0; j < SHUTTERNUM; j++) {
				ShowRect.top = i + (j * hImage / SHUTTERNUM);
				ShowRect.bottom = ShowRect.top + 1; 	      //显示一行
				ShowBMPImageRect1(Image, wImage, hImage, xPos, yPos, ShowRect);
				Sleep(1);
			}
		}
	}
}
void ShowLeftRightShutter(char* Image, int wImage, int hImage, int xPos, int yPos)
{
	int i, j;
	RECT ShowRect;
	InvalidateRgn(hWind, NULL, TRUE); UpdateWindow(hWind);     //清空窗口
	if (wImage == hImage)
	{
		ShowRect.top = 0;
		ShowRect.bottom = wImage;
		for (i = 0; i < hImage / SHUTTERNUM; i++) {	 //SHUTTERNUM是百叶窗格数
			for (j = 0; j < SHUTTERNUM; j++) {
				ShowRect.left = i + (j * hImage / SHUTTERNUM);
				ShowRect.right = ShowRect.left + 1; 	      //显示一行
				ShowImageRect(Image, wImage, hImage, xPos, yPos, ShowRect);
				Sleep(1);
			}
		}
	}
	else
	{
		ShowRect.top = 0;
		ShowRect.bottom = hImage;
		for (i = 0; i < wImage / SHUTTERNUM; i++) {	 //SHUTTERNUM是百叶窗格数
			for (j = 0; j < SHUTTERNUM; j++) {
				ShowRect.left = i + (j * wImage / SHUTTERNUM);
				ShowRect.right = ShowRect.left + 1; 	      //显示一行
				ShowBMPImageRect(Image, wImage, hImage, xPos, yPos, ShowRect);
				Sleep(1);
			}
		}
	}
}
void ShowRightLeftShutter(char* Image, int wImage, int hImage, int xPos, int yPos)
{
	int i, j;
	RECT ShowRect;
	InvalidateRgn(hWind, NULL, TRUE); UpdateWindow(hWind);     //清空窗口
	if (wImage == hImage)
	{
		ShowRect.top = 0;
		ShowRect.bottom = wImage;
		//for (i = 0; i < wImage / SHUTTERNUM; i++) {	 //SHUTTERNUM是百叶窗格数
		for (i = hImage / SHUTTERNUM; i >= 0; i--) {
			for (j = 0; j < SHUTTERNUM; j++) {
				ShowRect.left = i + (j * hImage / SHUTTERNUM);
				ShowRect.right = ShowRect.left + 1; 	      //显示一行
				ShowImageRect(Image, wImage, hImage, xPos, yPos, ShowRect);
				Sleep(1);
			}
		}
	}
	else
	{
		ShowRect.top = 0;
		ShowRect.bottom = hImage;
		//for (i = 0; i < wImage / SHUTTERNUM; i++) {	 //SHUTTERNUM是百叶窗格数
		for (i = wImage / SHUTTERNUM; i >= 0; i--) {
			for (j = 0; j < SHUTTERNUM; j++) {
				ShowRect.left = i + (j * wImage / SHUTTERNUM);
				ShowRect.right = ShowRect.left + 1; 	      //显示一行
				ShowBMPImageRect(Image, wImage, hImage, xPos, yPos, ShowRect);
				Sleep(1);
			}
		}
	}
}

void ShowImageRect(char* Image, int wImage, int hImage, int xPos, int yPos, RECT ShowRect)
{
	int i, j;
	int r, g, b;
	for (i = ShowRect.top; i < ShowRect.bottom; i++) {
		for (j = ShowRect.left; j < ShowRect.right; j++) {
			r = g = b = (BYTE)Image[i * wImage + j];
			SetPixel(hWinDC, j + yPos, i + xPos, RGB(r, g, b));
		}
	}
}
void ShowBMPImageRect(char* Image, int wImage, int hImage, int xPos, int yPos, RECT ShowRect)
{
	int i, j;
	int r, g, b;
	char* tmp = Image + wImage * 3 / 2 + (ShowRect.left - (wImage / 2)) * 3 - 3;
	for (i = ShowRect.top; i < ShowRect.bottom; i++) {
		
		for (j = ShowRect.left; j < ShowRect.right; j++) {
			b = *tmp++;
			g = *tmp++;
			r = *tmp++;
			SetPixel(hWinDC, j + yPos, xPos + hImage - i, RGB(r, g, b));
		}
		tmp += (wImage * 3-3);
	}
}
void ShowBMPImageRect1(char* Image, int wImage, int hImage, int xPos, int yPos, RECT ShowRect)
{
	int i, j;
	int r, g, b;
	char* tmp = Image + (hImage / 2 - (ShowRect.top - (hImage / 2))) * wImage * 3;;
	for (j = ShowRect.top; j < ShowRect.bottom; j++) {
		for (i = ShowRect.left; i < ShowRect.right; i++) {


			b = *tmp++;
			g = *tmp++;
			r = *tmp++;
			SetPixel(hWinDC, i + yPos, xPos + j, RGB(r, g, b));		
		}
	}
}


void ImageHistogramShowProcessing(char* oImage, char* nImage, int wImage, int hImage)
{
	int i, j, Gray, MaxGrayCount;	
	int GrayCount[256];
	for (i = 0; i < 256; i++) 
		GrayCount[i] = 0;
	for (i = 0; i < wImage * hImage; i++) 
	{
		Gray = (BYTE)oImage[i];
		GrayCount[Gray]++; 
	}
	MaxGrayCount = GrayCount[0];
	for (i = 0; i < 256; i++) 
		if (MaxGrayCount < GrayCount[i]) 
			MaxGrayCount = GrayCount[i];
	for (i = 0; i < 256 * 256; i++) 
		nImage[i] = (BYTE)192;
	if (wImage == hImage)
	{
		for (j = 0; j < 256; j++)
		{
			GrayCount[j] = (int)(GrayCount[j] * HISTOGRAMHIEGHT / MaxGrayCount);
			for (i = 0; i < GrayCount[j]; i++)
				nImage[(255 - i) * 256 + j] = 0;
		}
	}
	else
	{
		for (j = 0; j < 256; j++)
		{
			GrayCount[j] = (int)(GrayCount[j] * HISTOGRAMHIEGHT / MaxGrayCount);
			for (i = 0; i < GrayCount[j]; i++)
				nImage[(255-i) * 255 + j] = 0;
		}
	}
}

void ImageGrayReversionProcessing(char* Image, int wImage, int hImage)
{
	int i;

	for (i = 0; i < wImage * hImage; i++)
		Image[i] = (BYTE)(255 - (BYTE)Image[i]);
}

void ImageHistogramEvenProcess(char* oImage, int wImage, int hImage)
{
	double h[256] = { 0 };
	for (int i = 0; i < wImage * hImage; i++)
	{
		h[(BYTE)oImage[i]]++;
	}
	double sum = 0;
	for (int i = 0; i < 256; i++)
	{
		h[i] /= (wImage * hImage);
		h[i] += sum;
		sum = h[i];
	}
	for (int i = 0; i < wImage * hImage; i++)
	{
		oImage[i] = (BYTE)(h[(BYTE)oImage[i]] * 255);
	}
	
}

int iThreshold = 128;
void ThresholdImage(char* Image, int wImage, int hImage)
{
	int i;

	for (i = 0; i < wImage * hImage; i++)
	{
		if ((BYTE)Image[i] < iThreshold)
			Image[i] = 0;
		else
			Image[i] = (BYTE)255;
	}
}

void ImageChangeByLog(char* oImage, int wImage, int hImage)//对数变换
{
	for (int i = 0; i < wImage * hImage; i++)
	{
		oImage[i] = (BYTE)((255/log(255)) * log((BYTE)oImage[i] + 1));
	}
}

void ImageChangeByPow(char* oImage, int wImage, int hImage, double k)//幂次变换
{
	for (int i = 0; i < wImage * hImage; i++)
	{
		oImage[i] = (BYTE)255 * pow((BYTE)oImage[i], k) / pow(255, k);
	}
}
void ImageChangeBySeptFunc(char* oImage, int wImage, int hImage, float r1, float s1, float r2, float s2)//分段函数
{
	for (int i = 0; i < wImage * hImage; i++)
	{
		if ((BYTE)oImage[i] < r1)
		{
			oImage[i] = (BYTE)(s1 / r1) * (BYTE)oImage[i];
		}
		else if ((BYTE)oImage[i] < r2)
		{
			oImage[i] = (BYTE)(((s2 - s1) / (r2 - r1)) * ((BYTE)oImage[i] - r1) + s1);
		}
		else
		{
			oImage[i] = (BYTE)(((255 - s2) / (255 - r2)) * ((BYTE)oImage[i] - r2) + s2);
		}
	}
}

void ImageChangeByBits(char* oImage, char* nImage, int wImage, int hImage, int k)//位图分割
{
	for (int i = 0; i < wImage * hImage; i++)
	{
		if ((BYTE)oImage[i] >> k & 1)
			nImage[i] = (BYTE)255;
		else
			nImage[i] = (BYTE)0;
	}
}

void ImageMaskProcessing(char* oImage, char* nImage, int wImage, int hImage, int* Mask, int MaskWH, int MaskCoff)
{
	int Coff;	int i, j, m, n;	int k;
	k = (MaskWH - 1) / 2;
	for (i = k; i < hImage - k; i++) {
		for (j = k; j < wImage - k; j++) {
			Coff = 0;
			for (m = -k; m <= k; m++) {
				for (n = -k; n <= k; n++) {
					Coff += (BYTE)oImage[(i + m) * wImage + (j + n)] *
						Mask[(m + k) * MaskWH + (n + k)];
				}
			}
			nImage[i * wImage + j] = (unsigned char)(Coff / MaskCoff);
			if (Coff < 0) nImage[i * wImage + j] = 0;
		}
	}
}

void AverageMaskProcessing(char* oImage, char* nImage, int wImage, int hImage)
{
	int Mask[9] = { 1, 1, 1,
			   1, 1, 1,
			   1, 1, 1 };

	ImageMaskProcessing(oImage, nImage, wImage, hImage, Mask, 3, 9);
}

void GuassAverageMaskProcessing(char* oImage, char* nImage, int wImage, int hImage)
{
	int Mask[9] = { 1, 2, 1,
			   2, 4, 2,
			   1, 2, 1 };

	ImageMaskProcessing(oImage, nImage, wImage, hImage, Mask, 3, 16);
}

void MaxFilterProcessing(char* oImage, char* nImage, int wImage, int hImage)
{
	ImageMaskProcessing(oImage, nImage, wImage, hImage, 3, 1);
}
void MinFilterProcessing(char* oImage, char* nImage, int wImage, int hImage)
{
	ImageMaskProcessing(oImage, nImage, wImage, hImage, 3, -1);
}
void MidFilterProcessing(char* oImage, char* nImage, int wImage, int hImage)
{
	ImageMaskProcessing(oImage, nImage, wImage, hImage, 3, 0);
}

void ImageMaskProcessing(char* oImage, char* nImage, int wImage, int hImage, int MaskWH, int sign)
{
	int i, j, m, n;	int k;
	k = (MaskWH - 1) / 2;
	for (i = k; i < hImage - k; i++) {
		for (j = k; j < wImage - k; j++) {
			int tmp;
			int flag = (BYTE)oImage[(i) * wImage + (j)];
			{
				int* tmp = new int[MaskWH * MaskWH];
				int count = 0;
				for (m = -k; m <= k; m++) {
					for (n = -k; n <= k; n++) {
						tmp[count] = (BYTE)oImage[(i + m) * wImage + (j + n)];
						count++;
					}
				}
				sort(tmp, tmp + MaskWH * MaskWH);
				if (sign == 0)
				{
					flag = tmp[MaskWH * MaskWH / 2];
				}
				else if (sign == 1)
				{
					flag = tmp[MaskWH * MaskWH - 1];
				}
				else if (sign == -1)
				{
					flag = tmp[0];
				}
				
			}
			nImage[i * wImage + j] = (unsigned char)flag;
			
		}
	}
}

void SALTNOISEGENERATOR(char* oImage, int wImage, int hImage, double rate)
{
	if (wImage == hImage)
	{
		for (int i = 0; i < wImage * hImage; i++)
		{
			double tmp = (double)rand() / RAND_MAX;
			if (tmp < rate)
			{
				oImage[i] = (BYTE)255;
			}
		}
	}
	else
	{
		for (int i = 0; i < wImage * hImage*3; i++)
		{
			double tmp = (double)rand() / RAND_MAX;
			if (tmp < rate)
			{
				oImage[i] = (BYTE)255;
				i++;
				oImage[i] = (BYTE)255;
				i++;
				oImage[i] = (BYTE)255;
			}
		}
	}
}

void PEPPERNOISEGENERATOR(char* oImage, int wImage, int hImage, double rate)
{
	if (wImage == hImage)
	{
		for (int i = 0; i < wImage * hImage; i++)
		{
			double tmp = (double)rand() / RAND_MAX;
			if (tmp < rate)
			{
				oImage[i] = (BYTE)0;
			}
		}
	}
	else
	{
		for (int i = 0; i < wImage * hImage*3; i++)
		{
			double tmp = (double)rand() / RAND_MAX;
			if (tmp < rate)
			{
				oImage[i] = (BYTE)0;
				i++;
				oImage[i] = (BYTE)0;
				i++;
				oImage[i] = (BYTE)0;
			}
		}
	}
}

void LaplaceEdgeProcessing(char* oImage, char* nImage, int wImage, int hImage)
{
	int Mask[9] = { 0, 1, 0, 			//Laplace边缘检测模板
			   1,-4, 1,
			   0, 1, 0 };

	ImageMaskProcessing(oImage, nImage, wImage, hImage, Mask, 3, 1);
}
void RobertsEdgeProcessing(char* oImage, char* nImage, int wImage, int hImage)
{
	int Mask1[4] = { 0, 1,
		-1, 0 };
	int Mask2[4] = { 1, 0,
		0, -1 };

	ImageMaskProcessing(oImage, nImage, wImage, hImage, Mask1, Mask2, 2, 1, 1);
}

void PrewittEdgeProcessing(char* oImage, char* nImage, int wImage, int hImage, int sign)
{
	int Mask1[9] = { 1, 1, 1, 			//Prewitt边缘检测模板
			   0, 0, 0,
			   -1, -1, -1 };
	int Mask2[9] = { 1, 0, -1, 			//Prewitt边缘检测模板
			   1, 0, -1,
			   1, 0, -1 };

	ImageMaskProcessing(oImage, nImage, wImage, hImage, Mask1, Mask2, 3, 1, sign);
}

void SobelEdgeProcessing(char* oImage, char* nImage, int wImage, int hImage, int sign)
{
	int Mask1[9] = { 1, 2, 1, 			
			   0, 0, 0,
			   -1, -2, -1 };
	int Mask2[9] = { 1, 0, -1, 			
			   2, 0, -2,
			   1, 0, -1 };

	ImageMaskProcessing(oImage, nImage, wImage, hImage, Mask1, Mask2, 3, 1, sign);
}


void ImageMaskProcessing(char* oImage, char* nImage, int wImage, int hImage, int* Mask1, int* Mask2, int MaskWH, int MaskCoff, int sign)
{
	int Coff1, Coff2;
	int i, j, m, n;	int k;
	k = (MaskWH - 1) / 2;
	for (i = k; i < hImage - k; i++) {
		for (j = k; j < wImage - k; j++) {
			Coff1 = 0;
			Coff2 = 0;
			if (k != 0)
			{
				for (m = -k; m <= k; m++) {
					for (n = -k; n <= k; n++) {
						Coff1 += (BYTE)oImage[(i + m) * wImage + (j + n)] *
							Mask1[(m + k) * MaskWH + (n + k)];
						Coff2 += (BYTE)oImage[(i + m) * wImage + (j + n)] *
							Mask2[(m + k) * MaskWH + (n + k)];
					}
				}
			}
			else
			{
				for (m = 0; m <= k+1; m++) {
					for (n = 0; n <= k+1; n++) {
						Coff1 += (BYTE)oImage[(i + m) * wImage + (j + n)] *
							Mask1[(m + k) * MaskWH + (n + k)];
						Coff2 += (BYTE)oImage[(i + m) * wImage + (j + n)] *
							Mask2[(m + k) * MaskWH + (n + k)];
					}
				}
			}
			double tmp;
			if (sign == 1)
			{
				if (Coff1 > Coff2)
					tmp = Coff1;
				else
					tmp = Coff2;
			}
			else
			{
				tmp = sqrt(pow(Coff1, 2) + pow(Coff2, 2));
			}
			tmp /= MaskCoff;
			nImage[i * wImage + j] = (unsigned char)(tmp);
			if (tmp < 0) nImage[i * wImage + j] = tmp * -1;
			else if (tmp > 255) nImage[i * wImage + j] = 255;
		}
	}
}

void SimpleGeometricTranslation(char* oImage, char* nImage, int wImage, int hImage, double* sgt_Array)
{
	double axy, bxy;		int i, sx, sy, ox, oy;
	for (i = 0; i < wImage * hImage; i++) nImage[i] = 0;		  //清空新图像
	for (sy = 0; sy < hImage; sy++) {
		for (sx = 0; sx < wImage; sx++) {
			ox = sx - wImage / 2;				  //以图像中心为原点
			oy = sy - hImage / 2;
			axy = sgt_Array[0] * (double)ox + sgt_Array[1] * (double)oy + sgt_Array[2];
			bxy = sgt_Array[3] * (double)ox + sgt_Array[4] * (double)oy + sgt_Array[5];
			axy += wImage / 2;				  //恢复图像原点位置
			bxy += hImage / 2;
			if ((axy < 0.0) || (axy >= wImage - 1) || (bxy < 0.0) || (bxy >= hImage - 1))
				nImage[sy * wImage + sx] = 0;			  //超出范围部分置0
			else
				nImage[sy * wImage + sx] = oImage[((int)bxy) * wImage + (int)axy];
		}
	}
}
void ImageTranslation(char* oImage, char* nImage, int wImage, int hImage, int xPos, int yPos)
{
	double gt_Array[9] = { 1, 0, 0,
			 0, 1, 0,
			 0, 0, 1 };

	gt_Array[2] = xPos;				 //水平方向平移距离
	gt_Array[5] = yPos;				 //垂直方向平移距离

	SimpleGeometricTranslation(oImage, nImage, wImage, hImage, gt_Array);
}
void ImageScaling(char* oImage, char* nImage, int wImage, int hImage, double xScaling, double yScaling)
{
	double gt_Array[9] = { 0, 0, 0,
			 0, 0, 0,
			 0, 0, 1 };

	gt_Array[0] = 1.0 / xScaling;		 //水平方向放大倍数
	gt_Array[4] = 1.0 / yScaling;		 //垂直方向放大倍数

	SimpleGeometricTranslation(oImage, nImage, wImage, hImage, gt_Array);
}
void ImageRotation(char* oImage, char* nImage, int wImage, int hImage, double iAngle)
{
	double pi = 3.14159;
	double gt_Array[9] = { 0, 0, 0,
				 0, 0, 0,
				 0, 0, 1 };
	iAngle = (iAngle / 360) * 2 * pi;
	gt_Array[0] = cos(iAngle);
	gt_Array[1] = -sin(iAngle);
	gt_Array[3] = sin(iAngle);
	gt_Array[4] = cos(iAngle);

	SimpleGeometricTranslation(oImage, nImage, wImage, hImage, gt_Array);
}

void DivideBMP(char* rImage, char* gImage, char* bImage, char* oImage, int wImage, int hImage)
{
	char* tmp = oImage;
	BYTE r, g, b;
	for (int i = 0; i < hImage; i++) {
		for (int j = 0; j < wImage; j++) {
			b = *tmp++;
			*bImage++ = b;
			g = *tmp++;
			*gImage++ = g;
			r = *tmp++;
			*rImage++ = r;
		}
	}
}
void UnionBMP(char* rImage, char* gImage, char* bImage, char* nImage, int wImage, int hImage)
{
	for (int i = 0; i < hImage; i++) {
		for (int j = 0; j < wImage; j++) {
			*nImage++ = *bImage++;
			*nImage++ = *gImage++;
			*nImage++ = *rImage++;
		}
	}
}

void Change2Grey(char* oImage, char* nImage, int wImage, int hImage)
{
	int i, j;
	char* tmp = oImage;
	int r, g, b;
	for (i = hImage - 1; i >= 0; i--) {
		for (j = 0; j < wImage; j++) {
			b = *tmp++;
			g = *tmp++;
			r = *tmp++;
			nImage[i * wImage + j] = (BYTE)max(r, g, b);
		}
	}
}