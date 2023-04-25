#pragma once

#include<graphics.h>

//mci接口的参数
#define DETAILS NULL,0,NULL

//文字显示的范围（左，上，右，下）
#define TEXTAREA 450,20,630,170

//报时的时间（分：十位，分：个位，秒：十位，秒：个位）
#define ONTIME t[14]=='0'&&t[15]=='0'&&t[17]=='0'&&t[18]=='0'

void textStyle(const TCHAR* fontFamily, int fontSize, COLORREF fontColor); //设置字体样式
void displayWords(char h_chara, char h1, char h2);       //显示台词
void volumeText(int volume);                             //显示音量按键
void changeVolume(int volume);
void timeVoice(char h_chara, char h1, char h2, int volume); //播放整点报时语音
void charaText();                                        //显示角色切换按键

