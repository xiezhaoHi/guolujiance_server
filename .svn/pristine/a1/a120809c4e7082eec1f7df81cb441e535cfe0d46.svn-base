using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;

namespace RTMC.Core
{
    /// <summary>
    /// String扩展类
    /// </summary>
    public static class StringExt
    {
        /// <summary>
        /// 验证手机号码
        /// </summary>
        /// <param name="input"></param>
        /// <returns></returns>
        public static bool IsCellphone(this string input)
        {
            return Regex.IsMatch(input, @"^/1[3|5|8]/d{9}$");
        }
    }
}
