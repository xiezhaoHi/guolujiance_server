using RTMC.BLL;
using RTMC.Core;
using RTMC.Permissions;
using RTMC.Source;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Linq.Expressions;
using System.Web;
using System.Xml;

namespace RTMC.Web.Handler {
	/// <summary>
	/// AlarmRecordHandler 的摘要说明
	/// </summary>
	public class AlarmRecordHandler : BaseHandler {


		//public bool IsReusable {
		//	get {
		//		return false;
		//	}
		//}

		public override void ExecuteToDo(HttpContext context, string action, out string result) {
			switch (action) {
				case "getAlarmRecordByPage":
					result = this.getAlarmRecordByPage(context);
					break;
				case "getAlarmRecordById":
					result = this.getAlarmRecordById(context);
					break;
				default:
					result = "无效请求！";
					break;
			}
		}

		private string getAlarmRecordById(HttpContext context) {
			Guid id = Parse.TryGuid(context.Request.Form["id"]);
			var con = PermissionManager<ViewDeviceAlarm>.GetFormula(_user);
			Expression<Func<ViewDeviceAlarm, bool>> where = m => m.Id == id;
			where = PredicateBuilder.And(con, where);
			return Function.SerializeObject(AlarmRecordManager.GetAlarmRecordById(where));
		}

		private string getAlarmRecordByPage(HttpContext context) {
			int index = Parse.TryInt(context.Request.Form["index"]);
			int size = Parse.TryInt(context.Request.Form["size"]);
			string key = context.Request.Form["key"];
			string keyy = "";
			XmlDocument doc = new XmlDocument();  //创建xml对象
			var path = HttpRuntime.AppDomainAppPath.ToString();
			doc.Load(path + @"AlertLine.xml");  //加载xml文件
			XmlNode xn = doc.SelectSingleNode("AppSetting"); //获得需要的节点
			XmlNode x = xn.SelectSingleNode("EnglishToChinese");
			XmlNodeList xx = x.ChildNodes;
			foreach (var item in xx ) {
				XmlElement xe = (XmlElement)item;
				if (key!=null&&key!="") {
					if (xe.GetAttribute("Chinese").ToString().Contains(key)) {
						keyy = xe.GetAttribute("English");
						break;
					}
				}
			}
			int total;
			string strStart = context.Request.Form["start"];
			string strEnd = context.Request.Form["end"];
			List<ViewDeviceAlarm> list_all = new List<ViewDeviceAlarm>();
			SYS_Users user = (SYS_Users)context.Session["User"];
			Expression<Func<ViewDeviceAlarm, bool>> where = null;
			if (strEnd != null && strEnd != "" && strStart != null && strStart != "") {
				DateTime start = Parse.TryDateTime(strStart);
				DateTime end = Parse.TryDateTime(strEnd);
				where = m => (m.Name.Contains(key) || m.Number.Contains(key) || m.AlarmDataType_English == keyy) && (m.AlarmTime >= start && m.AlarmTime <= end);
				//var con = PermissionManager<ViewDeviceAlarm>.GetFormula(_user);
				//Expression<Func<ViewDeviceAlarm, bool>> where = m => (m.Name.Contains(key) || m.Number.Contains(key) || m.AlarmDataType_English == keyy) && (m.AlarmTime >= start && m.AlarmTime <= end);
				//where = PredicateBuilder.And(con, where);
				list_all = AlarmRecordManager.GetAlarmRecordByPage(index, size, user.Code, where, out total);
			} else {
				where = m => m.Name.Contains(key) || m.Number.Contains(key) || m.AlarmDataType_English == keyy;
				//var con = PermissionManager<ViewDeviceAlarm>.GetFormula(_user);
				//Expression<Func<ViewDeviceAlarm, bool>> where = m => m.Name.Contains(key) || m.Number.Contains(key) || m.AlarmDataType_English == keyy;
				//where = PredicateBuilder.And(con, where);
				list_all = AlarmRecordManager.GetAlarmRecordByPage(index, size, user.Code, where, out total);
			}
			foreach (var item in list_all) {
				var type = item.AlarmDataType_English;
				foreach (var item1 in xx) {
					XmlElement xe = (XmlElement)item1;
					if (xe.GetAttribute("English").ToString() == type) {
						item.AlarmDataType_Chinese = xe.GetAttribute("Chinese");
					}
				}
			}  //将英文转换为中文
			return Function.SerializeObject(new { data = list_all, total = total });
		}
	}
}