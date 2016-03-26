## Simper User Interface

### 简要说明

该SUI实现了一个简单的图形系统, 实现了按钮以及文本框等控件, 并且可以很方便的自定义新的控件以丰富完善该系统

### 结构说明

主要有五个部分构成
- Driver/下主要为和硬件相关的驱动
- Fonts/下主要为字库
- fonts.c/h为文字相关函数
- Graphics.c/.h主要为图形相关函数
- SUI.c/h主要为系统空间相关函数

### 基本数据结构介绍

#### Graphics.h

- point 主要定义了一个点
- size 主要定义了一个框的大小
- range 表示的是一个数的范围
- frame 结合point和size表示出了屏幕上的一个区域
- bound 表示出了一个区域的绝对位置(坐标系位置)

#### font.h

- font_info 表示了一个字体的相关信息, 使程序决定如何去读取字库

#### SUI.h

- messages 定义了控件之间的传递的消息
- widget 定义某一个具体控件的信息
- widget_info 用于从数组方便生成图形界面
- widget_table 是存储空间信息的全局表

### 功能介绍

####　生成一个图形界面

1. 你需要在硬件上配置一个定时器TIMER0, 默认配置即可. 
2. 使用void SUI_Init(color c_background);初始化界面. 
3. 使用ret_code create_user_interface_from_array(const widget_info *pawidget, int length);来创建界面.
4. 为控件写不同的回调函数以实现功能.