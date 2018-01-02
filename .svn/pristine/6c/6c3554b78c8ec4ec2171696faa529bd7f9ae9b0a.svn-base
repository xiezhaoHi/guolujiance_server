using RTMC.Core;
using RTMC.Source;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Linq.Expressions;

namespace RTMC.Permissions {
	public static class PermissionManager<T> {
		public static IDictionary<int, Permission<T>> Library { get; set; }

		/// <summary>
		/// 获取用户关于T类型的数据权限表达式。
		/// </summary>
		/// <param name="user"></param>
		/// <returns></returns>
		public static Expression<Func<T, bool>> GetFormula(SYS_Users user) {
			var lambda = PredicateBuilder.False<T>();
			foreach (var role in user.SYS_Roles) {
				var p = GetPermission(role);
				if (p == null) continue;
				lambda = lambda.Or(p.Formula(user));
			}
			return lambda;
		}

		/// <summary>
		/// 获取角色关于T类型的数据权限定义，若没有找到关于T类型的数据权限，则默认权限为不能访问任何T类型对象。
		/// </summary>
		/// <param name="role"></param>
		/// <returns></returns>
		private static Permission<T> GetPermission(SYS_Roles role) {
			if (role.Permissions == null) role.Permissions = new Dictionary<Type, int>();
			if (role.Permissions.Where(p => p.Key == typeof(T)).Count() == 0) {
				foreach (var r2p in role.SYS_Role2Permission) {
					if (PermissionManager<T>.Library.Keys.Contains(r2p.PermissionId)) {
						role.Permissions.Add(typeof(T), r2p.PermissionId);
						break;
					}
				}
			}
			if (role.Permissions.Keys.Contains(typeof(T))) {
				return PermissionManager<T>.Library[role.Permissions[typeof(T)]];
			} else {
				return new Permission<T>() { Formula = oper => (user => false) };
			}
		}
	}
}
