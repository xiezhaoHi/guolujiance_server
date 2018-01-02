using Newtonsoft.Json;
using RTMC.BLL;
using RTMC.Core;
using RTMC.Source;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.SessionState;

namespace RTMC.Web.Handler {
	/// <summary>
	/// BoilerHandler 的摘要说明
	/// </summary>
	public class BoilerHandler : BaseHandler {
		public override void ExecuteToDo(HttpContext context, string action, out string result) {
			switch (action) {
				case "GetBoiler":
					result = this.GetBoiler(context);
					break;
				case "getBoilerByPage":
					result = this.GetBoilerByPage(context);
					break;
				case "addBoiler":
					result = this.AddBoiler(context);
					break;
				case "editBoiler":
					result = this.EditBoiler(context);
					break;
				case "delBoiler":
					result = this.DelBoiler(context);
					break;
				case "getBoilerById":
					result = this.GetBoilerById(context);
					break;
				case "allotBoiler":  //分配设备
					result = this.AllotBoiler(context);
					break;
				case "getAllotDeviceList":
					result = this.GetAllotDeviceList(context);
					break;
				case "getDevIdByBoilerId":
					result = this.GetDevIdByBoilerId(context);
					break;
				default:
					result = "无效请求！";
					break;
			}
		}

		private string GetDevIdByBoilerId(HttpContext context) {
			Guid boilerId = Parse.TryGuid(context.Request["boilerId"]);
			return JsonConvert.SerializeObject(DeviceManager.GetDeviceByBoilerId(boilerId));
		}

		private string GetAllotDeviceList(HttpContext context) {
			string code = Parse.ToString(context.Request["code"]);
			Guid bId = Parse.TryGuid(context.Request["bId"]);
			var list = DeviceManager.GetDeviceList(bId, code);
			return JsonConvert.SerializeObject(list);
		}

		private string AllotBoiler(HttpContext context) {
			Guid boilerId = Parse.TryGuid(context.Request["boilerId"]);
			Guid devId = Parse.TryGuid(context.Request["devId"]);
			if (BoilerManager.AllotBoiler(boilerId, devId)) {
				return JsonConvert.SerializeObject("成功");
			} else {
				return JsonConvert.SerializeObject("失败");
			}
		}

		private string GetBoilerById(HttpContext context) {
			Guid id = Parse.TryGuid(context.Request["id"]);
			var model = BoilerManager.GetBoilerById(id);
			return JsonConvert.SerializeObject(model);
		}

		private string GetBoilerByPage(HttpContext context) {
			string keyword = Parse.ToString(context.Request["key"]);
			int index = Parse.ToInt(context.Request["index"]);
			int size = Parse.ToInt(context.Request["size"]);
			int total = 0;
			SYS_Users user = (SYS_Users)context.Session["User"];
			var list = BoilerManager.GetBoilerByPage(user.Code, keyword, index, size, out total);
			return JsonConvert.SerializeObject(new { Data = list, Total = total });
		}

		private string DelBoiler(HttpContext context) {
			Guid id = Parse.TryGuid(context.Request["id"]);
			if (BoilerManager.DelBoiler(id)) {
				return JsonConvert.SerializeObject("成功");
			} else {
				return JsonConvert.SerializeObject("失败");
			}
		}

		private string EditBoiler(HttpContext context) {
			string data = Parse.ToString(context.Request["data"]);
			var model = Function.DeserializeObject<Boiler>(data);
			if (BoilerManager.EditBoiler(model)) {
				return JsonConvert.SerializeObject("成功");
			} else {
				return JsonConvert.SerializeObject("失败");
			}
		}

		private string AddBoiler(HttpContext context) {
			string data = Parse.ToString(context.Request["data"]);
			var model = Function.DeserializeObject<Boiler>(data);
			model.Id = Guid.NewGuid();
			if (BoilerManager.AddBoiler(model)) {
				return JsonConvert.SerializeObject("成功");
			} else {
				return JsonConvert.SerializeObject("失败");
			}
		}

		private string GetBoiler(HttpContext context) {
			return Function.SerializeObject(BoilerManager.GetBoiler());
		}

		//public bool IsReusable {
		//	get {
		//		return false;
		//	}
		//}
	}
}