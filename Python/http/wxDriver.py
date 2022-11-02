import ctypes
import json
import copy
import threading
import requests
import socketserver

if ctypes.sizeof(ctypes.c_void_p) == ctypes.sizeof(ctypes.c_ulonglong):
    driver = ctypes.cdll.LoadLibrary('./wxDriver64.dll')
else:
    driver = ctypes.cdll.LoadLibrary('./wxDriver.dll')

new_wechat = driver.new_wechat
new_wechat.argtypes = None
new_wechat.restype = ctypes.c_int

start_listen = driver.start_listen
start_listen.argtypes = [ctypes.c_int,ctypes.c_int]
start_listen.restype = ctypes.c_int

stop_listen = driver.stop_listen
stop_listen.argtypes = [ctypes.c_int]
stop_listen.restype = ctypes.c_int

class WECHAT_HTTP_APIS:
    # login check
    WECHAT_IS_LOGIN = 0                         # 登录检查

    # self info
    WECHAT_GET_SELF_INFO = 1                    # 获取个人信息

    # send message
    WECHAT_MSG_SEND_TEXT = 2                    # 发送文本
    WECHAT_MSG_SEND_AT = 3                      # 发送群艾特
    WECHAT_MSG_SEND_CARD = 4                    # 分享好友名片
    WECHAT_MSG_SEND_IMAGE = 5                   # 发送图片
    WECHAT_MSG_SEND_FILE = 6                    # 发送文件
    WECHAT_MSG_SEND_ARTICLE = 7                 # 发送xml文章
    WECHAT_MSG_SEND_APP = 8                     # 发送小程序

    # receive message
    WECHAT_MSG_START_HOOK = 9                   # 开启接收消息HOOK，只支持socket监听
    WECHAT_MSG_STOP_HOOK = 10                   # 关闭接收消息HOOK
    WECHAT_MSG_START_IMAGE_HOOK = 11            # 开启图片消息HOOK
    WECHAT_MSG_STOP_IMAGE_HOOK = 12             # 关闭图片消息HOOK
    WECHAT_MSG_START_VOICE_HOOK = 13            # 开启语音消息HOOK
    WECHAT_MSG_STOP_VOICE_HOOK = 14             # 关闭语音消息HOOK

    # contact
    WECHAT_CONTACT_GET_LIST = 15                # 获取联系人列表
    WECHAT_CONTACT_CHECK_STATUS = 16            # 检查是否被好友删除
    WECHAT_CONTACT_DEL = 17                     # 删除好友
    WECHAT_CONTACT_SEARCH_BY_CACHE = 18         # 从内存中获取好友信息
    WECHAT_CONTACT_SEARCH_BY_NET = 19           # 网络搜索用户信息
    WECHAT_CONTACT_ADD_BY_WXID = 20             # wxid加好友
    WECHAT_CONTACT_ADD_BY_V3 = 21               # v3数据加好友
    WECHAT_CONTACT_ADD_BY_PUBLIC_ID = 22        # 关注公众号
    WECHAT_CONTACT_VERIFY_APPLY = 23            # 通过好友请求
    WECHAT_CONTACT_EDIT_REMARK = 24             # 修改备注

    # chatroom
    WECHAT_CHATROOM_GET_MEMBER_LIST = 25        # 获取群成员列表
    WECHAT_CHATROOM_GET_MEMBER_NICKNAME = 26    # 获取指定群成员昵称
    WECHAT_CHATROOM_DEL_MEMBER = 27             # 删除群成员
    WECHAT_CHATROOM_ADD_MEMBER = 28             # 添加群成员
    WECHAT_CHATROOM_SET_ANNOUNCEMENT = 29       # 设置群公告
    WECHAT_CHATROOM_SET_CHATROOM_NAME = 30      # 设置群聊名称
    WECHAT_CHATROOM_SET_SELF_NICKNAME = 31      # 设置群内个人昵称

    # database
    WECHAT_DATABASE_GET_HANDLES = 32            # 获取数据库句柄
    WECHAT_DATABASE_BACKUP = 33                 # 备份数据库
    WECHAT_DATABASE_QUERY = 34                  # 数据库查询

    # version
    WECHAT_SET_VERSION = 35                     # 修改微信版本号

    # log
    WECHAT_LOG_START_HOOK = 36                  # 开启日志信息HOOK
    WECHAT_LOG_STOP_HOOK = 37                   # 关闭日志信息HOOK

    # browser
    WECHAT_BROWSER_OPEN_WITH_URL = 38           # 打开微信内置浏览器
    WECHAT_GET_PUBLIC_MSG = 39                  # 获取公众号历史消息

    WECHAT_MSG_FORWARD_MESSAGE = 40             # 转发消息
    WECHAT_GET_QRCODE_IMAGE = 41                # 获取二维码
    WECHAT_GET_A8KEY = 42                       # 获取A8Key
    WECHAT_MSG_SEND_XML = 43                    # 发送xml消息
    WECHAT_LOGOUT = 44                          # 退出登录
    WECHAT_GET_TRANSFER = 45                    # 收款
    WECHAT_MSG_SEND_EMOTION = 46                # 发送表情
    WECHAT_GET_CDN = 47                         # 下载文件、视频、图片

APIS = WECHAT_HTTP_APIS

# http api 参数模板
class WECHAT_HTTP_API_PARAM_TEMPLATES:
    __HTTP_API_PARAM_TEMPLATE = {
        # login check
        APIS.WECHAT_IS_LOGIN: {},


        # self info
        APIS.WECHAT_GET_SELF_INFO: {},


        # send message
        APIS.WECHAT_MSG_SEND_TEXT: {"wxid": "",
                                    "msg": ""},
        # wxids需要以`,`分隔，例如`wxid1,wxid2,wxid3`
        APIS.WECHAT_MSG_SEND_AT: {"chatroom_id":"",
                                  "wxids": "",
                                  "msg": "",
                                  "auto_nickname": 1},
        APIS.WECHAT_MSG_SEND_CARD: {"receiver":"",
                                    "shared_wxid":"",
                                    "nickname":""},
        APIS.WECHAT_MSG_SEND_IMAGE: {"receiver":"",
                                     "img_path":""},
        APIS.WECHAT_MSG_SEND_FILE: {"receiver":"",
                                    "file_path":""},
        APIS.WECHAT_MSG_SEND_ARTICLE: {"wxid":"",
                                       "title":"",
                                       "abstract":"",
                                       "url":"",
                                       "img_path":""},
        APIS.WECHAT_MSG_SEND_APP: {"wxid":"",
                                   "appid":""},


        # receive message
        APIS.WECHAT_MSG_START_HOOK: {"port": 10808},
        APIS.WECHAT_MSG_STOP_HOOK: {},
        APIS.WECHAT_MSG_START_IMAGE_HOOK: {"save_path":""},
        APIS.WECHAT_MSG_STOP_IMAGE_HOOK: {},
        APIS.WECHAT_MSG_START_VOICE_HOOK: {"save_path":""},
        APIS.WECHAT_MSG_STOP_VOICE_HOOK: {},


        # contact
        APIS.WECHAT_CONTACT_GET_LIST: {},
        APIS.WECHAT_CONTACT_CHECK_STATUS: {"wxid":""},
        APIS.WECHAT_CONTACT_DEL: {"wxid":""},
        APIS.WECHAT_CONTACT_SEARCH_BY_CACHE: {"wxid":""},
        APIS.WECHAT_CONTACT_SEARCH_BY_NET: {"keyword":""},
        APIS.WECHAT_CONTACT_ADD_BY_WXID: {"wxid":"",
                                          "msg":""},
        APIS.WECHAT_CONTACT_ADD_BY_V3: {"v3":"",
                                        "msg":"",
                                        "add_type": 0x6},
        APIS.WECHAT_CONTACT_ADD_BY_PUBLIC_ID: {"public_id":""},
        APIS.WECHAT_CONTACT_VERIFY_APPLY: {"v3":"",
                                           "v4":""},
        APIS.WECHAT_CONTACT_EDIT_REMARK: {"wxid":"",
                                          "remark":""},


        # chatroom
        APIS.WECHAT_CHATROOM_GET_MEMBER_LIST: {"chatroom_id":""},
        APIS.WECHAT_CHATROOM_GET_MEMBER_NICKNAME: {"chatroom_id":"",
                                                   "wxid":""},
        # wxids需要以`,`分隔，例如`wxid1,wxid2,wxid3`
        APIS.WECHAT_CHATROOM_DEL_MEMBER: {"chatroom_id":"",
                                          "wxids":""},
        # wxids需要以`,`分隔，例如`wxid1,wxid2,wxid3`
        APIS.WECHAT_CHATROOM_ADD_MEMBER: {"chatroom_id":"",
                                          "wxids":""},
        APIS.WECHAT_CHATROOM_SET_ANNOUNCEMENT: {"chatroom_id":"",
                                                "announcement":""},
        APIS.WECHAT_CHATROOM_SET_CHATROOM_NAME: {"chatroom_id":"",
                                                 "chatroom_name":""},
        APIS.WECHAT_CHATROOM_SET_SELF_NICKNAME: {"chatroom_id":"",
                                                 "nickname":""},


        # database
        APIS.WECHAT_DATABASE_GET_HANDLES: {},
        APIS.WECHAT_DATABASE_BACKUP: {"db_handle":0,
                                      "save_path":""},
        APIS.WECHAT_DATABASE_QUERY: {"db_handle":0,
                                     "sql":""},


        # version
        APIS.WECHAT_SET_VERSION: {"version": "3.7.0.30"},


        # log
        APIS.WECHAT_LOG_START_HOOK: {},
        APIS.WECHAT_LOG_STOP_HOOK: {},

        # browser
        APIS.WECHAT_BROWSER_OPEN_WITH_URL: {"url": "https://www.baidu.com/"},
        APIS.WECHAT_GET_PUBLIC_MSG: {"public_id": "","offset": ""},

        APIS.WECHAT_MSG_FORWARD_MESSAGE: {"wxid": "filehelper","msgid": 2 ** 64 - 1},
        APIS.WECHAT_GET_QRCODE_IMAGE: {},
        APIS.WECHAT_GET_A8KEY: {"url":""},
        APIS.WECHAT_MSG_SEND_XML: {"wxid":"filehelper","xml":"","img_path":""},
        APIS.WECHAT_LOGOUT: {},
        APIS.WECHAT_GET_TRANSFER: {"wxid":"","transcationid":"","transferid":""},
        APIS.WECHAT_MSG_SEND_EMOTION: {"wxid":"","img_path":""},
        APIS.WECHAT_GET_CDN: {"msgid":2 ** 64 - 1},
    }

    def get_http_template(self, api_number):
        try:
            return copy.deepcopy(self.__HTTP_API_PARAM_TEMPLATE[api_number])
        except KeyError:
            raise ValueError("There is no interface numbered %s." % api_number)

get_http_template = WECHAT_HTTP_API_PARAM_TEMPLATES().get_http_template

class ReceiveMsgSocketServer(socketserver.BaseRequestHandler):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

    def handle(self):
        conn = self.request
        while True:
            try:
                ptr_data = b""
                while True:
                    data = conn.recv(1024)
                    ptr_data += data
                    if len(data) == 0 or data[-1] == 0xA:
                        break
                msg = json.loads(ptr_data.decode('utf-8'))
                ReceiveMsgSocketServer.msg_callback(msg)
            except OSError:
                break
            except json.JSONDecodeError:
                pass
            conn.sendall("200 OK".encode())
        conn.close()

    @staticmethod
    def msg_callback(msg):
        print(msg)
        # TODO: 在这里写额外的消息处理逻辑

def post_wechat_http_api(api,port,data = {}):
    url = "http://127.0.0.1:{}/api/?type={}".format(port,api)
    resp = requests.post(url = url,data = json.dumps(data))
    return resp.json()

def get_wechat_http_api(api,port,data = {}):
    url = "http://127.0.0.1:{}/api/?type={}".format(port,api)
    resp = requests.get(url = url,params = data)
    return resp.json()

def get_wechat_pid_list() -> list:
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

def start_socket_server(port: int = 10808,
                        request_handler = ReceiveMsgSocketServer,
                        main_thread: bool = True) -> int or None:
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

def test_send_msg(test_port):
    post_wechat_http_api(APIS.WECHAT_LOG_START_HOOK,port = test_port)
    if post_wechat_http_api(APIS.WECHAT_IS_LOGIN,port = test_port)["is_login"] == 1:
        print(post_wechat_http_api(APIS.WECHAT_GET_SELF_INFO,port = test_port))

        data = {"wxid":"filehelper","msg":"hello http"}
        post_wechat_http_api(APIS.WECHAT_MSG_SEND_TEXT,data = data,port = test_port)

        data = {"receiver":'filehelper',"shared_wxid":"filehelper","nickname":"文件传输助手"}
        post_wechat_http_api(APIS.WECHAT_MSG_SEND_CARD,data = data,port = test_port)

        data = {"receiver":'filehelper',"img_path":r"D:\VS2019C++\MyWeChatRobot\test\测试图片.png"}
        post_wechat_http_api(APIS.WECHAT_MSG_SEND_IMAGE,data = data,port = test_port)

        data = {"receiver":'filehelper',"file_path":r"D:\VS2019C++\MyWeChatRobot\test\测试文件"}
        post_wechat_http_api(APIS.WECHAT_MSG_SEND_FILE,data = data,port = test_port)

        data = {"wxid":'filehelper',
                "title":"百度",
                "abstract":"百度一下，你就知道",
                "url":"https://www.baidu.com/",
                "img_path":""}
        post_wechat_http_api(APIS.WECHAT_MSG_SEND_ARTICLE,data = data,port = test_port)
        print(post_wechat_http_api(APIS.WECHAT_CONTACT_GET_LIST,port = test_port))
        data = {"wxid":"filehelper"}
        print(post_wechat_http_api(APIS.WECHAT_CONTACT_CHECK_STATUS,data = data,port = test_port))
    post_wechat_http_api(APIS.WECHAT_LOG_STOP_HOOK,port = test_port)

def test_get_public_msg(test_port,public_id):
    import time
    param = {"public_id": public_id,"offset": ""}
    data = post_wechat_http_api(APIS.WECHAT_GET_PUBLIC_MSG,test_port,param)
    msg_list = json.loads(data['msg'])['MsgList']
    next_offset = msg_list['PagingInfo']['Offset']
    for msg in msg_list['Msg']:
        detail_info = msg['AppMsg']['DetailInfo']
        for info in detail_info:
            Title = info['Title']
            Digest = info['Digest']
            ContentUrl = info['ContentUrl']
            a8key_dict = post_wechat_http_api(APIS.WECHAT_GET_A8KEY,
                                              port = test_port,
                                              data = {"url":ContentUrl})
            print(a8key_dict)
            post_wechat_http_api(APIS.WECHAT_BROWSER_OPEN_WITH_URL,
                                 test_port,
                                 {"url":ContentUrl}
                                 )
            time.sleep(3)
            break
        break

def test_get_chatroom_list_from_db(test_port):
    dbs = post_wechat_http_api(APIS.WECHAT_DATABASE_GET_HANDLES,port = test_port)
    db_handle = [i for i in dbs['data'] if i['db_name'] == 'MicroMsg.db'][0]['handle']
    sql = "select UserName,Alias,EncryptUserName,Type,VerifyFlag,Remark,NickName,ChatRoomType,ExtraBuf from Contact where Type=2;"
    data = {"db_handle":db_handle,"sql":sql}
    res = post_wechat_http_api(APIS.WECHAT_DATABASE_QUERY,data = data,port = test_port)
    return res['data']

if __name__ == '__main__':
    port = 8000
    pids = get_wechat_pid_list()
    if len(pids) == 0:
        pids.append(new_wechat())
    start_listen(pids[0],port)
    post_wechat_http_api(APIS.WECHAT_LOG_START_HOOK,port)
    print(post_wechat_http_api(APIS.WECHAT_GET_SELF_INFO, port))
    post_wechat_http_api(APIS.WECHAT_MSG_START_HOOK,port,{"port":10808})
    start_socket_server()
    stop_listen(pids[0])
