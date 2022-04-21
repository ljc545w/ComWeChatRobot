# -*- coding: utf-8 -*-
"""
Created on Sat Apr 16 14:06:24 2022

@author: lijinchao-002
"""
import time
from wxRobot import WeChatRobot

# 一个示例回调，将收到的文本消息转发给filehelper
def ReceiveMessageCallBack(robot,message):
    if message['type'] == 1 and message['sender'] != 'filehelper':
        robot.robot.CSendText('filehelper',message['message'])
    wxSender = robot.GetWxUserInfo(message['sender'])
    sender = wxSender['wxNickName'] if wxSender['wxNickName'] != 'null' else message['sender']
    if '@chatroom' in message['sender']:
        wxUser = robot.GetWxUserInfo(message['wxid'])
        print("来自 {} {},type {}".format(sender,wxUser['wxNickName'],message['type']))
    else:
        print("来自 {},type {}".format(sender,message['type']))
    if message['type'] == 1:
        print(message['message'])
    elif message['type'] == 3:
        print(message['message'])
        print(message['filepath'])
    elif message['type'] == 49:
        print(message['message'])
        if not message['filepath']: print(message['filepath'])
    else:
        print(message['message'])
    
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
    try:
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        pass
    wx.StopService(wx)
    
def test_ExecuteSQL():
    wx = WeChatRobot()
    wx.StartService()
    try:
        dbs = wx.GetDbHandles()
        dbname = 'MicroMsg.db'
        handle = dbs.get(dbname).get('Handle')
        sql = 'select a.UserName as `wxID`,a.Alias as `微信号`,a.EncryptUserName as `V3数据`,\
               a.Type as `联系人类型`,a.VerifyFlag as `添加方式`,a.Remark as `备注`,a.NickName as `昵称`,b.bigHeadImgUrl as `头像` \
               from Contact a inner join ContactHeadImgUrl b where a.UserName=b.usrName and a.Type=3 limit 10'
        result = wx.ExecuteSQL(handle,sql)
        print(result)
    except:
        pass
    wx.StopService()
    
def test_BackupDb():
    wx = WeChatRobot()
    wx.StartService()
    try:
        dbs = wx.GetDbHandles()
        dbname = 'MicroMsg.db'
        handle = dbs.get(dbname).get('Handle')
        rc = wx.BackupSQLiteDB(handle,'D:\\WeChatBackup\\{}'.format(dbname))
        print(rc)
    except:
        pass
    wx.StopService()

if __name__ == '__main__':
    test_ExecuteSQL()