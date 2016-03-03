#pragma once
#include <Windows.h>
#include <tchar.h>
#include <EGL/egl.h>
#include <gles2/gl2.h>

#include "CELLMath.hpp"
#include "CELLShader.hpp"

namespace CELL
{
	class CELLWinApp
	{
	public:
		//! 实例句柄
		HINSTANCE _hInstance;
		//! 窗口句柄
		HWND      _hWnd;
		//! 窗口高度
		int _width;
		//! 窗口宽度
		int _height;
		//! for opengles2.0
		EGLDisplay display;
		EGLSurface window;
		EGLConfig config;
		EGLContext context;
		
		//! 增加shader
		PROGRAM_P2_C4 _shader;
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
		/*初始化OpenGL ES2.0*/
		bool initOpenGLES20()
		{
			EGLint format(0);//*value,指定返回值
			EGLint    numConfigs(0);
			EGLint majorVersion,minorVersion;//！主版本号、次版本号

			//! 指定EGL属性
			const EGLint configAttribs[] =
			{
				EGL_RENDERABLE_TYPE, EGL_WINDOW_BIT,
				EGL_RED_SIZE, 8,
				EGL_GREEN_SIZE, 8,
				EGL_BLUE_SIZE, 8,
				EGL_DEPTH_SIZE, 24,
				EGL_NONE
			};
			

			//!  初始化 egl 与窗口系统通信，打开与EGL显示服务器的连接
			display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
			if (display == EGL_NO_DISPLAY)
			{
				return EGL_FALSE;
				//Unable to open connection to local windowing system 
			}
			//!  初始化 egl 
			
			if (!eglInitialize(display, &majorVersion, &minorVersion))
			{
				return EGL_FALSE;
				//Unable to initialize EGL ,handle an recover
			}

			//! 让EGL选择配置
			
			if (!eglChooseConfig(display, configAttribs, &config, 1, &numConfigs))
			{
				return EGL_FALSE;
			}

			//! 查询eglconfig属性
			
			eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

			//! 创建一个window窗口(EGLNativeWindowType：_hWnd,窗口句柄，这里改写为需要的窗口句柄）
			window = eglCreateWindowSurface(display, config, _hWnd, NULL);

			if (window == EGL_NO_SURFACE)
			{
				return EGL_FALSE;
			}
			
			//! 创建一个渲染上下文
			//! contextAttribs用于指定创建上下文使用的属性列表，
			//! 只有一个可以接受的属性EGL_CONTEXT_CLIENT_VERSION
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

			//! 指定某个eglcontext为当前上下文
			if (!eglMakeCurrent(display, window, window, context))
			{
				return EGL_FALSE;
			}

			eglQuerySurface(display, window, EGL_WIDTH, &_width);
			eglQuerySurface(display, window, EGL_HEIGHT, &_height);

			return EGL_TRUE;

		}

		/*销毁OpenGL Es2.0*/
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
		/*绘制函数*/
		virtual void render()
		{
			//glClearColor(0, 1, 0, 1);
			//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			//glViewport(0, 0, _width, _heigth);
			GLfloat vVertices[]={
					0.0f,0.5f,0.0f,
					-0.5f,-0.5f,0.0f,
					0.5f,-0.5f,0.0f
			};
			glViewport(0,0,_width,_height);
			glClear(GL_COLOR_BUFFER_BIT);
			//glUseProgram();
			glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,vVertices);
			glEnableVertexAttribArray(0);
			glDrawArrays(GL_TRIANGLES,0,3);
		}
		virtual void    render2()
        {
            //! 清空缓冲区
            glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
            //! 视口，在Windows窗口指定的位置和大小上绘制OpenGL内容
            glViewport(0,0,_width,_height);

            //! 创建一个投影矩阵
			//扩展学习正交投影矩阵，透视投影glOrthodox（）、gluPerspective（）
            CELL::matrix4   screenProj  =   CELL::ortho<float>(0,float(_width),float(_height),0,-100.0f,100);//映射到窗口坐标
			/*char *uniforname;//不匹配
			const GLfloat *uniforname;
			GLint maxUniformLen;
			uniforname = malloc(sizeof(char) *maxUniformLen);*/
            _shader.begin();
            {//起始位置和宽高
                float   x   =   100;
                float   y   =   50;
                float   w   =   100;
                float   h   =   100;
                CELL::float2  pos[]   =   
                {//50,50,50,50,50,50,50,50,
                     CELL::float2(x,y),
                     CELL::float2(x + w,y),
                     CELL::float2(x,y + h),
                     CELL::float2(x + w, y + h),
                };
                glUniformMatrix4fv(_shader._MVP, 1, false,screenProj.data());//给MVP赋值。这里学习下显卡程序，vertex/Fragment shader
                glUniform4f(_shader._color,1,0,0,1);//要画的颜色，红色1 0 0
              	glVertexAttribPointer(_shader._position,2,GL_FLOAT,false,sizeof(CELL::float2),pos);
                glDrawArrays(GL_TRIANGLE_STRIP,0,4);//赋值完再调用才能执行。调用的次数4次，每次_position的值变化，因而设置为局部变量，而_MVP的值不变，故为全局变量Uniform
            }
            _shader.end();

        }
		//! 主函数
		int main(int width,int height){
			_hWnd = CreateWindowEx( NULL,
										_T("CELLWinApp"),
										_T("CELLWinApp"),
										WS_OVERLAPPEDWINDOW,
										CW_USEDEFAULT,//0,
										CW_USEDEFAULT,//0,
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
			_shader.initialize();

			MSG msg = { 0 };
			while (msg.message != WM_QUIT)
			{
				if (msg.message == WM_DESTROY ||
					msg.message == WM_CLOSE)
				{
					break;
				}
				/*有消息，处理消息，无消息，则进行渲染*/
				if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				else
				{
					render2();
					eglSwapBuffers(display,window);
					//Sleep(1);
				}
			}
			destoryOPENGLES20();
			return 0;

		}


	};
}
