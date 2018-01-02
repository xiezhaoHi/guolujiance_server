using RTMC.Source;
using System;
using System.Collections.Generic;
using System.Data.Entity;
using System.Linq;

namespace RTMC.BLL {
    public class MenuManager {
        /// <summary>
        /// 获取所有菜单
        /// </summary>
        /// <returns></returns>
        public static List<Menus> GetMenusByUser2(SYS_Users user) {
            using (var db = new SystemContext()) {
                List<Menus> list = new List<Menus>();

                var menus = new List<SYS_Menus>();
                foreach (var item in user.SYS_Roles) {
                    menus.AddRange(item.SYS_Menus.OrderBy(m => m.Sort));
                }
                menus = menus.Distinct().ToList();
                foreach (var parent in menus.Where(m => m.ParentId == null)) {
                    Menus menu = new Menus { Id = parent.Id, Name = parent.Name, Url = parent.Url, Icon = parent.Icon, };
                    foreach (var item in menus.Where(m => m.ParentId == parent.Id)) {
                        menu.Child.Add(new Menus { Id = item.Id, Name = item.Name, Url = item.Url, Icon = item.Icon });
                    }
                    list.Add(menu);
                }
                return list;
            }
        }


        /// <summary>
        /// 获取所有菜单
        /// </summary>
        /// <returns></returns>
        public static List<Menus> GetMenusByUser(SYS_Users user) {
            using (var db = new SystemContext()) {
                List<Menus> list = new List<Menus>();

                var menus = new List<SYS_Menus>();
                foreach (var item in user.SYS_Roles.Where(r => r.IsEnabled == true)) {
                    menus.AddRange(item.SYS_Menus.OrderBy(m => m.Sort));
                }
                menus = menus.Distinct().ToList();
                foreach (var parent in menus.Where(m => m.ParentId == null)) {
                    Menus menu = new Menus { Id = parent.Id, Name = parent.Name, Url = parent.Url, Icon = parent.Icon, };
                    GetChildMenu(menu, menus);
                    list.Add(menu);
                }
                return list;
            }
        }
        public static void GetChildMenu(Menus menu, List<SYS_Menus> list) {
            foreach (var item in list.Where(m => m.ParentId == menu.Id)) {
                var temp = new Menus { Id = item.Id, Name = item.Name, Url = item.Url, Icon = item.Icon };
                menu.Child.Add(temp);
                GetChildMenu(temp, list);
            }
        }

        public static List<ViewMenu> GetMenuByPage(int index, int size, string key, out int total) {
            using (var db = new SystemContext()) {
                var list = db.ViewMenu.Where(m => m.IsEnabled == true && m.Name.Contains(key));
                total = list.Count();
                return list.OrderBy(t => t.Sort).Skip((index - 1) * size).Take(size).ToList();
            }
        }
        public static List<Menus> GetParentMenu() {
            using (var db = new SystemContext()) {
                return db.SYS_Menus.Where(m => m.IsEnabled == true && m.ParentId == null).Select(t => new Menus() { Id = t.Id, Name = t.Name }).ToList();
            }
        }

        public static object AddMenu(SYS_Menus men) {
            using (var db = new SystemContext()) {
                db.SYS_Menus.Add(men);
                return db.SaveChanges() > 0;
            }
        }

        public static bool DelMenu(Guid id) {
            using (var db = new SystemContext()) {
                var menu = db.SYS_Menus.Create();
                menu.Id = id;
                db.SYS_Menus.Attach(menu);
                db.SYS_Menus.Remove(menu);
                return db.SaveChanges() > 0;
            }
        }

        public static SYS_Menus GetMenuById(Guid id) {
            using (var db = new SystemContext()) {
                var info = db.SYS_Menus.Include(m => m.SYS_Authorities).FirstOrDefault(m => m.Id == id);
                info.SYS_Authorities = info.SYS_Authorities.OrderBy(a => a.CreateDate).ToList();
                return info;
            }
        }

        public static bool UpdateMenu(SYS_Menus men) {
            using (var db = new SystemContext()) {
                db.Entry<SYS_Menus>(men).State = System.Data.Entity.EntityState.Modified;
                return db.SaveChanges() > 0;
            }
        }


        public static List<SYS_Menus> getMenusAndAuth() {
            using (var db = new SystemContext()) {
                var list = db.SYS_Menus.Include(m => m.SYS_Authorities).Where(m => m.IsEnabled == true).OrderBy(m=>m.Sort).ToList();
                foreach (var item in list) {
                    item.SYS_Authorities = item.SYS_Authorities.OrderBy(a => a.CreateDate).ToList();
                }
                return list;
            }
        }


    }
}
