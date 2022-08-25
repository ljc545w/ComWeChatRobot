# -*- coding: utf-8 -*-
"""
Created on Sat Apr 16 14:06:24 2022

@author: ljc545w
"""
import time
import os
import wxRobot
from wxRobot import WeChatRobot


def test_send_text(instance):
    path = os.path.split(os.path.realpath(__file__))[0]
    # image full path
    img_path = os.path.join(path, 'test\\测试图片.png')
    # file full path
    filepath = os.path.join(path, 'test\\测试文件')
    self_info = instance.GetSelfInfo()
    chat_with = instance.GetFriendByWxNickName("文件传输助手")
    session = instance.GetChatSession(chat_with.get('wxid'))
    filehelper = instance.GetWxUserInfo(chat_with.get('wxid'))
    session.SendText('个人信息：{}'.format(str(self_info.get('wxNickName'))))
    session.SendText('好友信息：{}'.format(str(filehelper.get('wxNickName'))))
    if os.path.exists(img_path):
        session.SendImage(img_path)
    if os.path.exists(filepath):
        session.SendFile(filepath)
    session.SendArticle("天气预报", "点击查看", "http://www.baidu.com")
    shared = instance.GetFriendByWxNickName("码农翻身")
    if shared:
        session.SendCard(shared.get('wxid'), shared.get('wxNickName'))


def test_friend_status(instance):
    f = open('friend_status.txt', 'wt', encoding='utf-8')
    friend_list = instance.GetFriendList()
    index = "\t".join(['微信号', '昵称', '备注', '状态', '\n'])
    f.writelines(index)
    for Friend in friend_list:
        result = '\t'.join(
            [Friend.get('wxNumber'), Friend.get('wxNickName'), Friend.get('wxRemark'),
             instance.CheckFriendStatus(Friend.get('wxid'))])
        print(result)
        result += '\n'
        f.writelines(result)
        time.sleep(1)
        break
    f.close()


def test_execute_sql(instance):
    try:
        dbs = instance.GetDbHandles()
        dbname = 'MicroMsg.db'
        handle = dbs.get(dbname).get('Handle')
        sql = 'select a.UserName as `wxID`,a.Alias as `微信号`,a.EncryptUserName as `V3数据`,\
               a.Type as `联系人类型`,a.VerifyFlag as `添加方式`,a.Remark as `备注`,a.NickName as `昵称`,b.bigHeadImgUrl as `头像`,\
               a.ExtraBuf as `扩展数据` \
               from Contact a inner join ContactHeadImgUrl b where a.UserName=b.usrName and a.Type=3 limit 10'
        result = instance.ExecuteSQL(handle, sql)
        print(result)
    except Exception as e:
        print(e)


def test_BackupDb(instance):
    try:
        dbs = instance.GetDbHandles()
        dbname = 'MicroMsg.db'
        handle = dbs.get(dbname).get('Handle')
        rc = instance.BackupSQLiteDB(handle, 'D:\\WeChatBackup\\{}'.format(dbname))
        print(rc)
    except Exception as e:
        print(e)


def show_interfaces():
    robot = wxRobot.WeChatRobot(0).robot
    print(robot.CGetWeChatVer())
    interfaces = [i for i in dir(robot) if '_' not in i and i[0] == 'C']
    for interface in interfaces:
        print(interface)


if __name__ == '__main__':
    pid_list = wxRobot.get_wechat_pid_list()
    wx = WeChatRobot(pid_list[0])
    wx.StartService()
    wx.StartReceiveMessage()
    wxRobot.register_msg_event(pid_list[0])
    wx.StopService()
