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
# 注册COM
以管理员权限执行以下命令：  
```shell
# 安装
CWeChatRobot.exe /regserver
# 卸载
CWeChatRobot.exe /unregserver
```
# 调用
Python：  
参考[wxRobot.py](/wxRobot.py)  
C#（参考如下代码）：  
```csharp
System.Type wxProtId = Type.GetTypeFromProgID("WeChatRobot.CWeChatRobot");
if (wxProtId == null)
    return;
string workpath = System.AppDomain.CurrentDomain.BaseDirectory;
string basePath = workpath.Replace("wxRobot\\bin\\Release\\","");
dynamic wx = Activator.CreateInstance(wxProtId);
wx.CStartRobotService();
wx.CSendText("filehelper", "来自C艹艹的消息");
wx.CSendImage("filehelper", basePath + "test\\测试图片.png");
wx.CSendFile("filehelper", basePath + "test\\测试文件");
wx.CSendArticle("filehelper","PC微信逆向--获取通讯录", "确定不来看看么?", "https://www.ljczero.top/article/2022/3/13/133.html");
string selfinfo = wx.CGetSelfInfo();
Console.WriteLine(selfinfo);
wx.CStopRobotService();
```
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
# 已知BUG
~~获取个人信息的接口不能很好的工作，因为需要判断数据是否是一个指针。~~  
~~使用该接口将导致微信崩溃。~~  
已对个人信息接口做了简单的修复，如果还有问题可报issue或搜索报错解决。
# 打赏作者
请给作者一个star，感谢感谢  
# 免责声明
代码仅供交流学习使用，请勿用于非法用途和商业用途！如因此产生任何法律纠纷，均与作者无关！
