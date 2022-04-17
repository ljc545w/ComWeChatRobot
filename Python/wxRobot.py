# -*- coding: utf-8 -*-
"""
Created on Thu Feb 24 16:19:48 2022

@author: ljc545w
"""

# Before use,execute `CWeChatRebot.exe /regserver` in cmd by admin user
# need `pip install comtypes`
import comtypes.client
import ast
import os
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
    
    def SendMp4(self,mp4path):
        return self.robot.CSendImage(self.chatwith,mp4path)
        
    def SendArticle(self,title,abstract,url):
        return self.robot.CSendArticle(self.chatwith,title,abstract,url)
    
    def SendCard(self,sharedwxid,nickname):
        return self.robot.CSendCard(self.chatwith,sharedwxid,nickname)
    
    def SendAtText(self,wxid:list or str or tuple,msg):
        if '@chatroom' not in self.chatwith:
            return 1
        return self.robot.CSendAtText(self.chatwith,wxid,msg)
        

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
        import psutil
        self.StopReceiveMessage()
        cpid = self.robot.CStopRobotService()
        try:
            cprocess = psutil.Process(cpid)
            cprocess.kill()
        except psutil.NoSuchProcess:
            pass
        return cpid
    
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
    
    def GetChatRoomMembers(self,chatroomid):
        info = dict(self.robot.CGetChatRoomMembers(chatroomid))
        if not info:
            return None
        members = info['members'].split('^G')
        data = self.GetWxUserInfo(chatroomid)
        data['members'] = []
        for member in members:
            memberinfo = self.GetWxUserInfo(member)
            data['members'].append(memberinfo)
        return data
        
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
            return "未知状态：{}".format(hex(status).upper().replace('0X','0x'))
        
    def ReceiveMessage(self,CallBackFunc = None):
        comtypes.CoInitialize()
        # 线程中必须新建一个对象，但无需重复注入
        ThreadRobot = WeChatRobot()
        while self.ReceiveMessageStarted:
            try:
                message = dict(ThreadRobot.robot.CReceiveMessage())
                if CallBackFunc and message:
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
    
    def GetDbHandles(self):
        tablesTuple = self.robot.CGetDbHandles()
        tables = [dict(i) for i in tablesTuple]
        dbs = {}
        for table in tables:
            dbname = table['dbname']
            if dbname not in dbs.keys():
                dbs[dbname] = {'Handle':table['Handle'],'tables':[]}
            dbs[dbname]['tables'].append(
                {'name': table['name'],'tbl_name': table['tbl_name'],
                 'rootpage': table['rootpage'],'sql': table['sql']}
                )
        return dbs
    
    def ExecuteSQL(self,handle,sql):
        result = self.robot.CExecuteSQL(handle,sql)
        return result
    
    def BackupSQLiteDB(self,handle,BackupFile):
        BackupFile = BackupFile.replace('/','\\')
        savepath = BackupFile.replace(BackupFile.split('\\')[-1],'')
        if not os.path.exists(savepath):
            os.makedirs(savepath)
        return self.robot.CBackupSQLiteDB(handle,BackupFile)

    def StopReceiveMessage(self):
        self.ReceiveMessageStarted = False
        try:
            self.ReceiveMessageThread.join()
        except:
            pass
        status = self.robot.CStopReceiveMessage()
        return status