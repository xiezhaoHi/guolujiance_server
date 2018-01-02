using RTMC.Source;
using System;
using System.Collections.Generic;
using System.Data.Entity;
using System.Linq;

namespace RTMC.BLL {
	public class AuthorityManager {

		/// <summary>
		/// 权限验证
		/// </summary>
		/// <param name="uid">用户ID</param>
		/// <param name="url">操作页面（Aspx）</param>
		/// <param name="h">控制器（Handler）</param>
		/// <param name="a">动作（Action）</param>
		/// <returns></returns>
		public static bool IsAuthority(SYS_Users user, string url, string handler, string action) {
			using (var db = new SystemContext()) {
				foreach (var role in user.SYS_Roles) {
					int count = role.SYS_Authorities.Where(f => f.Action.Contains(action) && f.Handler.Contains(handler)).Join(role.SYS_Menus.Where(m => m.Url == url), f => f.MenuId, m => m.Id, (f, m) => new { f.Id }).Count();
					if (count > 0)
						return true;
				}
				return false;
			}
		}

		public static List<string> GetBtnsByUser(SYS_Users user, string url) {
			using (var db = new SystemContext()) {
				var btnList = new List<string>();
				foreach (var roles in user.SYS_Roles) {
					var rids = roles.SYS_Authorities.Join(roles.SYS_Menus.Where(m => m.Url == url), fa => fa.MenuId, m => m.Id, (fa, m) => fa.BtnClass).ToList();
					btnList.AddRange(rids);
				}
				return btnList;
			}
		}

		public static bool AddAuthority(SYS_Authorities authority) {
			using (var db = new SystemContext()) {
				db.SYS_Authorities.Add(authority);
				return db.SaveChanges() > 0;
			}
		}

		public static bool DelAuthority(Guid id) {
			using (var db = new SystemContext()) {
				var info = db.SYS_Authorities.Create();
				info.Id = id;
				db.SYS_Authorities.Attach(info);
				db.SYS_Authorities.Remove(info);
				return db.SaveChanges() > 0;
			}
		}

		//编辑
        public static bool EditAuthority(SYS_Authorities fa) {
            using (var db = new SystemContext()) {
				db.SYS_Authorities.Attach(fa);
				db.Entry(fa).State = EntityState.Modified;
				//db.Entry<SYS_Authorities>(fa).State = EntityState.Modified;
                return db.SaveChanges() > 0;
            }
        }
    }
}
