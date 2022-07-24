# -*- coding: utf-8 -*-
"""
Created on Thu Feb 24 16:19:48 2022

@author: ljc545w
"""

# Before use,execute `CWeChatRebot.exe /regserver` in cmd by admin user
# need `pip install comtypes`
import comtypes.client
from ctypes import wintypes
import ast
import os
import socketserver
import threading
from comtypes.client import GetEvents
from comtypes.client import PumpEvents

class _WeChatRobotClient():
    _instance = None
    
    @classmethod
    def instance(cls):
        if not cls._instance:
            cls._instance = cls()
        return cls._instance
    
    def __init__(self):
        self.robot = comtypes.client.CreateObject("WeChatRobot.CWeChatRobot")
        self.event = comtypes.client.CreateObject("WeChatRobot.RobotEvent")
        self.cpid = self.robot.CStopRobotService(0)
    
    @classmethod
    def __del__(cls):
        import psutil
        if cls._instance is not None:
            try:
                cprocess = psutil.Process(cls._instance.cpid)
                cprocess.kill()
            except psutil.NoSuchProcess:
                pass
        cls._instance = None

class WeChatEventSink():
    """
    接收消息的默认回调，可以自定义，并将实例化对象作为StartReceiveMsgByEvent参数
    自定义的类需要包含以下所有成员
    """
    def OnGetMessageEvent(self,msg,*args,**kwargs):
        print(msg)

class ReceviveMsgBaseServer(socketserver.BaseRequestHandler):
    def __init__(self,*args,**kwargs):
        super().__init__(*args,**kwargs)
        
    class ReceiveMsgStruct(comtypes.Structure):
            _fields_ = [("pid",wintypes.DWORD),
                        ("type", wintypes.DWORD),
                        ("isSendMsg", wintypes.DWORD),
                        ("sender",comtypes.c_wchar * 80),
                        ("wxid",comtypes.c_wchar * 80),
                        ("message",comtypes.c_wchar * 0x1000B),
                        ("filepath",comtypes.c_wchar * 260),
                        ("time",comtypes.c_wchar * 30)
                        ]
        
    def handle(self):
        conn = self.request
        comtypes.CoInitialize()
        while True:
            try:
                ptrdata = conn.recv(1024)
                try:
                    if ptrdata.decode() == 'bye':
                        break
                except:
                    pass
                while len(ptrdata) < comtypes.sizeof(self.ReceiveMsgStruct):
                    data = conn.recv(1024)
                    if len(data) == 0:
                        break
                    ptrdata += data
                if ptrdata:
                    pReceiveMsgStruct = comtypes.cast(ptrdata,comtypes.POINTER(self.ReceiveMsgStruct))
                    self.msgcallback(pReceiveMsgStruct.contents)
                response = "200 OK"
                conn.sendall(response.encode())
            except OSError:
                break
            except Exception as e:
                print(e)
                conn.sendall("200 OK".encode())
        conn.close()
        comtypes.CoUninitialize()
        
    def msgcallback(self,data):
        # 主线程中已经注入，此处禁止调用StartService和StopService
        msg = {'pid':data.pid,'time':data.time,'type':data.type,'isSendMsg':data.isSendMsg,'wxid':data.wxid,
               'sendto' if data.isSendMsg else 'from':data.sender,'message':data.message}
        robot = comtypes.client.CreateObject("WeChatRobot.CWeChatRobot")
        event = comtypes.client.CreateObject("WeChatRobot.RobotEvent")
        wx = WeChatRobot(data.pid,robot,event)
        userinfo = wx.GetWxUserInfo(data.wxid)
        msg['alias'] = userinfo['wxNumber']
        if data.isSendMsg == 0:
            if '@chatroom' in data.sender:
                chatroominfo = wx.GetWxUserInfo(data.sender)
                msg['chatroomname'] = chatroominfo['wxNickName']
                msg['nickname'] = wx.GetChatRoomMemberNickname(data.sender, data.wxid)
            else:
                msg['nickname'] = userinfo['wxNickName']
        # TODO: 在这里写额外的消息处理逻辑
        
        print(msg)
        robot.Release()
        event.Release()
        

class ChatSession():
    def __init__(self,pid,robot,wxid):
        self.pid = pid
        self.robot = robot
        self.chatwith = wxid
        
    def SendText(self,msg):
        return self.robot.CSendText(self.pid,self.chatwith,msg)
        
    def SendImage(self,imgpath):
        return self.robot.CSendImage(self.pid,self.chatwith,imgpath)
    
    def SendFile(self,filepath):
        return self.robot.CSendFile(self.pid,self.chatwith,filepath)
    
    def SendMp4(self,mp4path):
        return self.robot.CSendImage(self.pid,self.chatwith,mp4path)
        
    def SendArticle(self,title,abstract,url,imgpath = None):
        return self.robot.CSendArticle(self.pid,self.chatwith,title,abstract,url,imgpath)
    
    def SendCard(self,sharedwxid,nickname):
        return self.robot.CSendCard(self.pid,self.chatwith,sharedwxid,nickname)
    
    def SendAtText(self,wxid:list or str or tuple,msg,AutoNickName = True):
        if '@chatroom' not in self.chatwith:
            return 1
        return self.robot.CSendAtText(self.pid,self.chatwith,wxid,msg,AutoNickName)

    def SendAppMsg(self,appid):
        return self.robot.CSendAppMsg(self.pid,self.chatwith,appid)

class WeChatRobot():
    
    def __init__(self,pid:int = 0,robot = None,event = None):
        self.pid = pid
        self.robot = robot or _WeChatRobotClient.instance().robot
        self.event = event or _WeChatRobotClient.instance().event
        self.AddressBook = []
        self.myinfo = {}
        
    def StartService(self) -> int:
        """
        注入DLL到微信以启动服务

        Returns
        -------
        int
            0成功,非0失败.

        """
        status = self.robot.CStartRobotService(self.pid)
        return status
    
    def IsWxLogin(self) -> int:
        """
        获取微信登录状态

        Returns
        -------
        bool
            微信登录状态.

        """
        return self.robot.CIsWxLogin(self.pid)

    def SendText(self,receiver:str,msg:str) -> int:
        """
        发送文本消息

        Parameters
        ----------
        receiver : str
            消息接收者wxid.
        msg : str
            消息内容.

        Returns
        -------
        int
            0成功,非0失败.

        """
        return self.robot.CSendText(self.pid,receiver,msg)
        
    def SendImage(self,receiver:str,imgpath:str) -> int:
        """
        发送图片消息

        Parameters
        ----------
        receiver : str
            消息接收者wxid.
        imgpath : str
            图片绝对路径.

        Returns
        -------
        int
            0成功,非0失败.

        """
        return self.robot.CSendImage(self.pid,receiver,imgpath)
    
    def SendFile(self,receiver:str,filepath:str) -> int:
        """
        发送文件

        Parameters
        ----------
        receiver : str
            消息接收者wxid.
        filepath : str
            文件绝对路径.

        Returns
        -------
        int
            0成功,非0失败.

        """
        return self.robot.CSendFile(self.pid,receiver,filepath)
        
    def SendArticle(self,receiver:str,title:str,abstract:str,url:str,imgpath:str or None = None) -> int:
        """
        发送XML文章

        Parameters
        ----------
        receiver : str
            消息接收者wxid.
        title : str
            消息卡片标题.
        abstract : str
            消息卡片摘要.
        url : str
            文章链接.
        imgpath : str or None, optional
            消息卡片显示的图片绝对路径，不需要可以不指定. The default is None.

        Returns
        -------
        int
            0成功,非0失败.

        """
        return self.robot.CSendArticle(self.pid,receiver,title,abstract,url,imgpath)
    
    def SendCard(self,receiver:str,sharedwxid:str,nickname:str) -> int:
        """
        发送名片

        Parameters
        ----------
        receiver : str
            消息接收者wxid.
        sharedwxid : str
            被分享人wxid.
        nickname : str
            名片显示的昵称.

        Returns
        -------
        int
            0成功,非0失败.

        """
        return self.robot.CSendCard(self.pid,receiver,sharedwxid,nickname)
    
    def SendAtText(self,chatroomid:str,AtUsers:list or str or tuple,msg:str,AutoNickName:bool = True) -> int:
        """
        发送群艾特消息，艾特所有人可以将AtUsers设置为`notify@all`
        无目标群管理权限请勿使用艾特所有人
        Parameters
        ----------
        chatroomid : str
            群聊ID.
        AtUsers : list or str or tuple
            被艾特的人列表.
        msg : str
            消息内容.
        AutoNickName : bool, optional
            是否自动填充被艾特人昵称. 默认自动填充.

        Returns
        -------
        int
            0成功,非0失败.

        """
        if '@chatroom' not in chatroomid:
            return 1
        return self.robot.CSendAtText(self.pid,chatroomid,AtUsers,msg,AutoNickName)

    def GetSelfInfo(self) -> dict:
        """
        获取个人信息

        Returns
        -------
        dict
            调用成功返回个人信息，否则返回空字典.

        """
        myinfo = self.robot.CGetSelfInfo(self.pid).replace('\n','\\n')
        try:
            myinfo = ast.literal_eval(myinfo)
        except SyntaxError:
            return {}
        self.myinfo = myinfo
        return self.myinfo
    
    def StopService(self) -> int:
        """
        停止服务，会将DLL从微信进程中卸载

        Returns
        -------
        int
            COM进程pid.

        """
        cpid = self.robot.CStopRobotService(self.pid)
        return cpid
    
    def GetAddressBook(self) -> list:
        """
        获取联系人列表

        Returns
        -------
        list
            调用成功返回通讯录列表，调用失败返回空列表.

        """
        try:
            FriendTuple = self.robot.CGetFriendList(self.pid)
            self.AddressBook = [dict(i) for i in list(FriendTuple)]
        except IndexError:
            self.AddressBook = []
        return self.AddressBook
    
    def GetFriendList(self) -> list:
        """
        从通讯录列表中筛选出好友列表

        Returns
        -------
        list
            好友列表.

        """
        if not self.AddressBook:
            self.GetAddressBook()
        FriendList = []
        for item in self.AddressBook:
            if 'wxid_' == item['wxid'][0:5]:
                FriendList.append(item)
        return FriendList
    
    def GetChatRoomList(self) -> list:
        """
        从通讯录列表中筛选出群聊列表

        Returns
        -------
        list
            群聊列表.

        """
        if not self.AddressBook:
            self.GetAddressBook()
        ChatRoomList = []
        for item in self.AddressBook:
            if '@chatroom' in item['wxid']:
                ChatRoomList.append(item)
        return ChatRoomList
    
    def GetOfficialAccountList(self) -> list:
        """
        从通讯录列表中筛选出公众号列表

        Returns
        -------
        list
            公众号列表.

        """
        if not self.AddressBook:
            self.GetAddressBook()
        OfficialAccountList = []
        for item in self.AddressBook:
            if 'wxid_' != item['wxid'][0:5] and '@chatroom' not in item['wxid']:
                OfficialAccountList.append(item)
        return OfficialAccountList
    
    def GetFriendByWxRemark(self,remark:str) -> dict or None:
        """
        通过备注搜索联系人

        Parameters
        ----------
        remark : str
            好友备注.

        Returns
        -------
        dict or None
            搜索到返回联系人信息，否则返回None.

        """
        if not self.AddressBook:
            self.GetAddressBook()
        for item in self.AddressBook:
            if item['wxRemark'] == remark:
                return item
        return None
    
    def GetFriendByWxNumber(self,wxnumber:str) -> dict or None:
        """
        通过微信号搜索联系人

        Parameters
        ----------
        wxnumber : str
            联系人微信号.

        Returns
        -------
        dict or None
            搜索到返回联系人信息，否则返回None.

        """
        if not self.AddressBook:
            self.GetAddressBook()
        for item in self.AddressBook:
            if item['wxNumber'] == wxnumber:
                return item
        return None
    
    def GetFriendByWxNickName(self,wxnickname:str) -> dict or None:
        """
        通过昵称搜索联系人

        Parameters
        ----------
        wxnickname : str
            联系人昵称.

        Returns
        -------
        dict or None
            搜索到返回联系人信息，否则返回None.

        """
        if not self.AddressBook:
            self.GetAddressBook()
        for item in self.AddressBook:
            if item['wxNickName'] == wxnickname:
                return item
        return None
    
    def GetChatSession(self,wxid:str) -> 'ChatSession':
        """
        创建一个会话，没太大用处

        Parameters
        ----------
        wxid : str
            联系人wxid.

        Returns
        -------
        'ChatSession'
            返回ChatSession类.

        """
        return ChatSession(self.pid,self.robot, wxid)
        
    def GetWxUserInfo(self,wxid:str) -> dict:
        """
        通过wxid查询联系人信息

        Parameters
        ----------
        wxid : str
            联系人wxid.

        Returns
        -------
        dict
            联系人信息.

        """
        userinfo = self.robot.CGetWxUserInfo(self.pid,wxid).replace('\n','\\n')
        return ast.literal_eval(userinfo)
    
    def GetChatRoomMembers(self,chatroomid:str) -> list:
        """
        获取群成员信息

        Parameters
        ----------
        chatroomid : str
            群聊id.

        Returns
        -------
        list
            群成员信息.

        """
        info = dict(self.robot.CGetChatRoomMembers(self.pid,chatroomid))
        if not info:
            return None
        members = info['members'].split('^G')
        data = self.GetWxUserInfo(chatroomid)
        data['members'] = []
        for member in members:
            memberinfo = self.GetWxUserInfo(self.pid,member)
            data['members'].append(memberinfo)
        return data
    
    def CheckFriendStatus(self,wxid:str) -> int:
        """
        获取好友状态码

        Parameters
        ----------
        wxid : str
            好友wxid.

        Returns
        -------
        int
            0x0: 'Unknown',
            0xB0:'被删除',
            0xB1:'是好友',
            0xB2:'已拉黑',
            0xB5:'被拉黑',

        """
        return self.robot.CCheckFriendStatus(self.pid,wxid)
    
    # 接收消息的函数
    def StartReceiveMessage(self,port:int = 10808) -> int:
        """
        启动接收消息Hook

        Parameters
        ----------
        port : int
            socket的监听端口号.如果要使用连接点回调，则将端口号设置为0.

        Returns
        -------
        int
            启动成功返回0,失败返回非0值.

        """
        status = self.robot.CStartReceiveMessage(self.pid,port)
        return status
    
    def StopReceiveMessage(self) -> int:
        """
        停止接收消息Hook

        Returns
        -------
        int
            成功返回0,失败返回非0值.

        """
        status = self.robot.CStopReceiveMessage(self.pid)
        return status
    
    def GetDbHandles(self) -> dict:
        """
        获取数据库句柄和表信息

        Returns
        -------
        dict
            数据库句柄和表信息.

        """
        tablesTuple = self.robot.CGetDbHandles(self.pid)
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
    
    def ExecuteSQL(self,handle:int,sql:str) -> list:
        """
        执行SQL

        Parameters
        ----------
        handle : int
            数据库句柄.
        sql : str
            SQL.

        Returns
        -------
        list
            查询结果.

        """
        result = self.robot.CExecuteSQL(self.pid,handle,sql)
        if len(result) == 0:
            return []
        query_list = []
        keys = list(result[0])
        for item in result[1:]:
            query_dict = {}
            for key,value in zip(keys,item):
                query_dict[key] = value if not isinstance(value, tuple) else bytes(value)
            query_list.append(query_dict)
        return query_list
    
    def BackupSQLiteDB(self,handle:int,BackupFile:int) -> int:
        """
        备份数据库

        Parameters
        ----------
        handle : int
            数据库句柄.
        BackupFile : int
            备份文件保存位置.

        Returns
        -------
        int
            成功返回0,失败返回非0值.

        """
        BackupFile = BackupFile.replace('/','\\')
        savepath = BackupFile.replace(BackupFile.split('\\')[-1],'')
        if not os.path.exists(savepath):
            os.makedirs(savepath)
        return self.robot.CBackupSQLiteDB(self.pid,handle,BackupFile)
        
    def VerifyFriendApply(self,v3:str,v4:str) -> int:
        """
        通过好友请求

        Parameters
        ----------
        v3 : str
            v3数据(encryptUserName).
        v4 : str
            v4数据(ticket).

        Returns
        -------
        int
            成功返回0,失败返回非0值..

        """
        return self.robot.CVerifyFriendApply(self.pid,v3,v4)
    
    def AddFriendByWxid(self,wxid:str,message:str or None) -> int:
        """
        wxid加好友

        Parameters
        ----------
        wxid : str
            要添加的wxid.
        message : str or None
            验证信息.

        Returns
        -------
        int
            请求发送成功返回0,失败返回非0值.

        """
        return self.robot.CAddFriendByWxid(self.pid,wxid,message)
    
    def AddFriendByV3(self,v3:str,message:str or None,AddType:int) -> int:
        """
        v3数据加好友

        Parameters
        ----------
        v3 : str
            v3数据(encryptUserName).
        message : str or None
            验证信息.
        AddType : int
            添加方式(来源).手机号: 0xF;微信号: 0x3;QQ号: 0x1;朋友验证消息: 0x6.

        Returns
        -------
        int
            请求发送成功返回0,失败返回非0值.

        """
        return self.robot.CAddFriendByV3(self.pid,v3,message,AddType)
    
    def GetWeChatVer(self) -> str:
        """
        获取微信版本号

        Returns
        -------
        str
            微信版本号.

        """
        return self.robot.CGetWeChatVer()
    
    def GetUserInfoByNet(self,keyword:str) -> dict or None:
        """
        网络查询用户信息

        Parameters
        ----------
        keyword : str
            查询关键字，可以是微信号、手机号、QQ号.

        Returns
        -------
        dict or None
            查询成功返回用户信息,查询失败返回None.

        """
        userinfo = self.robot.CSearchContactByNet(self.pid,keyword)
        if userinfo:
            return dict(userinfo)
        return None
    
    def AddBrandContact(self,PublicId:str) -> int:
        """
        关注公众号

        Parameters
        ----------
        PublicId : str
            公众号id.

        Returns
        -------
        int
            请求成功返回0,失败返回非0值.

        """
        return self.robot.CAddBrandContact(self.pid,PublicId)
    
    def ChangeWeChatVer(self,version:str) -> int:
        """
        自定义微信版本号，一定程度上防止自动更新

        Parameters
        ----------
        version : str
            版本号，类似`3.7.0.26`

        Returns
        -------
        int
            成功返回0,失败返回非0值.

        """
        return self.robot.CChangeWeChatVer(self.pid,version)
    
    def HookImageMsg(self,savepath:str) -> int:
        """
        开始Hook未加密图片

        Parameters
        ----------
        savepath : str
            图片保存路径(绝对路径).

        Returns
        -------
        int
            成功返回0,失败返回非0值.

        """
        return self.robot.CHookImageMsg(self.pid,savepath)
    
    def UnHookImageMsg(self) -> int:
        """
        取消Hook未加密图片

        Returns
        -------
        int
            成功返回0,失败返回非0值.

        """
        return self.robot.CUnHookImageMsg(self.pid)
    
    def HookVoiceMsg(self,savepath:str) -> int:
        """
        开始Hook语音消息

        Parameters
        ----------
        savepath : str
            语音保存路径(绝对路径).

        Returns
        -------
        int
            成功返回0,失败返回非0值.

        """
        return self.robot.CHookVoiceMsg(self.pid,savepath)
    
    def UnHookVoiceMsg(self) -> int:
        """
        取消Hook语音消息

        Returns
        -------
        int
            成功返回0,失败返回非0值.

        """
        return self.robot.CUnHookVoiceMsg(self.pid)

    def DeleteUser(self,wxid:str) -> int:
        """
        删除好友

        Parameters
        ----------
        wxid : str
            被删除好友wxid.

        Returns
        -------
        int
            成功返回0,失败返回非0值.

        """
        return self.robot.CDeleteUser(self.pid,wxid)

    def SendAppMsg(self,wxid:str,appid:str) -> int:
        """
        发送小程序

        Parameters
        ----------
        wxid : str
            消息接收者wxid.
        appid : str
            小程序id (在xml中是username，不是appid).

        Returns
        -------
        int
            成功返回0,失败返回非0值.

        """
        return self.robot.CSendAppMsg(self.pid,wxid,appid)
    
    def EditRemark(self,wxid:str,remark:str or None) -> int:
        """
        修改好友或群聊备注

        Parameters
        ----------
        wxid : str
            wxid或chatroomid.
        remark : str or None
            要修改的备注.

        Returns
        -------
        int
            成功返回0,失败返回非0值.

        """
        return self.robot.CEditRemark(self.pid,wxid,remark)
    
    def SetChatRoomName(self,chatroomid:str,name:str) -> int:
        """
        修改群名称.请确认具有相关权限再调用。

        Parameters
        ----------
        chatroomid : str
            群聊id.
        name : str
            要修改为的群名称.

        Returns
        -------
        int
            成功返回0,失败返回非0值.

        """
        return self.robot.CSetChatRoomName(self.pid,chatroomid,name)
    
    def SetChatRoomAnnouncement(self,chatroomid:str,announcement:str or None) -> int:
        """
        设置群公告.请确认具有相关权限再调用。

        Parameters
        ----------
        chatroomid : str
            群聊id.
        announcement : str or None
            公告内容.

        Returns
        -------
        int
            成功返回0,失败返回非0值.

        """
        return self.robot.CSetChatRoomAnnouncement(self.pid,chatroomid,announcement)
    
    def SetChatRoomSelfNickname(self,chatroomid:str,nickname:str) -> int:
        """
        设置群内个人昵称

        Parameters
        ----------
        chatroomid : str
            群聊id.
        nickname : str
            要修改为的昵称.

        Returns
        -------
        int
            成功返回0,失败返回非0值.

        """
        return self.robot.CSetChatRoomSelfNickname(self.pid,chatroomid,nickname)
    
    def GetChatRoomMemberNickname(self,chatroomid:str,wxid:str) -> str:
        """
        获取群成员昵称

        Parameters
        ----------
        chatroomid : str
            群聊id.
        wxid : str
            群成员wxid.

        Returns
        -------
        str
            成功返回群成员昵称,失败返回空字符串.

        """
        return self.robot.CGetChatRoomMemberNickname(self.pid,chatroomid,wxid)
    
    def DelChatRoomMember(self,chatroomid:str,wxids:str or list or tuple) -> str:
        """
        删除群成员.请确认具有相关权限再调用。

        Parameters
        ----------
        chatroomid : str
            群聊id.
        wxids : str or list or tuple
            要删除的成员wxid或wxid列表.

        Returns
        -------
        int
            成功返回0,失败返回非0值.

        """
        return self.robot.CDelChatRoomMember(self.pid,chatroomid,wxids)
    
    def AddChatRoomMember(self,chatroomid:str,wxids:str or list or tuple) -> str:
        """
        添加群成员.请确认具有相关权限再调用。

        Parameters
        ----------
        chatroomid : str
            群聊id.
        wxids : str or list or tuple
            要添加的成员wxid或wxid列表.

        Returns
        -------
        int
            成功返回0,失败返回非0值.

        """
        return self.robot.CAddChatRoomMember(self.pid,chatroomid,wxids)
    
def GetWeChatPids() -> list:
    """
    获取所有微信pid

    Returns
    -------
    list
        微信pid列表.

    """
    import psutil
    pids = []
    process_list = psutil.pids()
    for pid in process_list:
        if psutil.Process(pid).name() == 'WeChat.exe':
            pids.append(pid)
    return pids
    
def StartWeChat() -> 'WeChatRobot':
    """
    启动微信

    Returns
    -------
    WeChatRobot
        成功返回WeChatRobot对象,失败返回False.

    """
    pid = _WeChatRobotClient.instance().robot.CStartWeChat()
    if pid != 0:
        return WeChatRobot(pid)
    return False

def RegisterMsgEvent(EventSink:'WeChatEventSink' or None = None) -> None:
    """
    通过COM组件连接点接收消息，真正的回调

    Parameters
    ----------
    EventSink : object, optional
        回调的实现类，该类要继承`WeChatEventSink`类或实现其中的方法.

    Returns
    -------
    None
        .

    """
    event = _WeChatRobotClient.instance().event
    if event is not None:
        sink = EventSink or WeChatEventSink()
        ConnectionPoint = GetEvents(event,sink)
        assert ConnectionPoint != None
        while True:
            try:
                PumpEvents(2)
            except:
                break
        del ConnectionPoint
        
def StartSocketServer(port:int = 10808,
                      RequestHandler: 'ReceviveMsgBaseServer' = ReceviveMsgBaseServer,
                      mainThread = True) -> int or None:
    """
    创建消息监听线程

    Parameters
    ----------
    port : int
        socket的监听端口号.
        
    RequestHandler : ReceviveMsgBaseServer
        用于处理消息的类，需要继承自socketserver.BaseRequestHandler或ReceviveMsgBaseServer
        
    mainThread : bool
        是否在主线程中启动server

    Returns
    -------
    int or None
        mainThread为False时返回线程id,否则返回None.

    """
    ip_port=("127.0.0.1",port)
    try:
        s = socketserver.ThreadingTCPServer(ip_port,RequestHandler)
        if mainThread:
            s.serve_forever()
        else:
            socket_server = threading.Thread(target = s.serve_forever)
            socket_server.setDaemon(True)
            socket_server.start()
            return socket_server.ident
    except KeyboardInterrupt:
        pass
    except Exception as e:
        print(e)
    return None
        
def StopSocketServer(threadid:int) -> None:
    """
    强制结束消息监听线程

    Parameters
    ----------
    threadid : int
        消息监听线程ID.

    Returns
    -------
    None
        .

    """
    if not threadid:
        return
    import inspect
    try:
        tid = comtypes.c_long(threadid)
        if not inspect.isclass(SystemExit):
            exctype = type(SystemExit)
        res = comtypes.pythonapi.PyThreadState_SetAsyncExc(tid, comtypes.py_object(exctype))
        if res == 0:
            raise ValueError("invalid thread id")
        elif res != 1:
            comtypes.ctypes.pythonapi.PyThreadState_SetAsyncExc(tid, None)
            raise SystemError("PyThreadState_SetAsyncExc failed")
    except:
        pass