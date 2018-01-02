using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.SessionState;
using System.IO;
using NPOI.HSSF.UserModel;
using NPOI.SS.UserModel;
using RTMC.Core;

namespace RTMC.Web.Handler {
	/// <summary>
	/// DownloadHandler 的摘要说明
	/// </summary>
	public class DownloadHandler : IHttpHandler, IRequiresSessionState {

		public void ProcessRequest(HttpContext context) {
			//var action = context.Request.Form["action"];
			var url = context.Request.RawUrl;
			var key = url.Split('?')[1].Split('&')[1].Split('=')[1];
			var action = url.Split('?')[1].Split('&')[0].Split('=')[1]; 
			switch (action) {
				case "excel":
				    this.returnExcel(context, key);
					break;
				default:
					break;
			}
		}

		private void returnExcel(HttpContext context,string key) {
			HSSFWorkbook workbook = context.Session[key] as HSSFWorkbook;
			if (workbook!=null) {
				MemoryStream ms = new MemoryStream();
				workbook.Write(ms);
				//设置输出编码格式
				context.Response.ContentEncoding = System.Text.Encoding.UTF8;
				//设置输出流
				context.Response.ContentType = "application/octet-stream";
				//防止中文乱码
				string fileName = HttpUtility.UrlEncode("报警记录" + DateTime.Now.GetDateTimeFormats('D')[1].ToString());
				//设置输出文件名
				context.Response.AppendHeader("Content-Disposition", "attachment; filename=" + fileName + ".xls");
				//输出
				context.Response.BinaryWrite(ms.GetBuffer());
				//只提供一次下载
				context.Session.Remove(key);
			} 
		}

		public bool IsReusable {
			get {
				return false;
			}
		}
	}
}