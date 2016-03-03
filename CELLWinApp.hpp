#pragma once
#include <Windows.h>
#include <tchar.h>
#include <EGL/egl.h>
#include <gles2/gl2.h>
namespace CELL
{
	class CELLWinApp
	{
	public:
		//! ʵ�����
		HINSTANCE _hInstance;
		//! ���ھ��
		HWND      _hWnd;
		//! ���ڸ߶�
		int _width;
		//! ���ڿ��
		int _heigth;
		//! for opengles2.0
		EGLDisplay display;
		EGLSurface window;
		EGLConfig config;
		EGLContext context;
		
	public:
		CELLWinApp(HINSTANCE hInstance)
			:_hInstance(hInstance)
		{
			WNDCLASSEX winClass;
			memset(&winClass, 0, sizeof(winClass));
			winClass.cbSize          =  sizeof(winClass);
			winClass.lpszClassName   =  _T("CELLWinApp");
			winClass.style           =  CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
			winClass.lpfnWndProc     =  wndProc;
			winClass.hInstance       =  hInstance;
			winClass.hIcon           =  0;
			winClass.hIconSm         =  0;
			winClass.hCursor         =  LoadCursor(hInstance, IDC_ARROW);
			winClass.hbrBackground   =  (HBRUSH)GetStockObject(BLACK_BRUSH);
			winClass.lpszMenuName    =  NULL;
			winClass.cbClsExtra      =  0;
			winClass.cbWndExtra      =  0;
			RegisterClassEx(&winClass);

		}
		virtual ~CELLWinApp(){
			UnregisterClass(_T("CELLWinApp"), _hInstance);
		}
		/*��ʼ��OpenGL ES2.0*/
		bool initOpenGLES20()
		{
			EGLint format(0);//*value,ָ������ֵ
			EGLint    numConfigs(0);
			EGLint majorVersion,minorVersion;//�����汾�š��ΰ汾��

			//! ָ��EGL����
			const EGLint configAttribs[] =
			{
				EGL_RENDERABLE_TYPE, EGL_WINDOW_BIT,
				EGL_RED_SIZE, 8,
				EGL_GREEN_SIZE, 8,
				EGL_BLUE_SIZE, 8,
				EGL_DEPTH_SIZE, 24,
				EGL_NONE
			};
			

			//!  ��ʼ�� egl �봰��ϵͳͨ�ţ�����EGL��ʾ������������
			display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
			if (display == EGL_NO_DISPLAY)
			{
				return EGL_FALSE;
				//Unable to open connection to local windowing system 
			}
			//!  ��ʼ�� egl 
			
			if (!eglInitialize(display, &majorVersion, &minorVersion))
			{
				return EGL_FALSE;
				//Unable to initialize EGL ,handle an recover
			}

			//! ��EGLѡ������
			
			if (!eglChooseConfig(display, configAttribs, &config, 1, &numConfigs))
			{
				return EGL_FALSE;
			}

			//! ��ѯeglconfig����
			
			eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

			//! ����һ��window����(EGLNativeWindowType��_hWnd,���ھ���������дΪ��Ҫ�Ĵ��ھ����
			window = eglCreateWindowSurface(display, config, _hWnd, NULL);

			if (window == EGL_NO_SURFACE)
			{
				return EGL_FALSE;
			}
			
			//! ����һ����Ⱦ������
			//! contextAttribs����ָ������������ʹ�õ������б�
			//! ֻ��һ�����Խ��ܵ�����EGL_CONTEXT_CLIENT_VERSION
			const EGLint contextAttribs[] =
			{
				EGL_CONTEXT_CLIENT_VERSION, 2,
				EGL_NONE
			};
			context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs);
			if (context == EGL_NO_CONTEXT)
			{
				return EGL_FALSE;
			}

			//! ָ��ĳ��eglcontextΪ��ǰ������
			if (!eglMakeCurrent(display, window, window, context))
			{
				return EGL_FALSE;
			}

			eglQuerySurface(display, window, EGL_WIDTH, &_width);
			eglQuerySurface(display, window, EGL_HEIGHT, &_heigth);

			return EGL_TRUE;

		}

		/*����OpenGL Es2.0*/
		void destoryOPENGLES20()
		{
			if (display!=EGL_NO_DISPLAY)
			{
				eglMakeCurrent(display,EGL_NO_SURFACE,EGL_NO_SURFACE,EGL_NO_CONTEXT);
				if(context!=EGL_NO_CONTEXT)
				{
					eglDestroyContext(display,context);
				}
				if(window!=EGL_NO_SURFACE)
				{
					eglDestroySurface(display,window);
				}
				eglTerminate(display);

			}
			display=EGL_NO_DISPLAY;
			context=EGL_NO_CONTEXT;
			window=EGL_NO_SURFACE;
		}

	protected:
		static LRESULT CALLBACK wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			CELLWinApp* pThis = (CELLWinApp*)GetWindowLong(hWnd, GWL_USERDATA);
			if (pThis){
				return pThis->onEvent(hWnd, msg, wParam, lParam);
			}
			if (WM_CREATE == msg)
			{
				CREATESTRUCT* pCreat = (CREATESTRUCT*)lParam;
				SetWindowLong(hWnd, GWL_USERDATA, (DWORD_PTR)pCreat->lpCreateParams);
			}
			return DefWindowProc(hWnd, msg, wParam, lParam);
		}
	
	public:
		virtual LRESULT onEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			switch (msg)
			{
			case WM_CLOSE:
			case WM_DESTROY:{
								::PostQuitMessage(0);
			}
				break;
			case WM_MOUSEMOVE:
				break;
			default:
				return  DefWindowProc(hWnd, msg, wParam, lParam);
		
			}
			return S_OK;
		}
		/*���ƺ���*/
		virtual void render()
		{
			glClearColor(0, 1, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glViewport(0, 0, _width, _heigth);
		}

		//! ������
		int main(int width,int height){
			_hWnd = CreateWindowEx( NULL,
										_T("CELLWinApp"),
										_T("CELLWinApp"),
										WS_OVERLAPPEDWINDOW,
										0,//CW_USEDEFAULT,//0,
										0,//CW_USEDEFAULT,//0,
										width,
										height, 
										0, 
										0,
										_hInstance, 
										this
										);
			if (_hWnd == 0){
				return -1;
			}
		
			//UpdateWindow(_hWnd);
			ShowWindow(_hWnd, SW_MAXIMIZE);

			if(!initOpenGLES20())
			{
				return false;
			}

			MSG msg = { 0 };
			while (msg.message != WM_QUIT)
			{
				if (msg.message == WM_DESTROY ||
					msg.message == WM_CLOSE)
				{
					break;
				}
				/*����Ϣ��������Ϣ������Ϣ���������Ⱦ*/
				if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				else
				{
					render();
					eglSwapBuffers(display,window);
					//Sleep(1);
				}
			}
			destoryOPENGLES20();
			return 0;

		}


	};
}
