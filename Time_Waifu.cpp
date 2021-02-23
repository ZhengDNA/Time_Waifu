/*
 *  IDE			Visual Studio 2019
 *  EasyX版本	20200902
 */

#include<stdio.h>
#include<Windows.h>
#include<graphics.h>
#include<time.h>
 //mci接口必须
#pragma comment(lib,"Winmm.lib")

//mci接口的参数
#define DETAILS NULL,0,NULL

//文字显示的范围（左，上，右，下）
#define TEXTAREA 300,20,620,180

//报时的时间（分：十位，分：个位，秒：十位，秒：个位）
#define ONTIME t[14]=='0'&&t[15]=='0'&&t[17]=='0'&&t[18]=='0'

//设置字体样式
void textStyle(const TCHAR* fontFamily, int fontSize, COLORREF fontColor)
{
	LOGFONT logf;
	gettextstyle(&logf);
	logf.lfHeight = fontSize;
	_tcscpy_s(logf.lfFaceName, fontFamily);
	logf.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&logf);
	settextcolor(fontColor);
}
//显示台词
void displayWords(char h1, char h2)
{
	RECT prect = { TEXTAREA };		//文字显示范围
	textStyle(L"微软雅黑", 30, RGB(186, 32, 32));	
	char fileName[13] = "words/00.txt";
	fileName[6] = h1, fileName[7] = h2;		//确定文件名
	FILE* fp;
	fopen_s(&fp, fileName, "r");
	char words[100] = {0};
	int i;
	char ch;
	for (i = 0, ch = fgetc(fp); ch != EOF; i++, ch = fgetc(fp))
	{
		words[i] = ch;
	}
	words[i] = '\0';	//读取台词文件的内容
	TCHAR wWords[100];
	MultiByteToWideChar(CP_ACP, 0, words, -1, wWords, 100);	//转换为宽字符
	drawtext(wWords, &prect, DT_WORDBREAK);	//显示台词
	fclose(fp);
}
//显示音量按键
void volumeText(int volume)
{
	textStyle(L"微软雅黑", 30, RGB(186, 32, 32));
	TCHAR volumetext[] = L"音量 7";
	volumetext[3] = (volume % 8) + 48;
	outtextxy(560, 440, volumetext);
}
//改变音量
void changeVolume(int volume)
{
	char a[] = "setaudio voice volume to 000";
	int temp = volume * 60;
	a[25] = temp / 100 + 48;
	a[26] = (temp / 10) % 10 + 48;
	a[27] = temp % 10 + 48;
	mciSendStringA(a, DETAILS);
	volumeText(volume);
}
//播放整点报时语音
void timeVoice(char h1, char h2, int volume)
{
	//21,22 
	char a[] = "open voice\\0000.mp3 alias voice";
	a[11] = h1, a[12] = h2;
	mciSendStringA("close voice", DETAILS);
	mciSendStringA(a, DETAILS);		//打开音频文件
	changeVolume(volume);
	mciSendStringA("play voice", DETAILS);
	displayWords(h1, h2);
}
//主函数
int main()
{
	bool wordsDisplay = false;		//台词文本显示
	bool windowDisplay = true;		//窗口显示
	int volume = 7;					//音量大小

	initgraph(640, 480);	//初始化绘图窗口
	setbkcolor(WHITE);		//设置白色背景
	cleardevice();

	//加载图片
	IMAGE painting;		//图片地址
	loadimage(&painting, L"setpainting_W.png");
	putimage(0, 0, &painting);
	volumeText(volume);

	HWND hwnd = GetForegroundWindow();	//获取窗口句柄

	while (1)
	{
		time_t a;	//存储time()函数获取的时间
		time(&a);	//获取当前计算机的时间
		char t[26];		//存储时间字符串
		ctime_s(t, sizeof(t), &a);		//把当前时间赋值给t[] "Wed Dec 30 12:01:13 2020\n"

		//设置字体样式
		textStyle(L"Consolas", 36, RGB(186, 32, 32));
		TCHAR timeText[] = L"00:00:00";		//显示时间的文本
		timeText[0] = t[11], timeText[1] = t[12], timeText[3] = t[14], timeText[4] = t[15];
		timeText[6] = t[17], timeText[7] = t[18];	//显示秒要把text[]定义成_T("00:00:00")
		outtextxy(400, 320, timeText);

		//物理下标 时：11 12    分：14 15    秒：17 18
		if (ONTIME)
		{
			//整点报时
			timeVoice(t[11], t[12], volume);
			wordsDisplay = true;
			Sleep(1000);
		}
		else
		{
			//20秒之后清除台词
			if (wordsDisplay && t[17] == '2' && t[18] == '0')
			{
				clearrectangle(TEXTAREA);
				wordsDisplay = false;
			}
		}
		//控制音量按钮
		MOUSEMSG mmsg;
		if (PeekMouseMsg(&mmsg, true))
		{
			//鼠标点击到按钮位置时
			if (mmsg.uMsg == WM_LBUTTONDOWN && mmsg.x > 560 && mmsg.x < 630 && mmsg.y>440 && mmsg.y < 470)
			{
				//调节音量
				volume = (volume + 1) % 8;
				changeVolume(volume);
			}
		}
		//按 F8 隐藏/显示窗口
		if (GetAsyncKeyState(VK_F8) & 0x8000)
		{
			if (windowDisplay)
			{
				windowDisplay = !windowDisplay;
				ShowWindow(hwnd, SW_HIDE);
			}
			else
			{
				windowDisplay = !windowDisplay;
				ShowWindow(hwnd, SW_SHOWNORMAL);
			}
		}
		FlushMouseMsgBuffer();
		Sleep(50);	//根据计算机性能调整冻结时间
	}
	return 0;
}