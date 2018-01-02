using NPOI.HSSF.UserModel;
using NPOI.SS.UserModel;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using System.Web;

namespace RTMC.Core {
	public class ExcelHelper {

		public static HSSFWorkbook ListToExcel<T>(List<List<T>> list,List<string> sheetName, string name, params string[] propertyName) {
			//将参数写入到一个临时集合中
			List<string> propertyNameList = new List<string>();
			if (propertyName != null)
				propertyNameList.AddRange(propertyName);
			//创建NOPI的相关对象
			HSSFWorkbook workbook = new HSSFWorkbook();
			for (int k = 0; k < list.Count; k++) {
				ISheet sheet = workbook.CreateSheet(sheetName[k]);
				IRow headerRow = sheet.CreateRow(0);
				if (list[k].Count > 0) {
					//通过反射得到对象的属性集合
					PropertyInfo[] propertys = list[k][0].GetType().GetProperties();
					//遍历属性集合生成excel的表头标题
					for (int i = 0; i < propertys.Count(); i++) {
						//判断此属性是否是用户定义属性
						if (propertyNameList.Count == 0) {
							headerRow.CreateCell(i).SetCellValue(propertys[i].Name);
						} else {
							if (propertyNameList.Contains(propertys[i].Name))
								headerRow.CreateCell(i).SetCellValue(propertys[i].Name);
						}
					}
					int rowIndex = 1;
					//遍历集合生成excel的行集数据
					for (int i = 0; i < list[k].Count; i++) {
						IRow dataRow = sheet.CreateRow(rowIndex);
						for (int j = 0; j < propertys.Count(); j++) {
							if (propertyNameList.Count == 0) {
								object obj = propertys[j].GetValue(list[k][i], null);
								dataRow.CreateCell(j).SetCellValue(obj == null ? "" : obj.ToString());
							} else {
								if (propertyNameList.Contains(propertys[j].Name)) {
									object obj = propertys[j].GetValue(list[k][i], null);
									dataRow.CreateCell(j).SetCellValue(obj == null ? "" : obj.ToString());
								}
							}
						}
						rowIndex++;
					}
				}
			}
			return workbook;
		}

		public static string ListToExcel<T>(IList<T> list, string name, params string[] propertyName) {
			//将参数写入到一个临时集合中
			List<string> propertyNameList = new List<string>();
			if (propertyName != null)
				propertyNameList.AddRange(propertyName);
			//创建NOPI的相关对象
			IWorkbook workbook = new HSSFWorkbook();

			ISheet sheet = workbook.CreateSheet();
			IRow headerRow = sheet.CreateRow(0);
			if (list.Count > 0) {
				//通过反射得到对象的属性集合
				PropertyInfo[] propertys = list[0].GetType().GetProperties();
				//遍历属性集合生成excel的表头标题
				for (int i = 0; i < propertys.Count(); i++) {
					//判断此属性是否是用户定义属性
					if (propertyNameList.Count == 0) {
						headerRow.CreateCell(i).SetCellValue(propertys[i].Name);
					} else {
						if (propertyNameList.Contains(propertys[i].Name))
							headerRow.CreateCell(i).SetCellValue(propertys[i].Name);
					}
				}
				int rowIndex = 1;
				//遍历集合生成excel的行集数据
				for (int i = 0; i < list.Count; i++) {
					IRow dataRow = sheet.CreateRow(rowIndex);
					for (int j = 0; j < propertys.Count(); j++) {
						if (propertyNameList.Count == 0) {
							object obj = propertys[j].GetValue(list[i], null);
							dataRow.CreateCell(j).SetCellValue(obj == null ? "" : obj.ToString());
						} else {
							if (propertyNameList.Contains(propertys[j].Name)) {
								object obj = propertys[j].GetValue(list[i], null);
								dataRow.CreateCell(j).SetCellValue(obj == null ? "" : obj.ToString());
							}
						}
					}
					rowIndex++;
				}
			}

			string d = @"DownExcel\" + name + DateTime.Now.GetDateTimeFormats('D')[1].ToString().Replace(" ", "") + ".xls";
			string path = HttpRuntime.AppDomainAppPath.ToString() + d;
			//创建流对象
			using (FileStream fs = File.OpenWrite(path)) {
				workbook.Write(fs);
			}
			return (@"\" + d).Replace(@"\", "/");
		}
	}
}
