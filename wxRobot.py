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
        
    def SendArticle(self,title,abstract,url):
        return self.robot.CSendArticle(self.chatwith,title,abstract,url)
    
    def SendCard(self,sharedwxid,nickname):
        return self.robot.CSendCard(self.chatwith,sharedwxid,nickname)
        

class WeChatRobot():
    
    def __init__(self,dllpath):
        self.robot = comtypes.client.CreateObject("WeChatRobot.CWeChatRobot")
        self.dllpath = dllpath
        self.AddressBook = []
        self.myinfo = {}
        
    def StartService(self):
        status = self.robot.CStartRobotService(self.dllpath)
        return status

    # 有bug待修复，需要判断某项信息是否是指针，修复前不要使用
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
        return self.robot.CStopRobotService()
    
    def GetAddressBook(self):
        AddressBookString = self.robot.CGetFriendList()
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
        
    def GetWxUserInfo(self,wxid):
        userinfo = self.robot.CGetWxUserInfo(wxid).replace('\n','\\n')
        return ast.literal_eval(userinfo)
        
def test():
    import os
    # DWeChatRobot.dll path
    path = os.path.split(os.path.realpath(__file__))[0]
    dllpath = os.path.join(path,'Release')
    # image full path
    imgpath = os.path.join(path,'test\\测试图片.png')
    # file full path
    filepath = os.path.join(path,'test\\测试文件')
    wx = WeChatRobot(dllpath)
    wx.StartService()
    myinfo = wx.GetSelfInfo()
    chatwith = wx.GetFriendByWxNickName("文件传输助手")
    session = wx.GetChatSession(chatwith.get('wxid'))
    filehelper = wx.GetWxUserInfo(chatwith.get('wxid'))
    session.SendText('个人信息：{}'.format(str(myinfo.get('wxNickName'))))
    session.SendText('好友信息：{}'.format(str(filehelper.get('wxNickName'))))
    if os.path.exists(imgpath): session.SendImage(imgpath)
    if os.path.exists(filepath): session.SendFile(filepath)
    session.SendArticle("PC微信逆向--获取通讯录","确定不来看看么?","https://www.ljczero.top/article/2022/3/13/133.html")
    shared = wx.GetFriendByWxNickName("小冰的宇宙")
    if shared:
        session.SendCard(shared.get('wxid'),shared.get('wxNickName'))
    wx.StopService()

if __name__ == '__main__':
    test()
    