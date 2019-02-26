## Simper User Interface

### 简要说明

该SUI实现了一个简单的图形系统, 实现了按钮以及文本框等控件, 并且可以很方便的自定义新的控件以丰富完善该系统.

### 设计思路

GUI以控件形式进行组织, 每一个控件包含自己的数据内容`char* data`以及状态信息`union state_info`, 并包含一个callback`void (*callback)(messages)`用于解析处理来自其他控件或者是系统中断发送的消息. 
各个空间的主要功能是通过不同的回调函数实现的, 目前回调函数的调用方式是直接调用:
```c
void send_message(messages message){
    widget *dest;
    dest = get_widget_from_id(message.dest);
    // Only unhide widget and redraw msg can be called
    if(dest != NULL && (dest->ishided == FALSE || message.msgid == WIDGET_REDRAW)){
        dest->callback(message);
    }else{
#ifdef DEBUG_DUMP
        printf("Wrong Destination when sending message. id:%d\n", message.dest);
#endif
    }
}
```
可以考虑将其修改为一个消息队列, 对不同消息增加优先级, 并结合时钟中断以及汇编保存函数上下文的形式实现一个任务调度器.

### 结构说明

主要有五个部分构成
- Driver/下主要为和硬件相关的驱动
- Fonts/下主要为字库
- fonts.c/h为文字相关函数
- Graphics.c/.h主要为图形相关函数
- SUI.c/h主要为系统空间相关函数

### 基本数据结构介绍

#### SUI.h

- messages 定义了控件之间的传递的消息
- widget 定义某一个具体控件的信息
- widget_info 用于从数组方便生成图形界面
- widget_table 是存储空间信息的全局表

#### Graphics.h

- point 主要定义了一个点
- size 主要定义了一个框的大小
- range 表示的是一个数的范围
- frame 结合point和size表示出了屏幕上的一个区域
- bound 表示出了一个区域的绝对位置(坐标系位置)

#### font.h

- font_info 表示了一个字体的相关信息, 使程序决定如何去读取字库

### 功能介绍

####　生成一个图形界面

1. 你需要在硬件上配置一个定时器TIMER0, 默认配置即可. 
2. 使用void SUI_Init(color c_background);初始化界面. 
3. 使用ret_code create_user_interface_from_array(const widget_info *pawidget, int length);来创建界面.
4. 为控件写不同的回调函数以实现功能.
