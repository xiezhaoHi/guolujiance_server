using Newtonsoft.Json;
using RTMC.BLL;
using RTMC.Core;
using RTMC.Source;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Security;
using System.Web.SessionState;
using System.Configuration;

namespace RTMC.Web.Handler {
    /// <summary>
    /// NoAuthorityHandler 的摘要说明
    /// </summary>
    public class NoAuthorityHandler : IHttpHandler, IRequiresSessionState {

        public void ProcessRequest(HttpContext context) {
            string result = "";
            string action = context.Request["action"];
            if (action != "login" && context.Session["User"] == null) {
                result = "请重新登陆！";
            } else {
                switch (action) {
                    case "getMenusByUser"://获取用户可见菜单
                        result = this.getMenusByUser(context);
                        break;
                    case "login"://系统登录
                        result = this.login(context);
                        break;
                    case "getBtnsByUser"://获取用户功能权限
                        result = this.getBtnsByUser(context);
                        break;
                    case "ChangePassword"://修改密码
                        result = this.ChangePassword(context);
                        break;
                    default:
                        break;
                }
            }
            context.Response.ContentType = "application/json";
            context.Response.Write(result);
        }


	

		

        private string ChangePassword(HttpContext context) {
            string oldpass = context.Request.Form["oldpass"];
            string newpass = context.Request.Form["newpass"];
            var user = GetUser(context);
            if (user.Password == Function.MD5(oldpass)) {
                return JsonConvert.SerializeObject(UserManager.ChangePassword(user.Id, Function.MD5(newpass)));
            } else
                return "旧密码错误！";
        }

        private string login(HttpContext context) {
            string name = context.Request.Form["name"];
            string pwd = context.Request.Form["pwd"];
            string code = context.Request.Form["code"];
            string sesCode = Parse.ToString(context.Session["ValidCode"]);
            if (!code.Equals(sesCode, StringComparison.CurrentCultureIgnoreCase)) {
                return "验证码错误！";
            }

            var encodePassword = Function.MD5(pwd);
            var user = UserManager.Login(name, encodePassword);
            if (user != null) {
                if (!user.IsEnabled) {
                    return "该用户已被禁用！";
                }
                context.Session["User"] = user;//UserManager.GetUserByUserName(name);
                FormsAuthentication.SetAuthCookie(name, false);
                return JsonConvert.SerializeObject(true);
            } else {
                return "用户名或密码错误！";
            }
        }

        private string getBtnsByUser(HttpContext context) {
            var user = GetUser(context);
            string url = context.Request.UrlReferrer.AbsolutePath;
            return JsonConvert.SerializeObject(AuthorityManager.GetBtnsByUser(user, url));
        }


        private string getMenusByUser(HttpContext context) {
            var user = GetUser(context);
            List<Menus> list = RTCache.Get("Menus_" + user.LoginName) as List<Menus>; // 将用户菜单缓存起来。
            if (list == null) {
                list = MenuManager.GetMenusByUser(user);
                RTCache.Insert("Menus_" + user.LoginName, list, 60 * 60 * 24);
            }
            return JsonConvert.SerializeObject(new { userName = user.LoginName, menus = list });
        }

        public static SYS_Users GetUser(HttpContext context) {
            var cUser = context.Session["User"];
            if (cUser == null) {
                context.Response.Write("请重新登陆！");
                context.Response.End();
                return null;
            }
            return (SYS_Users)cUser;
        }


        public bool IsReusable {
            get {
                return false;
            }
        }
    }
}