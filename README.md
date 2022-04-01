# 描述
PC微信机器人，实现获取通讯录、发送文本、图片、文件、xml文章、分享名片消息  
封装COM接口，可供Python、C#调用
# 可用版本
微信电脑版**3.5.0.46**  
微信电脑版**3.6.0.18**  
主分支对应微信3.6.0.18版本，其他版本请查看对应分支。    
# 编译环境
**Visual Studio 2019**(平台配置：win32(x86))
# 原理
通过逆向PC微信，定位到关键CALL，dll内联汇编调用  
注册32位COM组件，供64位/32位进程外部调用  
# 目录说明
`./CWeChatRobot`：COM组件的实现代码  
`./DWeChatRobot`：注入的DLL实现代码  
`./wxRobot`:  包含C#的调用示例  
`./wxRobot.py`：python示例文件  
`./Release/CWeChatRobot.exe`：编译的COM组件  
`./Release/DWeChatRobot.dll`：编译的DLL文件  
`./Release/WeChatTools.exe`：用于调试时注入或卸载DLL程序，具体参看相关代码  
# 注册COM
以管理员权限执行以下命令：  
```shell
# 安装
CWeChatRobot.exe /regserver
# 卸载
CWeChatRobot.exe /unregserver
```
# 调用
**Python：**  
参考[wxRobot.py](/wxRobot.py)  
**C#：**  
参考[Program.cs](/wxRobot/Program.cs)
# 更多功能
目前没有添加更多功能的计划  
已找到如下功能的CALL或HOOK位置：  
1. 检测好友状态（已添加的功能）  
2. 文本消息和各类xml消息  
3. 未加密图片数据  
4. 未加密聊天表情  
5. 语音消息（SILK_V3格式）  
6. 微信Duilib界面xml文件  

暂时没有空闲时间继续开发，如果想开发相关功能请发邮件到ljc545w@qq.com  
**也欢迎您提交PR**  
# 更新记录
## 2022.04.01
1. 使用SAFEARRAY返回通讯录列表，可正确显示好友昵称中的特殊符号  
2. README中添加目录说明  
3. 更新C#示例代码，添加好友列表的遍历示例
# 打赏作者
请给作者一个star，感谢感谢  
# 免责声明
代码仅供交流学习使用，请勿用于非法用途和商业用途！如因此产生任何法律纠纷，均与作者无关！
