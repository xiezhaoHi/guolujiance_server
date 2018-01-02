using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using RTMC.Source;
using System.Configuration;
using System.Data.SqlClient;
using System.Data;
using System.Data.Entity.Core.Objects;
using RTMC.Source.Model;
using RTMC.Core;
using System.Xml;
using System.Web;
using System.Reflection;

namespace RTMC.BLL {
    public class MapManager {

        //private static int count = 0;
        private static DeviceData panduan = new DeviceData();
        public static List<Guid> GetIdByKey(string key, DateTime StartTime, DateTime EndTime) {
            using (var db = new SystemContext()) {
                var models = db.ViewDeviceAlarm.Where(m => (m.Name.Contains(key) || m.Number.Contains(key) || m.AlarmDataType_Chinese.Contains(key)) && m.AlarmTime >= StartTime && m.AlarmTime <= EndTime).ToList();
                List<Guid> list = new List<Guid>();
                if (models.Count > 1) {
                    foreach (var item in models) {
                        if (list.Contains(Parse.TryGuid(item.DeviceId))) {
                            continue;
                        } else {
                            list.Add(Parse.TryGuid(item.DeviceId));
                        }
                    }
                } else {
                    Guid id = Parse.TryGuid(models[0].DeviceId);
                    list.Add(id);
                }
                return list;
            }
        }  //根据传进来的关键字来获得所符合条件的ID集合

        public static object ExportToExcel_Alarm(List<Guid> ids, DateTime StartDay, DateTime EndDay) {
            using (var db = new SystemContext()) {
                List<List<AlarmRecord>> list = new List<List<AlarmRecord>>();
                List<string> name = new List<string>();
                for (int i = 0 ; i < ids.Count ; i++) {
                    Guid id = ids[i];
                    var models = db.Device.Where(m => m.Id == id).ToList()[0].Name;
                    name.Add(models);
                    var modelss = db.AlarmRecord.Where(m => m.AlarmTime >= StartDay && m.AlarmTime <= EndDay && m.DeviceId == id).OrderByDescending(m => m.AlarmTime).ToList();
                    list.Add(modelss);
                }
                var workbook = ExcelHelper.ListToExcel<AlarmRecord>(list, name, "报警记录");
                return workbook;
            }
        }  //导出所有的报警记录,数据库的数据

        #region GetAllDeviceData
        public static List<Device> GetAllDeviceData(string code) {
            using (var db = new SystemContext()) {
                //List<Device> list = new List<Device>();
                //根据登录用户的行政区域查看设备信息
                var list = db.Device.Where(d => d.IsDelete == false && d.State == 1).ToList();
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
                var poco = db.AlarmRecord.ToList();
                if (poco.Count() == 0) return list;
                foreach (var item in list) {
                    poco = poco.Where(a => a.DeviceId == item.Id).ToList();
                    if (poco.Count() == 0) {
                        item.AlarmState = 0;  //表示正常
                    } else {
                        foreach (var i in poco) {
                            if (i.AlarmData > i.AlarmDataFloor && i.AlarmDataCeiling > i.AlarmData) {
                                item.AlarmState = 0;  //正常
                            } else if (i.AlarmData < i.AlarmDataFloor && i.AlarmData > i.AlarmDataCeiling) {
                                item.AlarmState = 1;  //警告
                            } else if (i.AlarmData > i.AlarmDataCeiling && i.AlarmData > 700) {
                                item.AlarmState = 2;  //超标
                            }
                        }
                    }
                }
                return list;
            }
        }  //真实数据，作用为显示在地图上的点的坐标

        public static List<Boiler> GetBoilerData(string code, SystemContext db) {
            var list = db.Boiler.ToList();
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
        #endregion

        public static List<DeviceData> GetDeviceDataById(Guid id) {
            //GetData.GetSmoothData(id);
            using (var db = new SystemContext()) {
                ObjectParameter ob = new ObjectParameter("str", "");
                db.sql_DeviceDataById(id.ToString(), ob);
                string sql = ob.Value.ToString();
                var list = db.Database.SqlQuery<DeviceData>(sql).ToList();
                return list;
            }
        }   //真实数据，得到最新的一条数据，按照每个设备来查找

        #region GetDeviceDataByIdFirst and GetDeviceDataByIdFirstXiangxi
        public static object GetDeviceDataByIdFirst(Guid id) {
            using (var db = new SystemContext()) {
                //var sql = "SELECT name FROM sys.tables WHERE [type]='U' AND name LIKE 'DeviceData%'";
                //var tables = db.Database.SqlQuery<string>(sql, null).ToList();
                //for (var i = 0; i < tables.Count;i++ ) {
                //	tables[i] = "SELECT * FROM " + tables[i];
                //}
                //sql = string.Format("SELECT * FROM ({0}) A WHERE DeviceId=@DeviceId ORDER BY CurrentTime DESC", string.Join(" UNION ALL ", tables));

                ObjectParameter ob = new ObjectParameter("str", "");
                db.sql_DeviceDataByIdFirst(id.ToString(), ob);
                string sql = ob.Value.ToString();
                //string sql_duo = sql.Substring(sql.LastIndexOf("UNION ALL"), 43) + " " + sql.Substring(sql.LastIndexOf("UNION ALL"), 43);
                //sql = sql.Replace(sql_duo, sql.Substring(sql.LastIndexOf("UNION ALL"), 43));
                var model = db.Database.SqlQuery<DeviceData>(sql).OrderByDescending(d => d.CurrentTime).Take(500).ToList();
                List<DeviceData> list = new List<DeviceData>();
                if (model.Count == 0) return null;
                //if (model.Count >= 120) {  //大于120条数据时就进行平滑
                //	SetModelData(model, list);
                //} else {  //否则直接输出该集合
                list.AddRange(model);
                //}
                panduan = list[0];
                return list;
            }
        }  //初始化界面时加载的数据,真实数据

        public static object GetDeviceDataByIdFirstXiangxi(Guid id) {
            using (var db = new SystemContext()) {
                ObjectParameter ob = new ObjectParameter("str", "");
                db.sql_DeviceDataByIdFirstXiangxi(id.ToString(), ob);
                string sql = ob.Value.ToString();
                //string sql_duo = sql.Substring(sql.LastIndexOf("UNION ALL"), 43) + " " + sql.Substring(sql.LastIndexOf("UNION ALL"), 43);
                //sql = sql.Replace(sql_duo, sql.Substring(sql.LastIndexOf("UNION ALL"), 43));
                var model = db.Database.SqlQuery<DeviceData>(sql).OrderByDescending(d => d.CurrentTime).ToList();
                List<DeviceData> list = new List<DeviceData>();
                if (model.Count >= 390) {  //大于390条数据时就进行平滑
                    SetModelData(model, list);
                } else {  //否则直接输出该集合
                    list.AddRange(model);
                }
                panduan = list[0];
                return list;
            }
        }//初始化详细界面时加载的数据,真实数据

        private static void SetModelData(List<DeviceData> model, List<DeviceData> list) {
            for (int i = 0 ; i < model.Count - 90 ; i++) {
                long sum1 = 0;
                long sum2 = 0;
                long sum3 = 0;
                long sum4 = 0;
                decimal sum5 = 0;
                decimal sum6 = 0;
                GetNumData(model, i, ref sum1, ref sum2, ref sum3, ref sum4, ref sum5, ref sum6);
                model[i].SulfurDioxide = sum1 / 90;
                model[i].NitricOxide = sum2 / 90;
                model[i].CarbonMonoxide = sum3 / 90;
                model[i].Hydrogen = sum4 / 90;
                model[i].Oxygen = sum5 / 100; //百分比
                model[i].CarbonDioxide = sum6 / 100;  //百分比
                list.Add(model[i]);
            }
        }

        private static void GetNumData(List<DeviceData> model, int i, ref long sum1, ref long sum2, ref long sum3, ref long sum4, ref decimal sum5, ref decimal sum6) {
            for (int j = i + 1 ; j < i + 90 ; j++) {
                sum1 += model[j].SulfurDioxide;
                sum2 += model[j].NitricOxide;
                sum3 += model[j].CarbonMonoxide;
                sum4 += model[j].Hydrogen;
                sum5 += model[j].Oxygen;
                sum6 += model[j].CarbonDioxide;
            }
        }
        #endregion

        #region GetDataByBili
        public static object GetDataByBili(int bili, Guid id) {
            List<DeviceData> list_model = new List<DeviceData>();
            using (var db = new SystemContext()) {
                ObjectParameter ob = new ObjectParameter("str", "");
                db.sql_DeviceDataHistorical(id.ToString(), ob);
                string sql = ob.Value.ToString();
                //string sql_duo = sql.Substring(sql.LastIndexOf("UNION ALL"), 43) + " " + sql.Substring(sql.LastIndexOf("UNION ALL"), 43);
                //sql = sql.Replace(sql_duo, sql.Substring(sql.LastIndexOf("UNION ALL"), 43));
                list_model = db.Database.SqlQuery<DeviceData>(sql).ToList();
            }
            var bili1 = (300.000000 / list_model.Count) * 100;
            int b = 0;
            var p = bili1.ToString().Split('.');
            b = GetSiSheWuRu(b, p);
            List<DeviceData> list_data = new List<DeviceData>();
            GetListBySiSheWuRu(bili, list_model, b, list_data);
            List<DeviceData> list = new List<DeviceData>();
            if (list_data.Count > 90) {
                SetModelData(list_data, list);
            } else {
                list.AddRange(list_data);
            }
            return list;
        }  //拿到当前进度条所在比例的数据，真实数据

        private static void GetListBySiSheWuRu(int bili, List<DeviceData> list_model, int b, List<DeviceData> list_data) {
            if (100 - bili <= b) {
                for (int i = list_model.Count - 300 ; i < list_model.Count ; i++) {
                    list_data.Add(list_model[i]);
                }
            } else {
                int w = list_model.Count / 100 * bili;
                for (int j = w ; j < w + 300 ; j++) {
                    list_data.Add(list_model[j]);
                }
            }
        }

        private static int GetSiSheWuRu(int b, string[] p) {
            if (p.Length == 1) {
                b = Parse.TryInt(p[0]);
            } else {
                if (Parse.TryInt(p[1].Substring(0, 1)) >= 5) {
                    b = Parse.TryInt(p[0]) + 1;
                } else {
                    b = Parse.TryInt(p[0]);
                }
            }
            return b;
        }
        #endregion

        public static object GetDataLength(Guid id) {
            using (var db = new SystemContext()) {
                ObjectParameter ob = new ObjectParameter("str", "");
                db.sql_DeviceDataHistorical(id.ToString(), ob);
                string sql = ob.Value.ToString();
                //string sql_duo = sql.Substring(sql.LastIndexOf("UNION ALL"), 43) + " " + sql.Substring(sql.LastIndexOf("UNION ALL"), 43);
                //sql = sql.Replace(sql_duo, sql.Substring(sql.LastIndexOf("UNION ALL"), 43));
                var models = db.Database.SqlQuery<DeviceData>(sql).Count();
                return models;
            }
            //return GetData.GetDataLength(id);
        }  //得到数据的总条数，真实数据

        public static List<DeviceData> GetDeviceHistoryListByPage(Guid devId, int index, int size, out int total) {
            using (var db = new SystemContext()) {
                ObjectParameter ob = new ObjectParameter("str", "");
                db.sql_DeviceDataHistorical(devId.ToString(), ob);
                string sql = ob.Value.ToString();
                //string sql_duo = sql.Substring(sql.LastIndexOf("UNION ALL"), 43) + " " + sql.Substring(sql.LastIndexOf("UNION ALL"), 43);
                //sql = sql.Replace(sql_duo, sql.Substring(sql.LastIndexOf("UNION ALL"), 43));

                //string sql1 = sql.Replace("order by CurrentTime desc", " ");
                //string sql2 = "select count(1) from (" + sql1 + ") as c ";

                #region 取消 DISTINCT  sql4= select sum(num1) from (select count(1)  as num1 from DeviceData_20170207 UNION ALL select count(1)  as num1 from DeviceData_20170208 UNION ALL ........)
                string sql3 = sql.Replace("order by CurrentTime desc", " ").Replace("select DISTINCT* from (", "  ").Replace(") as tab ", "  ").Replace("*", " count(1)  as num1 ");
                string sql4 = "select sum(num1) from (" + sql3 + ") as c ";
                string sql5 = sql.Replace("order by CurrentTime desc", " ").Replace("select DISTINCT* from (", "  ").Replace(") as tab ", "  ");
                #endregion 
                var co = db.Database.SqlQuery<int>(sql4).First();
                total = Convert.ToInt32(co);
                if (total == 0) return null;
                string sqlQuery = " select  * from (select row_number()over(order by CurrentTime desc ) rownumber,* from (" + sql5 + ") as a ) x where rownumber>= " + ((index - 1) * size) + " and rownumber<=" + (index * size);
                var list = db.Database.SqlQuery<DeviceData>(sqlQuery).ToList();
                return list;
                //return null;
                //var list = db.Database.SqlQuery<DeviceData>(sql).ToList();
                //total = list.Count();
                //if (list.Count() == 0) return null;
                //list = list.Skip((index - 1) * size).Take(size).ToList();
                //return list;
            }
        }

        #region Chart Data
        public static List<DeviceData> GetChartDataById(Guid id) {
            using (var db = new SystemContext()) {
                //var sql = "SELECT name FROM sys.tables WHERE [type]=@Type AND name LIKE @Name";
                var sql = "SELECT TOP 7 name FROM sys.tables WHERE [type]=@Type AND name LIKE @Name ORDER BY name DESC";
                var tables = db.Database.SqlQuery<string>(sql, new SqlParameter("@Type", "U"), new SqlParameter("@Name", "DeviceData%")).ToArray();
                for (var i = 0 ; i < tables.Length ; i++) {
                    tables[i] = "SELECT * FROM " + tables[i];
                }
                sql = string.Format("SELECT * FROM ({0}) A WHERE DeviceId=@DeviceId AND CurrentTime>@DateTime ORDER BY CurrentTime", string.Join(" UNION ALL ", tables));
                var list = db.Database.SqlQuery<DeviceData>(sql, new SqlParameter("@DeviceId", id), new SqlParameter("@DateTime", DateTime.Now.AddHours(-24))).ToList();
                return ReGroup(list);
            }
        }

        /// <summary>
        /// 获取指定日期的曲线数据
        /// </summary>
        /// <param name="id">设备编号</param>
        /// <param name="date">日期，时间部分会被忽略</param>
        /// <returns></returns>
        public static List<DeviceData> GetChartDataById(Guid id, DateTime date) {
            using (var db = new SystemContext()) {
                var sql = string.Format("SELECT * FROM dbo.DeviceData_{0} WHERE DeviceId=@DeviceId ORDER BY CurrentTime", date.ToString("yyyyMMdd"));
                var list = db.Database.SqlQuery<DeviceData>(sql, new SqlParameter("@DeviceId", id)).ToList();
                return ReGroup(list);
            }
        }

        public static List<DeviceData> UpdateChartDataById(Guid id, DateTime date) {
            using (var db = new SystemContext()) {
                var sql = "SELECT TOP 7 name FROM sys.tables WHERE [type]=@Type AND name LIKE @Name ORDER BY name DESC";
                var tables = db.Database.SqlQuery<string>(sql, new SqlParameter("@Type", "U"), new SqlParameter("@Name", "DeviceData%")).ToArray();
                for (var i = 0 ; i < tables.Length ; i++) {
                    tables[i] = "SELECT * FROM " + tables[i];
                }
                sql = string.Format("SELECT * FROM ({0}) A WHERE DeviceId=@DeviceId AND CurrentTime>@StartTime AND CurrentTime<=@EndTime ORDER BY CurrentTime", string.Join(" UNION ALL ", tables));
                var list = db.Database.SqlQuery<DeviceData>(sql, new SqlParameter("@DeviceId", id), new SqlParameter("@StartTime", date), new SqlParameter("@EndTime", DateTime.Now)).ToList();
                return ReGroup(list);
            }
        }

        private static List<DeviceData> ReGroup(List<DeviceData> list) {
            list = list.GroupBy(g => new { TimeKey = (long) g.TimeStamp.TotalMinutes })
                .Select(d => new DeviceData {
                    TimeStamp = TimeSpan.FromMinutes(d.Key.TimeKey),
                    O2Min = d.Min(a => a.Oxygen), O2Max = d.Max(a => a.Oxygen),
                    CO2Min = d.Min(a => a.CarbonDioxide), CO2Max = d.Max(a => a.CarbonDioxide),
                    SO2Min = d.Min(a => a.SulfurDioxide), SO2Max = d.Max(a => a.SulfurDioxide),
                    NOMin = d.Min(a => a.NitricOxide), NOMax = d.Max(a => a.NitricOxide),
                    COMin = d.Min(a => a.CarbonMonoxide), COMax = d.Max(a => a.CarbonMonoxide),
                    NOxMin = d.Min(a => a.Hydrogen), NOxMax = d.Max(a => a.Hydrogen)
                }).OrderBy(a => a.TimeStamp).ToList();
            return list;
        }
        #endregion


    }
}
