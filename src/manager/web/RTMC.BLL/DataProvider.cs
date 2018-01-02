using RTMC.Permissions;
using System;
using System.Linq;

namespace RTMC.Source {
	public class DataProvider : SystemContext {
		private SYS_Users currentUser;
		public DataProvider(SYS_Users user) {
			if (user == null) throw new Exception("当前登录用户信息不存在或已超时，无权调用查询。");
			this.currentUser = user;
		}

		/// <summary>
		/// 只读，数据添加了数据权限相关过滤。
		/// </summary>
		public IQueryable<SYS_Users> Users {
			get { return base.SYS_Users.Where(PermissionManager<SYS_Users>.GetFormula(currentUser)); }
		}

		// TODO 封装父类DbSet属性
	}

}
