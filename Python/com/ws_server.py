# -*- coding:utf-8 -*-
# create: 2022/10/20 10:23
# author: ly1102
"""
    websocket服务端，对各个api进行了转换，可以支持收发微信消息
"""
import sys
import json
import inspect
import asyncio
import comtypes
import websockets
from wxRobot import WeChatRobot, get_wechat_pid_list

TCP_HOOK_PORT = 10808  # hook的微信消息接收端口，保证不被占用了就行
SERVER_IP = '0.0.0.0'  # websocket 监听的ip
SERVER_PORT = 14514  # websocket 监听的端口
SEC_TOKEN = ''  # websocket连接需要在headers带上token： <Authorization: Bearer SEC_TOKEN>，为空则不验证


def get_methods(obj):
    return set(filter(lambda m: not m.startswith("_") and callable(getattr(obj, m)), dir(obj)))  # 获取类所有方法名称


class WebsocketServer:
    """
    同时启动TCP server监听微信的消息hook推送 和 Websocket的服务器监听
    接收到的微信消息会通过websocket推送到连接的客户端
    客户端发过来的指令也会去WeChatRobot类找到对应的方法并执行，最后返回执行结果

    接收到微信消息：WeChat         ---> TCP server       ---> Websocket server         ---> Websocket client
    发送微信指令：Websocket client ---> Websocket server ---> WeChatRobot  ---> WeChat ---> Websocket client
    """
    def __init__(self):
        self.websocket = None  # 连接成功后赋值
        self.ip = SERVER_IP
        self.port = SERVER_PORT
        self.token = SEC_TOKEN  # 用来做用户验证的，保证安全性
        self.ws_clients = {}
        self.ping_interval = 5  # 多久发送一次ping，保持连接。单位秒
        self.wx_bot_api = get_methods(wx_bot)  # 获取所有可执行方法，方便ws通过api调用

    async def handle(self, reader: asyncio.StreamReader, writer: asyncio.StreamWriter):
        """
        TCP Server的主函数，微信收到消息后通过Hook推送到该函数，本系统使用的短链接，即微信收到
        :param reader: tcp reader
        :param writer:  tcp writer
        :return: None
        """
        comtypes.CoInitialize()
        # address = writer.get_extra_info('peername')
        # print(f'new tcp connection: {address}')
        while True:
            ptr_data = b""
            try:
                while True:
                    # Python源码中并未找到设置eof的地方，所以无奈之下采用直接修改的方式，
                    # 使得没有接收到数据就一直保持await状态，否则一await就会自动返回空字节
                    reader._eof = False  # 这一行非常非常重要，如果不添加这个语句，会导致reader一直返回空字节
                    data = await reader.read(1024)
                    # print(reader.at_eof(), reader._eof)
                    ptr_data += data
                    if len(data) == 0 or data[-1] == 0xA:
                        break
                print(f'receive:{ptr_data}')
                try:
                    if ptr_data:
                        msg = json.loads(ptr_data.decode('utf-8'))
                        await self.msg_callback(msg)
                except json.JSONDecodeError as e:
                    print(f'JSON解码失败：{e}')
            except OSError as e0:
                print(f'OS Error: {e0}')
                break
            except Exception as e:
                print(f'TCP server未知异常：{e}')
            writer.write("200 OK".encode())
            await writer.drain()
            # 每条消息都有一条新的连接，接收到空字节的时候即为接收完成，此时应跳出循环关闭链接，
            # 否则报错Task was destroyed but it is pending!
            if ptr_data == b'':
                break
        writer.close()
        comtypes.CoUninitialize()

    async def msg_callback(self, data):
        """
        回调函数，收到TCP Server的消息后，通过该函数对内容进行处理和转发到websocket客户端
        :param data: ReceiveMsgStruct
        :return: None
        """
        # 主线程中已经注入，此处禁止调用StartService和StopService
        # msg = {'pid': data.pid, 'time': data.time, 'type': data.type, 'isSendMsg': data.isSendMsg, 'wxid': data.wxid,
        #        'sendto' if data.isSendMsg else 'from': data.sender, 'message': data.message}
        msg = data
        # print(data)
        robot = comtypes.client.CreateObject("WeChatRobot.CWeChatRobot")
        event = comtypes.client.CreateObject("WeChatRobot.RobotEvent")
        wx = WeChatRobot(data['pid'], robot, event)
        userinfo = wx.GetWxUserInfo(data['wxid'])
        # print(f'userinfo: {userinfo}')
        if 'wxRemark' in userinfo and userinfo['wxRemark']:  # 有备注就显示备注名
            msg['alias'] = userinfo['wxRemark']
        else:
            msg['alias'] = userinfo['wxNumber']  # 无备注就显示微信号
        if data["isSendMsg"] == 0:
            if '@chatroom' in data['sender']:
                chatroom_info = wx.GetWxUserInfo(data['sender'])
                msg['chatroom_name'] = chatroom_info['wxNickName']
                msg['nickname'] = wx.GetChatRoomMemberNickname(data['sender'], data['wxid'])
            else:
                msg['nickname'] = userinfo['wxNickName']

        print(f'tcp server receive: {msg}')
        # for name, ws in self.ws_clients.items():  # 转发多个客户端
        #     try:
        #         await ws.send(json.dumps(msg, ensure_ascii=False))
        #     except Exception as e:
        #         print(f'转发消息到{name}ws失败：{e}')
        try:
            if self.websocket:
                await self.websocket.send(json.dumps(msg, ensure_ascii=False))
        except Exception as e:
            print(f'转发消息到ws失败：{e}')
        robot.Release()
        event.Release()

    async def start_tcp_server(self):
        """
        启动监听微信tcp发送过来的消息推送服务器
        :return:
        """
        server = await asyncio.start_server(self.handle, '127.0.0.1', TCP_HOOK_PORT)  # 开始监听微信消息
        addr = server.sockets[0].getsockname()
        print(f'TCP Serving on {addr}')
        async with server:
            await server.serve_forever()

    async def receive_connection(self, websocket, path):
        """
        接收到websocket的请求链接，然后保持连接并长期监听客户端发过来的请求
        :param websocket: websocket连接
        :param path: ws请求过来的地址 默认：/
        :return: None
        """
        # print(path)
        print(f'接收到新的连接：{websocket.remote_address} at:{path}')
        headers = dict(websocket.request_headers)  # 全部会变成小写
        if self.token:  # 如果设置了token就需要对token进行验证
            print(headers)
            if 'Authorization' not in headers or headers['Authorization'] != f'Bearer {self.token}':
                websocket.send('认证失败')
                print(f'客户端：{websocket.remote_address}token认证失败')
                websocket.close()
                return
        self.ws_clients[websocket.remote_address] = websocket
        await self.receive_ws_msg(websocket)

    async def receive_ws_msg(self, websocket):
        """
        websocket的消息接收函数，保持监听，并将客户端发过来的指令拿去调 WeChatRobot 对应的方法，比如发送消息

        接收格式为 JSON：
        {"action": "要执行的WeChatRobot函数名", "params": {"参数1": "xxx", "参数2": "xxx"}}
            例如：{ "action": "SendText", "params": {"receiver": "filehelper", "msg": "66666666666"}}
        :param websocket: websocket 链接
        :return: None
        """
        self.websocket = websocket
        while True:
            try:
                recv_text = await websocket.recv()
                print("ws server receive:", recv_text)
                try:
                    action = json.loads(recv_text)
                except json.JSONDecodeError:
                    await websocket.send('Json 解码失败，请确认json格式是否规范')
                    continue
                result = await self.call_api(action)
                try:
                    response = json.dumps(result, ensure_ascii=False)
                except TypeError as e:
                    response = json.dumps({'code': 500, 'data': None, 'message': f'json encode error: {e}'})
                await websocket.send(response)
            except websockets.ConnectionClosed:
                print(f'链接断开:{websocket.remote_address}')
                if websocket.remote_address in self.ws_clients:
                    del self.ws_clients[websocket.remote_address]
                    self.websocket = None
                return
            except Exception as e:
                print(f'未知异常：{e}')

    async def call_api(self, action):
        """
        调用wx_bot的发送消息、获取消息api

        :param action: 要执行的api，必要参数action，指明执行什么哪个方法名，params将对应参数传递进去。
        例如
            {'action': 'SendFile', params: {'receiver': 'filehelper', 'filepath': 'C:\\1.jpg'}}
        action的方法名为wxRobot.py->WeChatRobot类的方法，params则是方法名对应的参数

        :return:{
            'code': 200(成功)/500(报错),
            'data': [None, dict(), list(), object](根据函数返回值确定),
            'message': '操作成功/失败理由'
        }
        """
        if 'action' not in action:
            return {'code': 500, 'data': None, 'message': '缺少指定的action名称，请查看示例代码'}
        method_name = action['action']
        if method_name not in self.wx_bot_api:
            return {'code': 500, 'data': None, 'message': f'无{method_name}方法，请查看WeChatRobot类的方法列表'}
        params = action['params']
        func = getattr(wx_bot, method_name)
        kwargs = {}
        func_info = inspect.getfullargspec(func)
        default_length = 0
        if func_info.defaults:
            defaults = func_info.defaults
            # print(defaults)
            default_length = len(defaults)
        else:
            defaults = tuple()
        func_args = func_info.args
        args_length = len(func_args)
        for index, arg in enumerate(func_args):
            if arg == 'self':  # 方法本身的self参数不用传递
                continue
            if arg in params:
                kwargs[arg] = params[arg]
            else:
                if index < args_length - default_length:
                    print(f'缺少必要参数：{arg}')
                    return {'code': 500, 'data': None,
                            'message': f'{method_name}方法，缺少必要参数：{arg}。{{"params": {{"{arg}": "xxx"}}'}
                # print('a', index - (args_length - default_length))
                kwargs[arg] = defaults[index - (args_length - default_length)]
        # comtypes.CoInitialize()
        try:
            result = func(**kwargs)  # 调用对应api
            return {'code': 200, 'data': result, 'message': f'操作成功'}
        except Exception as e:
            return {'code': 500, 'data': None, 'message': f'执行{method_name}方法出错：{e}'}

    async def start_ws_server(self):
        """
        启动websocket监听
        :return:
        """
        async with websockets.serve(self.receive_connection, self.ip, self.port, ping_interval=self.ping_interval):
            print(f'websocket server listening on {self.ip}:{self.port}!')
            await asyncio.Future()  # run forever

    def run(self):
        """
        启动程序，开启TCP Server的监听和websocket的监听
        :return: None
        """
        server_loop = asyncio.new_event_loop()
        server_loop.create_task(self.start_tcp_server())
        server_loop.create_task(self.start_ws_server())
        # asyncio.gather(self.start_tcp_server(), self.start_ws_server())
        server_loop.run_forever()


if __name__ == '__main__':
    # 直接启动即可
    pid_list = get_wechat_pid_list()
    if not pid_list:
        print('没有发现微信进程！')
        sys.exit(-1)
    print(f'微信进程：{pid_list}')
    wx_bot = WeChatRobot(pid_list[0])
    wx_bot.StartService()
    print('StartReceiveMessage')
    wx_bot.StartReceiveMessage(port=TCP_HOOK_PORT)  # 微信的消息都会推送到TCP server来

    my_server = WebsocketServer()  # 具体配置请在文件头自己配置
    my_server.run()

    # 正常情况，以下代码不会执行
    print('StopService')
    wx_bot.StopService()
