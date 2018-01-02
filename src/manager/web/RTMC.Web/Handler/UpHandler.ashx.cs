using RTMC.Core;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Web;
using System.Web.SessionState;

namespace RTMC.Web.Handler {
	/// <summary>
	/// UpHandler 的摘要说明
	/// </summary>
	public class UpHandler : IHttpHandler, IRequiresSessionState {

		public void ProcessRequest(HttpContext context) {
			string action = context.Request.Form["action"];
			string result = null;
			switch (action) {
				case "UploadFile": //上传excel文件
					result = this.UploadFile(context);
					break;
				default:
					result = "无效请求！";
					break;
			}
			context.Response.ContentType = "application/json";
			context.Response.Write(result);
		}


		private string UploadFile(HttpContext context) {
			var paths = new List<string>();
			var files = context.Request.Files;
			if (files.Count > 0) {
				var keys = files.AllKeys;
				foreach (var key in keys) {
					var name = files[key].FileName;
					var path = string.Format("/Files/{0}", name);
					var dir = Path.Combine(context.Server.MapPath("/Files"), name);
					files[key].SaveAs(dir);
					paths.Add(path);
				}
			}
			return Function.SerializeObject(paths);
		}
		public bool IsReusable {
			get {
				return false;
			}
		}
	}
}