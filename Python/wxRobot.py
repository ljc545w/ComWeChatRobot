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
from comtypes.client import GetEvents
from comtypes.client import PumpEvents

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
        
    def SendArticle(self,title,abstract,url,imgpath = None):
        return self.robot.CSendArticle(self.chatwith,title,abstract,url,imgpath)
    
    def SendCard(self,sharedwxid,nickname):
        return self.robot.CSendCard(self.chatwith,sharedwxid,nickname)
    
    def SendAtText(self,wxid:list or str or tuple,msg,AutoNickName = True):
        if '@chatroom' not in self.chatwith:
            return 1
        return self.robot.CSendAtText(self.chatwith,wxid,msg,AutoNickName)

    def SendAppMsg(self,appid):
        return self.robot.CSendAppMsg(self.chatwith,appid)
    
class WeChatEventSink():
    """
    接收消息的默认回调，可以自定义，并将实例化对象作为StartReceiveMsgByEvent参数
    自定义的类需要包含以下所有成员
    """
    def OnGetMessageEvent(self,msg,*args,**kwargs):
        # 为了兼容原有接口，需要在接收到广播后调用此接口，否则会导致严重的内存泄漏
        WeChatRobot().robot.CReceiveMessage()
        print(msg)

class WeChatRobot():
    
    def __init__(self):
        self.robot = comtypes.client.CreateObject("WeChatRobot.CWeChatRobot")
        self.event = comtypes.client.CreateObject("WeChatRobot.RobotEvent")
        self.AddressBook = []
        self.myinfo = {}
        self.ReceiveMessageStarted = False
        
    def StartService(self) -> int:
        """
        注入DLL到微信以启动服务

        Returns
        -------
        int
            0成功,非0失败.

        """
        status = self.robot.CStartRobotService()
        return status

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
        return self.robot.CSendText(receiver,msg)
        
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
        return self.robot.CSendImage(receiver,imgpath)
    
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
        return self.robot.CSendFile(receiver,filepath)
        
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
        return self.robot.CSendArticle(receiver,title,abstract,url,imgpath)
    
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
        return self.robot.CSendCard(receiver,sharedwxid,nickname)
    
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
        return self.robot.CSendAtText(chatroomid,AtUsers,msg,AutoNickName)

    def GetSelfInfo(self) -> dict:
        """
        获取个人信息

        Returns
        -------
        dict
            调用成功返回个人信息，否则返回空字典.

        """
        myinfo = self.robot.CGetSelfInfo().replace('\n','\\n')
        try:
            myinfo = ast.literal_eval(myinfo)
        except SyntaxError:
            return {}
        self.myinfo = myinfo
        return self.myinfo
    
    def StopService(self) -> int:
        """
        停止服务，会将DLL从微信进程中卸载并杀死COM进程

        Returns
        -------
        int
            COM进程pid.

        """
        import psutil
        self.StopReceiveMessage()
        cpid = self.robot.CStopRobotService()
        try:
            cprocess = psutil.Process(cpid)
            cprocess.kill()
        except psutil.NoSuchProcess:
            pass
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
            FriendTuple = self.robot.CGetFriendList()
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
        return ChatSession(self.robot, wxid)
        
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
        userinfo = self.robot.CGetWxUserInfo(wxid).replace('\n','\\n')
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
        return self.robot.CCheckFriendStatus(wxid)
        
    def ReceiveMessage(self,CallBackFunc = None) -> None:
        """
        消息监听函数

        Parameters
        ----------
        CallBackFunc : 'function' or None, optional
            消息回调函数. 默认为空.

        Returns
        -------
        None
            .

        """
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
    def StartReceiveMessage(self,CallBackFunc = None) -> int:
        """
        启动接收消息Hook，并创建监听线程

        Parameters
        ----------
        CallBackFunc : 'function' or None, optional
            指定的回调函数. 默认为空.

        Returns
        -------
        int
            启动成功返回0,失败返回非0值.

        """
        self.ReceiveMessageStarted = True
        status = self.robot.CStartReceiveMessage()
        if status == 0:
            self.ReceiveMessageThread = threading.Thread(target = self.ReceiveMessage,args= (CallBackFunc,))
            self.ReceiveMessageThread.daemon = True
            self.ReceiveMessageThread.start()
        return status
    
    def StartReceiveMsgByEvent(self,EventSink:object or None = None) -> None:
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
        if self.robot.CStartReceiveMessage() != 0:
            return
        if self.event is not None:
            sink = EventSink or WeChatEventSink()
            self.ConnectionPoint = GetEvents(self.event,sink)
            while True:
                try:
                    PumpEvents(10)
                except:
                    break
    
    def GetDbHandles(self) -> dict:
        """
        获取数据库句柄和表信息

        Returns
        -------
        dict
            数据库句柄和表信息.

        """
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
        result = self.robot.CExecuteSQL(handle,sql)
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
        return self.robot.CBackupSQLiteDB(handle,BackupFile)

    def StopReceiveMessage(self) -> int:
        """
        停止接收消息Hook

        Returns
        -------
        int
            成功返回0,失败返回非0值.

        """
        self.ReceiveMessageStarted = False
        try:
            self.ReceiveMessageThread.join()
        except:
            pass
        status = self.robot.CStopReceiveMessage()
        return status
        
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
        return self.robot.CVerifyFriendApply(v3,v4)
    
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
        return self.robot.CAddFriendByWxid(wxid,message)
    
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
        return self.robot.CAddFriendByV3(v3,message,AddType)
    
    def GetWeChatVer(self) -> str:
        """
        获取微信版本号

        Returns
        -------
        str
            微信版本号.

        """
        return self.robot.CGetWeChatVer()
    
    def StartWeChat(self) -> int:
        """
        启动微信

        Returns
        -------
        int
            成功返回0,失败返回非0值.

        """
        return self.robot.CStartWeChat()
    
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
        userinfo = self.robot.CSearchContactByNet(keyword)
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
        return self.robot.CAddBrandContact(PublicId)
    
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
        return self.robot.CChangeWeChatVer(version)
    
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
        return self.robot.CHookImageMsg(savepath)
    
    def UnHookImageMsg(self) -> int:
        """
        取消Hook未加密图片

        Returns
        -------
        int
            成功返回0,失败返回非0值.

        """
        return self.robot.CUnHookImageMsg()
    
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
        return self.robot.CHookVoiceMsg(savepath)
    
    def UnHookVoiceMsg(self) -> int:
        """
        取消Hook语音消息

        Returns
        -------
        int
            成功返回0,失败返回非0值.

        """
        return self.robot.CUnHookVoiceMsg()

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
        return self.robot.CDeleteUser(wxid)

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
        return self.robot.CSendAppMsg(wxid,appid)