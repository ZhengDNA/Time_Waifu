/*
 *  IDE			Visual Studio 2019
 *  EasyX版本	20200902
 */

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

 //台词文本
TCHAR word[24][100] = {
	L"午夜0点。\n新的一天呢。",
	L"〇一〇〇。\n哈欠…我也稍微睡会吧。你还不去休息么？",
	L"〇二〇〇。\n不知道初风睡了没有？雪风睡得正香呢，嗯。",
	L"〇三〇〇。\n你…今天不会打算通宵吧？（我）可不陪你哟。",
	L"〇四〇〇。\n说过不陪你了…真犟…这都早上了。",
	L"〇五〇〇。\n看外面啦。已经早上啦。真是！啊啊…真是的…！",
	L"〇六〇〇。\n话说回来，清晨的风吹拂着心情舒畅…呐，你觉得呢？",
	L"〇七〇〇。\n…神清气爽的风呢。呐，稍微去海边走走么？",
	L"〇八〇〇。\n…来咯，早餐准备好了哟。日式的没问题吧？",
	L"〇九〇〇。\n差不多该准备舰队的正式出航了。你打算怎么安排？",
	L"一〇〇〇。\n岛风！？…嗯，是呢。算、算是朋友啊。…怎么？干嘛这种表情看着我？",
	L"一一〇〇。\n什么？连装炮君？可爱吧，一直乖乖的坐在舰首。",
	L"一二〇〇。\n午餐吃咖喱饭怎么样呢？这可是我的拿手菜哟？真的哟？",
	L"一三〇〇。\n我做的咖喱饭味道如何？嗯嗯？那是当然！",
	L"一四〇〇。\n我和岛风谁的速度更快？我又没和她比过不好说啊。",
	L"一五〇〇。\n因为我是原型舰。所以用我做过很多实验的。速度绝对没有问题的哟。",
	L"一六〇〇。\n奇怪，连装炮君有点闹情绪…怎么回事？",
	L"一七〇〇。\n晚风也是很舒服呢。…过来这边、陪我一起看夕阳好么？",
	L"一八〇〇。\n快到晚餐的时间呢。烧鱼怎么样？",
	L"一九〇〇。\n…给，晚餐。…怎么样，好吃么…是么，那就好！",
	L"二〇〇〇。\n啊，岛风很精神嘛？哎，什么！？现在比试！？不要啦已经晚上了！",
	L"二一〇〇。\n…所以说啦现在不想比试了啦！岛风你很烦啊啊！",
	L"二二〇〇。\n终、终于回去了。真是的…想干什么啊！",
	L"二三〇〇。\n今天就要结束了。辛苦你了，明天见。"
};
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
	drawtext(word[h1 * 10 + h2], &prect, DT_WORDBREAK);
}
//显示音量按键
void volumeText(int volume)
{
	textStyle(L"微软雅黑", 30, RGB(186, 32, 32));
	TCHAR volumetext[] = _T("音量 7");
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
	displayWords(h1 - 48, h2 - 48);
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
		Sleep(50);
	}

	system("pause");
	closegraph();
	return 0;
}