# QtGoBang说明文档

计53 王润基

## 支持的功能

* 联机五子棋游戏
* 发送消息
* 认输
* 危险提示

## 内部实现说明

* 类与结构说明：
  * GoBangManager：五子棋逻辑类
  * GoBangMessager：数据通信类
    * 封装了网络连接和数据通信。
    * 对网络连接处理：转发QTcpSocket/QTcpServer的接口和信号
    * 对每种需要支持的通信内容：给出若干接口，用于向对方发送信息；给出若干信号，用于接收对方的信息。
    * 用一个QMessageLogger记录日志
  * GridView：棋盘显示类
    * 由一个QWidget提升所得
    * 常量引用GoBangManager，用于显示棋盘信息
    * 接收用户点击，如果是合法的，触发信号
  * MainWindow：主窗口，担任控制器的角色
    * 拥有GoBangManager，GoBangMessager，GridView的指针
    * 提供若干槽函数，分别对接UI->Widget、GoBangMessager、GridView的信号，并执行相关操作
    * 负责与GoBangManager交互
  * 交互过程举例：
    * 用户点击棋盘->GridView发射信号报告位置->MainWindow槽接收
      * ->报告GoBangManager->通知GridView更新显示
      * ->调用GoBangMessager向对方发送信息->对方GoBangMessager接收+解析+发射操作信号->对方MainWindow槽接收->报告GoBangManager->通知GridView更新显示
* 通信数据表示：
  * 开头一个int，表示操作的种类
  * 之后是操作的参数信息（例如落子的参数就是QPoint表示位置）
  * 目前支持的操作有：
    * 落子
    * 新开局
    * 设置黑白手
    * 认输
    * 发送信息

## 可扩展性

* 在通信中增加新的操作类型
  * 例如：提出悔棋，是否同意对方悔棋……
  * 实现：约定数据格式，在GoBangMessager中提供对应的接口和信号，在MainWindow中提供接口和槽与前者对接，实现功能。