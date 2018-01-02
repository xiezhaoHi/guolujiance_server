using RTMC.Source;
using System;
using System.Collections.Generic;
using System.Data.Entity;
using System.Linq;

namespace RTMC.BLL {
	public class RoleManager {
		public static List<SYS_Roles> GetRoleByPage(int index, int size, string key, out int total) {
			using (var db = new SystemContext()) {
				var list = db.SYS_Roles.Where(m => m.Name.Contains(key));
				total = list.Count();
				return list.OrderBy(t => t.CreateDate).Skip((index - 1) * size).Take(size).ToList();
			}
		}

		public static bool AddRole(SYS_Roles role) {
			using (var db = new SystemContext()) {
				db.SYS_Roles.Add(role);
				return db.SaveChanges() > 0;
			}
		}

		public static SYS_Roles GetRoleById(Guid id) {
			using (var db = new SystemContext()) {
				return db.SYS_Roles.FirstOrDefault(r => r.Id == id);
			}
		}

		public static bool UpdateRole(SYS_Roles role) {
			using (var db = new SystemContext()) {
				db.Entry<SYS_Roles>(role).State = System.Data.Entity.EntityState.Modified;
				return db.SaveChanges() > 0;
			}
		}

		public static bool DelRole(Guid id) {
			using (var db = new SystemContext()) {
				var role = db.SYS_Roles.Create();
				role.Id = id;
				db.SYS_Roles.Attach(role);
				db.SYS_Roles.Remove(role);
				return db.SaveChanges() > 0;
			}
		}

        public static SYS_Roles GetMenuAndAuthority(Guid roleId) {
            using (var db = new SystemContext()) {
                return db.SYS_Roles.Include(r => r.SYS_Menus).Include(r => r.SYS_Authorities).Include(r => r.SYS_Role2Permission).FirstOrDefault(r => r.Id == roleId);
            };
		}

		public static bool GetRoleByName(string Name, Guid id) {
			using (var db = new SystemContext()) {
				return db.SYS_Roles.Where(r => r.Name == Name && r.Id != id).Count() > 0;
			}
		}

		public static List<SYS_Roles> GetRoleAll() {
			using (var db = new SystemContext()) {
				return db.SYS_Roles.Where(r => r.IsEnabled == true).ToList();
			}
		}


		public static bool SaveAuthority(Guid roleId, List<Guid> menus, List<Guid> fas,List<SYS_Role2Permission> roleRelatPermission) {
			using (var db = new SystemContext()) {
				var role = db.SYS_Roles.Include(r=>r.SYS_Menus).Include(r=>r.SYS_Authorities).Include(r=>r.SYS_Role2Permission).FirstOrDefault(r => r.Id == roleId);
				role.SYS_Menus = db.SYS_Menus.Where(m => menus.Contains(m.Id)).ToList();
				role.SYS_Authorities = db.SYS_Authorities.Where(f => fas.Contains(f.Id)).ToList();
                role.SYS_Role2Permission = roleRelatPermission;
				db.Entry<SYS_Roles>(role).State = System.Data.Entity.EntityState.Modified;
				return db.SaveChanges() > 0;
			}
		}
	}
}
