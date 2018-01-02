using System;
using System.Linq.Expressions;

namespace RTMC.Source {

	/// <summary>
	/// 基于对象的数据权限
	/// </summary>
	/// <typeparam name="T">数据权限管理对象。</typeparam>
	public partial class Permission<T> {
		/// <summary>
		/// 数据权限自定义编号。
		/// </summary>
		public int Code { get; set; }
		
		/// <summary>
		/// 数据权限显示名称，用于页面配置。
		/// </summary>
		public string Name { get; set; }

		/// <summary>
		/// 基于系统用户SYS_Users的数据权限表达式。
		/// </summary>
		public Func<SYS_Users, Expression<Func<T, bool>>> Formula { get; set; }
	}
}
