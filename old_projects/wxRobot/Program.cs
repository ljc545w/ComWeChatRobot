using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using System;
using System.ComponentModel;
using System.Runtime.InteropServices;
using System.Threading;

namespace wxRobot
{
    class Program
    {
        static void Main()
        {
            var wx = new WeChatRobotCOMLib.WeChatRobotClass();
            try
            {
                string workpath = System.AppDomain.CurrentDomain.BaseDirectory;
                string basePath = workpath.Replace("wxRobot\\bin\\Release\\", "");

                wx.CStartRobotService();

                wx.CSendText("filehelper", "来自C艹艹的消息");
                wx.CSendImage("filehelper", basePath + "test\\测试图片.png");
                wx.CSendFile("filehelper", basePath + "test\\测试文件");
                wx.CSendArticle("filehelper", "PC微信逆向--获取通讯录", "确定不来看看么?", "https://www.ljczero.top/article/2022/3/13/133.html");

                var selfinfo = wx.CGetSelfInfo();
                if (string.IsNullOrWhiteSpace(selfinfo))
                {
                    Console.WriteLine("无法获取用户信息");
                    wx.CStopRobotService();
                    return;
                }
                Console.WriteLine(selfinfo);
                var info = JsonConvert.DeserializeObject<WxUserInfo>(selfinfo);

                wx.CStartReceiveMessage();

                try
                {
                    while (true)
                    {
                        var message = wx.CReceiveMessage() as System.Object[,];
                        if (message == null)
                        {
                            Thread.Sleep(500);
                            continue;
                        }
                        var msgType = (MessageType)Convert.ToInt32(message[0, 1]);
                        var sender = message[1, 1].ToString();
                        var wxId = message[2, 1].ToString();
                        var roomType = RoomType.Private;
                        if (msgType == MessageType.ServiceGroup)
                        {
                            roomType = RoomType.Service;
                        }
                        else if (sender != wxId)
                        {
                            roomType = RoomType.Group;
                        }
                        var content = message[3, 1].ToString();
                        var filePath = message[4, 1].ToString();
                        var wxUser = JsonConvert.DeserializeObject<WxUserInfo>(wx.CGetWxUserInfo(wxId));
                        Console.WriteLine($"消息类型:{msgType}");
                        Console.WriteLine($"对话类型:{roomType}");
                        if (roomType == RoomType.Group)
                        {
                            var groupInfo = JsonConvert.DeserializeObject<WxUserInfo>(wx.CGetWxUserInfo(sender));
                            Console.WriteLine($"群:{groupInfo.WxId}\t{groupInfo.WxNickName}");
                        }
                        Console.WriteLine($"来自:{wxUser.WxNumber}\t{wxUser.WxNickName}");
                        Console.WriteLine($"内容:{content}");
                        Console.WriteLine();
                        Thread.Sleep(500);
                    }
                }
                catch (Exception ex)
                {
                    Console.WriteLine(ex);
                }
                finally
                {
                    wx.CStopReceiveMessage();
                }

                System.Object[,,] FriendList = wx.CGetFriendList() as System.Object[,,];
                int length = FriendList.GetLength(0);
                for (int i = 0; i < 1 /*length*/; i++)
                {
                    Console.WriteLine(FriendList[i, 1, 1]);
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex);
            }
            finally
            {

                wx.CStopRobotService();
            }
        }
    }

    enum MessageType
    {
        Text = 1,

        Image = 3,

        /// <summary>
        /// 服务号群发的消息
        /// </summary>
        ServiceGroup = 49,
    }

    enum RoomType
    {
        Private = 0,

        Group = 1,

        Service = 2,

        Subscription = 3,
    }

    public class WxUserInfo
    {
        /// <summary>
        /// 原始 id
        /// </summary>
        public string WxId { get; set; }
        /// <summary>
        /// 修改过的微信号
        /// </summary>
        public string WxNumber { get; set; }
        public string WxV3 { get; set; }
        public string WxRemark { get; set; }
        public string WxNickName { get; set; }
        public string WxBigAvatar { get; set; }
        public string WxSmallAvatar { get; set; }
        public string WxSignature { get; set; }
        public string WxNation { get; set; }
        public string WxProvince { get; set; }
        public string WxCity { get; set; }
        public string WxBackground { get; set; }
        public string PhoneNumber { get; set; }
    }

}
