using RTMC.Core;
using RTMC.Source;
using RTMC.Source.Model;
using System;
using System.Collections.Generic;
using System.Data.Entity.Core.Objects;
using System.Linq;
using System.Linq.Expressions;
using System.Text;
using System.Threading.Tasks;

namespace RTMC.BLL {
	public class AlarmRecordManager {

		public static List<ViewDeviceAlarm> GetAlarmRecordByPage(int index, int size, string code, Expression<Func<ViewDeviceAlarm, bool>> where, out int total) {
			using (var db = new SystemContext()) {
				var devList = db.Device.Where(d => d.IsDelete == false && d.State == 1).ToList();
				devList = DeviceManager.GetDeviceByCode(code, devList);
				List<Guid?> dguid = new List<Guid?>();
				if (devList.Count() > 0) {
					foreach (var d in devList) {
						dguid.Add(d.Id);
					}
				}
				var lambda = PredicateBuilder.True<ViewDeviceAlarm>();
				lambda = lambda.And(m => dguid.Contains(m.DeviceId));
				if (where != null) { lambda = lambda.And(where); }
				var list = db.ViewDeviceAlarm.Where(where).ToList();
				total = list.Count();
				return list.OrderByDescending(t => t.AlarmTime).Skip((index - 1) * size).Take(size).ToList();
			}
		}

		public static ViewDeviceAlarm GetAlarmRecordById(Expression<Func<ViewDeviceAlarm, bool>> where) {
			using (var db = new SystemContext()) {
				var model = db.ViewDeviceAlarm.FirstOrDefault(where);
				return model;
			}
		}


	}
}
