/*
 *  IDE			Visual Studio 2019
 *  EasyX版本	20200902
 */


 //mci接口必须
#pragma comment(lib,"Winmm.lib")
using namespace std;


#include "Time_Waifu.h"
#include<stdio.h>
#include<Windows.h>
#include<time.h>
#include <string>


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
void displayWords(char h_chara, char h1, char h2)
{
	RECT prect = { TEXTAREA };		//文字显示范围
	textStyle(L"微软雅黑", 23, RGB(186, 32, 32));	
	string fileName ="words/"; 
	fileName += h_chara;
	fileName += '/';
	fileName += h1; fileName += h2;
	fileName += ".txt";

	FILE* fp;
	fopen_s(&fp, fileName.c_str(), "r");

	char words[150] = {0};
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
//显示角色切换按键
void charaText()
{	
	textStyle(L"微软雅黑", 30, RGB(186, 32, 32));
	TCHAR charatext[] = L"角色";
	outtextxy(10, 10, charatext);
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
void timeVoice(char h_chara,char h1, char h2, int volume)
{
	//21,22 
	mciSendStringA("close voice", DETAILS);

	string fileName = " open voice\\";
	fileName += h_chara;
	fileName += '\\';
	fileName += h1; fileName += h2;
	fileName += "00.mp3 alias voice"; 

	mciSendStringA(fileName.c_str(), DETAILS);		//打开音频文件
	changeVolume(volume);
	mciSendStringA("play voice", DETAILS);
	displayWords(h_chara,h1, h2);
}


//主函数
int main()
{
	bool wordsDisplay = false;		//台词文本显示
	bool windowDisplay = true;		//窗口显示
	int volume = 7;					//音量大小
	char charaNameArray[5] = {'0','1','2','3','4'};              //舰娘角色选择
	char charaName= charaNameArray[0]; int charaId = 0;

	initgraph(640, 480);	//初始化绘图窗口
	setbkcolor(WHITE);		//设置白色背景
	cleardevice();

	//加载图片
	IMAGE painting;		//图片地址
	loadimage(&painting,L"setpainting_0.png");
	putimage(0, 0, &painting);
	volumeText(volume);  	//显示音量切换按键 
	charaText();

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
			timeVoice(charaName,t[11], t[12], volume);
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

		
		//控制音量按钮  	//控制角色按钮
		MOUSEMSG mmsg;
		if (PeekMouseMsg(&mmsg, true))
		{
			//鼠标点击到按钮位置时
			if ((mmsg.uMsg ==  WM_LBUTTONDOWN || mmsg.uMsg == WM_MOUSEWHEEL) && mmsg.x > 560 && mmsg.x < 630 && mmsg.y>440 && mmsg.y < 470)
			{
				//调节音量
				volume = (volume + 1) % 8;
				changeVolume(volume);
				//test the volume upon each peek
				timeVoice(charaName,t[11], t[12], volume);
			}
			else if (mmsg.uMsg == WM_LBUTTONDOWN && mmsg.x > 10 && mmsg.x < 40 && mmsg.y>10 && mmsg.y < 40) {

				charaId = (++charaId) % 5;
				charaName = charaNameArray[charaId];

				if (charaName == '1') {
					loadimage(&painting, L"setpainting_1.png");
					putimage(0, 0, &painting);
				}
				else if (charaName == '2') {
					loadimage(&painting, L"setpainting_2.png");
					putimage(0, 0, &painting);
				}
				else if (charaName == '3') {
					loadimage(&painting, L"setpainting_3.png");
					putimage(0, 0, &painting);
				}
				else if (charaName == '4') {
					loadimage(&painting, L"setpainting_4.png");
					putimage(0, 0, &painting);
				}
				else if (charaName == '0') {
					loadimage(&painting, L"setpainting_0.png");
					putimage(0, 0, &painting);
				}
				clearrectangle(TEXTAREA);
				charaText();
			
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