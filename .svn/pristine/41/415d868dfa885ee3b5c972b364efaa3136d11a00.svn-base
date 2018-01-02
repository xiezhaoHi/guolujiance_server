using NPOI.HSSF.UserModel;
using RTMC.BLL;
using RTMC.Core;
using RTMC.Source;
using RTMC.Source.Model;
using System;
using System.Collections.Generic;
using System.Configuration;
using System.Linq;
using System.Web;
using System.Web.SessionState;
using System.Xml;

namespace RTMC.Web.Handler {
	/// <summary>
	/// MapHandler 的摘要说明
	/// </summary>
	public class MapHandler : IHttpHandler, IRequiresSessionState {
		public void ProcessRequest(HttpContext context) {
			string result = "";
			string action = context.Request["action"];
			if (action != "login" && context.Session["User"] == null) {
				result = "请重新登陆！";
			} else {
				switch (action) {
					case "getAllDeviceData":  //首页加载地图数据
						result = this.getAllDeviceData(context);
						break;
					case "getDeviceDataById":  //Charts页面每10秒加载一次数据
						result = this.getDeviceDataById(context);
						break;
					case "getDeviceDataByIdFirst":  //Charts页面初始化加载一次数据
						result = this.getDeviceDataByIdFirst(context);
						break;
					case "getDeviceDataByIdFirstXiangXi":  //Charts详细页面初始化加载一次数据
						result = this.getDeviceDataByIdFirstXiangXi(context);
						break;
					case "GetDataByBili":  //根据滑动条查看数据
						result = this.GetDataByBili(context);
						break;
					case "GetDataLength": //得到已有数据的条数
						result = this.GetDataLength(context);
						break;
					case "GetThresholdValue": //拿到阈值
						result = this.GetThresholdValue(context);
						break;
					case "ExportToExcel_Alarm":  //总页面报警记录
						result = this.ExportToExcel_Alarm(context);
						break;
					case "getDeviceInfoById":
						result = this.GetDeviceInfoById(context);
						break;
					case "getDeviceHistoryListByPage":
						result = this.GetDeviceHistoryListByPage(context);
						break;
					case "GetChartDataById":
						result = this.GetChartDataById(context);
						break;
					case "UpdateChartDataById":
						result = this.UpdateChartDataById(context);
						break;
					case "GetDailyChartDataById":
						result = this.GetDailyChartDataById(context);
						break;
					default:
						break;
				}
			}
			context.Response.ContentType = "application/json";
			context.Response.Write(result);
		}

		private string GetDailyChartDataById(HttpContext context) {
			var id = Parse.TryGuid(context.Request["id"]);
			var date = Parse.TryDateTime(context.Request["date"]);
			try {
				var list = MapManager.GetChartDataById(id, date);
				return Function.SerializeObject(list);
			} catch (Exception) {
				return "[]";
			}
		}

		private string UpdateChartDataById(HttpContext context) {
			var id = Parse.TryGuid(context.Request["id"]);
			var date = new DateTime(1970, 1, 1).AddMilliseconds(Parse.TryDouble(context.Request["date"])).AddMinutes(1);
			var list = MapManager.UpdateChartDataById(id, date);
			return Function.SerializeObject(list);
		}

		private string GetChartDataById(HttpContext context) {
			var id = Parse.TryGuid(context.Request["id"]);
			var list = MapManager.GetChartDataById(id);
			return Function.SerializeObject(list);
		}

		private string GetDeviceHistoryListByPage(HttpContext context) {
			Guid devId = Parse.TryGuid(context.Request["devId"]);
			int index = Parse.ToInt(context.Request["index"]);
			int size = Parse.ToInt(context.Request["size"]);
			int total = 0;
			var list = MapManager.GetDeviceHistoryListByPage(devId, index, size, out total);
			return Function.SerializeObject(new { Data = list, Total = total });
		}

		private string GetDeviceInfoById(HttpContext context) {
			Guid id = Parse.TryGuid(context.Request.Form["id"]);
			return Function.SerializeObject(MapManager.GetDeviceDataById(id));
		}

		public string ExportToExcel_Alarm(HttpContext context) {
			var p = HttpRuntime.AppDomainAppPath.ToString();
			string key = context.Request.Form["key"];
			DateTime start = Parse.TryDateTime(context.Request.Form["start"]);
			DateTime end = Parse.TryDateTime(context.Request.Form["end"]);
			List<Guid> ids = MapManager.GetIdByKey(key, start, end);
			DateTime StartDay = Parse.TryDateTime(context.Request.Form["start"]);
			DateTime EndDay = Parse.TryDateTime(context.Request.Form["end"]);
			HSSFWorkbook workbook = MapManager.ExportToExcel_Alarm(ids, StartDay, EndDay) as HSSFWorkbook;
			string key1 = Guid.NewGuid().ToString().Replace("-", "");
			context.Session.Add(key1, workbook);
			return Function.SerializeObject(key1);
		}


		#region GetThresholdValue
		private string GetThresholdValue(HttpContext context) {
			string key = context.Request.Form["data"];
			Guid id = Parse.TryGuid(context.Request.Form["id"]);
			int max = 0;
			int min = 0;
			XmlDocument doc = new XmlDocument();  //创建xml对象
			var path = HttpRuntime.AppDomainAppPath.ToString();
			doc.Load(path + @"AlertLine.xml");  //加载xml文件
			XmlNode xn = doc.SelectSingleNode("AppSetting");
			XmlNode x = xn.SelectSingleNode("AlertLine" + id.ToString().ToUpper());//获得需要的节点
			XmlNodeList xnl = x.ChildNodes;  //获得所有的子节点
			foreach (var item in xnl) {
				XmlElement xe = (XmlElement)item;
				GetMaxAndMinByXml(key, ref max, ref min, xe);
			}
			return Function.SerializeObject(new { MaxThresholdValue = max, MinThresholdValue = min });
		}

		private static void GetMaxAndMinByXml(string key, ref int max, ref int min, XmlElement xe) {
			if (xe.GetAttribute("key").Contains(key)) {
				if (xe.GetAttribute("key").Contains("Ceiling")) {
					max = Parse.TryInt(xe.GetAttribute("value"));
				} else {
					min = Parse.TryInt(xe.GetAttribute("value"));
				}
			}
		}
		#endregion


		private string GetDataLength(HttpContext context) {
			Guid id = Parse.TryGuid(context.Request.Form["id"]);
			return Function.SerializeObject(MapManager.GetDataLength(id));
		}

		private string GetDataByBili(HttpContext context) {
			int bili = Parse.TryInt(context.Request.Form["Bili"]);
			Guid id = Parse.TryGuid(context.Request.Form["id"]);
			return Function.SerializeObject(MapManager.GetDataByBili(bili, id));
		}

		private string getAllDeviceData(HttpContext context) {
			SYS_Users user = (SYS_Users)context.Session["User"];
			return Function.SerializeObject(MapManager.GetAllDeviceData(user.Code));
		}

		private string getDeviceDataById(HttpContext context) {
			Guid id = Parse.TryGuid(context.Request.Form["id"]);
			return Function.SerializeObject(MapManager.GetDeviceDataById(id));
		}

		#region getDeviceDataByIdFirst
		private string getDeviceDataByIdFirst(HttpContext context) {
			Guid id = Parse.TryGuid(context.Request.Form["id"]);
			List<string> list = new List<string>();
			List<string> listChinese = new List<string>();
			Dictionary<string, int> max = new Dictionary<string, int>();
			Dictionary<string, int> min = new Dictionary<string, int>();
			XmlDocument doc = new XmlDocument();  //创建xml对象
			var path = HttpRuntime.AppDomainAppPath.ToString();
			doc.Load(path + @"AlertLine.xml");  //加载xml文件
			XmlNode xn = doc.SelectSingleNode("AppSetting");  //根节点
			XmlNode x = xn.SelectSingleNode("AlertLine" + id.ToString().ToUpper());//获得阈值
			XmlNode xx = xn.SelectSingleNode("Name" + id.ToString().ToUpper());  //获得数据名称
			XmlNode aa = xn.SelectSingleNode("EnglishToChinese"); //中英文转换
			var data = MapManager.GetDeviceDataByIdFirst(id);
			if (x != null && xx != null) {
				GetNameAndValueByXml(list, listChinese, max, min, x, xx, aa);
				int total = list.Count;
				//GetData.GetThresholdData(path,id);
				return Function.SerializeObject(new { data = data, total = total, list = list, max = max, min = min, listChinese = listChinese });
			} else {
				return Function.SerializeObject(new { });
			}

		}

		private static void GetNameAndValueByXml(List<string> list, List<string> listChinese, Dictionary<string, int> max, Dictionary<string, int> min, XmlNode x, XmlNode xx, XmlNode aa) {
			XmlNodeList xnl = x.ChildNodes;  //获得所有的子节点
			XmlNodeList xnll = xx.ChildNodes;
			XmlNodeList aam = aa.ChildNodes;
			for (int i = 0; i < xnll.Count; i++) {
				XmlElement xe = (XmlElement)xnll[i];
				list.Add(xe.GetAttribute("name"));
				foreach (var item1 in aam) {
					XmlElement xee = (XmlElement)item1;
					if (xee.GetAttribute("English").ToString() == xe.GetAttribute("name")) {
						listChinese.Add(xee.GetAttribute("Chinese"));
					}
				}
			}
			for (int i = 0; i < xnl.Count; i++) {
				for (int j = 0; j < list.Count; j++) {
					XmlElement xe = (XmlElement)xnl[i];
					GetMaxAndMinByXml(list, max, min, j, xe);
				}
			}
		}

		private static void GetMaxAndMinByXml(List<string> list, Dictionary<string, int> max, Dictionary<string, int> min, int j, XmlElement xe) {
			if (xe.GetAttribute("key").Contains(list[j])) {
				if (xe.GetAttribute("key").Contains("Ceiling")) {
					max.Add(list[j], Parse.TryInt(xe.GetAttribute("value")));
				} else {
					min.Add(list[j], Parse.TryInt(xe.GetAttribute("value")));
				}
			}
		}
		#endregion


		private string getDeviceDataByIdFirstXiangXi(HttpContext context) {
			Guid id = Parse.TryGuid(context.Request.Form["id"]);
			return Function.SerializeObject(MapManager.GetDeviceDataByIdFirstXiangxi(id));
		}

		public bool IsReusable {
			get {
				return false;
			}
		}
	}
}