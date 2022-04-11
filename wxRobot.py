# -*- coding: utf-8 -*-
"""
Created on Thu Feb 24 16:19:48 2022

@author: ljc545w
"""

# Before use,execute `CWeChatRebot.exe /regserver` in cmd by admin user
# need `pip install comtypes`
import comtypes.client
import ast
import threading
import time

class ChatSession():
    def __init__(self,robot,wxid):
        self.robot = robot
        self.chatwith = wxid
        
    def SendText(self,msg):
        return self.robot.CSendText(self.chatwith,msg)
        
    def SendImage(self,imgpath):
        return self.robot.CSendImage(self.chatwith,imgpath)
    
    def SendFile(self,filepath):
        return self.robot.CSendFile(self.chatwith,filepath)
    
    # 其实发送图片的函数比较智能，也可以发送视频和文件
    def SendMp4(self,mp4path):
        return self.robot.CSendImage(self.chatwith,mp4path)
        
    def SendArticle(self,title,abstract,url):
        return self.robot.CSendArticle(self.chatwith,title,abstract,url)
    
    def SendCard(self,sharedwxid,nickname):
        return self.robot.CSendCard(self.chatwith,sharedwxid,nickname)
        

class WeChatRobot():
    
    def __init__(self):
        self.robot = comtypes.client.CreateObject("WeChatRobot.CWeChatRobot")
        self.AddressBook = []
        self.myinfo = {}
        self.ReceiveMessageStarted = False
        
    def StartService(self):
        status = self.robot.CStartRobotService()
        return status

    def GetSelfInfo(self):
        myinfo = self.robot.CGetSelfInfo().replace('\n','\\n')
        try:
            myinfo = ast.literal_eval(myinfo)
        except SyntaxError:
            return {}
        myinfo['wxBigAvatar'] = myinfo['wxBigAvatar'].replace("/132","/0")
        self.myinfo = myinfo
        return self.myinfo
        
    def StopService(self):
        self.StopReceiveMessage()
        return self.robot.CStopRobotService()
    
    def GetAddressBook(self):
        try:
            FriendTuple = self.robot.CGetFriendList()
            self.AddressBook = [dict(i) for i in list(FriendTuple)]
        except IndexError:
            self.AddressBook = []
        return self.AddressBook
    
    def GetFriendList(self):
        if not self.AddressBook:
            self.GetAddressBook()
        FriendList = []
        for item in self.AddressBook:
            if 'wxid_' == item['wxid'][0:5]:
                FriendList.append(item)
        return FriendList
    
    def GetChatRoomList(self):
        if not self.AddressBook:
            self.GetAddressBook()
        ChatRoomList = []
        for item in self.AddressBook:
            if '@chatroom' in item['wxid']:
                ChatRoomList.append(item)
        return ChatRoomList
    
    def GetOfficialAccountList(self):
        if not self.AddressBook:
            self.GetAddressBook()
        OfficialAccountList = []
        for item in self.AddressBook:
            if 'wxid_' != item['wxid'][0:5] and '@chatroom' not in item['wxid']:
                OfficialAccountList.append(item)
        return OfficialAccountList
    
    def GetFriendByWxRemark(self,remark:str):
        if not self.AddressBook:
            self.GetAddressBook()
        for item in self.AddressBook:
            if item['wxRemark'] == remark:
                return item
        return None
    
    def GetFriendByWxNumber(self,wxnumber:str):
        if not self.AddressBook:
            self.GetAddressBook()
        for item in self.AddressBook:
            if item['wxNumber'] == wxnumber:
                return item
        return None
    
    def GetFriendByWxNickName(self,wxnickname:str):
        if not self.AddressBook:
            self.GetAddressBook()
        for item in self.AddressBook:
            if item['wxNickName'] == wxnickname:
                return item
        return None
    
    def GetChatSession(self,wxid):
        return ChatSession(self.robot, wxid)
        
    def GetWxUserInfo(self,wxid):
        userinfo = self.robot.CGetWxUserInfo(wxid).replace('\n','\\n')
        return ast.literal_eval(userinfo)
    
    def CheckFriendStatusInit(self):
        return self.robot.CCheckFriendStatusInit()
    
    def CheckFriendStatusFinish(self):
        return self.robot.CCheckFriendStatusFinish()
    
    def CheckFriendStatus(self,wxid):
        _EnumFriendStatus = {
            0xB0:'被删除',
            0xB1:'是好友',
            0xB2:'已拉黑',
            0xB5:'被拉黑',
            }
        status = self.robot.CCheckFriendStatus(wxid)
        if status == 0x0:
            print('请先初始化再进行检测！')
            assert False
        try:
            return _EnumFriendStatus[status]
        except KeyError:
            return "未知状态：{}".format(
                    hex(status).upper().replace('0X','0x'))
        
    def ReceiveMessage(self,CallBackFunc = None):
        comtypes.CoInitialize()
        ThreadRobot = comtypes.client.CreateObject("WeChatRobot.CWeChatRobot")
        
        while self.ReceiveMessageStarted:
            try:
                message = dict(ThreadRobot.CReceiveMessage())
                if CallBackFunc:
                    CallBackFunc(ThreadRobot,message)
            except IndexError:
                message = None
            time.sleep(0.5)
        comtypes.CoUninitialize()
    
    # 接收消息的函数，可以添加一个回调
    def StartReceiveMessage(self,CallBackFunc = None):
        self.ReceiveMessageStarted = True
        status = self.robot.CStartReceiveMessage()
        self.ReceiveMessageThread = threading.Thread(target = self.ReceiveMessage,args= (CallBackFunc,))
        self.ReceiveMessageThread.daemon = True
        self.ReceiveMessageThread.start()
        return status
        
    def StopReceiveMessage(self):
        self.ReceiveMessageStarted = False
        try:
            self.ReceiveMessageThread.join()
        except:
            pass
        status = self.robot.CStopReceiveMessage()
        return status

# 一个示例回调，将收到的文本消息转发给filehelper
def ReceiveMessageCallBack(robot,message):
    if message['type'] == 1 and message['sender'] != 'filehelper':
        robot.CSendText('filehelper',message['message'])
    if message['sender'] != 'filehelper': print(message)
    
def test_SendText():
    import os
    path = os.path.split(os.path.realpath(__file__))[0]
    # image full path
    imgpath = os.path.join(path,'test\\测试图片.png')
    # file full path
    filepath = os.path.join(path,'test\\测试文件')
    wx = WeChatRobot()
    wx.StartService()
    myinfo = wx.GetSelfInfo()
    chatwith = wx.GetFriendByWxNickName("文件传输助手")
    session = wx.GetChatSession(chatwith.get('wxid'))
    filehelper = wx.GetWxUserInfo(chatwith.get('wxid'))
    session.SendText('个人信息：{}'.format(str(myinfo.get('wxNickName'))))
    session.SendText('好友信息：{}'.format(str(filehelper.get('wxNickName'))))
    if os.path.exists(imgpath): session.SendImage(imgpath)
    if os.path.exists(filepath): session.SendFile(filepath)
    session.SendArticle("天气预报","点击查看","http://www.baidu.com")
    shared = wx.GetFriendByWxNickName("码农翻身")
    if shared: session.SendCard(shared.get('wxid'),shared.get('wxNickName'))
    wx.StopService()
    
def test_FriendStatus():
    f = open('Friendstatus.txt','wt',encoding = 'utf-8')
    wx = WeChatRobot()
    wx.StartService()
    FriendList = wx.GetFriendList()
    wx.CheckFriendStatusInit()
    index = "\t".join(['微信号','昵称','备注','状态','\n'])
    f.writelines(index)
    for Friend in FriendList:
        result = '\t'.join(
            [Friend.get('wxNumber'),Friend.get('wxNickName'),Friend.get('wxRemark'),
              wx.CheckFriendStatus(Friend.get('wxid'))])
        print(result)
        result += '\n'
        f.writelines(result)
        time.sleep(1)
        break
    f.close()
    wx.StopService()
    
def test_ReceiveMessage():
    wx = WeChatRobot()
    wx.StartService()
    wx.StartReceiveMessage(CallBackFunc = ReceiveMessageCallBack)
    input('按Enter可退出')
    wx.StopService()

if __name__ == '__main__':
    test_ReceiveMessage()