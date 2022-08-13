import ctypes
import json
import requests

PORT = 8000

def call_http_api(api,data,port = PORT):
    url = "http://127.0.0.1:{}/api/?type={}".format(port,api)
    resp = requests.post(url = url,data = json.dumps(data))
    print("POST",resp.json())

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

def test():
    data = {"wxid":"filehelper","msg":"hello http"}
    call_http_api(2,data)
    data = {"receiver":'filehelper',"shared_wxid":"filehelper","nickname":"文件传输助手"}
    call_http_api(4,data)
    data = {"receiver":'filehelper',"img_path":r"D:\C++\ComWeChatRobot\test\测试图片.png"}
    call_http_api(5,data)
    data = {"receiver":'filehelper',"file_path":r"D:\C++\ComWeChatRobot\test\测试文件"}
    call_http_api(6,data)
    data = {"wxid":'filehelper',
            "title":"百度",
            "abstract":"百度一下，你就知道",
            "url":"https://www.baidu.com/",
            "img_path":""}
    call_http_api(7,data)
    
wx = ctypes.cdll.LoadLibrary('./wxDriver64.dll')
pids = get_wechat_pid_list()
if len(pids) == 0:
    pids.append(wx.new_wechat())
wx.start_listen(pids[0],PORT)