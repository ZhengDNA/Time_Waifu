###将txt文件按行数分成多个txt文件
#encoding=utf-8意思是编码格式为UTF-8格式，可用于多种语言的字符，包括中文。上一行这种表达，txt文档有中文就报错
open_diff = open('E:/BaiNetDiskDL/GithubDesk/Time_Waifu/words/all.txt', 'r',encoding='ANSI') # 源文本文件
diff_line = open_diff.readlines()
 
line_list = []
for line in diff_line:
    line_list.append(line)
 
count = len(line_list) # 文件行数
n = 1 #按多少行来分割txt文件
print('源文件数据行数：',count) 
# 切分diff
diff_match_split = [line_list[i:i+n] for i in range(0,len(line_list),n)]# 每个文件的数据行数
 
# 将切分的写入多个txt中
for i,j in zip(range(0,int(count/n+1)),range(0,int(count/n+1))): # 写入txt，计算需要写j入的文件数
   if j<=9:
    with open('E:/BaiNetDiskDL/GithubDesk/Time_Waifu/words/0%d.txt'%(j),'w+') as temp:
        for line in diff_match_split[i]:
            temp.write(line)
   else:
    with open('E:/BaiNetDiskDL/GithubDesk/Time_Waifu/words/%d.txt'%(j),'w+') as temp:
        for line in diff_match_split[i]:
            temp.write(line)

print('拆分后文件的个数：',i+1)