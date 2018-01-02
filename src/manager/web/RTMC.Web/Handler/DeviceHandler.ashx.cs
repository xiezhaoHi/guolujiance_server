using RTMC.BLL;
using RTMC.Core;
using RTMC.Permissions;
using RTMC.Source;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Linq.Expressions;
using System.Web;

namespace RTMC.Web.Handler {
    /// <summary>
    /// DeviceHandler 的摘要说明
    /// </summary>
    public class DeviceHandler : BaseHandler {

        public override void ExecuteToDo(HttpContext context, string action, out string result) {
            switch (action) {
                case "getDeviceByPage"://分页列表
                    result = this.getDeviceByPage(context);
                    break;
                case "addDevice":
                    result = this.addDevice(context);
                    break;
                case "updateDevice":
                    result = this.updateDevice(context);
                    break;
                case "delDevice":
                    result = this.delDevice(context);
                    break;
				case"getDeviceById":
					result = this.GetDeviceById(context);
					break;
				//case "getAllotDeviceList":
				//	result = this.GetAllotDeviceList(context);
				//		break;
                default:
                    result = "无效请求！";
                    break;
            }
        }

		//private string GetAllotDeviceList(HttpContext context) {
		//	string code = Parse.ToString(context.Request["code"]);
		//	var list = DeviceManager.GetDeviceList(code);
		//	return Function.SerializeObject(list);
		//}

		private string GetDeviceById(HttpContext context) {
			Guid id = Parse.TryGuid(context.Request.Form["id"]);
			return Function.SerializeObject(DeviceManager.getDeviceById(id));
		}

        private string delDevice(HttpContext context) {
            string data = context.Request.Form["data"];
            try {
                Device dev = Function.DeserializeObject<Device>(data);
                dev.IsDelete = true;
                return Function.SerializeObject(DeviceManager.EditDevice(dev));

            } catch (Exception e) {
                return "系统错误：" + e.Message;
            }
        }

        private string updateDevice(HttpContext context) {
            string data = context.Request.Form["data"];
            try {
                Device dev = Function.DeserializeObject<Device>(data);
                if (DeviceManager.IsExist(dev.Id, dev.Number))
                    return "设备编号已存在！";
                return Function.SerializeObject(DeviceManager.EditDevice(dev));

            } catch (Exception e) {
                return "系统错误：" + e.Message;
            }
        }

        private string addDevice(HttpContext context) {
            string data = context.Request.Form["data"];
            try {
                Device dev = Function.DeserializeObject<Device>(data);
                if (DeviceManager.IsExist(dev.Id, dev.Number))
                    return "设备编号已存在！";
                dev.CreateDate = DateTime.Now;
                dev.CreateUser = _user.Id;
                return Function.SerializeObject(DeviceManager.AddDevice(dev));

            } catch (Exception e) {
                return "系统错误：" + e.Message;
            }
        }

        private string getDeviceByPage(HttpContext context) {
            int index = Parse.ToInt(context.Request.Form["index"]);
            int size = Parse.ToInt(context.Request.Form["size"]);
            string keyword = context.Request.Form["key"];
            int total;
			SYS_Users user = (SYS_Users)context.Session["User"];
			var list = DeviceManager.GetDeviceByPage(keyword, user.Code, index, size, out total);
			return Function.SerializeObject(new { data = list, total = total });
			//var con = PermissionManager<Device>.GetFormula(_user);
			//Expression<Func<Device, bool>> where = d => d.IsDelete == false && (d.Name.Contains(key) || d.Number.Contains(key));
			//where = PredicateBuilder.And(con, where);
			//return Function.SerializeObject(new { data = DeviceManager.GetDeviceByPage(index, size, out total, where), total = total });
        }

    }
}