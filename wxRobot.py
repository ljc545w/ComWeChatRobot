# -*- coding: utf-8 -*-
"""
Created on Thu Feb 24 16:19:48 2022

@author: ljc545w
"""

# Before use,execute `CWeChatRebot.exe /regserver` in cmd by admin user
import comtypes.client
import ast

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
        

class WeChatRobot():
    
    def __init__(self,dllpath):
        self.robot = comtypes.client.CreateObject("WeChatRobot.CWeChatRobot")
        self.dllpath = dllpath
        self.AddressBook = []
        
    def StartService(self):
        return self.robot.CStartRobotService(self.dllpath)
        
    def StopService(self):
        return self.robot.CStopRobotService()
    
    def GetAddressBook(self):
        AddressBookString = wx.robot.CGetFriendList()
        AddressBookString = AddressBookString.replace("\n","\\n")
        self.AddressBook = ast.literal_eval(AddressBookString)
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


if __name__ == '__main__':
    # DWeChatRobot.dll path
    dllpath = r'D:\VS2019C++\MyWeChatRobot\Release'
    # image full path
    imgpath = r"C:\Users\Administrator\Desktop\快捷\wechat\测试图片.jpg"
    # file full path
    filepath = r"C:\Users\Administrator\Desktop\快捷\wechat\MyWeChatRobot.zip"
    # mp4 full path
    mp4path = r"C:\Users\Administrator\Desktop\快捷\wechat\wxsend.mp4"

    wx = WeChatRobot(dllpath)
    wx.StartService()

    me = wx.GetFriendByWxNickName("文件传输助手")
    session = wx.GetChatSession(me.get('wxid'))

    session.SendText('来自python的消息')
    session.SendImage(imgpath)
    session.SendFile(filepath)
    session.SendMp4(mp4path)

    wx.StopService()