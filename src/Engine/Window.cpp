#include <Engine/Window.hpp>
#include <iostream>
#include <Engine/Event/ApplicationEvent.hpp>
#include <Engine/Event/KeyEvent.hpp>
#include <Engine/Event/MouseEvent.hpp>

static bool s_GLFWInitialized = false;

static void GLFWErrorCallback(int error, const char* description)
{
	std::cerr << error << " : " << description << std::endl;
}

Window* Window::create(const WindowProps& props)
{
	return new Window(props);
}

Window::Window(const WindowProps& props)
{
	Init(props);
}

Window::~Window()
{
	Shutdown();
}

void Window::Init(const WindowProps& props)
{
	m_data.Title = props.Title;
	m_data.Width = props.Width;
	m_data.Height = props.Height;

	std::cout << "Creating window " << props.Title << "(" << props.Width << ", " << props.Height << ")" << std::endl;

	if (!s_GLFWInitialized)
	{
		// TODO: glfwTerminate on system shutdown
		int success = glfwInit();
		if(success == 0)
		{
			std::cerr << "Could not intialize GLFW!" << std::endl;
			return;
		}
		glfwSetErrorCallback(GLFWErrorCallback);
		s_GLFWInitialized = true;
	}

	m_window = glfwCreateWindow((int)props.Width, (int)props.Height, m_data.Title.c_str(), nullptr, nullptr);
	glfwMakeContextCurrent(m_window);
	glfwSetWindowUserPointer(m_window, &m_data);
	setVSync(true);

	if (glewInit() != GLEW_OK)
    {
		std::cerr << "ERROR!" << std::endl;
		glfwTerminate();
		return;
    }
	
    // Hide one useless faces
	//glEnable(GL_CULL_FACE); 

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  

    // Manage depth
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Set GLFW callbacks
	glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height)
	{
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
		data.Width = width;
		data.Height = height;

		WindowResizeEvent event(width, height);
		data.EventCallback(event);
	});

	glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window)
	{
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
		WindowCloseEvent event;
		data.EventCallback(event);
	});

	glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

		switch (action)
		{
			case GLFW_PRESS:
			{
				KeyPressedEvent event(key, 0);
				data.EventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				KeyReleasedEvent event(key);
				data.EventCallback(event);
				break;
			}
			case GLFW_REPEAT:
			{
				KeyPressedEvent event(key, 1);
				data.EventCallback(event);
				break;
			}
		}
	});

	glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods)
	{
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

		switch (action)
		{
			case GLFW_PRESS:
			{
				MouseButtonPressedEvent event(button);
				data.EventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				MouseButtonReleasedEvent event(button);
				data.EventCallback(event);
				break;
			}
		}
	});

	glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xOffset, double yOffset)
	{
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

		MouseScrolledEvent event((float)xOffset, (float)yOffset);
		data.EventCallback(event);
	});

	glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xPos, double yPos)
	{
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

		double deltaX = xPos - data.CursorX;
		double deltaY = yPos - data.CursorY;

		data.CursorX = xPos;
		data.CursorY = yPos;

		MouseMovedEvent event((float)xPos, (float)yPos, (float) deltaX, (float) deltaY);
		data.EventCallback(event);
	});
}

void Window::Shutdown()
{
	glfwDestroyWindow(m_window);
}

void Window::onUpdate()
{
	WindowData& data = *(WindowData*)glfwGetWindowUserPointer(m_window);
	double time = glfwGetTime();
	double deltaTime = time - data.Time;
	data.Time = glfwGetTime();

	AppUpdateEvent event(this, deltaTime);
	
	data.EventCallback(event);

	glfwPollEvents();
	glfwSwapBuffers(m_window);
}

void Window::setVSync(bool enabled)
{
	if (enabled)
		glfwSwapInterval(1);
	else
		glfwSwapInterval(0);

	m_data.VSync = enabled;
}

bool Window::isVSync() const
{
	return m_data.VSync;
}

void Window::setTitle(std::string title) 
{
	glfwSetWindowTitle(m_window, title.c_str());
	
	m_data.Title = title;
}
