using RTMC.Source;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq.Expressions;
using System.Text.RegularExpressions;
using System.Linq;

namespace RTMC.Permissions {
    /// <summary>
    /// 数据权限库。
    /// </summary>
    public static class PermissionLibrary {
        public static ArrayList PermissionAll { get; set; }
        public static void Initial() {

            PermissionAll = new ArrayList();
            // 第二种使用方式初始化
            var libUsers = PermissionManager<SYS_Users>.Library = new Dictionary<int, Permission<SYS_Users>>();
            libUsers.Add(1001, new Permission<SYS_Users> { Code = 1001, Name = "所有", Formula = oper => (user => true) });
            libUsers.Add(1002, new Permission<SYS_Users> { Code = 1002, Name = "区域用户", Formula = UserFormula });
            libUsers.Add(1003, new Permission<SYS_Users> { Code = 1003, Name = "单位用户", Formula = oper => (user => oper.OrganizationId == user.OrganizationId) });
            libUsers.Add(1004, new Permission<SYS_Users> { Code = 1004, Name = "个人用户", Formula = oper => (user => oper.Id == user.Id) });
            PermissionAll.Add(new { name = "用户权限", list = libUsers.ToList() });

			//设备数据权限管理
			var libDevices = PermissionManager<Device>.Library = new Dictionary<int, Permission<Device>>();
			libDevices.Add(2001, new Permission<Device> { Code = 2001, Name = "所有", Formula = oper => (device => true) });
			libDevices.Add(2002, new Permission<Device> { Code = 2002, Name = "个人", Formula = oper => (device => device.CreateUser==oper.Id) });
			PermissionAll.Add(new { name = "设备权限", list = libDevices.ToList() });

			//报警记录权限管理
			var libAlarm = PermissionManager<ViewDeviceAlarm>.Library = new Dictionary<int, Permission<ViewDeviceAlarm>>();
			libAlarm.Add(3001, new Permission<ViewDeviceAlarm> { Code = 3001, Name = "所有", Formula = oper => (Alarm => true) });
			PermissionAll.Add(new { name = "报警记录权限", list = libAlarm.ToList() });
        }

        /// <summary>
        /// 代理方法，根据当前用户的所在单位地区信息，生成判断条件。
        /// </summary>
        /// <param name="oper"></param>
        /// <returns></returns>
        private static Expression<Func<SYS_Users, bool>> UserFormula(SYS_Users oper) {
            var code = GetCodeHead(oper.SYS_Organization.AreaCode);
            return u => u.SYS_Organization.AreaCode.StartsWith(code);
        }

        /// <summary>
        /// 代理方法，根据当前用户信息生成区域单位的判断条件。
        /// </summary>
        /// <param name="oper"></param>
        /// <returns></returns>
        private static Expression<Func<SYS_Organization, bool>> OrgFormula(SYS_Users oper) {
            var code = GetCodeHead(oper.SYS_Organization.AreaCode);
            return o => o.AreaCode.StartsWith(code);
        }

        /// <summary>
        /// 截取行政区编码非0位。
        /// </summary>
        /// <param name="code"></param>
        /// <returns></returns>
        private static string GetCodeHead(string code) {
            return Regex.Replace(code, @"0{0,4}$", "", RegexOptions.Compiled);
        }

    }
}
