#include "mainwindow.h"
#include "Window.h"

#include <QSlider>
#include <QFormLayout>
#include <QDockWidget>

namespace
{
constexpr auto g_sampels = 8;
constexpr auto g_gl_major_version = 3;
constexpr auto g_gl_minor_version = 3;
}// namespace


MainWindow::MainWindow()
{
	QSlider* innerCircleAngleSlider = new QSlider(Qt::Horizontal);

	innerCircleAngleSlider->setMinimum(Window::MIN_ANGLE);
	innerCircleAngleSlider->setMaximum(Window::MAX_ANGLE);
	innerCircleAngleSlider->setValue(Window::DEFAULT_ANGLE);


	QSlider* outerCircleAngleSlider = new QSlider(Qt::Horizontal);

	outerCircleAngleSlider->setMinimum(Window::MIN_ANGLE);
	outerCircleAngleSlider->setMaximum(Window::MAX_ANGLE);
	outerCircleAngleSlider->setValue(Window::DEFAULT_ANGLE);

	QSlider* lightXSlider = new QSlider(Qt::Horizontal);

	lightXSlider->setMinimum(Window::MIN_COORD);
	lightXSlider->setMaximum(Window::MAX_COORD);
	lightXSlider->setValue(Window::DEFAULT_X);

	QSlider* lightZSlider = new QSlider(Qt::Horizontal);

	lightZSlider->setMinimum(Window::MIN_COORD);
	lightZSlider->setMaximum(Window::MAX_COORD);
	lightZSlider->setValue(Window::DEFAULT_Z);

	fpsLabel_ = new QLabel();
	fpsLabel_->setText("...");

	QFormLayout* formLayout = new QFormLayout();

	formLayout->addRow("Spot inner circle angle: ", innerCircleAngleSlider);
	formLayout->addRow("Spot outer circle angle:", outerCircleAngleSlider);
	formLayout->addRow("Light X:", lightXSlider);
	formLayout->addRow("Light Z:", lightZSlider);
	formLayout->addRow("FPS:", fpsLabel_);

	QSurfaceFormat format;
	format.setSamples(g_sampels);
	format.setVersion(g_gl_major_version, g_gl_minor_version);
	format.setProfile(QSurfaceFormat::CoreProfile);

	Window* windowWidget = new Window;
	windowWidget->setFormat(format);

	connect(innerCircleAngleSlider, &QSlider::valueChanged, windowWidget, &Window::setInnerCircleAngle);
	connect(outerCircleAngleSlider, &QSlider::valueChanged, windowWidget, &Window::setOuterCircleAngle);
	connect(lightXSlider, &QSlider::valueChanged, windowWidget, &Window::setLightX);
	connect(lightZSlider, &QSlider::valueChanged, windowWidget, &Window::setLightZ);
	connect(windowWidget, &Window::updateFPS, this, &MainWindow::updateFPS);

	setCentralWidget(windowWidget);

	auto dock = new QDockWidget;
	auto settingsWidget = new QWidget;
	settingsWidget->setFocusPolicy(Qt::NoFocus);
	dock->setFocusPolicy(Qt::NoFocus);
	settingsWidget->setLayout(formLayout);
	dock->setWidget(settingsWidget);

	this->addDockWidget(Qt::BottomDockWidgetArea, dock);
}

void MainWindow::updateFPS(uint fps)
{
	fpsLabel_->setText(QString::asprintf("FPS:%u", fps));
}


