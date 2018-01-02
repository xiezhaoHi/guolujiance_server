using RTMC.Core;
using RTMC.Source.Model;
using System;
using System.Collections.Generic;
using System.Configuration;
using System.Linq;
using System.Web;
using System.Web.Configuration;
using System.Xml;

namespace RTMC.Web.Handler {
	/// <summary>
	/// ConfigHandler 的摘要说明
	/// </summary>
	public class ConfigHandler : BaseHandler {


		//public bool IsReusable {
		//	get {
		//		return false;
		//	}
		//}

		public override void ExecuteToDo(HttpContext context, string action, out string result) {
			switch (action) {
				case "getConfigData":
					result = this.getConfigData(context);
					break;
				case "setConfigData":
					result = this.setConfigData(context);
					break;
				case "addConfigData":
					result = this.addConfigData(context);
					break;
				default:
					result = "无效请求！";
					break;
			}
		}

		private string addConfigData(HttpContext context) {
			throw new NotImplementedException();
		}

		private string setConfigData(HttpContext context) {
			string Device1 = context.Request.Form["ThresholdData"];
			Guid id = Parse.TryGuid(context.Request.Form["id"]);
			try {
				ThresholdData de1 = Function.DeserializeObject<ThresholdData>(Device1);
				XmlDocument doc = new XmlDocument();  //创建xml对象
				var path = HttpRuntime.AppDomainAppPath.ToString();   //当前web应用程序所在的根目录
				doc.Load(path + "AlertLine.xml");  //加载xml文件
				XmlNode xn = doc.SelectSingleNode("AppSetting");
				XmlNode x = xn.SelectSingleNode("AlertLine" + id.ToString().ToUpper()); //获得需要的节点
				GetValueByXml(de1, x);
				doc.Save(path + "AlertLine.xml");
				return Function.SerializeObject(true);
			} catch (Exception ex) {
				return "系统错误：" + ex.Message;
			}
		}
		private string getConfigData(HttpContext context) {
			Guid id = Parse.TryGuid(context.Request.Form["id"]);
			ThresholdData model = new ThresholdData();
			List<string> list = new List<string>();
			List<string> listChinese = new List<string>();
			XmlDocument doc = new XmlDocument();  //创建xml对象
			var path = HttpRuntime.AppDomainAppPath.ToString();
			doc.Load(path + @"AlertLine.xml");  //加载xml文件
			XmlNode xn = doc.SelectSingleNode("AppSetting"); //获得需要的节点
			XmlNode x = xn.SelectSingleNode("AlertLine" + id.ToString().ToUpper());  //阈值
			XmlNode xx = xn.SelectSingleNode("Name" + id.ToString().ToUpper());  //数据名称
			XmlNode aa = xn.SelectSingleNode("EnglishToChinese"); //中英文转换
			if (x != null && xx != null) {
				GetNameAndValueByXml(model, list, listChinese, x, xx, aa);
				return Function.SerializeObject(new { model = model, list = list, listChinese = listChinese });
			} else {
				return Function.SerializeObject(new { model = "", list = "", listChinese = "" });
			}

		}

		private static void GetNameAndValueByXml(ThresholdData model, List<string> list, List<string> listChinese, XmlNode x, XmlNode xx, XmlNode aa) {
			XmlNodeList xnl = x.ChildNodes;  //获得所有的子节点
			XmlNodeList aam = aa.ChildNodes;
			foreach (var item in xnl) {
				XmlElement xe = (XmlElement)item;
				string key = xe.GetAttribute("key");
				var expType = model.GetType().GetProperties().FirstOrDefault(m => m.Name == key);
				expType.SetValue(model, Parse.TryDouble(xe.GetAttribute("value")));
			}
			XmlNodeList xnll = xx.ChildNodes;  //获得所有的子节点
			foreach (var item in xnll) {
				XmlElement xe = (XmlElement)item;
				list.Add(xe.GetAttribute("name"));
				foreach (var item1 in aam) {
					XmlElement xee = (XmlElement)item1;
					if (xee.GetAttribute("English").ToString() == xe.GetAttribute("name")) {
						listChinese.Add(xee.GetAttribute("Chinese"));
					}
				}
			}
		}
		private static void GetValueByXml(ThresholdData de1, XmlNode x) {
			XmlNodeList xnl = x.ChildNodes;  //获得所有的子节点
			foreach (var item in xnl) {
				XmlElement xe = (XmlElement)item;
				string key = xe.GetAttribute("key");
				var expType = de1.GetType().GetProperties().FirstOrDefault(m => m.Name == key);
				if (expType != null) {
					var d = expType.GetValue(de1);
					xe.SetAttribute("value", d.ToString());
				}
			}
		}
	}
}