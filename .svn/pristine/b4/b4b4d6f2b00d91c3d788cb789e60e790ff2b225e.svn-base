using RTMC.Core;
using RTMC.Source;
using RTMC.Source.Model;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;
using System.Web;
using System.IO;

namespace RTMC.BLL {
	public class GetDataManager {
		public GetDataManager() {

		}
		private static Random r = new Random(DateTime.Now.Millisecond * DateTime.Now.Second);


		public static ThresholdData GetThresholdData(Guid id) {
			ThresholdData model = new ThresholdData();
			XmlDocument doc = new XmlDocument();  //创建xml对象
			string path = System.AppDomain.CurrentDomain.BaseDirectory;  //获取程序的根目录,因为在类中不能使用HttpRuntime.AppDomainAppPath来获取根目录
			if (path.Contains("TestingServices")) {
				path = path.Substring(0, path.IndexOf("TestingServices")) + @"RTMC.Web\";
			}
			doc.Load(path + @"AlertLine.xml");  //加载xml文件
			XmlNode xn = doc.SelectSingleNode("AppSetting");
			XmlNode x = xn.SelectSingleNode("AlertLine" + id.ToString().ToUpper());//获得需要的节点
			if (x!=null) {
				XmlNodeList xnl = x.ChildNodes;  //获得所有的子节点
				foreach (var item in xnl) {
					XmlElement xe = (XmlElement)item;
					string key = xe.GetAttribute("key");
					var expType = model.GetType().GetProperties().FirstOrDefault(m => m.Name == key);
					expType.SetValue(model, Parse.TryDouble(xe.GetAttribute("value")));
				}
				return model;
			}
			return null;
		}
		public static bool GetVirData() {
			//拿到所有的id
			var model = new List<Device>();
			List<DeviceData> list = new List<DeviceData>();
			int i = 0;
			using (var db = new SystemContext()) {
				model = db.Device.Where(m => m.State == 1).ToList();
				foreach (var item in model) {
					DeviceData ll = new DeviceData();
					ll.DeviceId = item.Id;
					ll.Longitude = (decimal)item.Longitude;
					ll.Latitude = (decimal)item.Latitude;
					ll.SulfurDioxide = r.Next(1, 1000);
					ll.NitricOxide = r.Next(1, 1000);
					ll.CarbonMonoxide = r.Next(1, 1000);
					ll.Hydrogen = r.Next(1, 1000);
					ll.Oxygen = r.Next(1, 100);
					ll.CarbonDioxide = r.Next(1, 100);
					ll.CurrentTime = DateTime.Now;
					ll.Address = "";
					ll.CompanyName = "";
					list.Add(ll);
					i = InsertAlarmRecordTable(db, ll, item.Id)?i+1:i;
				}
				InsertDeviceDataTable(list, db);
				db.SaveChanges();
				return i > 0;
			}
		}//生成模拟数据，并且将其中的报警记录都插入到数据库中去。

		private static bool InsertAlarmRecordTable(SystemContext db, DeviceData ll, Guid id) {
			var models = GetThresholdData(id);
			bool b = false;
			if (models==null) {
				return false;
			}
			if (ll.SulfurDioxide > models.SulfurDioxideAlertLineCeiling || ll.SulfurDioxide < models.SulfurDioxideAlertLineFloor) {
				AlarmRecord l = GetAlarmRecordByDeviceData(ll, "FlameTemperature");
				l.AlarmDataCeiling = models.SulfurDioxideAlertLineCeiling;
				l.AlarmDataFloor = models.SulfurDioxideAlertLineFloor;
				l.AlarmData = ll.SulfurDioxide;
				db.AlarmRecord.Add(l);
				b = true;
			}
			if (ll.NitricOxide > models.NitricOxideAlertLineCeiling || ll.NitricOxide < models.NitricOxideAlertLineFloor) {
				AlarmRecord l = GetAlarmRecordByDeviceData(ll, "NegativePressure");
				l.AlarmDataCeiling = models.NitricOxideAlertLineCeiling;
				l.AlarmDataFloor = models.NitricOxideAlertLineFloor;
				l.AlarmData = ll.NitricOxide;
				db.AlarmRecord.Add(l);
				b = true;
			}
			if (ll.CarbonMonoxide > models.CarbonMonoxideAlertLineCeiling || ll.CarbonMonoxide < models.CarbonMonoxideAlertLineFloor) {
				AlarmRecord l = GetAlarmRecordByDeviceData(ll, "Data3");
				l.AlarmDataCeiling = models.CarbonMonoxideAlertLineCeiling;
				l.AlarmDataFloor = models.CarbonMonoxideAlertLineFloor;
				l.AlarmData = ll.CarbonMonoxide;
				db.AlarmRecord.Add(l);
				b = true;
			}
			if (ll.Hydrogen > models.HydrogenAlertLineCeiling || ll.Hydrogen < models.HydrogenAlertLineFloor) {
				AlarmRecord l = GetAlarmRecordByDeviceData(ll, "Data4");
				l.AlarmDataCeiling = models.HydrogenAlertLineCeiling;
				l.AlarmDataFloor = models.HydrogenAlertLineFloor;
				l.AlarmData = ll.Hydrogen;
				db.AlarmRecord.Add(l);
				b = true;
			}
			if (ll.Oxygen > models.OxygenAlertLineCeiling || ll.Oxygen < models.OxygenAlertLineFloor) {
				AlarmRecord l = GetAlarmRecordByDeviceData(ll, "Data4");
				l.AlarmDataCeiling = models.OxygenAlertLineCeiling;
				l.AlarmDataFloor = models.OxygenAlertLineFloor;
				l.AlarmData = ll.Oxygen;
				db.AlarmRecord.Add(l);
				b = true;
			}
			if (ll.CarbonDioxide > models.CarbonDioxideAlertLineCeiling || ll.CarbonDioxide < models.CarbonDioxideAlertLineFloor) {
				AlarmRecord l = GetAlarmRecordByDeviceData(ll, "Data4");
				l.AlarmDataCeiling = models.CarbonDioxideAlertLineCeiling;
				l.AlarmDataFloor = models.CarbonDioxideAlertLineFloor;
				l.AlarmData = ll.CarbonDioxide;
				db.AlarmRecord.Add(l);
				b = true;
			}
			return b;
		}

		public static void InsertDeviceDataTable(List<DeviceData> list, SystemContext db) {
			string year = DateTime.Now.Year.ToString();
			string month = DateTime.Now.Month.ToString().Length == 1 ? ("0" + DateTime.Now.Month.ToString()) : DateTime.Now.Month.ToString();
			string day = DateTime.Now.Day.ToString().Length == 1 ? ("0" + DateTime.Now.Day.ToString()) : DateTime.Now.Day.ToString();
			string name = "DeviceData_" + year + month + day;
			db.sql_DeviceDataTable(name);
			string sql = Function.GetSqlByInsert<DeviceData>(list, name);
			if (sql != null) {
				db.Database.ExecuteSqlCommand(sql);
			}
		}

		public static void InsertDeviceDataTable(Dictionary<string,List<DeviceData>> dir) {
			using (var db = new SystemContext()) {
				foreach (var item in dir) {
					db.sql_DeviceDataTable(item.Key);
					string sql = Function.GetSqlByInsert<DeviceData>(item.Value,item.Key);
					if (sql != null) {
						db.Database.ExecuteSqlCommand(sql);
					}
				}
			}
			
		}

		private static AlarmRecord GetAlarmRecordByDeviceData(DeviceData item, string type) {
			AlarmRecord l = new AlarmRecord();
			l.AlarmTime = DateTime.Now;
			l.CurrentTime = item.CurrentTime;
			l.AlarmDataType_English = type;
			l.DeviceId = item.DeviceId;
			l.Id = Guid.NewGuid();
			return l;
		}
	}
}
