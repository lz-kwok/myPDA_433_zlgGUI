# myPDA_433_zlgGUI
myPDA_433_zlgGUI

程序流程
1.通过单键双稳态电路，实现长按开关机、短按功能键
2.采用msOS，剥离GUI和业务逻辑，其中，scada为GUI部分，logic为业务逻辑部分
3.GUI部分采用zlgGUI，通过GUI_Exec()函数，全屏刷数据，100ms刷新一次；
4.zlgGUI中，独立开发了文本控件汉字显示部分
5.开机后，首先读取flash中的记忆数据，分别为AB值、延时关机时间和出入量值，若为首次
    开机，数据为默认值
6.分别对各个外设进行初始化，初始化操作在device.c中
7.进入Scada任务后，首先读取433模组的默认设置，对应在屏幕上显示目标机号和当前频段，
    具体见Init433Module()函数；
8.通过设置全局变量PowerOnFlag的bit值，在scada中，根据bit值，对应显示主页、设置菜单、
    选择机号界面等
9.进入设置菜单，密码默认为131，若修改密码，直接修改全局变量CodeValue即可
10.按键为独立IO设计，在系统节拍中，每10ms扫描一次，具体见KeyScan1000Routine()函数
11.在系统节拍中，每10ms读取一次电量值的AD，对应显示在屏幕上
12.菜单的业务逻辑，对应参考UE

重要外设说明
1.device_433.c，用于初始化433模组，并且设置433模组的相关参数，如波特率、频段、主机
    编号和目标机号
    Init433Module()  初始化获取433模组参数
    SetFrequency()   设置发送频段
    SetSlaverAddr()  设置目标机号
 
2.device_adc.c，用于采集电池电量，AdcSystick100Routine()函数在系统节拍中执行

3.device_crc.c,modbus协议实现
	Crc16()                 CRC16查表校验
	ModbusMasterAsk()       主机读取
	ModbusMasterRead()      主机写入

4.device_flash.c，开辟内部flash作为模拟eeprom，用于存储延时关机时间等参数
	FlashRead()             读取flash对应地址数据
	DevInforProgram()       模拟eeprom，写入数据到对应的地址

5.device_oled.c,使用硬件spi，操作oled

6.devic_menu.c,菜单业务逻辑
	HomepageParsing()        主页获取得从机参数的解析，具体业务逻辑见解析说明
	HomeWindow()             主页菜单显示，通过短按开机键，判断是否进入系统设置界面
	HomePageShow()           显示从机电量、主机电量、充电指示等
	SetupPageShow()          设置菜单，通过上下按键和确认键，确认是否进入对应菜单目录
	ParameterSetShow()       参数设置页面，主要包括强制、长度、出入量等
		ForceMenuShow
		InoutMenuShow
		SpacingMenuShow
		lengthMenuShow
		MovementMenuShow
		IndelayMenuShow
		OutnumMenuShow
		ABMenuShow         			子单元模块业务逻辑内容相同，通过数字按键进行操作
		SelectFrebandMenuShow()  	设置当前频段




2018.2.4    组合键稳定性
           
    