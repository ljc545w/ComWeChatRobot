using System;

namespace wxRobot
{
    class Program
    {
        static void Main()
        {
            System.Type wxProtId = Type.GetTypeFromProgID("WeChatRobot.CWeChatRobot");
            if (wxProtId == null)
                return;
            string workpath = System.AppDomain.CurrentDomain.BaseDirectory;
            string basePath = workpath.Replace("wxRobot\\bin\\Release\\","");
            dynamic wx = Activator.CreateInstance(wxProtId);
            wx.CStartRobotService();
            wx.CSendText("filehelper", "来自C艹艹的消息");
            wx.CSendImage("filehelper", basePath + "test\\测试图片.png");
            wx.CSendFile("filehelper", basePath + "test\\测试文件");
            wx.CSendArticle("filehelper","PC微信逆向--获取通讯录", "确定不来看看么?", "https://www.ljczero.top/article/2022/3/13/133.html");
            string selfinfo = wx.CGetSelfInfo();
            Console.WriteLine(selfinfo);
            System.Object[,,] FriendList = wx.CGetFriendList();
            int length = FriendList.GetLength(0);
            for (int i = 0; i < 1/*length*/; i++) {
                Console.WriteLine(FriendList[i, 0, 1]);
            }
            wx.CStopRobotService();
        }
    }
}
