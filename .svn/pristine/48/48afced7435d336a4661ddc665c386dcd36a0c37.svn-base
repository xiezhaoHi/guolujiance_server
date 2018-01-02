using RTMC.Source.Model;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Linq.Expressions;
using System.Text;
using System.Threading.Tasks;
using RTMC.Source;
using System.Data.Entity.Core.Objects;
using NPOI.SS.Formula.Functions;
using System.IO;
using RTMC.Core;

namespace RTMC.BLL {
	public class DeviceDataManager {

		private static CacheClass cache = null;

		public static List<ViewDeviceData> GetDeviceDataByPage(string code, int index, int size, string key, out int total) {
			using (var db = new SystemContext()) {
				List<ViewDeviceData> list = new List<ViewDeviceData>();
				var models_dev = db.Device.Where(m => m.IsDelete == false && m.State == 1 && (m.Name.Contains(key) || m.Number.Contains(key))).ToList();
				models_dev = DeviceManager.GetDeviceByCode(code, models_dev);
				if (cache != null) {
					if (cache.key == key && cache.start == null && cache.end == null) {
						var mod = cache.list;
						var da = mod.OrderByDescending(m => m.CurrentTime).Skip((index - 1) * size).Take(size).ToList();
						total = mod.Count;
						DeviceDataToViewDeviceData(models_dev, da, list);
						return list;
					} else {
						cache = null;
					}
				}
				ObjectParameter ob = new ObjectParameter("str", "");
				db.sql_DeviceData(ob);
				string sql = ob.Value.ToString();
				//string sql_duo = sql.Substring(sql.LastIndexOf("UNION ALL"), 43) + " " + sql.Substring(sql.LastIndexOf("UNION ALL"), 43);
				//sql = sql.Replace(sql_duo, sql.Substring(sql.LastIndexOf("UNION ALL"), 43));
				List<Guid> ids = new List<Guid>();
				foreach (var item in models_dev) {
					ids.Add(item.Id);
				}
				var model = db.Database.SqlQuery<DeviceData>(sql).Where(m => ids.Contains(m.DeviceId)).ToList();
				total = model.Count;
				var data = model.OrderByDescending(m => m.CurrentTime).Skip((index - 1) * size).Take(size).ToList();
				DeviceDataToViewDeviceData(models_dev, data, list);
				cache = new CacheClass();
				cache.key = key;
				cache.list = model;
				cache.start = null;
				cache.end = null;
				return list;
			}
		}

		public static List<DeviceData> GetDeviceDataByPage(string code, int index, int size, string keyword, string startTime, string endTime, out int total) {
			using (var db = new SystemContext()) {
				ObjectParameter ob = new ObjectParameter("str", "");
				db.sql_DeviceData(ob);
				string sql = ob.Value.ToString();
				var list = db.Database.SqlQuery<DeviceData>(sql).ToList();
				var device = db.Device.Where(d => d.IsDelete == false && d.State == 1).ToList();
				List<Guid> devids = new List<Guid>();
				if (code != "510000000") {
					if (code.Substring(4, 5) == "00000") {
						//code.Substring(4, 9) == 00000;
						//可查看市下面所以区县的
						code = code.Substring(0, 4);
						device = device.Where(b => b.Code.StartsWith(code)).ToList();
					} else {
						device = device.Where(b => b.Code == code).ToList();
					}
				}
				foreach (var i in device) {
					devids.Add(i.Id);
				}
				list = list.Where(v => devids.Contains(v.DeviceId) && v.DeviceName.Contains(keyword)).ToList();
				if (startTime != "") {
					var stime = Parse.TryDateTime(startTime);
					list = list.Where(v => v.CurrentTime >= stime).ToList();
				}
				if (endTime != "") {
					var etime = Parse.TryDateTime(endTime);
					list = list.Where(v => v.CurrentTime < etime).ToList();
				}
				total = list.Count();
				return list.Skip((index - 1) * size).Take(size).ToList();
				#region
				//List<ViewDeviceData> list = new List<ViewDeviceData>();
				//var models_dev = db.Device.Where(m => m.IsDelete == false && m.State == 1 && (m.Name.Contains(key) || m.Number.Contains(key))).ToList();
				//models_dev = DeviceManager.GetDeviceByCode(code, models_dev);
				//if (cache != null) {
				//	if (cache.key == key && cache.start == start && cache.end == end) {
				//		var mod = cache.list;
				//		var da = mod.OrderByDescending(m => m.CurrentTime).Skip((index - 1) * size).Take(size).ToList();
				//		total = mod.Count;
				//		DeviceDataToViewDeviceData(models_dev, da, list);
				//		return list;
				//	} else {
				//		cache = null;
				//	}
				//}
				//ObjectParameter ob = new ObjectParameter("str", "");
				//db.sql_DeviceData(ob);
				//string sql = ob.Value.ToString();
				//string sql_duo = sql.Substring(sql.LastIndexOf("UNION ALL"), 43) + " " + sql.Substring(sql.LastIndexOf("UNION ALL"), 43);
				//sql = sql.Replace(sql_duo, sql.Substring(sql.LastIndexOf("UNION ALL"), 43));
				//List<Guid> ids = new List<Guid>();
				//foreach (var item in models_dev) {
				//	ids.Add(item.Id);
				//}
				//var model = db.Database.SqlQuery<DeviceData>(sql).Where(m => ids.Contains(m.DeviceId) && m.CurrentTime >= start && m.CurrentTime <= end).ToList();
				//total = model.Count;
				//var data = model.OrderByDescending(m => m.CurrentTime).Skip((index - 1) * size).Take(size).ToList();
				//DeviceDataToViewDeviceData(models_dev, data, list);
				//cache = new CacheClass();
				//cache.key = key;
				//cache.list = model;
				//cache.start = start;
				//cache.end = end;
				//return list;
				#endregion
			}
		}

		public static List<ViewDeviceData> SetViewDeviceData(List<DeviceData> data) {
			using (var db = new SystemContext()) {
				List<ViewDeviceData> list = new List<ViewDeviceData>();
				var models_dev = db.Device.Where(m =>1==1).ToList();
				DeviceDataToViewDeviceData(models_dev, data, list);
				return list;
			}
		}

		private static void DeviceDataToViewDeviceData(List<Device> models_dev, List<DeviceData> data, List<ViewDeviceData> list) {
			foreach (var item in data) {
				ViewDeviceData vdd = new ViewDeviceData();
				vdd.Address = item.Address;
				vdd.CompanyName = item.CompanyName;
				vdd.CurrentTime = item.CurrentTime;
				vdd.SulfurDioxide = item.SulfurDioxide;
				vdd.NitricOxide = item.NitricOxide;
				vdd.DeviceId = item.DeviceId;
				vdd.CarbonMonoxide = item.CarbonMonoxide;
				vdd.Latitude = item.Latitude;
				vdd.Longitude = item.Longitude;
				vdd.Hydrogen = item.Hydrogen;
				vdd.Oxygen = item.Oxygen;
				vdd.CarbonDioxide = item.CarbonDioxide;
				vdd.Name = models_dev.FirstOrDefault(m => m.Id == item.DeviceId) == null ? "" : models_dev.FirstOrDefault(m => m.Id == item.DeviceId).Name;
				vdd.Number = models_dev.FirstOrDefault(m => m.Id == item.DeviceId) == null ? "" : models_dev.FirstOrDefault(m => m.Id == item.DeviceId).Number;
				list.Add(vdd);
			}
		}



	}
	public class CacheClass {
		public List<DeviceData> list { get; set; }
		public DateTime? start { get; set; }
		public DateTime? end { get; set; }
		public string key { get; set; }
	}
}
