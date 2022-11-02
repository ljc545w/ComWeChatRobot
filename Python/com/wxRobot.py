# -*- coding: utf-8 -*-
"""
Created on Thu Feb 24 16:19:48 2022

@author: ljc545w
"""

# Before use,execute `CWeChatRobot.exe /regserver` in cmd by admin user
import os
import ctypes
import time
import json
import ctypes.wintypes
import socketserver
import threading
# need `pip install comtypes`
import comtypes.client
from comtypes.client import GetEvents
from comtypes.client import PumpEvents


class _WeChatRobotClient:
    _instance = None

    @classmethod
    def instance(cls) -> '_WeChatRobotClient':
        if not cls._instance:
            cls._instance = cls()
        return cls._instance

    def __init__(self):
        self.robot = comtypes.client.CreateObject("WeChatRobot.CWeChatRobot")
        self.event = comtypes.client.CreateObject("WeChatRobot.RobotEvent")
        self.com_pid = self.robot.CStopRobotService(0)

    @classmethod
    def __del__(cls):
        import psutil
        if cls._instance is not None:
            try:
                com_process = psutil.Process(cls._instance.com_pid)
                com_process.kill()
            except psutil.NoSuchProcess:
                pass
        cls._instance = None


class WeChatEventSink:
    """
    接收消息的默认回调，可以自定义，并将实例化对象作为StartReceiveMsgByEvent参数
    自定义的类需要包含以下所有成员
    """

    def OnGetMessageEvent(self, msg):
        msg = json.loads(msg[0])
        print(msg)


class ReceiveMsgBaseServer(socketserver.BaseRequestHandler):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

    def handle(self):
        conn = self.request
        comtypes.CoInitialize()
        while True:
            try:
                ptr_data = b""
                while True:
                    data = conn.recv(1024)
                    ptr_data += data
                    if len(data) == 0 or data[-1] == 0xA:
                        break
                msg = json.loads(ptr_data.decode('utf-8'))
                ReceiveMsgBaseServer.msg_callback(msg)
            except OSError:
                break
            except json.JSONDecodeError:
                pass
            conn.sendall("200 OK".encode())
        conn.close()
        comtypes.CoUninitialize()

    @staticmethod
    def msg_callback(msg):
        # 主线程中已经注入，此处禁止调用StartService和StopService
        robot = comtypes.client.CreateObject("WeChatRobot.CWeChatRobot")
        event = comtypes.client.CreateObject("WeChatRobot.RobotEvent")
        wx = WeChatRobot(msg['pid'], robot, event)
        userinfo = wx.GetWxUserInfo(msg['wxid'])
        msg['alias'] = userinfo['wxNumber']
        if msg['isSendMsg'] == 0:
            if '@chatroom' in msg['sender']:
                chatroom_info = wx.GetWxUserInfo(msg['sender'])
                msg['chatroom_name'] = chatroom_info['wxNickName']
                msg['nickname'] = wx.GetChatRoomMemberNickname(msg['sender'], msg['wxid'])
            else:
                msg['nickname'] = userinfo['wxNickName']
        # TODO: 在这里写额外的消息处理逻辑

        print(msg)
        robot.Release()
        event.Release()


class ChatSession:
    def __init__(self, pid, robot, wxid):
        self.pid = pid
        self.robot = robot
        self.chat_with = wxid

    def SendText(self, msg):
        return self.robot.CSendText(self.pid, self.chat_with, msg)

    def SendImage(self, img_path):
        return self.robot.CSendImage(self.pid, self.chat_with, img_path)

    def SendFile(self, filepath):
        return self.robot.CSendFile(self.pid, self.chat_with, filepath)

    def SendMp4(self, mp4path):
        return self.robot.CSendImage(self.pid, self.chat_with, mp4path)

    def SendArticle(self, title, abstract, url, img_path=None):
        return self.robot.CSendArticle(self.pid, self.chat_with, title, abstract, url, img_path)

    def SendCard(self, shared_wxid, nickname):
        return self.robot.CSendCard(self.pid, self.chat_with, shared_wxid, nickname)

    def SendAtText(self, wxid: list or str or tuple, msg, auto_nickname=True):
        if '@chatroom' not in self.chat_with:
            return 1
        return self.robot.CSendAtText(self.pid, self.chat_with, wxid, msg, auto_nickname)

    def SendAppMsg(self, appid):
        return self.robot.CSendAppMsg(self.pid, self.chat_with, appid)


class WeChatRobot:

    def __init__(self, pid: int = 0, robot=None, event=None):
        self.pid = pid
        self.robot = robot or _WeChatRobotClient.instance().robot
        self.event = event or _WeChatRobotClient.instance().event
        self.AddressBook = []

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

    def SendText(self, receiver: str, msg: str) -> int:
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
        return self.robot.CSendText(self.pid, receiver, msg)

    def SendImage(self, receiver: str, img_path: str) -> int:
        """
        发送图片消息

        Parameters
        ----------
        receiver : str
            消息接收者wxid.
        img_path : str
            图片绝对路径.

        Returns
        -------
        int
            0成功,非0失败.

        """
        return self.robot.CSendImage(self.pid, receiver, img_path)

    def SendFile(self, receiver: str, filepath: str) -> int:
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
        return self.robot.CSendFile(self.pid, receiver, filepath)

    def SendArticle(self, receiver: str, title: str, abstract: str, url: str, img_path: str or None = None) -> int:
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
        img_path : str or None, optional
            消息卡片显示的图片绝对路径，不需要可以不指定. The default is None.

        Returns
        -------
        int
            0成功,非0失败.

        """
        return self.robot.CSendArticle(self.pid, receiver, title, abstract, url, img_path)

    def SendCard(self, receiver: str, shared_wxid: str, nickname: str) -> int:
        """
        发送名片

        Parameters
        ----------
        receiver : str
            消息接收者wxid.
        shared_wxid : str
            被分享人wxid.
        nickname : str
            名片显示的昵称.

        Returns
        -------
        int
            0成功,非0失败.

        """
        return self.robot.CSendCard(self.pid, receiver, shared_wxid, nickname)

    def SendAtText(self, chatroom_id: str, at_users: list or str or tuple, msg: str, auto_nickname: bool = True) -> int:
        """
        发送群艾特消息，艾特所有人可以将AtUsers设置为`notify@all`
        无目标群管理权限请勿使用艾特所有人
        Parameters
        ----------
        chatroom_id : str
            群聊ID.
        at_users : list or str or tuple
            被艾特的人列表.
        msg : str
            消息内容.
        auto_nickname : bool, optional
            是否自动填充被艾特人昵称. 默认自动填充.

        Returns
        -------
        int
            0成功,非0失败.

        """
        if '@chatroom' not in chatroom_id:
            return 1
        return self.robot.CSendAtText(self.pid, chatroom_id, at_users, msg, auto_nickname)

    def GetSelfInfo(self) -> dict:
        """
        获取个人信息

        Returns
        -------
        dict
            调用成功返回个人信息，否则返回空字典.

        """
        self_info = self.robot.CGetSelfInfo(self.pid)
        return json.loads(self_info)

    def StopService(self) -> int:
        """
        停止服务，会将DLL从微信进程中卸载

        Returns
        -------
        int
            COM进程pid.

        """
        com_pid = self.robot.CStopRobotService(self.pid)
        return com_pid

    def GetAddressBook(self) -> list:
        """
        获取联系人列表

        Returns
        -------
        list
            调用成功返回通讯录列表，调用失败返回空列表.

        """
        try:
            friend_tuple = self.robot.CGetFriendList(self.pid)
            self.AddressBook = [dict(i) for i in list(friend_tuple)]
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
        friend_list = [item for item in self.AddressBook \
                       if (item['wxType'] == 3 and item['wxid'][0:3] != 'gh_')]
        return friend_list

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
        chatroom_list = [item for item in self.AddressBook \
                         if item['wxType'] == 2]
        return chatroom_list

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
        official_account_list = [item for item in self.AddressBook \
                                 if (item['wxType'] == 3 and \
                                     item['wxid'][0:3] == 'gh_')]
        return official_account_list

    def GetFriendByWxRemark(self, remark: str) -> dict or None:
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

    def GetFriendByWxNumber(self, wx_number: str) -> dict or None:
        """
        通过微信号搜索联系人

        Parameters
        ----------
        wx_number : str
            联系人微信号.

        Returns
        -------
        dict or None
            搜索到返回联系人信息，否则返回None.

        """
        if not self.AddressBook:
            self.GetAddressBook()
        for item in self.AddressBook:
            if item['wxNumber'] == wx_number:
                return item
        return None

    def GetFriendByWxNickName(self, nickname: str) -> dict or None:
        """
        通过昵称搜索联系人

        Parameters
        ----------
        nickname : str
            联系人昵称.

        Returns
        -------
        dict or None
            搜索到返回联系人信息，否则返回None.

        """
        if not self.AddressBook:
            self.GetAddressBook()
        for item in self.AddressBook:
            if item['wxNickName'] == nickname:
                return item
        return None

    def GetChatSession(self, wxid: str) -> 'ChatSession':
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
        return ChatSession(self.pid, self.robot, wxid)

    def GetWxUserInfo(self, wxid: str) -> dict:
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
        userinfo = self.robot.CGetWxUserInfo(self.pid, wxid)
        return json.loads(userinfo)

    def GetChatRoomMembers(self, chatroom_id: str) -> dict or None:
        """
        获取群成员信息

        Parameters
        ----------
        chatroom_id : str
            群聊id.

        Returns
        -------
        dict or None
            获取成功返回群成员信息，失败返回None.

        """
        info = dict(self.robot.CGetChatRoomMembers(self.pid, chatroom_id))
        if not info:
            return None
        members = info['members'].split('^G')
        data = self.GetWxUserInfo(chatroom_id)
        data['members'] = []
        for member in members:
            member_info = self.GetWxUserInfo(member)
            data['members'].append(member_info)
        return data

    def CheckFriendStatus(self, wxid: str) -> int:
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
        return self.robot.CCheckFriendStatus(self.pid, wxid)

    # 接收消息的函数
    def StartReceiveMessage(self, port: int = 10808) -> int:
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
        status = self.robot.CStartReceiveMessage(self.pid, port)
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
        tables_tuple = self.robot.CGetDbHandles(self.pid)
        tables = [dict(i) for i in tables_tuple]
        dbs = {}
        for table in tables:
            dbname = table['dbname']
            if dbname not in dbs.keys():
                dbs[dbname] = {'Handle': table['Handle'], 'tables': []}
            dbs[dbname]['tables'].append(
                {'name': table['name'], 'tbl_name': table['tbl_name'],
                 'root_page': table['rootpage'], 'sql': table['sql']}
            )
        return dbs

    def ExecuteSQL(self, handle: int, sql: str) -> list:
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
        result = self.robot.CExecuteSQL(self.pid, handle, sql)
        if len(result) == 0:
            return []
        query_list = []
        keys = list(result[0])
        for item in result[1:]:
            query_dict = {}
            for key, value in zip(keys, item):
                query_dict[key] = value if not isinstance(value, tuple) else bytes(value)
            query_list.append(query_dict)
        return query_list

    def BackupSQLiteDB(self, handle: int, filepath: str) -> int:
        """
        备份数据库

        Parameters
        ----------
        handle : int
            数据库句柄.
        filepath : int
            备份文件保存位置.

        Returns
        -------
        int
            成功返回0,失败返回非0值.

        """
        filepath = filepath.replace('/', '\\')
        save_path = filepath.replace(filepath.split('\\')[-1], '')
        if not os.path.exists(save_path):
            os.makedirs(save_path)
        return self.robot.CBackupSQLiteDB(self.pid, handle, filepath)

    def VerifyFriendApply(self, v3: str, v4: str) -> int:
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
        return self.robot.CVerifyFriendApply(self.pid, v3, v4)

    def AddFriendByWxid(self, wxid: str, message: str or None) -> int:
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
        return self.robot.CAddFriendByWxid(self.pid, wxid, message)

    def AddFriendByV3(self, v3: str, message: str or None, add_type: int = 0x6) -> int:
        """
        v3数据加好友

        Parameters
        ----------
        v3 : str
            v3数据(encryptUserName).
        message : str or None
            验证信息.
        add_type : int
            添加方式(来源).手机号: 0xF;微信号: 0x3;QQ号: 0x1;朋友验证消息: 0x6.

        Returns
        -------
        int
            请求发送成功返回0,失败返回非0值.

        """
        return self.robot.CAddFriendByV3(self.pid, v3, message, add_type)

    def GetWeChatVer(self) -> str:
        """
        获取微信版本号

        Returns
        -------
        str
            微信版本号.

        """
        return self.robot.CGetWeChatVer()

    def GetUserInfoByNet(self, keyword: str) -> dict or None:
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
        userinfo = self.robot.CSearchContactByNet(self.pid, keyword)
        if userinfo:
            return dict(userinfo)
        return None

    def AddBrandContact(self, public_id: str) -> int:
        """
        关注公众号

        Parameters
        ----------
        public_id : str
            公众号id.

        Returns
        -------
        int
            请求成功返回0,失败返回非0值.

        """
        return self.robot.CAddBrandContact(self.pid, public_id)

    def ChangeWeChatVer(self, version: str) -> int:
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
        return self.robot.CChangeWeChatVer(self.pid, version)

    def HookImageMsg(self, save_path: str) -> int:
        """
        开始Hook未加密图片

        Parameters
        ----------
        save_path : str
            图片保存路径(绝对路径).

        Returns
        -------
        int
            成功返回0,失败返回非0值.

        """
        return self.robot.CHookImageMsg(self.pid, save_path)

    def UnHookImageMsg(self) -> int:
        """
        取消Hook未加密图片

        Returns
        -------
        int
            成功返回0,失败返回非0值.

        """
        return self.robot.CUnHookImageMsg(self.pid)

    def HookVoiceMsg(self, save_path: str) -> int:
        """
        开始Hook语音消息

        Parameters
        ----------
        save_path : str
            语音保存路径(绝对路径).

        Returns
        -------
        int
            成功返回0,失败返回非0值.

        """
        return self.robot.CHookVoiceMsg(self.pid, save_path)

    def UnHookVoiceMsg(self) -> int:
        """
        取消Hook语音消息

        Returns
        -------
        int
            成功返回0,失败返回非0值.

        """
        return self.robot.CUnHookVoiceMsg(self.pid)

    def DeleteUser(self, wxid: str) -> int:
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
        return self.robot.CDeleteUser(self.pid, wxid)

    def SendAppMsg(self, wxid: str, appid: str) -> int:
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
        return self.robot.CSendAppMsg(self.pid, wxid, appid)

    def EditRemark(self, wxid: str, remark: str or None) -> int:
        """
        修改好友或群聊备注

        Parameters
        ----------
        wxid : str
            wxid或chatroom_id.
        remark : str or None
            要修改的备注.

        Returns
        -------
        int
            成功返回0,失败返回非0值.

        """
        return self.robot.CEditRemark(self.pid, wxid, remark)

    def SetChatRoomName(self, chatroom_id: str, name: str) -> int:
        """
        修改群名称.请确认具有相关权限再调用。

        Parameters
        ----------
        chatroom_id : str
            群聊id.
        name : str
            要修改为的群名称.

        Returns
        -------
        int
            成功返回0,失败返回非0值.

        """
        return self.robot.CSetChatRoomName(self.pid, chatroom_id, name)

    def SetChatRoomAnnouncement(self, chatroom_id: str, announcement: str or None) -> int:
        """
        设置群公告.请确认具有相关权限再调用。

        Parameters
        ----------
        chatroom_id : str
            群聊id.
        announcement : str or None
            公告内容.

        Returns
        -------
        int
            成功返回0,失败返回非0值.

        """
        return self.robot.CSetChatRoomAnnouncement(self.pid, chatroom_id, announcement)

    def SetChatRoomSelfNickname(self, chatroom_id: str, nickname: str) -> int:
        """
        设置群内个人昵称

        Parameters
        ----------
        chatroom_id : str
            群聊id.
        nickname : str
            要修改为的昵称.

        Returns
        -------
        int
            成功返回0,失败返回非0值.

        """
        return self.robot.CSetChatRoomSelfNickname(self.pid, chatroom_id, nickname)

    def GetChatRoomMemberNickname(self, chatroom_id: str, wxid: str) -> str:
        """
        获取群成员昵称

        Parameters
        ----------
        chatroom_id : str
            群聊id.
        wxid : str
            群成员wxid.

        Returns
        -------
        str
            成功返回群成员昵称,失败返回空字符串.

        """
        return self.robot.CGetChatRoomMemberNickname(self.pid, chatroom_id, wxid)

    def DelChatRoomMember(self, chatroom_id: str, wxid_list: str or list or tuple) -> int:
        """
        删除群成员.请确认具有相关权限再调用。

        Parameters
        ----------
        chatroom_id : str
            群聊id.
        wxid_list : str or list or tuple
            要删除的成员wxid或wxid列表.

        Returns
        -------
        int
            成功返回0,失败返回非0值.

        """
        return self.robot.CDelChatRoomMember(self.pid, chatroom_id, wxid_list)

    def AddChatRoomMember(self, chatroom_id: str, wxid_list: str or list or tuple) -> int:
        """
        添加群成员.请确认具有相关权限再调用。

        Parameters
        ----------
        chatroom_id : str
            群聊id.
        wxid_list : str or list or tuple
            要添加的成员wxid或wxid列表.

        Returns
        -------
        int
            成功返回0,失败返回非0值.

        """
        return self.robot.CAddChatRoomMember(self.pid, chatroom_id, wxid_list)

    def OpenBrowser(self,url: str) -> int:
        """
        打开微信内置浏览器

        Parameters
        ----------
        url : str
            目标网页url.

        Returns
        -------
        int
            成功返回0,失败返回非0值.

        """
        return self.robot.COpenBrowser(self.pid,url)

    def GetHistoryPublicMsg(self,public_id:str,offset:str = "") -> str:
        """
        获取公众号历史消息，一次获取十条推送记录

        Parameters
        ----------
        public_id : str
            公众号id.
        offset : str, optional
            起始偏移，为空的话则从新到久获取十条，该值可从返回数据中取得. The default is "".

        Returns
        -------
        str
            成功返回json数据，失败返回错误信息或空字符串.

        """
        ret = self.robot.CGetHistoryPublicMsg(self.pid,public_id,offset)[0]
        try:
            ret = json.loads(ret)
        except json.JSONDecodeError:
            pass
        return ret

    def ForwardMessage(self,wxid:str,msgid:int) -> int:
        """
        转发消息，只支持单条转发

        Parameters
        ----------
        wxid : str
            消息接收人wxid.
        msgid : int
            消息id，可以在实时消息接口中获取.

        Returns
        -------
        int
            成功返回0，失败返回非0值.

        """
        return self.robot.CForwardMessage(self.pid,wxid,msgid)

    def GetQrcodeImage(self) -> bytes:
        """
        获取二维码，同时切换到扫码登录

        Returns
        -------
        bytes
            二维码bytes数据.
        You can convert it to image object,like this:
        >>> from io import BytesIO
        >>> from PIL import Image
        >>> buf = wx.GetQrcodeImage()
        >>> image = Image.open(BytesIO(buf)).convert("L")
        >>> image.save('./qrcode.png')

        """
        data = self.robot.CGetQrcodeImage(self.pid)
        return bytes(data)

    def GetA8Key(self,url:str) -> dict or str:
        """
        获取A8Key

        Parameters
        ----------
        url : str
            公众号文章链接.

        Returns
        -------
        dict
            成功返回A8Key信息，失败返回空字符串.

        """
        ret = self.robot.CGetA8Key(self.pid,url)
        try:
            ret = json.loads(ret)
        except json.JSONDecodeError:
            pass
        return ret

    def SendXmlMsg(self,wxid:str,xml:str,img_path:str="") -> int:
        """
        发送原始xml消息

        Parameters
        ----------
        wxid : str
            消息接收人.
        xml : str
            xml内容.
        img_path : str, optional
            图片路径. 默认为空.

        Returns
        -------
        int
            发送成功返回0，发送失败返回非0值.

        """
        return self.robot.CSendXmlMsg(self.pid,wxid,xml,img_path)

    def Logout(self) -> int:
        """
        退出登录

        Returns
        -------
        int
            成功返回0，失败返回非0值.

        """
        return self.robot.CLogout(self.pid)

    def GetTransfer(self,wxid:str,transcationid:str,transferid:str) -> int:
        """
        收款

        Parameters
        ----------
        wxid : str
            转账人wxid.
        transcationid : str
            从转账消息xml中获取.
        transferid : str
            从转账消息xml中获取.

        Returns
        -------
        int
            成功返回0，失败返回非0值.

        """
        return self.robot.CGetTransfer(self.pid,wxid,transcationid,transferid)

    def SendEmotion(self, wxid: str, img_path: str) -> int:
        """
        发送图片消息

        Parameters
        ----------
        wxid : str
            消息接收者wxid.
        img_path : str
            图片绝对路径.

        Returns
        -------
        int
            0成功,非0失败.

        """
        return self.robot.CSendEmotion(self.pid, wxid, img_path)

    def GetMsgCDN(self,msgid: int) -> str:
        """
        下载图片、视频、文件

        Parameters
        ----------
        msgid : int
            msgid.

        Returns
        -------
        str
            成功返回文件路径，失败返回空字符串.

        """
        path = self.robot.CGetMsgCDN(self.pid,msgid)
        if path != "":
            while not os.path.exists(path):
                time.sleep(0.5)
        return path


def get_wechat_pid_list() -> list:
    """
    获取所有微信pid

    Returns
    -------
    list
        微信pid列表.

    """
    import psutil
    pid_list = []
    process_list = psutil.pids()
    for pid in process_list:
        try:
            if psutil.Process(pid).name() == 'WeChat.exe':
                pid_list.append(pid)
        except psutil.NoSuchProcess:
            pass
    return pid_list


def start_wechat() -> 'WeChatRobot' or None:
    """
    启动微信

    Returns
    -------
    WeChatRobot or None
        成功返回WeChatRobot对象,失败返回None.

    """
    pid = _WeChatRobotClient.instance().robot.CStartWeChat()
    if pid != 0:
        return WeChatRobot(pid)
    return None


def register_msg_event(wx_pid: int, event_sink: 'WeChatEventSink' or None = None) -> None:
    """
    通过COM组件连接点接收消息，真正的回调
    只会收到wx_pid对应的微信消息

    Parameters
    ----------
    wx_pid: 微信PID
    event_sink : object, optional
        回调的实现类，该类要继承`WeChatEventSink`类或实现其中的方法.

    Returns
    -------
    None
        .

    """
    event = _WeChatRobotClient.instance().event
    if event is not None:
        sink = event_sink or WeChatEventSink()
        connection_point = GetEvents(event, sink)
        assert connection_point is not None
        event.CRegisterWxPidWithCookie(wx_pid, connection_point.cookie)
        while True:
            try:
                PumpEvents(2)
            except KeyboardInterrupt:
                break
        del connection_point


def start_socket_server(port: int = 10808,
                        request_handler: 'ReceiveMsgBaseServer' = ReceiveMsgBaseServer,
                        main_thread=True) -> int or None:
    """
    创建消息监听线程

    Parameters
    ----------
    port : int
        socket的监听端口号.

    request_handler : ReceiveMsgBaseServer
        用于处理消息的类，需要继承自socketserver.BaseRequestHandler或ReceiveMsgBaseServer

    main_thread : bool
        是否在主线程中启动server

    Returns
    -------
    int or None
        main_thread为False时返回线程id,否则返回None.

    """
    ip_port = ("127.0.0.1", port)
    try:
        s = socketserver.ThreadingTCPServer(ip_port, request_handler)
        if main_thread:
            s.serve_forever()
        else:
            socket_server = threading.Thread(target=s.serve_forever)
            socket_server.setDaemon(True)
            socket_server.start()
            return socket_server.ident
    except KeyboardInterrupt:
        pass
    except Exception as e:
        print(e)
    return None


def stop_socket_server(thread_id: int) -> None:
    """
    强制结束消息监听线程

    Parameters
    ----------
    thread_id : int
        消息监听线程ID.

    Returns
    -------
    None
        .

    """
    if not thread_id:
        return
    import inspect
    try:
        tid = comtypes.c_long(thread_id)
        res = 0
        if not inspect.isclass(SystemExit):
            exec_type = type(SystemExit)
            res = comtypes.pythonapi.PyThreadState_SetAsyncExc(tid, comtypes.py_object(exec_type))
        if res == 0:
            raise ValueError("invalid thread id")
        elif res != 1:
            ctypes.pythonapi.PyThreadState_SetAsyncExc(tid, None)
            raise SystemError("PyThreadState_SetAsyncExc failed")
    except (ValueError, SystemError):
        pass
