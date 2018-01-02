using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RTMC.Source {
	public partial class SYS_Roles {
		/// <summary>
		/// 角色具有的数据权限的字典缓存，其中Type为数据权限对象的类型，int为该权限在PermissionLibrary中的定义编号。
		/// </summary>
		public virtual IDictionary<Type, int> Permissions { get; set; }
	}
}
