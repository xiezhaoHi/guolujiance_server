using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace RTMC.Core
{
    /// <summary>
    /// 读取配置文件类
    /// </summary>
    public sealed class RTMCSetting
    {
        //private static string _Namespace;
        public static string Namespace
        {
            get
            {
                //if (string.IsNullOrEmpty(_Namespace))
                //    _Namespace = System.Configuration.ConfigurationManager.AppSettings["ServiceNamespace"];
                //return _Namespace;
                return "http://www.rtmc.com/";
            }
        }
        public static string AllowedOperationTarg = Namespace + "/allowed";
    }
}