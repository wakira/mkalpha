Specification
==============

Overview
-------------------------

Name: let’s name our project “mkalpha”

In a word, it's more like an Android other than Linux kernel. It is not a real OS because mbed has done most work for us. And we cannot have virtual address space for each process... So we simply utilize the thread mechanism of mbed RTOS to build a App abstraction. The "OS" acts as an intermediate layer between user app and IO devices, preventing the same device to be used simutaneously by different apps.

Components
-------------------------

Kernel: we use the singleton class Kernel to bookkeep information of running Apps, e.g., their requested IO resource lock and registered callback. It should intercept all IO and delegate them to propriate App. Joystick and LCD is always designated to the foreground App. Other devices such as LED can be given to background App. I have not decided yet what kind of interface would be good for App.

Library: we need to provide a menu library because most App would be using a menu. They do not want to use the LCD and joystick API everytime.

Launcher: the “shell” app, user can choose to launch new app or put to foreground some background application. It can be implemented as a plain App if we provide runApp and putForeground SYSCALL in the kernel.

App: each App executes its EventLoop in its own thread. The kernel is responsible for adding events to the EventQueue with callback functions that the App has registered. We might consider allowing App to create new thread. Since time is limited, we may not have time to wrap each mbed APIs so simply exposing the mbed API will be okay.
