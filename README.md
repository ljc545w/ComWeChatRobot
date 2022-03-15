# 描述
PC微信机器人，实现获取通讯录，发送文本、图片、文件、xml文章消息，封装COM接口，可供Python、C#调用
# 可用版本
微信电脑版**3.5.0.46**  
(已经不是最新版)
# 编译环境
**Visual Studio 2019**
# 原理
通过逆向PC微信，定位到关键CALL，dll内联汇编调用；注册32位COM组件，供64位/32位进程外部调用
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
参考[wxRobot.py](https://github.com/ljc545w/ComWeChatRobot/blob/master/wxRobot.py)  
C#：  
我不会，但应该可以，不可以的话我在描述中删掉相关内容。  
# 更多功能
发送名片，coming soon...  
(添加发送名片功能后将不再更新)  
代码里还有个保存gif表情的Hook函数，自行探索...
# 已知BUG
获取个人信息的接口不能很好的工作，因为需要判断数据是否是一个指针。  
使用该接口将导致微信崩溃。  
# 未知BUG
多的是...
# 打赏作者
？
# 免责声明
工程仅供交流学习使用，请勿用于非法用途和商业用途！如因此产生任何法律纠纷，均与作者无关！
