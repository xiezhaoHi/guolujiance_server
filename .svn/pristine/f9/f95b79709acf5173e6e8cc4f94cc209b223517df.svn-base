using Newtonsoft.Json;
using RTMC.BLL;
using RTMC.Source;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.SessionState;

namespace RTMC.Web.Handler {
	public abstract class BaseHandler : IHttpHandler, IRequiresSessionState {
		public SYS_Users _user;
		//public static void HasRole(HttpContext context, out SYS_Users user) {
		//	user = GetUser(context);
		//	string href = context.Request.Url.AbsolutePath;
		//	string url = context.Request.UrlReferrer.AbsolutePath;
		//	string action = context.Request["action"];
		//	if (!FuncAuthorityManager.IsAuthority(user, url, href, action)) {
		//		context.Response.Write("你无权操作此功能！");
		//		context.Response.End();
		//	}
		//}

		public void ProcessRequest(HttpContext context) {
			string result = "";
			string href = context.Request.Url.AbsolutePath;
			string url = context.Request.UrlReferrer.AbsolutePath;
			string action = context.Request["action"];
			var cUser = context.Session["User"];
			if (cUser != null) {
				_user = (SYS_Users)cUser;
				if (!AuthorityManager.IsAuthority(_user, url, href, action)) {
					result = "你无权操作此功能！";
				} else {
                    ExecuteToDo(context, action, out result);
				}
			} else {
				result = "请重新登陆！";
			}
			context.Response.Clear();
			context.Response.ContentType = "application/json";
			try {
				context.Response.Write(result);
			} catch (Exception ex) {
				context.Response.Write("系统错误：" + ex.Message);
			}
			context.Response.End();
		}

		public abstract void ExecuteToDo(HttpContext context, string action,out string result);

		public bool IsReusable {
			get { return false; }
		}
	}
}