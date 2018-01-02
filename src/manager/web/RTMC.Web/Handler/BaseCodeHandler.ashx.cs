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
	/// BaseCodeHandler 的摘要说明
	/// </summary>
	public class BaseCodeHandler : IHttpHandler, IRequiresSessionState {

		public void ProcessRequest(HttpContext context) {
			string action = Parse.ToString(context.Request["action"]);
			string result = "";
			switch (action) {
				case "getBaseCodeTreeModel":
					result = this.GetBaseCodeTreeModel(context);
					break;
				case "getSessionCodeByUserId":
					result = this.GetSessionCodeByUserId(context);
					break;
				default:
					break;
			}
			context.Response.ContentType = "application/json";
			context.Response.Write(result);
		}

		private string GetSessionCodeByUserId(HttpContext context) {
			SYS_Users user = (SYS_Users)context.Session["User"];
			var model = new BaseCode() {
				Code = user.Code,
				AreaName = user.AreaName
			};
			return JsonConvert.SerializeObject(model);
		}

		private string GetBaseCodeTreeModel(HttpContext context) {
			string code = Parse.ToString(context.Request["code"]);
			var list = BaseCodeManager.GetBaseCodeList(code);
			return JsonConvert.SerializeObject(list);
		}

		public bool IsReusable {
			get {
				return false;
			}
		}
	}

	public class BaseCode {
		public string Code { get; set; }
		public string AreaName { get; set; }
	}
}