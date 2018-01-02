using NPOI.HSSF.UserModel;
using NPOI.SS.UserModel;
using RTMC.BLL;
using RTMC.Core;
using RTMC.Source;
using RTMC.Source.Model;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Linq.Expressions;
using System.Reflection;
using System.Web;

namespace RTMC.Web.Handler {
	/// <summary>
	/// DeviceDataHandler 的摘要说明
	/// </summary>
	public class DeviceDataHandler : BaseHandler {
		public override void ExecuteToDo(HttpContext context, string action, out string result) {
			switch (action) {
				case "GetDeviceDataByPage":
					result = this.GetDeviceDataByPage(context);
					break;
				case "GetExcel": //读取excel文件
					result = this.GetData(context);
					break;
				case "InsertSource": //将excel文件中的内容插入数据库
					result = this.InsertSource(context);
					break;
				default:
					result = "无效请求！";
					break;
			}
		}

		private string InsertSource(HttpContext context) {
			Guid id = Parse.TryGuid(context.Request.Form["BoilerId"]);
			Boiler bb = BoilerManager.GetBoilerById(id);
			var data = datas;
			Guid idd = Guid.NewGuid();  //缺乏该字段。
			Dictionary<string, List<DeviceData>> dir = new Dictionary<string, List<DeviceData>>();
			foreach (var item in data) {
				DeviceData model = new DeviceData();
				model.BoilerId = id;
				model.SulfurDioxide = item.SulfurDioxide; //item.SulfurDioxide ? 1 : 0;
				model.NitricOxide = item.NitricOxide;
				model.CurrentTime = item.CurrentTime;
				model.Latitude = Parse.TryDecimal(bb.Latitude);
				model.Longitude = Parse.TryDecimal(bb.Longitude);
				model.DeviceId = idd;        //缺乏该字段。
				model.CarbonMonoxide = 0;
				model.Hydrogen = 0;
				model.Oxygen = 0;
				model.CarbonDioxide = 0;
				model.CompanyName ="";
				model.Address = "";
				string year = item.CurrentTime.Year.ToString();
				string month = item.CurrentTime.Month.ToString().Length == 1 ? ("0" + item.CurrentTime.Month.ToString()) : item.CurrentTime.Month.ToString();
				string day = item.CurrentTime.Day.ToString().Length == 1 ? ("0" + item.CurrentTime.Day.ToString()) : item.CurrentTime.Day.ToString();
				string TableName = "DeviceData_" + year + month + day;
				if (dir.Keys.Contains(TableName)) {
					dir[TableName].Add(model);
				} else {
					dir.Add(TableName, new List<DeviceData>() { model });
				}
			}
			GetDataManager.InsertDeviceDataTable(dir);
			return Function.SerializeObject(true);
		}

		private static List<DeviceData> models = new List<DeviceData>();
		private static List<TxtData> datas = new List<TxtData>();
		private static string p = null;

		public string GetData(HttpContext context) {
			string path = context.Request.Form["path"];
			int index = Parse.TryInt(context.Request.Form["index"]);
			int size = Parse.TryInt(context.Request.Form["size"]);
			if (path != p) {
				var pp = HttpRuntime.AppDomainAppPath + path.Replace("/", @"\").Substring(1);
				var houzhui = pp.Substring(pp.LastIndexOf('.'));
				switch (houzhui) {
					case ".txt":
						GetTxt(pp);
						var listTxt = datas.OrderByDescending(t => t.CurrentTime).Skip((index - 1) * size).Take(size).ToList();
						int total = datas.Count;
						File.Delete(pp);
						p = path;
						return Function.SerializeObject(new { total = total, data = listTxt });
					case ".xls":
					case ".xlsx":
						GetExcel(pp);
						var listExcel = models.OrderByDescending(t => t.CurrentTime).Skip((index - 1) * size).Take(size).ToList();
						var list_really = DeviceDataManager.SetViewDeviceData(listExcel);
						int totals = models.Count();
						File.Delete(pp);
						p = path;
						return Function.SerializeObject(new { total = totals, data = list_really });
					default:
						return "文件类型错误，请上传正确的文件！";
				}
			} else {
				var houzhui = p.Substring(p.LastIndexOf('.'));
				switch (houzhui) {
					case ".txt":
						var listTxt = datas.OrderByDescending(t => t.CurrentTime).Skip((index - 1) * size).Take(size).ToList();
						int total = datas.Count;
						return Function.SerializeObject(new { total = total, data = listTxt });
					case ".xls":
					case ".xlsx":
						var listExcel = models.OrderByDescending(t => t.CurrentTime).Skip((index - 1) * size).Take(size).ToList();
						var list_really = DeviceDataManager.SetViewDeviceData(listExcel);
						int totals = models.Count();
						return Function.SerializeObject(new { total = totals, data = list_really });
					default:
						return "文件类型错误，请上传正确的文件！";
				}
			}
		}

		private void GetExcel(string path) {
			using (FileStream fs = File.OpenRead(path))   //打开.xls文件
            {
				HSSFWorkbook wk = new HSSFWorkbook(fs);   //把xls文件中的数据写入wk中
				List<DeviceData> list = new List<DeviceData>();
				Type atype = typeof(DeviceData);
				for (int i = 0; i < wk.NumberOfSheets; i++)  //NumberOfSheets是.xls中总共的表数
                {
					ISheet sheet = wk.GetSheetAt(i);   //读取当前表数据
					List<string> heads = new List<string>();
					for (int j = 0; j <= sheet.LastRowNum; j++)  //LastRowNum 是当前表的总行数
                    {
						if (j != 0) {
							IRow row = sheet.GetRow(j);  //读取当前行数据
							if (row != null) {
								if (heads.Count > 0) {
									DeviceData model = new DeviceData();
									for (int k = 0; k <= row.LastCellNum; k++) {//LastCellNum 是当前行的总列数
										ICell cell = row.GetCell(k);  //当前表格
										if (cell != null) {
											SetValueToModel(heads, atype, model, k, cell);
										}
									}
									list.Add(model);
								}
							}
						} else {
							IRow row = sheet.GetRow(j);  //读取当前行数据
							if (row != null) {
								for (int k = 0; k <= row.LastCellNum; k++)  //LastCellNum 是当前行的总列数
                            {
									ICell cell = row.GetCell(k);  //当前表格
									if (cell != null) {
										heads.Add(cell.ToString());
									}
								}
							}
						}
					}
				}
				models = list;
			}
		}


		private void GetTxt(string path) {
			string[] reads = File.ReadAllLines(path);
			List<TxtData> list = new List<TxtData>();
			foreach (var item in reads) {
				TxtData models = new TxtData();
				string[] model = item.Split(',');
				models.CurrentTime = Parse.TryDateTime(model[0]);
				models.SulfurDioxide = Parse.TryLong(model[1]);
				models.NitricOxide = Parse.TryLong(model[2]);
				models.CarbonMonoxide = Parse.TryLong(model[3]);
				models.Hydrogen = Parse.TryLong(model[4]);
				models.Oxygen = Parse.TryDecimal(model[5]);
				models.CarbonDioxide = Parse.TryDecimal(model[6]);
			//	models.FlameTemperature = Parse.TryInt(model[2]) == 0 ? false : true;
				list.Add(models);
			}
			datas = list;
		}

		/// <summary>
		/// 给model中的字段赋值
		/// </summary>
		/// <param name="heads"></param>
		/// <param name="atype"></param>
		/// <param name="model"></param>
		/// <param name="k"></param>
		/// <param name="cell"></param>
		private static void SetValueToModel(List<string> heads, Type atype, DeviceData model, int k, ICell cell) {
			PropertyInfo pi = atype.GetProperties().FirstOrDefault(x => x.Name == heads[k]);
			if (pi != null) {
				string p = pi.PropertyType.GenericTypeArguments.Length > 0 ? pi.PropertyType.GenericTypeArguments[0].Name : pi.PropertyType.Name;
				switch (p) {
					case "String":
						pi.SetValue(model, cell.ToString());
						break;
					case "DateTime":
					case "DateTime?":
						pi.SetValue(model, cell.DateCellValue);
						break;
					case "Double":
					case "Double?":
						pi.SetValue(model, Parse.TryDoubleAble(cell));
						break;
					case "Int":
					case "Int?":
						pi.SetValue(model, Parse.TryIntAble(cell));
						break;
					case "Guid":
					case "Guid?":
						pi.SetValue(model, Parse.TryGuid(cell));
						break;
					default:
						break;
				}
			}

		}

		private string GetDeviceDataByPage(HttpContext context) {
			int index = Parse.TryInt(context.Request.Form["index"]);
			int size = Parse.TryInt(context.Request.Form["size"]);
			string keyword = Parse.ToString(context.Request.Form["key"]);
			string stime = Parse.ToString(context.Request.Form["start"]);
			string etime = Parse.ToString(context.Request.Form["end"]);
			SYS_Users user = (SYS_Users)context.Session["User"];
			int total = 0;
			var list = DeviceDataManager.GetDeviceDataByPage(user.Code, index, size, keyword, stime, etime, out total);
			//List<ViewDeviceData> list = new List<ViewDeviceData>();
			//if ((starttime == null || starttime == "") && (endtime == null || endtime == "")) {
			//	list = DeviceDataManager.GetDeviceDataByPage(user.Code, index, size, key, out total);
			//} else {
			//	DateTime start = Parse.TryDateTime(starttime);
			//	DateTime end = Parse.TryDateTime(endtime);
			//	list = DeviceDataManager.GetDeviceDataByPage(user.Code, index, size, key, start, end, out total);
			//}
			return Function.SerializeObject(new { total = total, data = list });
		}
		//public bool IsReusable {
		//	get {
		//		return false;
		//	}
		//}
	}
}