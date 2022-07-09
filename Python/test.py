# -*- coding: utf-8 -*-
"""
Created on Sat Apr 16 14:06:24 2022

@author: lijinchao-002
"""
import time
import os
from wxRobot import WeChatRobot
    
def test_SendText():
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
    wx = WeChatRobot()
    print([i for i in dir(wx.robot) if '_' not in i and i[0] == 'C'])
    print(wx.GetWeChatVer())
    wx.StartService()
    wx.StartReceiveMessage()
    wx.StopService()