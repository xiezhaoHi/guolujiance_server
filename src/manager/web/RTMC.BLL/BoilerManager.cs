using RTMC.Source;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RTMC.BLL {
	public class BoilerManager {

		public static List<Boiler> GetBoiler() {
			using (var db = new SystemContext()) {
				var model = db.Boiler.Where(m => 1 == 1).ToList();
				return model;
			}
		}

		public static Boiler GetBoilerById(Guid id) {
			using (var db = new SystemContext()) {
				var model = db.Boiler.FirstOrDefault(m => m.Id == id);
				return model;
			}
		}

		public static List<Boiler> GetBoilerByPage(string code,string keyword, int index, int size, out int total) {
			using (var db = new SystemContext()) {
				var list = db.Boiler.Where(b => b.Name.Contains(keyword)).ToList();
				if (code != "510000000") {
					var aa = code.Substring(4, 5);
					if (code.Substring(4, 5) == "00000") {
						//可查看市下面所以区县的
						code = code.Substring(0, 4);
						list = list.Where(b => b.Code.StartsWith(code)).ToList();
					} else {
						list = list.Where(b => b.Code == code).ToList();
					}
				}
				total = 0;
				if (list.Count() == 0) return null;
				total = list.Count();
				list = list.OrderBy(b => b.Code).Skip((index - 1) * size).Take(size).ToList();
				return list;
			}
		}

		//获取锅炉列表，除去已绑定设备的锅炉
		public static List<Boiler> GetBoilerList() {
			using (var db = new SystemContext()) {
				List<Boiler> list = new List<Boiler>();
				var poco = db.Boiler.ToList();
				var poco1 = db.Device.ToList();
				foreach (var boi in poco) {
					foreach (var dev in poco1) {
						if (dev.BoilerId == boi.Id) {
							continue;
						} else {
							list.Add(boi);
						}
					}
				}
				return list;
			}
		}

		public static bool AddBoiler(Boiler model) {
			using (var db = new SystemContext()) {
				db.Boiler.Add(model);
				return db.SaveChanges() > 0;
			}
		}

		public static bool EditBoiler(Boiler model) {
			using (var db = new SystemContext()) {
				db.Entry<Boiler>(model).State = System.Data.Entity.EntityState.Modified;
				return db.SaveChanges() > 0;
			}
		}

		public static bool DelBoiler(Guid id) {
			using (var db = new SystemContext()) {
				//判断是否有设备
				var poco = db.Device.Where(d => d.BoilerId == id).Count();
				if (poco > 0) {
					return false;
				}
				var model = new Boiler();
				model.Id = id;
				db.Boiler.Attach(model);
				db.Boiler.Remove(model);
				return db.SaveChanges() > 0;
			}
		}

		public static bool AllotBoiler(Guid boilerId, Guid devId) {
			using (var db = new SystemContext()) {
				var view = db.Device.Where(d => d.BoilerId == boilerId).ToList();
				if (view.Count() > 0) {
					foreach (var item in view) {
						item.BoilerId = null;
					}
				} 
				var poco = db.Device.FirstOrDefault(d => d.Id == devId);
				if (poco == null) return false;
				poco.BoilerId = boilerId;
				return db.SaveChanges() > 0;
			}
		}
	}
}
