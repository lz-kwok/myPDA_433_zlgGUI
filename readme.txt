��������
1.ͨ������˫��̬��·��ʵ�ֳ������ػ����̰����ܼ�
2.����msOS������GUI��ҵ���߼������У�scadaΪGUI���֣�logicΪҵ���߼�����
3.GUI���ֲ���zlgGUI��ͨ��GUI_Exec()������ȫ��ˢ���ݣ�100msˢ��һ�Σ�
4.zlgGUI�У������������ı��ؼ�������ʾ����
5.���������ȶ�ȡflash�еļ������ݣ��ֱ�ΪABֵ����ʱ�ػ�ʱ��ͳ�����ֵ����Ϊ�״�
    ����������ΪĬ��ֵ
6.�ֱ�Ը���������г�ʼ������ʼ��������device.c��
7.����Scada��������ȶ�ȡ433ģ���Ĭ�����ã���Ӧ����Ļ����ʾĿ����ź͵�ǰƵ�Σ�
    �����Init433Module()������
8.ͨ������ȫ�ֱ���PowerOnFlag��bitֵ����scada�У�����bitֵ����Ӧ��ʾ��ҳ�����ò˵���
    ѡ����Ž����
9.�������ò˵�������Ĭ��Ϊ131�����޸����룬ֱ���޸�ȫ�ֱ���CodeValue����
10.����Ϊ����IO��ƣ���ϵͳ�����У�ÿ10msɨ��һ�Σ������KeyScan1000Routine()����
11.��ϵͳ�����У�ÿ10ms��ȡһ�ε���ֵ��AD����Ӧ��ʾ����Ļ��
12.�˵���ҵ���߼�����Ӧ�ο�UE

��Ҫ����˵��
1.device_433.c�����ڳ�ʼ��433ģ�飬��������433ģ�����ز������粨���ʡ�Ƶ�Ρ�����
    ��ź�Ŀ�����
    Init433Module()  ��ʼ����ȡ433ģ�����
    SetFrequency()   ���÷���Ƶ��
    SetSlaverAddr()  ����Ŀ�����
 
2.device_adc.c�����ڲɼ���ص�����AdcSystick100Routine()������ϵͳ������ִ��

3.device_crc.c,modbusЭ��ʵ��
	Crc16()                 CRC16���У��
	ModbusMasterAsk()       ������ȡ
	ModbusMasterRead()      ����д��

4.device_flash.c�������ڲ�flash��Ϊģ��eeprom�����ڴ洢��ʱ�ػ�ʱ��Ȳ���
	FlashRead()             ��ȡflash��Ӧ��ַ����
	DevInforProgram()       ģ��eeprom��д�����ݵ���Ӧ�ĵ�ַ

5.device_oled.c,ʹ��Ӳ��spi������oled

6.devic_menu.c,�˵�ҵ���߼�
	HomepageParsing()        ��ҳ��ȡ�ôӻ������Ľ���������ҵ���߼�������˵��
	HomeWindow()             ��ҳ�˵���ʾ��ͨ���̰����������ж��Ƿ����ϵͳ���ý���
	HomePageShow()           ��ʾ�ӻ��������������������ָʾ��
	SetupPageShow()          ���ò˵���ͨ�����°�����ȷ�ϼ���ȷ���Ƿ�����Ӧ�˵�Ŀ¼
	ParameterSetShow()       ��������ҳ�棬��Ҫ����ǿ�ơ����ȡ���������
		ForceMenuShow
		InoutMenuShow
		SpacingMenuShow
		lengthMenuShow
		MovementMenuShow
		IndelayMenuShow
		OutnumMenuShow
		ABMenuShow         			�ӵ�Ԫģ��ҵ���߼�������ͬ��ͨ�����ְ������в���
		SelectFrebandMenuShow()  	���õ�ǰƵ��




2018.2.4    ��ϼ��ȶ���
           
    