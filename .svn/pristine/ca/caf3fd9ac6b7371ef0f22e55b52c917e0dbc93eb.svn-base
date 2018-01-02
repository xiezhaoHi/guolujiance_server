using RTMC.Source;
using System;
using System.Collections.Generic;
using System.Data.Entity;
using System.Linq;
using System.Linq.Expressions;

namespace RTMC.BLL {
    public class UserManager {
        public static SYS_Users Login(string name, string pwd) {
            using (var db = new SystemContext()) {
                return db.SYS_Users
                    .Include(u => u.SYS_Roles)
                    .Include(u => u.SYS_Roles.Select(r => r.SYS_Menus))
                    .Include(u => u.SYS_Roles.Select(r => r.SYS_Authorities))
                    .Include(u => u.SYS_Roles.Select(r => r.SYS_Role2Permission))
                    .Include(u => u.SYS_Organization)
                    .FirstOrDefault(u => u.LoginName == name && u.Password == pwd);
            }
        }

        public static bool AddUser(SYS_Users user) {
            using (var db = new SystemContext()) {
                db.SYS_Users.Add(user);
                return db.SaveChanges() > 0;
            }
        }

        public static List<SYS_Users> GetUserByPage(int index, int size, out int total, Expression<Func<SYS_Users, bool>> condition) {
            using (var db = new SystemContext()) {
                if (condition == null) condition = u => true;
                var list = db.SYS_Users.Where(condition);
                total = list.Count();
                return list.OrderBy(t => t.CreateDate).Skip((index - 1) * size).Take(size).ToList();
            }
        }

        /// <summary>
        /// 包含了数据权限过滤的业务方法。
        /// </summary>
        /// <param name="currentUser"></param>
        /// <param name="index"></param>
        /// <param name="size"></param>
        /// <param name="key"></param>
        /// <param name="total"></param>
        /// <returns></returns>
        public static List<SYS_Users> GetUserByPage(SYS_Users currentUser, int index, int size, string key, out int total) {
            using (var db = new DataProvider(currentUser)) {
                var list = db.Users.Where(m => m.LoginName.Contains(key));
                total = list.Count();
                return list.OrderBy(t => t.CreateDate).Skip(index * size).Take(size).ToList();
            }
        }

        public static bool GetUserByName(string Name, Guid id) {
            using (var db = new SystemContext()) {
                return db.SYS_Users.Where(r => r.LoginName == Name && r.Id != id).Count() > 0;
            }
        }

        public static SYS_Users GetUserById(Guid id) {
            using (var db = new SystemContext()) {
                return db.SYS_Users.FirstOrDefault(r => r.Id == id);
            }
        }

        public static bool UpdateUser(SYS_Users info) {
            using (var db = new SystemContext()) {
                db.Entry<SYS_Users>(info).State = System.Data.Entity.EntityState.Modified;
                return db.SaveChanges() > 0;
            }
        }

        public static bool DelUser(Guid id) {
            using (var db = new SystemContext()) {
                var info = db.SYS_Users.Create();
                info.Id = id;
                db.SYS_Users.Attach(info);
                db.SYS_Users.Remove(info);
                return db.SaveChanges() > 0;
            }
        }

        public static List<Guid> GetRoleIdByUser(Guid uid) {
            using (var db = new SystemContext()) {
                return db.SYS_Users.Include(u => u.SYS_Roles).FirstOrDefault(u => u.Id == uid).SYS_Roles.Select(r => r.Id).ToList();
            }
        }

        public static bool SaveAuthority(Guid userId, List<SYS_Roles> roles, out string userName) {
            using (var db = new SystemContext()) {
                var user = db.SYS_Users.Include(u => u.SYS_Roles).FirstOrDefault(u => u.Id == userId);
                foreach (SYS_Roles item in roles) {
                    if (item.EntityState == EntityState.Added) {
                        db.SYS_Roles.Attach(item);
                        user.SYS_Roles.Add(item);
                    }
                    if (item.EntityState == EntityState.Deleted) {
                        user.SYS_Roles.Remove(user.SYS_Roles.FirstOrDefault(r => r.Id == item.Id));
                    }
                }
                userName = user.LoginName;
                return db.SaveChanges() > 0;
            }
        }


        public static bool ChangePassword(Guid uid, string p) {
            using (var db = new SystemContext()) {
                var entity = db.SYS_Users.FirstOrDefault(x => x.Id == uid);
                if (entity == null) return false;
                entity.Password = p;
                return db.SaveChanges() > 0;
            }
        }
    }
}
