using RTMC.Core;
using RTMC.Source;
using System;
using System.Collections.Generic;
using System.Configuration;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RTMC.BLL {
	public class BaseCodeManager {
		public static List<BaseCode> GetBaseCode(string Code) {
			using (var db = new SystemContext()) {
				if (Code == null) Code = GetCurrentProvinceCode();
				var province = Code.Substring(0, 2);
				return db.BaseCode.Where(c => c.Code.StartsWith(province) && c.Code != Code).OrderBy(c => c.Code).ToList();
			}
		}

		public static string GetCurrentProvinceCode() {
			var code = Parse.ToString(ConfigurationManager.AppSettings["AreaCode"], "510000");
			return code;
		}

		public static List<BaseCodeTreeModel> GetBaseCodeList(string code) {
			using (var db = new SystemContext()) {
				List<BaseCodeTreeModel> list = new List<BaseCodeTreeModel>();
				var poco = db.BaseCode.Where(c => c.Code != code).OrderBy(c => c.Code).ToList();
				if (poco.Count() == 0) return null;
				foreach (var i in poco.Where(c => c.Code.EndsWith("00000"))) {
					var model = new BaseCodeTreeModel();
					model.Code = i.Code;
					model.Name = i.Name;
					var childs = new List<BaseCodeTreeModel>();
					var poco1 = poco.Where(c => c.Code.StartsWith(i.Code.Substring(0, 4)) && !c.Code.EndsWith("00000")).ToList();
					foreach (var j in poco1) {
						var child = new BaseCodeTreeModel();
						child.Code = j.Code;
						child.Name = j.Name;
						child.ParentCode = i.Code;
						childs.Add(child);
					}
					model.Childs = childs;
					list.Add(model);
				}
				return list;
			}
		}

	}
}
