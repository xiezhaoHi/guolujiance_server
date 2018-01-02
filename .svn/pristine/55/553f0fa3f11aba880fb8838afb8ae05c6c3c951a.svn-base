using Newtonsoft.Json;
using RTMC.BLL;
using RTMC.Core;
using RTMC.Permissions;
using RTMC.Source;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.SessionState;

namespace RTMC.Web.Handler {
    /// <summary>
    /// SystemHandler 的摘要说明
    /// </summary>
    public class SystemHandler : BaseHandler {

        public override void ExecuteToDo(HttpContext context, string action, out string result) {
            switch (action) {
                case "getMenuByPage"://分页获取菜单列表
                    result = this.getMenuByPage(context);
                    break;
                case "getRoleByPage"://分页获取角色列表
                    result = this.getRoleByPage(context);
                    break;
                case "getUserByPage"://分页获取用户列表
                    result = this.getUserByPage(context);
                    break;
                case "getParentMenu"://获取父级菜单
                    result = this.getParentMenu(context);
                    break;
                case "addMenu"://添加菜单
                    result = this.addMenu(context);
                    break;
                case "addRole"://添加角色
                    result = this.addRole(context);
                    break;
                case "addUser"://添加用户
                    result = this.addUser(context);
                    break;//添加功能权限
                case "addFuncAuthority":
                    result = this.addFuncAuthority(context);
                    break;
                case "editFuncAuthority"://编辑功能权限
                    result = this.editFuncAuthority(context);
                    break;
                case "updateMenu"://修改菜单
                    result = this.updateMenu(context);
                    break;
                case "updateRole"://修改角色
                    result = this.updateRole(context);
                    break;
                case "updateUser"://修改用户
                    result = this.updateUser(context);
                    break;
                case "delMenu"://删除菜单
                    result = this.delMenu(context);
                    break;
                case "delFuncAuthority"://上传功能权限
                    result = this.delFuncAuthority(context);
                    break;
                case "delRole"://删除角色
                    result = this.delRole(context);
                    break;
                case "delUser"://删除用户
                    result = this.delUser(context);
                    break;
                case "getMenuById"://获取菜单详情
                    result = this.getMenuById(context);
                    break;
                case "getRoleById"://获取角色详情
                    result = this.getRoleById(context);
                    break;
                case "getUserById"://获取用户详情
                    result = this.getUserById(context);
                    break;
                case "getMenusAndAuth"://获取菜单和权限
                    result = this.getMenusAndAuth(context);
                    break;
                case "saveAuthority"://保存权限
                    result = this.saveAuthority(context);
                    break;
                case "saveUserRelatRole"://保存权限
                    result = this.saveUserRelatRole(context);
                    break;
                case "getAuthorityByRole"://根据角色获取权限
                    result = this.getAuthorityByRole(context);
                    break;
                case "getAuthorityByUser"://根据用户获取权限
                    result = this.getAuthorityByUser(context);
                    break;
                case "getRoleAll"://获取所有角色
                    result = this.getRoleAll();
                    break;
                case "getDataPermissionAll"://获取所有数据权限
                    result = this.getDataPermissionAll();
                    break;
                default:
                    result = "无效请求！";
                    break;
            }
        }

        private string editFuncAuthority(HttpContext context) {
            string data = context.Request.Form["data"];
            try {
                SYS_Authorities fa = Function.DeserializeObject<SYS_Authorities>(data);
				if (AuthorityManager.EditAuthority(fa)) {
					return Function.SerializeObject("成功");
				} else {
					return Function.SerializeObject("失败");
				}
            } catch (Exception e) {
                return "系统错误：" + e.Message;
            }
        }

        private string getDataPermissionAll() {
            return Function.SerializeObject(PermissionLibrary.PermissionAll);
        }

        private string saveUserRelatRole(HttpContext context) {
            string role = context.Request.Form["role"];
            Guid userId = Parse.TryGuid(context.Request.Form["userId"]);
            string userName;
            try {
                List<SYS_Roles> roles = Function.DeserializeObject<List<SYS_Roles>>(role);
                bool isOk = UserManager.SaveAuthority(userId, roles, out userName);
                if (isOk) {
                    RTCache.Remove("Menus_" + userName);//清除菜单缓存
                }
                if (userId == _user.Id && isOk) {
                    return JsonConvert.SerializeObject(0);
                }
                return JsonConvert.SerializeObject(isOk);

            } catch (Exception e) {
                return "系统错误：" + e.Message;
            }
        }

        private string getAuthorityByUser(HttpContext context) {
            Guid userId = Parse.TryGuid(context.Request.Form["userId"]);
            return Function.SerializeObject(UserManager.GetRoleIdByUser(userId));
        }

        private string getRoleAll() {
            return Function.SerializeObject(RoleManager.GetRoleAll());
        }

        private string delUser(HttpContext context) {
            try {
                Guid id = Parse.TryGuid(context.Request.Form["id"]);
                return Function.SerializeObject(UserManager.DelUser(id));
            } catch (Exception e) {
                return "系统错误：" + e.Message;
            }
        }

        private string updateUser(HttpContext context) {
            string data = context.Request.Form["data"];
            try {
                SYS_Users info = Function.DeserializeObject<SYS_Users>(data);
                if (UserManager.GetUserByName(info.LoginName, info.Id))
                    return "用户名称已存在！";
                info.ModifyDate = DateTime.Now;
                info.ModifyUserId = _user.Id;
                return Function.SerializeObject(UserManager.UpdateUser(info));

            } catch (Exception e) {
                return "系统错误：" + e.Message;
            }
        }

        private string getUserById(HttpContext context) {
            Guid id = Parse.TryGuid(context.Request.Form["id"]);

            return Function.SerializeObject(UserManager.GetUserById(id));
        }

        private string addUser(HttpContext context) {
            string data = context.Request.Form["data"];
            try {
                SYS_Users info = Function.DeserializeObject<SYS_Users>(data);
                if (UserManager.GetUserByName(info.LoginName, info.Id))
                    return "用户名称已存在！";
                info.CreateDate = DateTime.Now;
                info.CreateUserId = _user.Id;
                info.Password = Function.MD5("123456");
                return Function.SerializeObject(UserManager.AddUser(info));
            } catch (Exception e) {
                return "系统错误：" + e.Message;
            }
        }

        private string getUserByPage(HttpContext context) {
            int index = Parse.ToInt(context.Request.Form["index"]);
            int size = Parse.ToInt(context.Request.Form["size"]);
            string key = context.Request.Form["key"];
            int total;
            var condition = PermissionManager<SYS_Users>.GetFormula(_user); // 获取当前登录用户关于系统用户的数据权限。
            condition = condition.And(m => m.LoginName.Contains(key));
            var list = UserManager.GetUserByPage(index, size, out total, condition);
            string json = Function.SerializeObject(list);
            return "{\"data\":" + json + ",\"total\": \"" + total + "\"}";
        }

        private string delFuncAuthority(HttpContext context) {
            try {
                Guid id = Parse.TryGuid(context.Request.Form["id"]);

                bool isOk = AuthorityManager.DelAuthority(id);
                if (isOk) {
                    RTCache.ClearMenu();
                }
                return JsonConvert.SerializeObject(isOk);
            } catch (Exception e) {
                return "系统错误：" + e.Message;
            }
        }

        private string addFuncAuthority(HttpContext context) {
            string data = context.Request.Form["data"];
            try {
                SYS_Authorities fa = Function.DeserializeObject<SYS_Authorities>(data);
                return Function.SerializeObject(AuthorityManager.AddAuthority(fa));
            } catch (Exception e) {
                return "系统错误：" + e.Message;
            }
        }

        private string getAuthorityByRole(HttpContext context) {
            Guid roleId = Parse.TryGuid(context.Request.Form["roleId"]);
            var role = RoleManager.GetMenuAndAuthority(roleId);
            return Function.SerializeObject(new { menus = role.SYS_Menus.Select(m => m.Id), fas = role.SYS_Authorities.Select(fa => fa.Id), per = role.SYS_Role2Permission.Select(p => p.PermissionId) });

        }

        private string saveAuthority(HttpContext context) {
            string menu = context.Request.Form["menu"];
            string authority = context.Request.Form["authority"];
            string dataAuthority = context.Request.Form["dataAuthority"];
            Guid roleId = Parse.TryGuid(context.Request.Form["roleId"]);

            try {
                List<Guid> roleRelatMenu = Function.DeserializeObject<List<Guid>>(menu);
                List<Guid> roleRelatFuncAuthority = Function.DeserializeObject<List<Guid>>(authority);
                List<SYS_Role2Permission> roleRelatPermission = Function.DeserializeObject<List<SYS_Role2Permission>>(dataAuthority);
                bool isOk = RoleManager.SaveAuthority(roleId, roleRelatMenu, roleRelatFuncAuthority, roleRelatPermission);
                if (isOk) {
                    RTCache.ClearMenu();//清除菜单缓存
                }
                var role = _user.SYS_Roles.FirstOrDefault(r => r.Id == roleId);
                if (role != null && isOk) {
                    return JsonConvert.SerializeObject(0);
                }
                return JsonConvert.SerializeObject(isOk);

            } catch (Exception e) {
                return "系统错误：" + e.Message;
            }
        }

        private string getMenusAndAuth(HttpContext context) {
            var list = MenuManager.getMenusAndAuth();
            return Function.SerializeObject(list);
        }

        private string delRole(HttpContext context) {
            try {
                Guid id = Parse.TryGuid(context.Request.Form["id"]);
                return Function.SerializeObject(RoleManager.DelRole(id));
            } catch (Exception e) {
                return "系统错误：" + e.Message;
            }
        }

        private string updateRole(HttpContext context) {
            string data = context.Request.Form["data"];
            
            try {
                SYS_Roles role = Function.DeserializeObject<SYS_Roles>(data);
                if (RoleManager.GetRoleByName(role.Name, role.Id))
                    return "角色名称已存在！";
                role.ModifyDate = DateTime.Now;
                role.ModifyUserId = _user.Id;
                return Function.SerializeObject(RoleManager.UpdateRole(role));

            } catch (Exception e) {
                return "系统错误：" + e.Message;
            }
        }

        private string getRoleById(HttpContext context) {
            Guid id = Parse.TryGuid(context.Request.Form["id"]);
            return Function.SerializeObject(RoleManager.GetRoleById(id));
        }

        private string addRole(HttpContext context) {
            string data = context.Request.Form["data"];
            try {
                SYS_Roles role = Function.DeserializeObject<SYS_Roles>(data);
                if (RoleManager.GetRoleByName(role.Name, role.Id))
                    return "角色名称已存在！";
                role.CreateDate = DateTime.Now;
                role.CreateUserId = _user.Id;
                return Function.SerializeObject(RoleManager.AddRole(role));

            } catch (Exception e) {
                return "系统错误：" + e.Message;
            }
        }

        private string getRoleByPage(HttpContext context) {
            int index = Parse.ToInt(context.Request.Form["index"]);
            int size = Parse.ToInt(context.Request.Form["size"]);
            string key = context.Request.Form["key"];
            int total;
            var list = RoleManager.GetRoleByPage(index, size, key, out total);
            string json = Function.SerializeObject(list);
            return "{\"data\":" + json + ",\"total\": \"" + total + "\"}";
        }

        private string updateMenu(HttpContext context) {
            string menu = context.Request.Form["menu"];
            
            try {
                SYS_Menus men = Function.DeserializeObject<SYS_Menus>(menu);
                men.ModifyDate = DateTime.Now;
                men.ModifyUserId = _user.Id;
                bool isOk = MenuManager.UpdateMenu(men);
                if (isOk) {
                    RTCache.ClearMenu();//清除菜单缓存
                }
                return JsonConvert.SerializeObject(isOk);

            } catch (Exception e) {
                return "系统错误：" + e.Message;
            }
        }

        private string getMenuById(HttpContext context) {
            
            Guid id = Parse.TryGuid(context.Request.Form["id"]);

            SYS_Menus menu = MenuManager.GetMenuById(id);
            return Function.SerializeObject(menu);//后面参数忽略循环引用

        }

        private string delMenu(HttpContext context) {
            try {
                Guid id = Parse.TryGuid(context.Request.Form["id"]);
                bool isOk = MenuManager.DelMenu(id);
                if (isOk) {
                    RTCache.ClearMenu();//清除菜单缓存
                }
                return JsonConvert.SerializeObject(isOk);
            } catch (Exception e) {
                return "系统错误：" + e.Message;
            }
        }

        private string addMenu(HttpContext context) {
            string menu = context.Request.Form["menu"];
            string auths = context.Request.Form["auths"];
           
            try {
                var fas = Function.DeserializeObject<List<SYS_Authorities>>(auths);
                var men = Function.DeserializeObject<SYS_Menus>(menu);
                men.CreateDate = DateTime.Now;
                men.CreateUserId = _user.Id;
                men.SYS_Authorities = fas;
                return Function.SerializeObject(MenuManager.AddMenu(men));

            } catch (Exception e) {
                return "系统错误：" + e.Message;
            }
        }

        private string getParentMenu(HttpContext context) {
            return Function.SerializeObject(MenuManager.GetParentMenu());
        }

        private string getMenuByPage(HttpContext context) {
            int index = Parse.ToInt(context.Request.Form["index"]);
            int size = Parse.ToInt(context.Request.Form["size"]);
            string key = context.Request.Form["key"];
            int total;
            string json = Function.SerializeObject(MenuManager.GetMenuByPage(index, size, key, out total));
            return "{\"data\":" + json + ",\"total\": \"" + total + "\"}";
        }
    }
}