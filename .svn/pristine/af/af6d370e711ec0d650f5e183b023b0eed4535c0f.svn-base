using RTMC.Source;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Linq.Expressions;
using System.Text;
using System.Threading.Tasks;

namespace RTMC.BLL {
	public class DeviceManager {
		public static List<Device> GetDeviceByPage(int index, int size, out int total, Expression<Func<Device, bool>> where) {
			using (var db = new SystemContext()) {
				var list = db.Device.Where(where);
				total = list.Count();
				return list.OrderByDescending(t => t.CreateDate).Skip((index - 1) * size).Take(size).ToList();
			}
		}

		public static List<Device> GetDeviceByPage(string keyword, string code, int index, int size, out int total) {
			using (var db = new SystemContext()) {
				var list = db.Device.Where(d => d.IsDelete == false && d.State == 1 && d.Name.Contains(keyword)).ToList();
				total = 0;
				//根据登录用户的行政区域查看设备信息
				list = GetDeviceByCode(code, list);
				total = list.Count();
				list = list.OrderBy(d => d.Number).Skip((index - 1) * size).Take(size).ToList();
				return list;
			}
		}


		public static List<Device> GetDeviceByCode(string code, List<Device> list) { 
			if (code != "510000000") {
				if (code.Substring(4, 5) == "00000") {
					//code.Substring(4, 9) == 00000;
					//可查看市下面所以区县的
					code = code.Substring(0, 4);
					list = list.Where(b => b.Code.StartsWith(code)).ToList();
				} else {
					list = list.Where(b => b.Code == code).ToList();
				}
			}
			return list;
		}

		public static bool IsExist(Guid id, string number) {
			using (var db = new SystemContext()) {
				return db.Device.FirstOrDefault(d => d.IsDelete == false && d.Id != id && d.Number == number) != null;
			}
		}

		public static bool AddDevice(Device dev) {
			using (var db = new SystemContext()) {
				db.Device.Add(dev);
				return db.SaveChanges() > 0;
			}
		}

		public static bool EditDevice(Device dev) {
			using (var db = new SystemContext()) {
				db.Entry<Device>(dev).State = System.Data.Entity.EntityState.Modified;
				return db.SaveChanges() > 0;
			}
		}

		public static Device getDeviceById(Guid id) {
			using (var db = new SystemContext()) {
				return db.Device.FirstOrDefault(m => m.Id == id);
			}
		}

		//根据行政区域获取设备列表，排除已有锅炉的设备
		public static List<Device> GetDeviceList(Guid bId, string code) {
			using (var db = new SystemContext()) {
				List<Device> list = new List<Device>();
				var device = db.Device.Where(d => d.IsDelete == false && d.State == 1).ToList();
				device = GetDeviceByCode(code, device);
				var boiler = db.Boiler.ToList();
				foreach (var dev in device) {
					if (dev.BoilerId == null) {
						list.Add(dev);
					} else if (bId == dev.BoilerId) {
						list.Add(dev);
					} else {
						var i = 0;
						foreach (var boi in boiler) {
							if (boi.Id == dev.BoilerId) {
								i++;
								break;
							}
						}
						if (i == 0) {
							list.Add(dev);
						}
					}
				}
				return list;
			}
		}

		public static Device GetDeviceByBoilerId(Guid bId) {
			using (var db = new SystemContext()) {
				var poco = db.Device.Where(d => d.BoilerId == bId).FirstOrDefault();
				return poco;
			}
		}
	}
}
